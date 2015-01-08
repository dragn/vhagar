#include "SpaceScene.hpp"

#include "../drawable/Wavefront.hpp"

SpaceScene::SpaceScene() {

  Wavefront *ship = new Wavefront("models/SimpleShip/Spaceship.obj");
  //Wavefront *ship = new Wavefront("/home/dragn/SportSpaceship/SportsSpaceship.obj");
  ship->pos(V3(0, 0, 0));

  addNamedObject("PlayerShip", ship);

  //  Drawable *arrow = new Wavefront("models/arrow.obj");
  //arrow->scale(V3(0.5f));
  //  addNamedObject("Arrow", arrow);
  Wavefront *box = new Wavefront("models/box/box.obj");
  box->pos(V3(10, 0, -40));
  box->scale(V3(3.0f));
  addObject(box);

  addObject(new Object());
}
