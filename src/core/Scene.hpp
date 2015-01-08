#pragma once

#include "Controllable.hpp"
#include <map>

class Scene {

  public:
    const std::vector<sptr<Object>> *objects() { return &_objects; };

    void addObject(Object *object) {
      _objects.push_back(sptr<Object>(object)); 
    };

    void addNamedObject(std::string name, Object *object) {
      sptr<Object> ptr(object);
      _objects.push_back(ptr);
      _namedObjects.insert(std::pair<std::string, sptr<Object>>(name, ptr));
    }

    Object *getNamedObject(std::string name) {
      return _namedObjects.at(name).get();
    }

  protected:
    std::vector<sptr<Object>> _objects;
    std::map<std::string, sptr<Object>> _namedObjects;
};
