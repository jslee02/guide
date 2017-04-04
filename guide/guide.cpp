#include <dart/dart.hpp>

#include "guide/WorldNode.h"
#include "guide/EventHandler.h"

int main()
{
  // Create a world
  dart::simulation::WorldPtr world(new dart::simulation::World);

  // Add a target object to the world
  dart::gui::osg::InteractiveFramePtr target(
        new dart::gui::osg::InteractiveFrame(dart::dynamics::Frame::World()));
  world->addSimpleFrame(target);

  // Wrap a WorldNode around it
  osg::ref_ptr<guide::CustomWorldNode> node = new guide::CustomWorldNode(world);

  // Create a Viewer and set it up with the WorldNode
  dart::gui::osg::ImGuiViewer viewer;
  viewer.addWorldNode(node);

  // Active the drag-and-drop feature for the target
  viewer.enableDragAndDrop(target.get());

  // Pass in the custom event handler
//  viewer.addEventHandler(new CustomEventHandler);

  // Set up the window to be 640x480
  viewer.setUpViewInWindow(0, 0, 640, 480);

  // Adjust the viewpoint of the Viewer
  viewer.getCameraManipulator()->setHomePosition(
        ::osg::Vec3( 2.57f,  3.14f, 1.64f),
        ::osg::Vec3( 0.00f,  0.00f, 0.00f),
        ::osg::Vec3(-0.24f, -0.25f, 0.94f));
  // We need to re-dirty the CameraManipulator by passing it into the viewer
  // again, so that the viewer knows to update its HomePosition setting
  viewer.setCameraManipulator(viewer.getCameraManipulator());

  // Begin running the application loop
  viewer.run();
}
