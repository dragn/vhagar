#pragma once

#include "../Common.hpp"
#include "../RenderingObject/RenderingObject.hpp"

namespace Vhagar {

  class UITexture : public RenderingObject {

    public:

      void setBounds(Rect rect);

      void setTexture(SDL_Surface *surf);

      /* RenderingObject methods */
      virtual void beforeRender();
      virtual void afterRender();
      virtual void render(glm::mat4 projection, glm::mat4 view);
      virtual void render(glm::mat4 projection, glm::mat4 view, LightSource lightSource) {}; // empty
      /* end of RenderingObject methods */

    private:

      GLuint vertexBuffer = -1;
      GLuint programID;

      GLfloat vertices[18];
      V4 bounds;
      SDL_Surface *surf = NULL;
      GLuint texId = -1;
  };

}
