
#pragma once

#include <Dashboard/Models/Game.hpp>
#include <Dashboard/Models/Group.hpp>

#include <KIT/Assets/Texture.hpp>
#include <WIR/Math.hpp>

#include <map>

namespace bootleg
{

  class DashboardMode;

  class Repository
  {
  public:
    static Repository *instance();

    Repository(DashboardMode *mode);
    virtual ~Repository();

    kit::TexturePtr gameBanner(uint64_t id);

    Group group(uint64_t id);
    Game game(uint64_t id);

    std::vector<Game> const &games() const;

  protected:

    bool createGame(Game specification, Game &out);
    bool loadGame(uint64_t id, Game &out);
    bool gameLoaded(uint64_t id);

    std::string gamesPath();
    std::string gamePath(uint64_t id);
    std::string metaPath(uint64_t id);
    std::string bannerPath(uint64_t id);

    DashboardMode *m_mode = nullptr;
    std::string m_path = "";

    std::vector<Group> m_groups;
    std::vector<Game> m_games;

    std::map<uint64_t, uint64_t> m_gameIndex;
    std::map<uint64_t, uint64_t> m_groupIndex;

  };

} // namespace bootleg