#ifndef GUIDE_WIDGET_VIEWWIDGET_H
#define GUIDE_WIDGET_VIEWWIDGET_H

#include <dart/dynamics/Skeleton.h>
#include <dart/renderer/OpenGLRenderInterface.h>
#include <dart/simulation/World.h>
#include <dart/utils/SkelParser.h>

#include <QtGui>
#include <QWidget>

#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>

class ViewerWidget;

class ViewWidget : public osgQt::GLWidget
{
    Q_OBJECT

public:
    ViewWidget(osg::Node* scene, ViewerWidget* viewer, QWidget *parent = 0);
    ~ViewWidget();

signals: void clicked();

protected:
    virtual void moveEvent(QMoveEvent *_e);
    virtual void paintEvent(QPaintEvent *_e);
    virtual void resizeEvent(QResizeEvent *_e);
    virtual void showEvent(QShowEvent *_e);
    virtual void enterEvent(QEvent * event);

    void keyPressEvent(QKeyEvent *_event);
    void keyReleaseEvent(QKeyEvent *_event);
    void wheelEvent(QWheelEvent *_event);
    void mousePressEvent(QMouseEvent *_event);
    void mouseDoubleClickEvent(QMouseEvent *_event);
    void mouseMoveEvent(QMouseEvent *_event);
    void mouseReleaseEvent(QMouseEvent *_event);

private:
    osgQt::GraphicsWindowQt* mOsgGraphicsWindow;
    osgViewer::View* mOsgView;

    QPoint rubberBandCorner1;
    QPoint rubberBandCorner2;
    bool rubberBandIsShown;
};

#endif
