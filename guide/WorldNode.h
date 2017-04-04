#pragma once

#include <dart/dart.hpp>
#include <dart/gui/osg/osg.hpp>

namespace guide {

class CustomWorldNode : public dart::gui::osg::WorldNode
{
public:
  CustomWorldNode(const dart::simulation::WorldPtr& world = nullptr);

  void customPreRefresh();

  void customPostRefresh();

  void customPreStep();

  void customPostStep();
};

} // namespace guide
