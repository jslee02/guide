#ifndef GUIDE_WORLDSCENE_H
#define GUIDE_WORLDSCENE_H

#include <dart/simulation/World.h>

#include <osg/Node>

class WorldScene
{
public:
    WorldScene();
    ~WorldScene();

    void setWorld(dart::simulation::World* _world);

    osg::Node* getRootNode() const;

protected:
    dart::simulation::World* mWorld;

    osg::Node* mRootNode;

private:

};

#endif
