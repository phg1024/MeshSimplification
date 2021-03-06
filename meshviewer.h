#ifndef MESHVIEWER_H
#define MESHVIEWER_H

#include "GL/glu.h"

#include "mesh.h"

#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <stack>
using namespace std;

#include <QGLWidget>
#include <QGLFunctions>
#include <QGLFormat>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>
#include <QGLFramebufferObject>

const QGLFormat qglformat_3d(
    QGL::DoubleBuffer       |
    QGL::DepthBuffer        |
    QGL::AccumBuffer        |
    //QGL::StencilBuffer      |
    //QGL::StereoBuffers      |
    QGL::SampleBuffers      |
    QGL::Rgba               |
    QGL::AlphaChannel       |
    QGL::DirectRendering    |
    QGL::HasOverlay
    );

class MeshViewer : public QGLWidget, protected QGLFunctions
{
  Q_OBJECT
public:
  explicit MeshViewer(QWidget *parent = 0);
  virtual ~MeshViewer();

signals:

protected:
  void keyPressEvent(QKeyEvent *e);
  void keyReleaseEvent(QKeyEvent *e);
  void mousePressEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent* e);

  void enterEvent(QEvent *e);
  void leaveEvent(QEvent *e);

  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

public slots:


private:
  struct ViewerState {
    ViewerState():zNear(1.0), zFar(7000.0), fov(45.0){
      translation = QVector3D(0, 0, -5);
      angularChange = 0;
    }

    void updateViewport(int w, int h) {
      viewport.x = 0;
      viewport.y = 0;
      viewport.w = w;
      viewport.h = h;

      aspect = (qreal)w / (qreal)h;
    }

    void updateProjection() {
      projection.setToIdentity();
      projection.perspective(fov, aspect, zNear, zFar);
    }

    void updateModelView() {
      QMatrix4x4 matrix;
      matrix.translate(translation);
      matrix.rotate(rotation);
      modelview = matrix;
    }

    QMatrix4x4 projectionModelView() const {
      return projection * modelview;
    }

    struct {
      int x, y, w, h;
    } viewport;

    void print() {
      qDebug() << "viewstate:";
      qDebug() << viewport.x << ", " << viewport.y << ", " << viewport.w << ", " << viewport.h;
      qDebug() << modelview;
      qDebug() << projection;
      qDebug() << rotationAxis;
      qDebug() << angularChange;
      qDebug() << rotation;
      qDebug() << translation;
      qDebug() << zNear << ", " << zFar << ", " << fov;
      qDebug() << aspect;
    }

    QMatrix4x4 modelview;
    QMatrix4x4 projection;

    QVector3D rotationAxis;
    qreal angularChange;
    QQuaternion rotation;
    QVector3D translation;

    qreal zNear, zFar, fov;
    qreal aspect;
  };

  ViewerState viewerState;

  struct MouseState {
    MouseState():isPressed(false){}
    QVector2D prev_pos;
    bool isPressed;
  };

  MouseState mouseState;

public:
  enum InteractionState {
    Camera = 0
  };
  void setInteractionMode(InteractionState state) { interactionState = state; }

  struct SelectionBox
  {
    GLdouble corner_win[4];
    GLdouble corner_global[12];
    QVector3D gcorners[4];
  } sbox;
  bool isSelecting;
  vector<unsigned char> selectionBuffer;
  int lastSelectedIndex;

  void computeGlobalSelectionBox();
  bool QtUnProject(const QVector3D &pos_screen, QVector3D &pos_world);

private:
  InteractionState interactionState;
  stack<InteractionState> interactionStateStack;

private:
  Mesh heMesh;   /// not own

private:

private:
  bool enableLighting;
  void enableLights();
  void disableLights();

private:

private:
};

#endif // MESHVIEWER_H
