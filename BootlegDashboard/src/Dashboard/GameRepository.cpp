
#include "Dashboard/GameRepository.hpp"
#include "Dashboard/DashboardMode.hpp"

#include <KIT/Engine.hpp>
#include <KIT/Configuration.hpp>
#include <KIT/Managers/AssetManager.hpp>

#include <WIR/XML/XMLDocument.hpp>
#include <WIR/XML/XMLParser.hpp>
#include <WIR/XML/XMLElement.hpp>

namespace 
{
  bootleg::Repository *instance = nullptr;
}

bootleg::Repository *bootleg::Repository::instance()
{
  return ::instance;
}

bootleg::Repository::Repository(DashboardMode *mode)
    : m_mode(mode)
{
  if (::instance)
  {
    LogError("Game repository already exists");
    return;
  }

  m_path = m_mode->configuration()->get<std::string>("RepositoryPath", "./Repository/");

  wir::Directory(gamesPath()).iterate(false, [this](auto entry) {
    auto dir = dynamic_cast<wir::Directory *>(entry);
    Game g;
    if (dir)
    {
      if(loadGame(wir::hextoint(dir->name()), g))
      {
        LogNotice("Found game \"%s\"", g.title.c_str());
      }
    }
  });


  ::instance = this;

}

bootleg::Repository::~Repository()
{
  if (::instance == this)
  {
    ::instance = nullptr;
  }
}

kit::TexturePtr bootleg::Repository::gameBanner(uint64_t id)
{
  return m_mode->assetManager()->load<kit::Texture>(bannerPath(id));
}

bootleg::Group bootleg::Repository::group(uint64_t id)
{
  auto finder = m_groupIndex.find(id);
  if (finder == m_groupIndex.end())
  {
    // todo: fetch group from storage
    return {};
  }

  if (finder->second >= m_groups.size())
    return {};

  return m_groups[finder->second];
}

bootleg::Game bootleg::Repository::game(uint64_t id)
{
  auto finder = m_gameIndex.find(id);
  if (finder == m_gameIndex.end())
  {
    // todo: fetch game from storage
    return {};
  }

  if (finder->second >= m_games.size())
    return {};

  return m_games[finder->second];
}

std::vector<bootleg::Game> const &bootleg::Repository::games() const
{
  return m_games;
}

bool bootleg::Repository::createGame(Game specification, Game &out)
{
  if (gameLoaded(specification.id))
  {
    LogError("Game already exists and is loaded: %u: %s", specification.id, specification.title.c_str());
    return false;
  }
  
  if (wir::Directory(gamePath(specification.id)).exist())
  {
    LogError("Game directory already exists: %u: %s", specification.id, specification.title.c_str());
    return false;  
  }

  if (wir::File(metaPath(specification.id)).exist())
  {
    LogError("Game meta already exists: %u: %s", specification.id, specification.title.c_str());
    return false;
  }

  if (wir::File(bannerPath(specification.id)).exist())
  {
    LogError("Game banner already exists: %u: %s", specification.id, specification.title.c_str());
    return false;
  }

  if (!wir::File(metaPath(specification.id)).createPath())
  {
    LogError("Failed to create output directory");
    return false;
  }

  std::string xml = wir::format("<Game title=\"%s\" />", specification.title);
  if (!wir::File(metaPath(specification.id)).writeString(xml))
  {
    LogError("Failed to write meta");
    return false;
  }

  return loadGame(specification.id, out);
}

bool bootleg::Repository::loadGame(uint64_t id, Game &out)
{
  if (gameLoaded(id))
  {
    LogWarning("Game already loaded");
    out = game(id);
    return true;
  }

  if (!wir::File(metaPath(id)).exist())
  {
    LogError("Failed to find metafile");
    return false;
  }

  if (!wir::File(bannerPath(id)).exist())
  {
    LogWarning("Failed to find bannerfile");
  }

  wir::XMLDocument xml;
  if (!wir::XMLParser().loadFromFile(metaPath(id), xml))
  {
    LogError("Failed to parse XML");
    return false;
  }

  if(xml.rootElements().empty())
  {
    LogError("No XML data");
    return false;
  }

  auto root = xml.rootElements()[0];
  if(root->name() != "Game")
  {
    LogError("Invalid XML data");
    return false;
  }

  std::string title = "Generic Title";
  root->string("title", title);

  Game newGame;
  newGame.id = id;
  newGame.title = title;

  m_games.push_back(newGame);
  m_gameIndex[newGame.id] = m_games.size() - 1;
  
  out = newGame;

  return true;
}

bool bootleg::Repository::gameLoaded(uint64_t game)
{
  return m_gameIndex.contains(game);
}

std::string bootleg::Repository::gamesPath()
{
  return wir::format("%s/Games/", m_path.c_str());
}

std::string bootleg::Repository::gamePath(uint64_t id)
{
  return wir::format("%s%s/", gamesPath().c_str(), wir::inttohex(id).c_str());
}

std::string bootleg::Repository::metaPath(uint64_t id)
{
  return wir::format("%s/Meta.xml", gamePath(id).c_str());
}

std::string bootleg::Repository::bannerPath(uint64_t id)
{
  return wir::format("%s/Banner.asset", gamePath(id).c_str());
}

