#include "SpaceScene.hpp"

SpaceScene::SpaceScene() {

  uptr<Drawable> box(new Box());

  box->pos(V3(0.2, 0, 2));

  box->scale(V3(0.5f));

  _objects.push_back(std::move(box));
}
