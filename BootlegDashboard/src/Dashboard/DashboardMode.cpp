
#include "Dashboard/DashboardMode.hpp"
#include "Dashboard/GameRepository.hpp"

#include "Dashboard/Resolution.hpp"
#include "Dashboard/UI/UIBox.hpp"
#include "Dashboard/UI/UIBanner.hpp"

#include <KIT/Engine.hpp>

#include <KIT/Assets/Texture.hpp>

#include <KIT/Managers/AssetManager.hpp>
#include <KIT/Managers/GameManager.hpp>
#include <KIT/Managers/InputManager.hpp>
#include <KIT/Managers/VRManager.hpp>
#include <KIT/Managers/RenderManager.hpp>
#include <KIT/Renderer/Renderer.hpp>

#include <KIT/Game/Object.hpp>
#include <KIT/Game/World.hpp>

#include <KIT/Game/PlayerState.hpp>


#include <KIT/Rendering/SpriteRenderer.hpp>
#include <KIT/Game/Components/StaticMeshComponent.hpp>
#include <KIT/Game/Components/CameraComponent.hpp>

#include <WIR/String.hpp>

#include <chrono>
#include <ctime>    

namespace 
{

  kit::Object *display = nullptr;
  kit::Transformable *camera = nullptr;
  float cameraX = 0.0f;
  float cameraY = 0.0f;
}

bootleg::DashboardMode::DashboardMode(wir::DynamicArguments const &args)
    : kit::GameMode(args)
{
}

bootleg::DashboardMode::DashboardMode(kit::GameManager *manager)
    : kit::GameMode(manager)
{
}

bootleg::DashboardMode::~DashboardMode()
{
}

