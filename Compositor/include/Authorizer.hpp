
#pragma once

#include <miral/application_authorizer.h>

#include <unistd.h>
#include <atomic>

namespace bootleg
{
    class Authorizer : public miral::ApplicationAuthorizer
    {
    public:
        Authorizer();

        virtual bool connection_is_allowed(miral::ApplicationCredentials const& creds) override;

        virtual bool configure_display_is_allowed(miral::ApplicationCredentials const& /*creds*/) override;

        virtual bool set_base_display_configuration_is_allowed(miral::ApplicationCredentials const& /*creds*/) override;

        virtual bool screencast_is_allowed(miral::ApplicationCredentials const& /*creds*/) override;

        virtual bool prompt_session_is_allowed(miral::ApplicationCredentials const& /*creds*/) override;

        bool configure_input_is_allowed(miral::ApplicationCredentials const& /*creds*/) override;

        bool set_base_input_configuration_is_allowed(miral::ApplicationCredentials const& /*creds*/) override;

        static std::atomic<bool> startup_only;
    };
}

