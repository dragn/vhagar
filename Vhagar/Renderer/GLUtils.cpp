#include "GLUtils.hpp"
#include "../Common.hpp"
#include "../Shaders/Shaders.hpp"
#include <string>
#include "SDL_image.h"

namespace Vhagar {

  std::map<std::string, GLuint> shaderPrograms;

  SDL_Surface *loadImage(const std::string &filename) {
    SDL_Surface *tex = IMG_Load(filename.c_str());
    if (tex == NULL) {
      LOG(ERROR) << "Could not load texture " << filename;
    } else {
      LOG(INFO) << "Loaded texture " << filename << " (" << tex->w << ", " << tex->h << ")";
    }
    return tex;
  }

  void compileShader(GLuint shaderID, const std::string &filename) {
    char buffer[256];
    int infoLogLength;

    std::string glslVersion;
    int majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

    if (majorVersion > 3 || (majorVersion == 3 && minorVersion > 2)) {
      glslVersion = "#version 330\n";
    } else {
      glslVersion = "#version 300 es";
    }

    // ???
    LOG(INFO) << "Compiling " << filename;

    std::string code_string = glslVersion + Vhagar::Shaders.at(filename);
    const char* code = code_string.c_str();

    glShaderSource(shaderID, 1, &code, NULL);
    glCompileShader(shaderID);

    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    glGetShaderInfoLog(shaderID, 256, &infoLogLength, buffer);
    if (buffer[0]) LOG(FATAL) << "Error compiling shader:\n" << buffer;
  }

  GLuint compileProgram(const std::string &vertexShader, const std::string &fragmentShader) {
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
    if (status != GL_TRUE) {
      return 0;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
  }

  GLuint bufferData(GLsizeiptr size, const GLfloat *data) {
    LOG(INFO) << "Allocating buffer of size: " << size;
    GLuint id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), data, GL_STATIC_DRAW);
    return id;
  }

  GLuint bufferElementArray(GLsizeiptr size, const GLuint *data) {
    LOG(INFO) << "Allocating buffer of size: " << size;
    GLuint id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), data, GL_STATIC_DRAW);
    return id;
  }

  GLuint bufferTexture2D(SDL_Surface *surf) {
    if (surf == NULL) return -1;

    SDL_PixelFormat *format = surf->format;

    GLenum layout = format->Amask ? GL_RGBA : GL_RGB;
    LOG(INFO) << "Buffering texture of size " << surf->w << "x" << surf->h;
    LOG(INFO) << "Layout is " << (layout == GL_RGBA ? "GL_RGBA" : "GL_RGB");
    LOG(INFO) << "Bits per pixel: " << (int) (format->BitsPerPixel);

    Uint8 *data = (Uint8*) surf->pixels;
    bool allocated = false;

    if (format->palette) {
      // convert to rgb mode
      size_t size = surf->w * surf->h;
      Uint8 *pixels = (Uint8*) surf->pixels;
      data = new Uint8[size * 3];
      allocated = true;
      layout = GL_RGB;
      SDL_Color *colors = format->palette->colors;

      for (size_t i = 0; i < size; i++) {
        Uint8 index = pixels[i];
        if (index >= 0 && index < format->palette->ncolors) {
          SDL_Color color = colors[index];
          Uint8 *p = &data[i * 3];
          *(p) = color.r;
          *(p + 1) = color.g;
          *(p + 2) = color.b;
        }
      }
    }

    GLuint texId;

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surf->w, surf->h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    if (allocated) {
      delete [] data;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return texId;
  }

  GLuint loadCubeMapTexture(const CubeMap &skyBox) {
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

    for (size_t i = 0; i < 6; i++) {
      SDL_Surface *tex = loadImage(files[i]);
      if (tex != NULL) {
        glTexImage2D(modes[i], 0, GL_RGB, tex->h, tex->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);
        SDL_FreeSurface(tex);
      }

    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texId;
  }

  GLuint loadTexture(const std::string &filename) {
    SDL_Surface *tex = SDL_LoadBMP(filename.c_str());

    if (tex == NULL) {
      LOG(ERROR) << "Could not load texture " << filename;
      return -1;
    } else {
      LOG(INFO) << "Loaded texture " << filename << " (" << tex->w << ", " << tex->h << ")";
    }

    GLuint texId = bufferTexture2D(tex);
    SDL_FreeSurface(tex);

    return texId;
  }

  void putUniformMat4(GLuint programID, const std::string &name, M4 &data) {
    GLuint id = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix4fv(id, 1, GL_FALSE, &data[0][0]);
  }

  void putUniformVec3(GLuint programID, const std::string &name, V3 &data) {
    GLuint id = glGetUniformLocation(programID, name.c_str());
    glUniform3fv(id, 1, &data[0]);
  }

  void putUniformFloat(GLuint programID, const std::string &name, float data) {
    GLuint id = glGetUniformLocation(programID, name.c_str());
    glUniform1f(id, data);
  }

  GLuint getShaderProgram(const std::string &key) {
    GLuint programID;

    if (shaderPrograms.count(key)) {
      return shaderPrograms[key];
    } else {
      programID = compileProgram(key + "_VS.glsl", key + "_FS.glsl");
      shaderPrograms.insert(std::pair<std::string, GLuint>(key, programID));
      return programID;
    }
  }
}