void bootleg::DashboardMode::onModeActivated()
{
  targetResolution = renderManager()->resolution();
  LogNotice("Target resolution: %ux%u", targetResolution.x, targetResolution.y);

  m_repository = new Repository(this);

  m_playerState = gameManager()->playerState(0);

  inputManager()->onDiscoveredDevice() += [=](auto dev) {
    dev->assign(m_playerState);
  };

  // Bind inputs to events

  m_playerState->bindAxis("MouseHorizontal", "ma_x", wir::AT_Normal);
  m_playerState->bindAxis("MouseVertical", "ma_y", wir::AT_Normal);

  m_playerState->bindAxis("NavigateHorizontal", "gamepad_analog_x", wir::AT_Normal);
  m_playerState->bindAxis("NavigateVertical", "gamepad_analog_y", wir::AT_Normal);
  m_playerState->bindButton("NavigateRight", "gamepad_dpad_right", wir::BT_Down);
  m_playerState->bindButton("NavigateDown", "gamepad_dpad_down", wir::BT_Down);
  m_playerState->bindButton("NavigateLeft", "gamepad_dpad_left", wir::BT_Down);
  m_playerState->bindButton("NavigateUp", "gamepad_dpad_up", wir::BT_Down);
  m_playerState->bindButton("NavigateRight", "right", wir::BT_Down);
  m_playerState->bindButton("NavigateDown", "down", wir::BT_Down);
  m_playerState->bindButton("NavigateLeft", "left", wir::BT_Down);
  m_playerState->bindButton("NavigateUp", "up", wir::BT_Down);
  m_playerState->bindButton("NextView", "gamepad_shoulder_right", wir::BT_Down);
  m_playerState->bindButton("PreviousView", "gamepad_shoulder_left", wir::BT_Down);
  m_playerState->bindButton("Select", "gamepad_a", wir::BT_AsAxisPositive);
  m_playerState->bindButton("Back", "gamepad_b", wir::BT_AsAxisNegative);
  
  // Bind events to functions
  m_playerState->getAxisEvent("MouseHorizontal") += [](float delta) {
    ::cameraX += delta;
  };

  m_playerState->getAxisEvent("MouseVertical") += [](float delta) {
    ::cameraY += delta;
  };

  m_playerState->getAxisEvent("NavigateHorizontal") += wir::MemberFunction(this, &bootleg::DashboardMode::handleNavigateHorizontal);
  m_playerState->getAxisEvent("NavigateVertical") += wir::MemberFunction(this, &bootleg::DashboardMode::handleNavigateVertical);

  m_playerState->getButtonEvent("NavigateDown") += wir::MemberFunction(this, &bootleg::DashboardMode::handleNavigateDown);
  m_playerState->getButtonEvent("NavigateLeft") += wir::MemberFunction(this, &bootleg::DashboardMode::handleNavigateLeft);
  m_playerState->getButtonEvent("NavigateRight") += wir::MemberFunction(this, &bootleg::DashboardMode::handleNavigateRight);
  m_playerState->getButtonEvent("NavigateUp") += wir::MemberFunction(this, &bootleg::DashboardMode::handleNavigateUp);

  m_playerState->getButtonEvent("NextView") += wir::MemberFunction(this, &bootleg::DashboardMode::handleNextView);
  m_playerState->getButtonEvent("PreviousView") += wir::MemberFunction(this, &bootleg::DashboardMode::handlePreviousView);
  m_playerState->getButtonEvent("Select") += wir::MemberFunction(this, &bootleg::DashboardMode::handleSelect);
  m_playerState->getButtonEvent("Back") += wir::MemberFunction(this, &bootleg::DashboardMode::handleBack);

  //m_backgroundNormal = assetManager()->loadSync<kit::Texture>("Content/Wallpaper/RuvimMiksanskiy_Winter_Normal.asset");
  //m_backgroundBlurred = assetManager()->loadSync<kit::Texture>("Content/Wallpaper/RuvimMiksanskiy_Winter_Blurred.asset");

  /*
  m_bannerMask = new UIMask(engine(),
    {0.0f, 0.0f}, targetSpace({1920.f, 1080.f}),
    assetManager()->loadSync<kit::Texture>("Content/Masks/GamesOverviewMask.asset") );*/
   
  /*
  
  for (uint32_t i = 0; i < m_dummyBanners.size(); i++)
  {
    m_dummyBanners[i] = new UIBanner(engine(), assetManager()->loadSync<kit::Texture>(wir::format("Content/Banners/DummyBanner%u.asset", i + 1)));
    m_dummyBanners[i]->mode(i < 4 ? BM_ActiveRow : BM_InactiveRow);
  }*/

  /*
  auto clockFont = assetManager()->loadSync<kit::Font>("Content/Fonts/TitilliumWeb-ExtraLight.asset");
  m_clock = new kit::Text(clockFont, targetSpace(36.0f), U"00:00");
  m_clock->alignment(kit::TA_TopRight);
  m_clock->color({1.f, 1.f, 1.f, 0.7f});
  m_clock->position(targetSpace({1920.f - 64.0f, 48.f}));
  

  auto headerFont = assetManager()->loadSync<kit::Font>("Content/Fonts/TitilliumWeb-Regular.asset");
  m_groupMostPlayed = new kit::Text(headerFont, targetSpace(72.0f), U"Most played");
  m_groupMostPlayed->position(targetSpace({120.f, 240.f}));

  m_groupNintendoSwitch = new kit::Text(headerFont, targetSpace(48.0f), U"Nintendo Switch");
  m_groupNintendoSwitch->position(targetSpace({120.f, 752.f}));
  m_groupNintendoSwitch->color({1.0f, 1.0f, 1.0f, 0.65f});

  auto menuFont = assetManager()->loadSync<kit::Font>("Content/Fonts/TitilliumWeb-ExtraLight.asset");
  m_menuGames = new kit::Text(menuFont, targetSpace(48.0f), U"Games");
  m_menuGames->position(targetSpace({112.f, 96.f}));

  m_menuStore = new kit::Text(menuFont, targetSpace(48.0f), U"Store");
  m_menuStore->color({1.0f, 1.0f, 1.0f, 0.25f});
  m_menuStore->position(targetSpace({328.f, 96.f}));

  m_menuDownloads = new kit::Text(menuFont, targetSpace(48.0f), U"Downloads");
  m_menuDownloads->color({1.0f, 1.0f, 1.0f, 0.25f});
  m_menuDownloads->position(targetSpace({512.f, 96.f}));

  m_menuSettings = new kit::Text(menuFont, targetSpace(48.0f), U"Settings");
  m_menuSettings->color({1.0f, 1.0f, 1.0f, 0.25f});
  m_menuSettings->position(targetSpace({800.f, 96.f}));
  */

  /*

  auto obj = world()->spawnObject("MilitaryRadio");
  for (uint32_t i = 0; i < 7; i++)
  {
    auto mesh = assetManager()->load<kit::Mesh>(wir::format("Content/Models/MilitaryRadio/MilitaryRadioMesh_%u.asset", i));
    auto comp = obj->spawnComponent<kit::StaticMeshComponent>(wir::format("MilitaryRadioMesh_%u", i));
    comp->mesh(mesh);

    comp->attach(obj);
  }

  */

  auto obj = world()->spawnObject("CartoonMesh");
  for (uint32_t i = 0; i < 1; i++)
  {
    auto mesh = assetManager()->load<kit::Mesh>(wir::format("Content/Models/CartoonKnight/CartoonKnight_%u.asset", i));
    auto comp = obj->spawnComponent<kit::StaticMeshComponent>(wir::format("MilitaryRadioMesh_%u", i));
   
    comp->mesh(mesh);

    comp->attach(obj);
  }
  obj->translate(glm::vec3(0.0f, 0.5f, 0.0f));

  ::display = obj;

  auto cmObj = world()->spawnObject("Camera");
  auto cm = cmObj->spawnComponent<kit::CameraComponent>("CameraComponent");
  cm->attach(cmObj);
  cm->primary();

  ::camera = cmObj;

  world()->start();

}

