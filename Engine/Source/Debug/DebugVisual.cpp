#include "Common.hpp"

#include "DebugVisual.hpp"
#include "Utils/GLUtils.hpp"
#include <glm/gtx/transform.hpp>

static const GLfloat crossData[] = {
    -0.1f, 0.0f, 0.0f,
     0.1f, 0.0f, 0.0f,
     0.0f,-0.1f, 0.0f,
     0.0f, 0.1f, 0.0f,
     0.0f, 0.0f,-0.1f,
     0.0f, 0.0f, 0.1f
};

void vh::DebugVisual::BeforeRender()
{
    CHECK(mGLBuf == 0);

    mProgramId = Utils::GetShaderProgram("Wireframe");

    uidColor = glGetUniformLocation(mProgramId, "uColor");
    uidMVP = glGetUniformLocation(mProgramId, "uMVP");

    glGenBuffers(1, &mGLBuf);
    glBindBuffer(GL_ARRAY_BUFFER, mGLBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crossData), crossData, GL_STATIC_DRAW);
}

void vh::DebugVisual::AfterRender()
{
    CHECK(mGLBuf > 0);

    glDeleteBuffers(1, &mGLBuf);
}

void vh::DebugVisual::Render(glm::mat4 projection, glm::mat4 view, const Renderer* renderer)
{
    glUseProgram(mProgramId);

    V3 color = V3(1.0f, 0.0f, 0.0f);
    glUniform3fv(uidColor, 1, reinterpret_cast<GLfloat*>(&color));

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mGLBuf);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    for (const DebugLabel& label : mLabels)
    {
        M4 MVP = projection * view * glm::translate(M4(1.0f), label.pos);
        glUniformMatrix4fv(uidMVP, 1, GL_FALSE, &MVP[0][0]);
        glDrawArrays(GL_LINES, 0, 6);
    }

    glDisableVertexAttribArray(0);
}
