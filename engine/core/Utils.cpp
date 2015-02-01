#include "Utils.hpp"

namespace Utils {

  M3 rotToM3(const Rot &rot) {
    float a = rot.yaw;
    float b = rot.pitch;
    float c = rot.roll;
    M3 yaw = M3(cos(a), 0, sin(a), 0, 1, 0, - sin(a) , 0, cos(a));
    M3 pitch = M3(1, 0, 0, 0, cos(b), - sin(b), 0, sin(b), cos(b));
    M3 roll = M3(cos(c), - sin(c), 0, sin(c), cos(c), 0, 0, 0, 1);

    return yaw * pitch * roll;
  }

  M4 rotToM4(const Rot &rot) {
    float a = rot.yaw;
    float b = rot.pitch;
    float c = rot.roll;
    M4 yaw   = M4(cos(a), 0, sin(a), 0, 0, 1, 0, 0, - sin(a) , 0, cos(a), 0, 0, 0, 0, 1);
    M4 pitch = M4(1, 0, 0, 0, 0, cos(b), - sin(b), 0, 0, sin(b), cos(b), 0, 0, 0, 0, 1);
    M4 roll  = M4(cos(c), - sin(c), 0, 0, sin(c), cos(c), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

    return yaw * pitch * roll;
  }
}
