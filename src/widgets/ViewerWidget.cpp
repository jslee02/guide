#include "ViewerWidget.h"

#include <QtGui>
#include <QtCore>
#include <QBoxLayout>

#include <dart/utils/SkelParser.h>

#include "WorldScene.h"
#include "widgets/ViewWidget.h"

using namespace dart;


ViewerWidget::ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel,
                           QWidget* parent)
    : QWidget(parent),
      mWorld(NULL),
      mScene(NULL)
{
    setThreadingModel(threadingModel);

    // disable the default setting of viewer.done() by pressing Escape.
    setKeyEventSetsDone(0);

    widget1 = new ViewWidget(mScene, this);
//    widget2 = new ViewWidget(mScene, this);
//    widget3 = new ViewWidget(mScene, this);
//    widget4 = new ViewWidget(mScene, this);
//    popupWidget = new ViewWidget(mScene, this);
    //popupWidget->show();

    QGridLayout* grid = new QGridLayout;
    grid->addWidget(widget1, 0, 0);
//    grid->addWidget(widget2, 0, 1);
//    grid->addWidget(widget3, 1, 0);
//    grid->addWidget(widget4, 1, 1);
    setLayout(grid);

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(update()));
    mTimer->start(10);
}

ViewerWidget::~ViewerWidget()
{
    if (mWorld)
        delete mWorld;

    delete mTimer;
}

bool ViewerWidget::readFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("GUIDE"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    file.close();

    QApplication::setOverrideCursor(Qt::WaitCursor);
    // TODO: Read other file formats such as sdf, urdf
    simulation::World* world = utils::SkelParser::readWorld(fileName.toStdString());
    QApplication::restoreOverrideCursor();

    if (world == NULL)
    {
        QMessageBox::warning(this, tr("GUIDE"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    if (mWorld)
        delete mWorld;

    mWorld = world;

    mWorldScene->setWorld(mWorld);

    return true;
}

bool ViewerWidget::writeFile(const QString& fileName)
{

}

void ViewerWidget::mousePressEvent(QMouseEvent *event)
{
}

void ViewerWidget::mouseMoveEvent(QMouseEvent *event)
{
}

void ViewerWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
}

void ViewerWidget::moveEvent(QMoveEvent* e)
{
    QWidget::moveEvent(e);
}

void ViewerWidget::paintEvent(QPaintEvent* e)
{
    frame();
}

void ViewerWidget::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent(e);
}

void ViewerWidget::showEvent(QShowEvent* e)
{
    QWidget::showEvent(e);
}

