#pragma once

#include "../Common.hpp"
#include "../RenderingObject/RenderingObject.hpp"
#include <forward_list>

namespace Vhagar {

  /**
   * Renderer options
   */
  struct RendererOptions {
    const char *resourceRoot = ".";
    size_t screenWidth = 1024;
    size_t screenHeight = 768;
  };

  /**
   * The Renderer
   **/
  class Renderer {

    public:

      Renderer(RendererOptions opts) : opts(opts) {}

      /**
       * Initialize the renderer. Should be called first before any other calls.
       */
      bool init();

      /**
       * Add the object to the rendered world.
       */
      void addObject(RenderingObject *object);

      /**
       * Remove the object from rendering
       */
      void removeObject(RenderingObject *object);

      /**
       * Renders all object using specified View matrix. (call from main loop)
       */
      void render();

      /**
       * Sets up a projection matrix
       */
      void setProjection(glm::mat4 _projection) { projection = _projection; };

      /**
       * Sets up view matrix (camera)
       */
      void setView(glm::mat4 _view) { view = _view; };

    private:
      
      RendererOptions opts;

      std::forward_list<RenderingObject*> objects;
     
      glm::mat4 projection;
      glm::mat4 view;

      bool isReady;
      bool checkState();

      /**
       * Override to provide custom actions before Rendering phase
       */
      virtual void beforeRender() {};

      /**
       * Override to provide custom actions after Rendering phase
       */
      virtual void afterRender() {};
  };

}
