#include "WorldScene.h"

WorldScene::WorldScene()
{
    mRootNode = NULL;
    mWorld = NULL;
}

WorldScene::~WorldScene()
{

}

void WorldScene::setWorld(dart::simulation::World* _world)
{
    mWorld = _world;
}

osg::Node*WorldScene::getRootNode() const
{
    return mRootNode;
}
