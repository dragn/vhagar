#include "Mesh.hpp"
#include "../Common.hpp"
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <limits>
#include <glm/gtx/string_cast.hpp>
#include <map>
#include <vector>

const size_t MAX_LEN = 512;

// Check that objFile is a valid Wavefront .obj file
bool checkFilename(const char *objFilename) {
  size_t len = strlen(objFilename);
  if (len > MAX_LEN) {
    LOG(ERROR) << "Filename too long: " << objFilename;
    return false;
  }
  return strncmp(objFilename + (len - 4), ".obj", 4) == 0;
}

// Get .mtl objFile for specified .obj file
void getMtlFilename(char *mtlFilename, const char *objFilename) {
  size_t len = strlen(objFilename);
  strncpy(mtlFilename, objFilename, MAX_LEN);
  strcpy(mtlFilename + (len - 4), ".mtl");
}

void readMaterials(FILE *mtlFile,
    std::map<std::string, V3> &aColorMap,
    std::map<std::string, V3> &dColorMap,
    std::map<std::string, V3> &sColorMap) {
  char line[MAX_LEN], material[MAX_LEN];
  V3 vec3;

  while (fgets(line, MAX_LEN, mtlFile) != NULL) {
    sscanf(line, "newmtl %s", material);
    if (sscanf(line, "Ka %f %f %f", &vec3.x, &vec3.y, &vec3.z)) {
      aColorMap.insert(std::pair<std::string, V3>(std::string(material), vec3));
      LOG(INFO) << material << ": " << glm::to_string(vec3); 
    }
    if (sscanf(line, "Kd %f %f %f", &vec3.x, &vec3.y, &vec3.z)) {
      dColorMap.insert(std::pair<std::string, V3>(std::string(material), vec3));
      LOG(INFO) << material << ": " << glm::to_string(vec3); 
    }
    if (sscanf(line, "Ks %f %f %f", &vec3.x, &vec3.y, &vec3.z)) {
      sColorMap.insert(std::pair<std::string, V3>(std::string(material), vec3));
      LOG(INFO) << material << ": " << glm::to_string(vec3); 
    }
  } 
}

/**
 * Constructor
 **/
Vhagar::ObjMesh::ObjMesh(const char *objFilename) {

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

  // Try to open material library for this object
  char mtlFilename[MAX_LEN];
  getMtlFilename(mtlFilename, objFilename);

  FILE *mtlFile = fopen(mtlFilename, "r");

  if (mtlFile != NULL) {
    LOG(INFO) << "Found material library file: " << mtlFilename;
  } else {
    LOG(INFO) << "No material library found at " << mtlFilename;
  }

  std::vector<V3> vertices, normals, uvs, aColor, dColor, sColor;

  char line[MAX_LEN] = {0}, temp[MAX_LEN] = "";
  std::string mtl = "";
  char s1[32], s2[32], s3[32];
  V3 vec3;
  std::map<std::string, size_t> map;
  std::vector<std::string> values;
  std::vector<size_t> indices;

  std::map<std::string, size_t> materials;
  std::map<std::string, V3> mtlAColor, mtlDColor, mtlSColor;

  if (mtlFile != NULL) {
    readMaterials(mtlFile, mtlAColor, mtlDColor, mtlSColor);
  }

  while (!feof(objFile)) {

    // read next line
    if (fgets(line, MAX_LEN, objFile) == NULL) break;

    if (sscanf(line, "usemtl %s", temp)) {
      mtl = temp;
      LOG(INFO) << "Using material: " << mtl;
      map.clear(); // clear the map, so vertices with different material won't get the same index
    }

    if (sscanf(line, "v %f %f %f", &vec3.x, &vec3.y, &vec3.z)) {
      vertices.push_back(vec3);

    } else if (sscanf(line, "vt %f %f %f", &vec3.x, &vec3.y, &vec3.z)) {
      uvs.push_back(vec3);

    } else if (sscanf(line, "vn %f %f %f", &vec3.x, &vec3.y, &vec3.z)) {
      normals.push_back(vec3);

    } else if (sscanf(line, "f %s %s %s", s1, s2, s3)) {

      // finding vertices with identical values for position/normals/uv (and material)
      if (map.count(std::string(s1))) {
        indices.push_back(map[s1]);
      } else {
        values.push_back(std::string(s1));
        if (!mtl.empty()) {
          aColor.push_back(mtlAColor[mtl]);
          dColor.push_back(mtlDColor[mtl]);
          sColor.push_back(mtlSColor[mtl]);
        }
        map.insert(std::pair<std::string, size_t>(s1, values.size() - 1));
        indices.push_back(values.size() - 1);
      }
      if (map.count(std::string(s2))) {
        indices.push_back(map[s2]);
      } else {
        values.push_back(std::string(s2));
        if (!mtl.empty()) {
          aColor.push_back(mtlAColor[mtl]);
          dColor.push_back(mtlDColor[mtl]);
          sColor.push_back(mtlSColor[mtl]);
        }
        map.insert(std::pair<std::string, size_t>(s2, values.size() - 1));
        indices.push_back(values.size() - 1);
      }
      if (map.count(std::string(s3))) {
        indices.push_back(map[s3]);
      } else {
        values.push_back(std::string(s3));
        if (!mtl.empty()) {
          aColor.push_back(mtlAColor[mtl]);
          dColor.push_back(mtlDColor[mtl]);
          sColor.push_back(mtlSColor[mtl]);
        }
        map.insert(std::pair<std::string, size_t>(s3, values.size() - 1));
        indices.push_back(values.size() - 1);
      }
    }
  }

  //  LOG(INFO) << "aColor size: " << aColor.size();
  //  LOG(INFO) << "dColor size: " << dColor.size();
  //  LOG(INFO) << "sColor size: " << sColor.size();

  if (mtlFile != NULL) fclose(mtlFile);
  fclose(objFile);

  size_t size = values.size() * 3;

  LOG(INFO) << "data size: " << size;

  _attribSize = size;
  _vertexData.reset(new GLfloat[size]);
  _normalData.reset(new GLfloat[size]);
  _aColorData.reset(new GLfloat[size]);
  _dColorData.reset(new GLfloat[size]);
  _sColorData.reset(new GLfloat[size]);

  _indexSize = indices.size();
  _indexData.reset(new GLuint[indices.size()]);

  unsigned int v, vt, vn;
  std::string str;

  // Fill vertex data and normal data
  for (size_t i = 0; i < values.size(); i++) {
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

    if (i < aColor.size()) {
      _aColorData[3*i]     = aColor[i].x;
      _aColorData[3*i + 1] = aColor[i].y;
      _aColorData[3*i + 2] = aColor[i].z;
    }

    if (i < dColor.size()) {
      _dColorData[3*i]     = dColor[i].x;
      _dColorData[3*i + 1] = dColor[i].y;
      _dColorData[3*i + 2] = dColor[i].z;
    }

    if (i < sColor.size()) {
      _sColorData[3*i]     = sColor[i].x;
      _sColorData[3*i + 1] = sColor[i].y;
      _sColorData[3*i + 2] = sColor[i].z;
    }
  }

  for (size_t i = 0; i < indices.size(); i++) {
    _indexData[i] = indices[i];
  }

  LOG(INFO) << "v: " << vertices.size();
  LOG(INFO) << "vn: " << normals.size();
  LOG(INFO) << "vt: " << uvs.size();
  LOG(INFO) << "values: " << values.size();
}
