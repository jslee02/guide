#ifndef GUIDE_WORLDSCENE_H
#define GUIDE_WORLDSCENE_H

#include <dart/dart.hpp>

///
class WorldScene
{
public:
  ///
  WorldScene();

  ///
  ~WorldScene();

  ///
  void setWorld(dart::simulation::World* _world);

protected:
  ///
  dart::simulation::World* mWorld;

private:

};

#endif
