#include "widgets/ViewerWidget.h"

#include "widgets/ViewWidget.h"

using namespace dart;

ViewWidget::ViewWidget(osg::Node* scene, ViewerWidget* viewer, QWidget *parent)
    : osgQt::GLWidget(parent),
      rubberBandIsShown(false)
{
    mOsgGraphicsWindow = new osgQt::GraphicsWindowQt(this);
    mOsgView           = new osgViewer::View;
    viewer->addView(mOsgView);

    osg::Camera* camera = mOsgView->getCamera();
    camera->setGraphicsContext(mOsgGraphicsWindow);

    const osg::GraphicsContext::Traits* traits = mOsgGraphicsWindow->getTraits();

    camera->setClearColor(osg::Vec4(0.8, 0.8, 0.8, 1.0));
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f);

    // TODO: scene
    mOsgView->setSceneData(scene);
    mOsgView->addEventHandler(new osgViewer::StatsHandler);
    mOsgView->setCameraManipulator(new osgGA::TrackballManipulator);
}

ViewWidget::~ViewWidget()
{
    delete mOsgGraphicsWindow;
}

void ViewWidget::moveEvent(QMoveEvent* _e)
{

}

void ViewWidget::paintEvent(QPaintEvent* _e)
{

}

void ViewWidget::resizeEvent(QResizeEvent* _e)
{

}

void ViewWidget::showEvent(QShowEvent* _e)
{

}

void ViewWidget::enterEvent(QEvent* event)
{

}

void ViewWidget::keyPressEvent(QKeyEvent* _event)
{

}

void ViewWidget::keyReleaseEvent(QKeyEvent* _event)
{

}

void ViewWidget::wheelEvent(QWheelEvent* _event)
{

}

void ViewWidget::mousePressEvent(QMouseEvent *event)
{
    rubberBandCorner1 = event->pos();
    rubberBandCorner2 = event->pos();
    rubberBandIsShown = true;
}

void ViewWidget::mouseDoubleClickEvent(QMouseEvent* _event)
{

}

void ViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (rubberBandIsShown) {
        rubberBandCorner2 = event->pos();
    }
}

void ViewWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    if (rubberBandIsShown) {
        rubberBandIsShown = false;
    }
}


