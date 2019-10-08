#include "Vhagar_PCH.hpp"

#include "GLUtils.hpp"

#include "Shaders/Shaders.hpp"
#include <string>
#include <exception>
#include "SDL_image.h"
#include "Logging.hpp"

namespace vh
{

namespace Utils
{

std::unordered_map<std::string, GLuint> shaderPrograms;

SDL_Surface *LoadImage(const std::string &filename)
{
    SDL_Surface *tex = IMG_Load(filename.c_str());
    if (tex == NULL)
    {
        LOG(ERROR) << "Could not load texture " << filename << ": " << IMG_GetError();
    }
    else
    {
        LOG(INFO) << "Loaded texture " << filename << " (" << tex->w << ", " << tex->h << ")";
    }
    return tex;
}

void compileShader(GLuint shaderID, const std::string &filename)
{
    char buffer[256];
    int infoLogLength;

    std::string glslVersion;
    int majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

    if (majorVersion > 3 || (majorVersion == 3 && minorVersion > 2))
    {
        glslVersion = "#version 330\n";
    }
    else
    {
        glslVersion = "#version 300 es";
    }

    // ???
    LOG(INFO) << "Compiling " << filename;

    std::string code_string = glslVersion + vh::Shaders.at(filename);
    const char* code = code_string.c_str();

    glShaderSource(shaderID, 1, &code, NULL);
    glCompileShader(shaderID);

    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    glGetShaderInfoLog(shaderID, 256, &infoLogLength, buffer);
    if (buffer[0]) LOG(FATAL) << "Error compiling shader:\n" << buffer;
}

GLuint compileProgram(const std::string &vertexShader, const std::string &fragmentShader)
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    compileShader(vertexShaderID, vertexShader);
    compileShader(fragmentShaderID, fragmentShader);

    LOG(INFO) << "Linking program";
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    char buffer[256];
    int infoLogLength;

    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    glGetProgramInfoLog(programID, 256, &infoLogLength, buffer);
    if (buffer[0]) LOG(FATAL) << buffer;

    GLint status;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        return 0;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

GLuint BufferData(GLsizeiptr size, const GLfloat *data)
{
    GLuint id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), data, GL_STATIC_DRAW);
    return id;
}

GLuint BufferElementArray(GLsizeiptr size, const GLuint *data)
{
    GLuint id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), data, GL_STATIC_DRAW);
    return id;
}

GLuint BufferTexture2D(SDL_Surface *surf)
{
    uint32_t* data = ConvertToRGBA(surf);

    GLuint texId;

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    delete[] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return texId;
}

uint32_t* ConvertToRGBA(SDL_Surface* surf)
{
    if (surf == nullptr) return nullptr;

    SDL_PixelFormat *format = surf->format;

    size_t size = surf->w * surf->h;
    uint32_t *data = new uint32_t[size];

    if (format->palette)
    {
        SDL_Color *colors = format->palette->colors;
        Uint8 *pixels = (Uint8*) surf->pixels;

        for (size_t i = 0; i < size; i++)
        {
            Uint8 &index = pixels[i];
            if (index >= 0 && index < format->palette->ncolors)
            {
                SDL_Color &color = colors[index];
                uint8_t* p = reinterpret_cast<uint8_t*>(&data[i]);
                p[0] = color.r;
                p[1] = color.g;
                p[2] = color.b;
                p[3] = 0xff;
            }
        }
    }
    else
    {
        if (format->BitsPerPixel == 32)
        {
            Uint32 *pixels = (Uint32*) surf->pixels;
            for (size_t i = 0; i < size; i++)
            {
                uint8_t *ptr = reinterpret_cast<uint8_t*>(&data[i]);
                SDL_GetRGBA(pixels[i], format, &ptr[0], &ptr[1], &ptr[2], &ptr[3]);
            }
        }
        else if (format->BitsPerPixel == 24)
        {
            Uint8 *pixels = (Uint8*) surf->pixels;
            for (size_t i = 0; i < size; i++)
            {
                uint8_t *ptr = reinterpret_cast<uint8_t*>(&data[i]);
                ptr[0] = pixels[i * 3];
                ptr[1] = pixels[i * 3 + 1];
                ptr[2] = pixels[i * 3 + 2];
                ptr[3] = 0xff;
            }
        }
        else
        {
            LOG(FATAL) << "Unsupported texture BitPerPixel: " << format->BitsPerPixel;
        }
    }

    return data;
}

GLuint LoadCubeMapTexture(const CubeMap &skyBox)
{
    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

    GLuint modes[] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

    const std::string files[] = { skyBox.pos_x, skyBox.neg_x, skyBox.pos_y, skyBox.neg_y, skyBox.pos_z, skyBox.neg_z };

    for (size_t i = 0; i < 6; i++)
    {
        SDL_Surface *tex = LoadImage(files[i]);
        if (tex != NULL)
        {
            uint32_t* data = ConvertToRGBA(tex);
            glTexImage2D(modes[i], 0, GL_RGBA, tex->h, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            delete[] data;

            int error = glGetError();
            if (error) ReportGLError(error);

            SDL_FreeSurface(tex);
        }
        else
        {
            LOG(WARNING) << "Could not load texture: " << files[i];
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texId;
}

void PutUniformMat4(GLuint programID, const std::string &name, const M4 &data)
{
    GLuint id = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix4fv(id, 1, GL_FALSE, &data[0][0]);
}

void PutUniformVec3(GLuint programID, const std::string &name, const V3 &data)
{
    GLuint id = glGetUniformLocation(programID, name.c_str());
    glUniform3fv(id, 1, &data[0]);
}

void PutUniformVec4(GLuint programID, const std::string& name, const V4& data)
{
    GLuint id = glGetUniformLocation(programID, name.c_str());
    glUniform4fv(id, 1, &data[0]);
}

void PutUniformFloat(GLuint programID, const std::string& name, float data)
{
    GLuint id = glGetUniformLocation(programID, name.c_str());
    glUniform1f(id, data);
}

GLuint GetShaderProgram(const std::string &key)
{
    GLuint programID;

    if (shaderPrograms.count(key))
    {
        return shaderPrograms[key];
    }
    else
    {
        programID = compileProgram(key + "_VS.glsl", key + "_FS.glsl");
        shaderPrograms.insert(std::pair<std::string, GLuint>(key, programID));
        return programID;
    }
}

void ReportGLError(int error)
{
    switch (error)
    {
    case GL_NO_ERROR:
        return;
    case GL_INVALID_OPERATION:
        LOG(ERROR) << "GL ERROR: INVALID_OPERATION";
        return;
    case GL_INVALID_VALUE:
        LOG(ERROR) << "GL ERROR: INVALID_VALUE";
        return;
    default:
        LOG(ERROR) << "GL ERROR: " << error;
    }
}

}
}
