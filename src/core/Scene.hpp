#pragma once

#include "../drawable/Drawable.hpp"
#include "Controllable.hpp"
#include <map>

class Scene {

  public:
    const std::vector<sptr<Drawable>> *objects() { return &_objects; };

    void addObject(Drawable *object) {
      _objects.push_back(sptr<Drawable>(object)); 
    };

    void addNamedObject(std::string name, Drawable *object) {
      sptr<Drawable> ptr(object);
      _objects.push_back(ptr);
      _namedObjects.insert(std::pair<std::string, sptr<Object>>(name, ptr));
    }

    Object *getNamedObject(std::string name) {
      return _namedObjects.at(name).get();
    }

  protected:
    std::vector<sptr<Drawable>> _objects;
    std::map<std::string, sptr<Object>> _namedObjects;
};
