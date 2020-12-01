
#pragma once

#include <Dashboard/UI/UIOrigin.hpp>

#include <KIT/Engine.hpp>
#include <KIT/Assets/Texture.hpp>

#include <WIR/Math.hpp>


namespace bootleg 
{

  class UIMask;

  class UIBox
  {
  public:
    UIBox(kit::Engine *engine, glm::vec2 position, glm::vec2 size, kit::TexturePtr texture, UIMask *mask = nullptr, glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f});
    UIBox(kit::Engine *engine, glm::vec2 position, glm::vec2 size, glm::vec4 color, UIMask *mask = nullptr);
    virtual ~UIBox();

    void render();


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

    void color(glm::vec4 const &newValue)
    {
      m_color = newValue;
    }

    void uvRect(glm::vec4 const &newValue)
    {
      m_uvRect = newValue;
    }

    void mask(UIMask *newValue)
    {
      m_mask = newValue;
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

    glm::vec4 const &color() const
    {
      return m_color;
    }

    glm::vec4 const &uvRect() const 
    {
      return m_uvRect;
    }

    UIMask *mask() const
    {
      return m_mask;
    }

  protected:
    kit::Engine *m_engine = nullptr;
    
    UIOrigin m_origin = O_TopLeft;
    glm::vec2 m_position;
    glm::vec2 m_size;
    glm::vec4 m_uvRect{0.0f, 0.0f, 1.0f, 1.0f};

    kit::TexturePtr m_texture;
    glm::vec4 m_color;

    UIMask *m_mask = nullptr;
  };

}