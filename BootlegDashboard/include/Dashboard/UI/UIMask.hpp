
#pragma once

#include <Dashboard/UI/UIOrigin.hpp>

#include <KIT/Assets/Texture.hpp>

#include <WIR/Math.hpp>



namespace bootleg
{

  class UIMask 
  {
  public:
    UIMask(kit::Engine *engine, glm::vec2 position, glm::vec2 size, kit::TexturePtr texture);
    virtual ~UIMask();

    
    void origin(UIOrigin newValue)
    {
      m_origin = newValue;
    }

    void position(glm::vec2 const &newValue)
    {
      m_position = newValue;
    }

    void size(glm::vec2 const &newValue)
    {
      m_size = newValue;
    }

    void texture(kit::TexturePtr newValue)
    {
      m_texture = newValue;
    }

    
    UIOrigin origin() const
    {
      return m_origin;
    }

    glm::vec2 const &position() const
    {
      return m_position;
    }

    glm::vec2 const &size() const
    {
      return m_size;
    }

    kit::TexturePtr texture() const
    {
      return m_texture;
    }

  protected:
    kit::Engine *m_engine = nullptr;

    UIOrigin m_origin = O_TopLeft;

    kit::TexturePtr m_texture;
    glm::vec2 m_position;
    glm::vec2 m_size;
  };

} // namespace bootleg