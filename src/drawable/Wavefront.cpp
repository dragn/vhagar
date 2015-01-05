#include "Wavefront.hpp"
#include "../core/Common.hpp"
#include <cstdio>
#include <iostream>
#include <string>
#include <limits>

Wavefront::Wavefront(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    LOG(ERROR) << "Can't open file: " << filename;
    return;
  }
  _isOk = true;

  std::vector<V3> vertices;
  std::vector<V3> normals;
  std::vector<V3> uvs;

  std::vector<unsigned int> vertexIndices;
  std::vector<unsigned int> normalIndices;
  std::vector<unsigned int> uvIndices;

  const int MAX_LEN = 512;
  char line[MAX_LEN] = {0};
  V3 vec3;
  unsigned int index;
  unsigned int v1, v2, v3, t1, t2, t3, n1, n2, n3;

  while (!feof(file)) {
    if (fgets(line, MAX_LEN, file) == NULL) break;
    if (line[0] == 'v' && line[1] == ' ') {
      sscanf(line, "v %f %f %f", &vec3.x, &vec3.y, &vec3.z);
      vertices.push_back(vec3);
    } else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
      sscanf(line, "vt %f %f %f", &vec3.x, &vec3.y, &vec3.z);
      uvs.push_back(vec3);
    } else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
      sscanf(line, "vn %f %f %f", &vec3.x, &vec3.y, &vec3.z);
      normals.push_back(vec3);
    } else if (line[0] == 'f' && line[1] == ' ') {
      if (strstr(line, "//")) {
        sscanf(line, "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
        t1 = 0;
        t2 = 0;
        t3 = 0;
      } else {
        sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
      }
      vertexIndices.push_back(v1 - 1);
      vertexIndices.push_back(v2 - 1);
      vertexIndices.push_back(v3 - 1);
      normalIndices.push_back(n1 - 1);
      normalIndices.push_back(n2 - 1);
      normalIndices.push_back(n3 - 1);
      uvIndices.push_back(t1 - 1);
      uvIndices.push_back(t2 - 1);
      uvIndices.push_back(t3 - 1);
    }
  }

  fclose(file);

  unsigned int size = vertexIndices.size() * 3;

  _vertexData.reset(new GLfloat[size]);
  _vertexDataSize = size;
  _colorData.reset(new GLfloat[size]);
  _colorDataSize = size;
  _normalDataSize = size;
  _normalData.reset(new GLfloat[size]);

  for (unsigned int i = 0; i < vertexIndices.size(); i++) {

    index = vertexIndices[i];

    _vertexData[i * 3] = vertices[index].x;
    _vertexData[i * 3 + 1] = vertices[index].y;
    _vertexData[i * 3 + 2] = vertices[index].z;

    index = normalIndices[i];

    _normalData[i * 3] = normals[index].x;
    _normalData[i * 3 + 1] = normals[index].y;
    _normalData[i * 3 + 2] = normals[index].z;

    _colorData[i * 3] = 0.5;
    _colorData[i * 3 + 1] = 0.5;
    _colorData[i * 3 + 2] = 0.5;
  }

  LOG(INFO) << "Reading file " << filename;
  LOG(INFO) << "v: " << vertices.size();
  LOG(INFO) << "vn: " << normals.size();
  LOG(INFO) << "vt: " << uvs.size();
  LOG(INFO) << "Indices: " << vertexIndices.size() << ", " << normalIndices.size() << ", " << uvIndices.size();

}
