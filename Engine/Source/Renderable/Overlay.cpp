#include "Vhagar.hpp"

using namespace vh;

void Overlay::setTexture(SDL_Surface *s) {
    surf = s;
    if (isReadyToRender) {
        if (texId > 0) {
            glDeleteTextures(1, &texId);
            texId = 0;
        }
        if (surf != NULL) {
            texId = Utils::bufferTexture2D(surf);
        }
    }
}

void Overlay::beforeRender() {
    if (surf != NULL) {
        texId = Utils::bufferTexture2D(surf);
        vertexBuffer = Utils::bufferData(12, vertices);
    }

    programID = Utils::getShaderProgram("OSD");
    if (programID < 0) {
        LOG(INFO) << "Unable to load program OSD";
        return;
    }

    isReadyToRender = true;
}

void Overlay::afterRender() {
    if (texId > 0) {
        glDeleteTextures(1, &texId);
        texId = 0;
    }
    glDeleteBuffers(1, &vertexBuffer);
    vertexBuffer = 0;
}

void Overlay::setBounds(Rect rect) {
    GLfloat v[] = {
        -1.0f + rect.x,                1.0f - rect.y,
        -1.0f + rect.x + rect.width,   1.0f - rect.y,
        -1.0f + rect.x + rect.width,   1.0f - rect.y - rect.height,
        -1.0f + rect.x + rect.width,   1.0f - rect.y - rect.height,
        -1.0f + rect.x,                1.0f - rect.y - rect.height,
        -1.0f + rect.x,                1.0f - rect.y
    };
    std::copy(v, v + 12, vertices);
    bounds = V4(rect.x, rect.y, rect.width, rect.height);

    if (isReadyToRender) {
        glDeleteBuffers(1, &vertexBuffer);
        vertexBuffer = Utils::bufferData(12, vertices);
    }
}

void Overlay::render(glm::mat4 projection, glm::mat4 view) {
    if (!isReadyToRender || surf == NULL) return;

    glDisable(GL_CULL_FACE);
    glUseProgram(programID);

    Utils::putUniformVec4(programID, "uBounds", bounds);

    glBindTexture(GL_TEXTURE_2D, texId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);

    glEnable(GL_CULL_FACE);
}
