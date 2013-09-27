#ifndef GUIDE_WIDGET_VIEWERWIDGET_H
#define GUIDE_WIDGET_VIEWERWIDGET_H

#include <dart/dynamics/Skeleton.h>
#include <dart/simulation/World.h>
#include <dart/utils/SkelParser.h>

#include <QWidget>

#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>

class ViewerWidget : public QWidget, public osgViewer::CompositeViewer
{
    Q_OBJECT

public:
    ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel =
                 osgViewer::CompositeViewer::SingleThreaded,
                 QWidget *parent = 0);
    ~ViewerWidget();

    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);

    QWidget* addViewWidget(osgQt::GraphicsWindowQt* gw);

    osgQt::GraphicsWindowQt* createGraphicsWindow(
            int x, int y, int w, int h, const std::string& name = "",
            bool windowDecoration = false);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);\

    virtual void moveEvent(QMoveEvent* e);
    virtual void paintEvent(QPaintEvent* e);
    virtual void resizeEvent(QResizeEvent* e);
    virtual void showEvent(QShowEvent* e);

private slots:
//    virtual void update();

private:
    QWidget* widget1;
    QWidget* widget2;
    QWidget* widget3;
    QWidget* widget4;
    QWidget* popupWidget;

    osg::Node* mScene;

    dart::simulation::World* mWorld;

    QTimer* mTimer;
};

#endif
