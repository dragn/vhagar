#include "SpaceScene.hpp"

#include "../drawable/Wavefront.hpp"

SpaceScene::SpaceScene() {

  //Wavefront *ship = new Wavefront("models/SimpleShip/Spaceship.obj");
  Wavefront *ship = new Wavefront("/home/dragn/SportSpaceship/SportsSpaceship.obj");
  ship->pos(V3(0, 0, 0));

  if (ship->isOk()) addNamedObject("PlayerShip", ship);

  //  Drawable *arrow = new Wavefront("models/arrow.obj");
  //arrow->scale(V3(0.5f));
  //  addNamedObject("Arrow", arrow);
}
