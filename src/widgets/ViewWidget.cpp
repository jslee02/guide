#include "ViewWidget.h"

using namespace dart;

ViewWidget::ViewWidget(QWidget *parent)
    : QWidget(parent)
{
    rubberBandIsShown = false;
}

ViewWidget::~ViewWidget()
{
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


