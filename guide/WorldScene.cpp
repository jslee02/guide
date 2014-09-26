#include "WorldScene.h"

//------------------------------------------------------------------------------
WorldScene::WorldScene()
{
  mWorld = NULL;
}

//------------------------------------------------------------------------------
WorldScene::~WorldScene()
{

}

//------------------------------------------------------------------------------
void WorldScene::setWorld(dart::simulation::World* _world)
{
  mWorld = _world;
}

