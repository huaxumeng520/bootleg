
#include "WindowManager.hpp"
#include "Authorizer.hpp"

#include <miral/runner.h>
#include <miral/display_configuration.h>
#include <miral/external_client.h>
#include <miral/command_line_option.h>
#include <miral/keymap.h>
#include <miral/set_window_management_policy.h>
#include <miral/internal_client.h>


int main(int argc, char const* argv[])
{
    using namespace miral;

    CommandLineOption startup_only{
        [&](bool startup_only) {bootleg::Authorizer::startup_only = startup_only; },
        "bootleg-startup-apps-only",
        "Only allow applications to connect during startup",
        bootleg::Authorizer::startup_only};

    ExternalClientLauncher launcher;

    auto run_startup_apps = [&](std::string const& apps)
    {
      for (auto i = begin(apps); i != end(apps); )
      {
          auto const j = find(i, end(apps), ':');
          launcher.launch(std::vector<std::string>{std::string{i, j}});
          if ((i = j) != end(apps)) ++i;
      }
    };

    MirRunner runner{argc, argv};

    DisplayConfiguration display_config{runner};
 
    return runner.run_with(
        {
            display_config,
            display_config.layout_option(),
            set_window_management_policy<bootleg::WindowManagerPolicy>(),
            SetApplicationAuthorizer<bootleg::Authorizer>{},
            Keymap{},
            startup_only,
            launcher,
            CommandLineOption{run_startup_apps, "startup-apps", "Colon separated list of startup apps", ""}
        });
}
