#include "SpaceScene.hpp"

#include "../drawable/Wavefront.hpp"

SpaceScene::SpaceScene() {

  skyBox.pos_x = "images/space/pink_planet_neg_x.tga";
  skyBox.neg_x = "images/space/pink_planet_pos_x.tga";
  skyBox.pos_y = "images/space/pink_planet_pos_y.tga";
  skyBox.neg_y = "images/space/pink_planet_neg_y.tga";
  skyBox.pos_z = "images/space/pink_planet_pos_z.tga";
  skyBox.neg_z = "images/space/pink_planet_neg_z.tga";

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

  addNamedObject("Asteroid", new Wavefront("models/Asteroid.obj"));
  getNamedObject("Asteroid")->pos(V3(0, -40, 0));
  getNamedObject("Asteroid")->scale(V3(15.f));
}
