#ifndef GUIDE_WIDGET_VIEWERWIDGET_H
#define GUIDE_WIDGET_VIEWERWIDGET_H

#include <dart/dynamics/Skeleton.h>
#include <dart/simulation/World.h>
#include <dart/utils/SkelParser.h>

#include <QtOpenGL>

class ViewWidget;

class ViewerWidget : public QWidget
{
    Q_OBJECT

public:
    ViewerWidget(QWidget *parent = 0);
    ~ViewerWidget();

    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    virtual void update();

private:
    void _drawWorld();

    dart::simulation::World* mWorld;

    ViewWidget* mViewWidget;
    QTimer* mTimer;
};

#endif
