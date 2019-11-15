#pragma once

namespace vh
{

    struct GLBuffers
    {
        /**
         * The number of vertex attribute arrays
         */
        GLuint attribCount = 0;

        /**
         * Vertex attributes buffer id and size
         */
        GLuint attribBuffer = 0;
        GLsizei attribBufferSize = 0;

        /**
         * Index array buffer id and size
         */
        GLuint indexBuffer = 0;
        GLsizei indexBufferSize = 0;

        /**
         *
         */
        GLuint texture = 0;
    };

} // namespace vh
