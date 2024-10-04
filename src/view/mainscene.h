#ifndef MAINSCENE_H
#define MAINSCENE_H

#define GL_SILENCE_DEPRECATION

#include <QOpenGLFramebufferObject>
#include <QOpenGLWidget>
#include <QTimer>
#include <QWidget>
#include <QtOpenGL>

extern "C" {
#include "../model/affine.h"
}

class MainScene : public QOpenGLWidget {
 public:
  MainScene(QWidget* parent = 0);
  ~MainScene();

  void setData(data_t* data);
  void setZone(GLdouble data, int zone);
  void setType(GLint type);
  void setPolygonWidth(GLint width);
  void setLineType(GLint type);
  void setLineColor(GLdouble color, GLint index);

  void setVertexEnable(GLint index);
  void setVertexColor(GLdouble color, GLint index);
  void setVertexWidth(GLint width);

  void setBackgroundColor(GLdouble color, GLint index);

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void update();

  void reshape(GLint type);

  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

 private:
  QTimer* timer;

  data_t* m_data;
  GLdouble zoneX;
  GLdouble zoneY;
  GLdouble zoneZ;

  GLint lineWidth;
  GLint lineType;
  GLdouble lineColor_r;
  GLdouble lineColor_g;
  GLdouble lineColor_b;

  GLint vertexWidth;
  GLdouble vertexColor_r;
  GLdouble vertexColor_g;
  GLdouble vertexColor_b;

  GLdouble backgroundColor_r;
  GLdouble backgroundColor_g;
  GLdouble backgroundColor_b;

  GLint projectionType;
  GLint vertexEnable;

  QPoint lastMousePos;
  bool leftMousePressed;
  bool rightMousePressed;

  void buildLines();
  void buildPoints();
};

#endif  // MAINSCENE_H
