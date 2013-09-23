#ifndef GUIDE_WIDGET_VIEWWIDGET_H
#define GUIDE_WIDGET_VIEWWIDGET_H

#include <dart/dynamics/Skeleton.h>
#include <dart/renderer/OpenGLRenderInterface.h>
#include <dart/simulation/World.h>
#include <dart/utils/SkelParser.h>

#include <QtGui>
#include <QWidget>

class ViewWidget : public QWidget
{
    Q_OBJECT

public:
    ViewWidget(QWidget *parent = 0);
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

    QPoint rubberBandCorner1;
    QPoint rubberBandCorner2;
    bool rubberBandIsShown;
};

#endif
