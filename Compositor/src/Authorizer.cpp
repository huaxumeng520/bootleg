
#include "Authorizer.hpp"


std::atomic<bool> bootleg::Authorizer::startup_only{false};

bootleg::Authorizer::Authorizer()
{
    
}

bool bootleg::Authorizer::connection_is_allowed(miral::ApplicationCredentials const& creds) 
{
    // Allow internal applications and (optionally) only ones that start "immediately"
    // (For the sake of an example "immediately" means while the spash is running)
    return getpid() == creds.pid() || !startup_only;
}

bool bootleg::Authorizer::configure_display_is_allowed(miral::ApplicationCredentials const& /*creds*/) 
{
    return false;
}

bool bootleg::Authorizer::set_base_display_configuration_is_allowed(miral::ApplicationCredentials const& /*creds*/) 
{
    return false;
}

bool bootleg::Authorizer::screencast_is_allowed(miral::ApplicationCredentials const& /*creds*/) 
{
    return true;
}

 bool bootleg::Authorizer::prompt_session_is_allowed(miral::ApplicationCredentials const& /*creds*/) 
{
    return false;
}

bool bootleg::Authorizer::configure_input_is_allowed(miral::ApplicationCredentials const& /*creds*/) 
{
    return false;
}

bool bootleg::Authorizer::set_base_input_configuration_is_allowed(miral::ApplicationCredentials const& /*creds*/) 
{
    return false;
}


