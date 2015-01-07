#include "Wavefront.hpp"
#include "../core/Common.hpp"
#include <cstdio>
#include <iostream>
#include <string>
#include <limits>
#include <map>

const size_t MAX_LEN = 512;

// Check that objFile is a valid Wavefront .obj file
bool checkFilename(const char *objFilename) {
  size_t len = strlen(objFilename);
  if (len > MAX_LEN) {
    LOG(ERROR) << "Filename too long: " << objFilename;
    return false;
  }
  LOG(INFO) << objFilename + (len - 4);
  return strncmp(objFilename + (len - 4), ".obj", 4) == 0;
}

// Get .mtl objFile for specified .obj file
void getMtlFilename(char *mtlFilename, const char *objFilename) {
  size_t len = strlen(objFilename);
  strncpy(mtlFilename, objFilename, MAX_LEN);
  strcpy(mtlFilename + (len - 4), ".mtl");
}

/**
 * Constructor
 **/
Wavefront::Wavefront(const char *objFilename) {

  // Open objFile specified by 'objFilename' (should and with .obj)
  if (!checkFilename(objFilename)) {
    LOG(ERROR) << "Invalid objFilename (should end with .obj): " << objFilename;
    return;
  }
  FILE *objFile = fopen(objFilename, "r");
  if (objFile == NULL) {
    LOG(ERROR) << "Can't open objFile: " << objFilename;
    return;
  }

  // File opened successfully, go on
  LOG(INFO) << "Reading objFile " << objFilename;
  _isOk = true;

  // Try to open material library for this object
  char mtlFilename[MAX_LEN];
  getMtlFilename(mtlFilename, objFilename);

  FILE *mtlFile = fopen(mtlFilename, "r");
  if (mtlFile != NULL) {
    LOG(INFO) << "Found material library file: " << mtlFilename;
  } else {
    LOG(INFO) << "No material library found at " << mtlFilename;
  }

  std::vector<V3> vertices, normals, uvs;

  char line[MAX_LEN] = {0};
  char s1[32], s2[32], s3[32];
  V3 vec3;
  size_t index;
  size_t v1, v2, v3, t1, t2, t3, n1, n2, n3;
  std::map<std::string, size_t> map;
  std::vector<std::string> values;
  std::vector<size_t> indices;

  std::map<std::string, size_t> materials;
  std::vector<V3> mtlAmbientColor, mtlDiffuseColor, mtlSpecularColor;

  while (!feof(objFile)) {

    // read next line
    if (fgets(line, MAX_LEN, objFile) == NULL) break;

    if (strncmp(line, "v ", 2) == 0) {
      sscanf(line, "v %f %f %f", &vec3.x, &vec3.y, &vec3.z);
      vertices.push_back(vec3);

    } else if (strncmp(line, "vt ", 3) == 0) {
      sscanf(line, "vt %f %f %f", &vec3.x, &vec3.y, &vec3.z);
      uvs.push_back(vec3);

    } else if (strncmp(line, "vn ", 3) == 0) {
      sscanf(line, "vn %f %f %f", &vec3.x, &vec3.y, &vec3.z);
      normals.push_back(vec3);

    } else if (strncmp(line, "f ", 2) == 0) {
      sscanf(line, "f %s %s %s", &s1, &s2, &s3);

      // finding vertices with identical values for position/normals/uv
      if (map.count(std::string(s1))) {
        indices.push_back(map[s1]);
      } else {
        values.push_back(std::string(s1));
        map.insert(std::pair<std::string, size_t>(s1, values.size() - 1));
        indices.push_back(values.size() - 1);
      }
      if (map.count(std::string(s2))) {
        indices.push_back(map[s2]);
      } else {
        values.push_back(std::string(s2));
        map.insert(std::pair<std::string, size_t>(s2, values.size() - 1));
        indices.push_back(values.size() - 1);
      }
      if (map.count(std::string(s3))) {
        indices.push_back(map[s3]);
      } else {
        values.push_back(std::string(s3));
        map.insert(std::pair<std::string, size_t>(s3, values.size() - 1));
        indices.push_back(values.size() - 1);
      }
    }
  }

  if (mtlFile != NULL) fclose(mtlFile);
  fclose(objFile);

  size_t size = values.size() * 3;

  LOG(INFO) << "data size: " << size;

  _vertexDataSize = size;
  _vertexData.reset(new GLfloat[size]);
  _normalDataSize = size;
  _normalData.reset(new GLfloat[size]);

  unsigned int v, vt, vn;
  std::string str;

  // Fill vertex data and normal data
  for (int i = 0; i < values.size(); i++) {
    str = values[i];
    if (str.find("//") != std::string::npos) {
      sscanf(str.c_str(), "%d//%d", &v, &vn);
    } else {
      sscanf(str.c_str(), "%d/%d/%d", &v, &vt, &vn);
    }
    if (v > 0 && v <= vertices.size()) {
      _vertexData[3*i]   = vertices[v - 1].x;
      _vertexData[3*i+1] = vertices[v - 1].y;
      _vertexData[3*i+2] = vertices[v - 1].z;
    }

    if (vn > 0 && vn <= normals.size()) {
      _normalData[3*i]   = normals[vn - 1].x;
      _normalData[3*i+1] = normals[vn - 1].y;
      _normalData[3*i+2] = normals[vn - 1].z;
    }
  }

  _indexDataSize = indices.size();
  _indexData.reset(new GLuint[indices.size()]);

  for (int i = 0; i < indices.size(); i++) {
    _indexData[i] = indices[i];
  }

  LOG(INFO) << "v: " << vertices.size();
  LOG(INFO) << "vn: " << normals.size();
  LOG(INFO) << "vt: " << uvs.size();
  LOG(INFO) << "values: " << values.size();
}