void bootleg::DashboardMode::onModeDeactivated()
{
  //delete m_bannerMask;
  for (uint32_t i = 0; i < m_dummyBanners.size(); i++)
  {
    delete m_dummyBanners[i];
  }

  /*
  
  delete m_menuGames;
  delete m_menuStore;
  delete m_menuDownloads;
  delete m_menuSettings;

  delete m_groupMostPlayed;
  delete m_groupNintendoSwitch;

  delete m_clock;
  */
  delete m_repository;
}



void renderBannerSelected(kit::TexturePtr texture, glm::vec2 position)
{
}

void renderBannerActiveRow(kit::TexturePtr texture, glm::vec2 position)
{

}

void renderBannerInactiveRow(kit::TexturePtr texture, glm::vec2 position)
{

}

void renderActiveRow(glm::vec2 position, uint32_t activeIndex)
{

}

void renderActiveRow(glm::vec2 position)
{
}

void bootleg::DashboardMode::update(double seconds)
{
  static double ss = 0.0;
  static bool b = false;
  /*
  if(ss >= 1.0f)
  {
    m_dummyBanners[2]->mode(b ? BM_Selected : BM_ActiveRow);
    m_dummyBanners[1]->mode(b ? BM_ActiveRow : BM_Selected);
    
    b = !b;

    ss = 0.0f;
  }

  ss += seconds;



  updateBackground(seconds);*/

  char str[26];
  char *strr = &str[0];
  time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
#if defined(WIR_Windows)
  ctime_s(str, sizeof str, &time);
#elif defined(WIR_Linux)
  strr = ctime(&time);
#endif

  glm::quat rotation = glm::rotate(glm::quat(), glm::radians(::cameraX * 0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
  rotation = glm::rotate(rotation, glm::radians(::cameraY * -0.5f), glm::vec3(1.0f, 0.0f, 0.0f));
  glm::vec3 position = rotation * kit::Transformable::forward() * -0.9f;
  
   ::camera->localRotation(rotation);
  ::camera->localPosition(position);
  //::display->rotateY(float(seconds) * 45.0f);

  /*
  m_clock->text(wir::utf8to32(wir::substring(str, 11, 5)));

  m_clock->render();

  m_menuGames->render();
  m_menuStore->render();
  m_menuDownloads->render();
  m_menuSettings->render();

  m_groupMostPlayed->render();
  m_groupNintendoSwitch->render();*/

  bool renderUi = false;
  if (renderUi)
  {

    auto r = renderManager();

    float x = 272.0f;
    float y = 496.0f;

    UIBanner *selected = nullptr;
    glm::vec2 selectedPos;
    for (int i = 0; i < m_dummyBanners.size(); i++)
    {
      m_dummyBanners[i]->update(seconds);
      if (m_dummyBanners[i]->mode() != BM_Selected)
        m_dummyBanners[i]->render(targetSpace({x, y}));
      else
      {
        selectedPos = {x, y};
        selected = m_dummyBanners[i];
      }

      x += 352.f;
      if (i == 3)
      {
        y += 192.f + 256.0f;
        x = 272.0f;
      }
    }

    if (selected)
      selected->render(targetSpace(selectedPos));
  }
}

void bootleg::DashboardMode::handleNavigateHorizontal(float delta)
{
  static std::deque<float> lastValues;
  
  bool still = true;
  for (auto val : lastValues)
  {
    if (glm::abs(val) > 0.0f)
    {
      still = false;
      break;
    }
  }


  lastValues.push_back(delta);

  if (lastValues.size() > 3)
    lastValues.pop_front();


  
  /*if (delta > 0.0f)
  {
    m_backgroundAlphaTarget = 1.0f;
  }
  else if (delta < 0.0f)
  {
    m_backgroundAlphaTarget = 0.0f;
  }*/
}

void bootleg::DashboardMode::handleNavigateVertical(float delta)
{
}

void bootleg::DashboardMode::handleNavigateDown()
{
  m_dummyBanners[2]->mode(BM_Selected);
  m_dummyBanners[1]->mode(BM_ActiveRow);
}

void bootleg::DashboardMode::handleNavigateLeft()
{
}

void bootleg::DashboardMode::handleNavigateRight()
{
}

void bootleg::DashboardMode::handleNavigateUp()
{
  m_dummyBanners[2]->mode(BM_ActiveRow);
  m_dummyBanners[1]->mode(BM_Selected);
}

void bootleg::DashboardMode::handleNextView()
{
}

void bootleg::DashboardMode::handlePreviousView()
{
}

void bootleg::DashboardMode::handleSelect()
{
}

void bootleg::DashboardMode::handleBack()
{
}

void bootleg::DashboardMode::updateBackground(double seconds)
{


  auto r = renderManager();

    //r->sprite(glm::vec2(0.0f, 0.0f), targetSpace({1920.f, 1080.f}), m_backgroundBlurred);
}


