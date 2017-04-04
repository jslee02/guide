#include "guide/WorldNode.h"

//==============================================================================
guide::CustomWorldNode::CustomWorldNode(const dart::simulation::WorldPtr& world)
  : ::dart::gui::osg::WorldNode(world)
{
  // Set up the customized WorldNode
}

//==============================================================================
void guide::CustomWorldNode::customPreRefresh()
{
  // Use this function to execute custom code before each time that the
  // window is rendered. This function can be deleted if it does not need
  // to be used.
}

//==============================================================================
void guide::CustomWorldNode::customPostRefresh()
{
  // Use this function to execute custom code after each time that the
  // window is rendered. This function can be deleted if it does not need
  // to be used.
}

//==============================================================================
void guide::CustomWorldNode::customPreStep()
{
  // Use this function to execute custom code before each simulation time
  // step is performed. This function can be deleted if it does not need
  // to be used.
}

//==============================================================================
void guide::CustomWorldNode::customPostStep()
{
  // Use this function to execute custom code after each simulation time
  // step is performed. This function can be deleted if it does not need
  // to be used.
}
