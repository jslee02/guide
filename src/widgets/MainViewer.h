#ifndef TEAPOTS_H
#define TEAPOTS_H

#include <dart/dynamics/Skeleton.h>
#include <dart/renderer/OpenGLRenderInterface.h>
#include <dart/simulation/World.h>
#include <dart/utils/SkelParser.h>

#include <QtOpenGL>

class MainViewer : public QGLWidget
{
    Q_OBJECT

public:
    MainViewer(QWidget *parent = 0);
    ~MainViewer();

    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void _drawWorld();

    dart::simulation::World* mWorld;
    dart::renderer::RenderInterface* mRI;

    QGLFramebufferObject *fbObject;
    GLuint glTeapotObject;
    QPoint rubberBandCorner1;
    QPoint rubberBandCorner2;
    bool rubberBandIsShown;
};

#endif
