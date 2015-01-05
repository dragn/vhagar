#pragma once

#include "Drawable.hpp"

class Wavefront : public Drawable {
  public:
    Wavefront(const char *filename);

    bool isOk() {
      return _isOk;
    }

  private:
    bool _isOk = false;
};
