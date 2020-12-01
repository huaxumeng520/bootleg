
#include "Dashboard/UI/UIMask.hpp"

bootleg::UIMask::UIMask(kit::Engine *engine, glm::vec2 position, glm::vec2 size, kit::TexturePtr texture)
    : m_engine(engine), m_position(position), m_size(size), m_texture(texture)
{
}

bootleg::UIMask::~UIMask()
{
}
