#include "MainViewer.h"

using namespace dart;

MainViewer::MainViewer(QWidget *parent)
    : QGLWidget(parent), mWorld(NULL), mRI(NULL)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    rubberBandIsShown = false;

    makeCurrent();
}

MainViewer::~MainViewer()
{
    makeCurrent();
}

bool MainViewer::readFile(const QString& fileName)
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

    mWorld = world;

    return true;
}

bool MainViewer::writeFile(const QString& fileName)
{

}

void MainViewer::initializeGL()
{
    static const GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    static const GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    static const GLfloat position[] = { 0.0, 3.0, 3.0, 0.0 };
    static const GLfloat lmodelAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
    static const GLfloat localView[] = { 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodelAmbient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, localView);

    glFrontFace(GL_CW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    qglClearColor(Qt::black);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    mRI = new dart::renderer::OpenGLRenderInterface();
}

void MainViewer::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(width) / height;
    glFrustum(-x, +x, -1.0, +1.0, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
    _drawWorld();
}

void MainViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _drawWorld();

//    glDisable(GL_LIGHTING);
//    glViewport(0, 0, width(), height());
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glDisable(GL_DEPTH_TEST);

//    glClear(GL_COLOR_BUFFER_BIT);
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, fbObject->texture());
//    glColor3f(1.0, 1.0, 1.0);
//    GLfloat s = width() / GLfloat(fbObject->size().width());
//    GLfloat t = height() / GLfloat(fbObject->size().height());

//    glBegin(GL_QUADS);
//    glTexCoord2f(0.0, 0.0);
//    glVertex2f(-1.0, -1.0);
//    glTexCoord2f(s, 0.0);
//    glVertex2f(1.0, -1.0);
//    glTexCoord2f(s, t);
//    glVertex2f(1.0, 1.0);
//    glTexCoord2f(0.0, t);
//    glVertex2f(-1.0, 1.0);
//    glEnd();

//    if (rubberBandIsShown) {
//        glMatrixMode(GL_PROJECTION);
//        glOrtho(0, width(), height(), 0, 0, 100);
//        glMatrixMode(GL_MODELVIEW);

//        glDisable(GL_TEXTURE_2D);
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        glLineWidth(4.0);
//        glColor4f(1.0, 1.0, 1.0, 0.2);
//        glRecti(rubberBandCorner1.x(), rubberBandCorner1.y(),
//                rubberBandCorner2.x(), rubberBandCorner2.y());
//        glColor4f(1.0, 1.0, 0.0, 0.5);
//        glLineStipple(3, 0xAAAA);
//        glEnable(GL_LINE_STIPPLE);

//        glBegin(GL_LINE_LOOP);
//        glVertex2i(rubberBandCorner1.x(), rubberBandCorner1.y());
//        glVertex2i(rubberBandCorner2.x(), rubberBandCorner1.y());
//        glVertex2i(rubberBandCorner2.x(), rubberBandCorner2.y());
//        glVertex2i(rubberBandCorner1.x(), rubberBandCorner2.y());
//        glEnd();

//        glLineWidth(1.0);
//        glDisable(GL_LINE_STIPPLE);
//        glDisable(GL_BLEND);
//    }
}

void MainViewer::mousePressEvent(QMouseEvent *event)
{
    rubberBandCorner1 = event->pos();
    rubberBandCorner2 = event->pos();
    rubberBandIsShown = true;
}

void MainViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (rubberBandIsShown) {
        rubberBandCorner2 = event->pos();
        updateGL();
    }
}

void MainViewer::mouseReleaseEvent(QMouseEvent * /* event */)
{
    if (rubberBandIsShown) {
        rubberBandIsShown = false;
        updateGL();
    }
}

void MainViewer::_drawWorld()
{
    if (mWorld == NULL)
        return;

    for (int i = 0; i < mWorld->getNumSkeletons(); i++)
        mWorld->getSkeleton(i)->draw(mRI);
}
