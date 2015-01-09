#include "Object.hpp"
#include <cmath>

Object::Object() {
  recalcModel();
}

/* Position setter */
void Object::pos(V3 pos) {
  _pos = pos;
  recalcModel(); 
};

/* Position getter */
const V3 Object::pos() { return _pos; };

/* Scale setter */
void Object::scale(V3 scale) {
  _scale = scale;
  recalcModel(); 
};

/* Scale getter */
V3 Object::scale() { return _scale; };

V3 Object::forward() { return V3(_rot * glm::vec4(0, 0, -1, 0)); };
V3 Object::up() { return V3(_rot * glm::vec4(0, 1, 0, 0)); };

void Object::orient(Rot rot) {
  _yaw = rot.yaw;
  while (_yaw > 2 * M_PI) _yaw -= 2 * M_PI;
  while (_yaw < 0) _yaw += 2 * M_PI;
  _pitch = rot.pitch;
  if (_pitch > 1.57) _pitch = 1.57;
  if (_pitch < - 1.57) _pitch = - 1.57;
  recalcModel(); 
}

Rot Object::rot() {
  return Rot(_yaw, _pitch, 0);
}

void Object::rotate(Rot rot) {
  _yaw += rot.yaw;
  while (_yaw > 2 * M_PI) _yaw -= 2 * M_PI;
  while (_yaw < 0) _yaw += 2 * M_PI;

  _pitch += rot.pitch;
  if (_pitch > 1.57) _pitch = 1.57;
  if (_pitch < - 1.57) _pitch = - 1.57;
  recalcModel(); 
}

void Object::recalcModel() {
  _rot = glm::rotate(glm::rotate(M4(1.f), _yaw, V3(0, 1, 0)), _pitch, V3(1, 0, 0));
  modelMatrix = glm::translate(M4(1.f), _pos) * _rot * glm::scale(M4(1.f), _scale);
  //  modelMatrix = glm::translate(glm::scale(_rot, _scale), _pos);
}
