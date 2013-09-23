#include "ViewerWidget.h"

#include <QtGui>
#include <QtCore>
#include <QBoxLayout>

#include "widgets/ViewWidget.h"

using namespace dart;

ViewerWidget::ViewerWidget(QWidget* parent)
    : QWidget(parent), mWorld(NULL)
{
    this->mTimer = new QTimer(this);
    connect(this->mTimer, SIGNAL(timeout()), this, SLOT(update()));
    this->mTimer->start(44);

    QBoxLayout* viewerLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    mViewWidget = new ViewWidget(this);
    viewerLayout->addWidget(mViewWidget);

    setLayout(viewerLayout);
}

ViewerWidget::~ViewerWidget()
{
    if (mWorld)
        delete mWorld;
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
    simulation::World* world = utils::SkelParser::readSkelFile(fileName.toStdString());
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

void ViewerWidget::update()
{
    mViewWidget->update();
}

void ViewerWidget::_drawWorld()
{
    if (mWorld == NULL)
        return;

//    for (int i = 0; i < mWorld->getNumSkeletons(); i++)
//        mWorld->getSkeleton(i)->draw(mRI);
}
