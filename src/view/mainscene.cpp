#include "mainscene.h"

MainScene::MainScene(QWidget *parent) : QOpenGLWidget(parent) {
  this->m_data = NULL;
  zoneX = 2.0;
  zoneY = 2.0;
  zoneZ = 2.0;

  projectionType = 0;

  lineWidth = 1;
  lineColor_r = 0.5;
  lineColor_g = 0.5;
  lineColor_b = 0.5;

  vertexWidth = 1;
  vertexColor_r = 0.5;
  vertexColor_g = 0.5;
  vertexColor_b = 0.5;

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainScene::update);
  timer->start(30);
}

MainScene::~MainScene() { delete timer; }

void MainScene::initializeGL() { glEnable(GL_DEPTH_TEST); }

void MainScene::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

void MainScene::paintGL() {
  reshape(projectionType);

  if (m_data != NULL) {
    if (vertexEnable) {
      buildPoints();
    }

    buildLines();
  }
}

void MainScene::update() { QOpenGLWidget::update(); }

void MainScene::reshape(GLint type) {
  glClearColor(backgroundColor_r, backgroundColor_g, backgroundColor_b, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (type == 0) {
    glOrtho(-zoneX, zoneX, -zoneY, zoneY, -zoneZ, zoneZ);
  } else if (type == 1) {
    GLfloat aspectRatio = (zoneX * 100) / (zoneY * 100);
    GLfloat nearPlane = 1.0;
    GLfloat farPlane = zoneZ;
    GLfloat fovY = zoneY * 10.0;

    GLfloat frustumHeight = nearPlane * tan(fovY / 2.0f);
    GLfloat frustumWidth = frustumHeight * aspectRatio;

    glFrustum(-frustumWidth, frustumWidth, -frustumHeight, frustumHeight,
              nearPlane, farPlane);
  }
  glMatrixMode(GL_MODELVIEW);
}

void MainScene::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    leftMousePressed = true;
  }
  if (event->button() == Qt::RightButton) {
    rightMousePressed = true;
  }
  lastMousePos = event->pos();
}

void MainScene::mouseMoveEvent(QMouseEvent *event) {
  if (leftMousePressed) {
    QPoint currentPos = event->pos();
    float dx = currentPos.x() - lastMousePos.x();
    float dy = currentPos.y() - lastMousePos.y();

    moveX(m_data, dx / (400.0 / zoneX));
    moveY(m_data, -dy / (400.0 / zoneY));
  }
  if (rightMousePressed) {
    QPoint currentPos = event->pos();
    float dx = currentPos.x() - lastMousePos.x();
    float dy = currentPos.y() - lastMousePos.y();

    rotateOX(m_data, -dy);
    rotateOY(m_data, dx);
  }
  lastMousePos = event->pos();
}

void MainScene::mouseReleaseEvent(QMouseEvent *event) {
  leftMousePressed = false;
  rightMousePressed = false;
}

void MainScene::wheelEvent(QWheelEvent *event) {
  int delta = event->angleDelta().y();
  ;
  if (delta > 0) {
    scale(m_data, 1.02);
  } else {
    scale(m_data, 0.98);
  }
}

void MainScene::setData(data_t *data) {
  m_data = NULL;
  this->m_data = data;
}

void MainScene::setZone(GLdouble data, int zone) {
  switch (zone) {
    case 0:
      zoneX = data;
      break;
    case 1:
      zoneY = data;
      break;
    case 2:
      zoneZ = data;
      break;
    default:
      break;
  }

  reshape(projectionType);
}

void MainScene::setType(GLint type) {
  projectionType = type;
  reshape(projectionType);
}

void MainScene::setPolygonWidth(GLint width) { lineWidth = width; }

void MainScene::setLineType(GLint type) { lineType = type; }

void MainScene::setLineColor(GLdouble color, GLint index) {
  switch (index) {
    case 0:
      lineColor_r = color;
      break;
    case 1:
      lineColor_g = color;
      break;
    case 2:
      lineColor_b = color;
      break;
    default:
      break;
  }
}

void MainScene::setVertexEnable(GLint index) { vertexEnable = index; }

void MainScene::setVertexColor(GLdouble color, GLint index) {
  switch (index) {
    case 0:
      vertexColor_r = color;
      break;
    case 1:
      vertexColor_g = color;
      break;
    case 2:
      vertexColor_b = color;
      break;
    default:
      break;
  }
}

void MainScene::setVertexWidth(GLint width) { vertexWidth = width; }

void MainScene::setBackgroundColor(GLdouble color, GLint index) {
  switch (index) {
    case 0:
      backgroundColor_r = color;
      break;
    case 1:
      backgroundColor_g = color;
      break;
    case 2:
      backgroundColor_b = color;
      break;
    default:
      break;
  }
}

void MainScene::buildLines() {
  glLineWidth(lineWidth);

  glColor3f(lineColor_r, lineColor_g, lineColor_b);
  if (lineType == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(10, 10);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }

  for (unsigned int i = 0; i < m_data->polygonsCount; i++) {
    glBegin(GL_LINE_LOOP);

    for (unsigned int j = 0; j < m_data->polygons[i].vertexCount; j++) {
      glVertex3d(m_data->matrix[m_data->polygons[i].vertexes[j] - 1].x,
                 m_data->matrix[m_data->polygons[i].vertexes[j] - 1].y,
                 m_data->matrix[m_data->polygons[i].vertexes[j] - 1].z);
    }

    glEnd();
  }
}

void MainScene::buildPoints() {
  glPointSize(vertexWidth);
  glColor3f(vertexColor_r, vertexColor_g, vertexColor_b);

  glEnable(GL_BLEND);
  if (vertexEnable == 1) {
    glEnable(GL_POINT_SMOOTH);
  } else {
    glDisable(GL_POINT_SMOOTH);
  }

  glBegin(GL_POINTS);
  for (unsigned int i = 0; i < m_data->polygonsCount; i++) {
    for (unsigned int j = 0; j < m_data->polygons[i].vertexCount; j++) {
      glVertex3d(m_data->matrix[m_data->polygons[i].vertexes[j] - 1].x,
                 m_data->matrix[m_data->polygons[i].vertexes[j] - 1].y,
                 m_data->matrix[m_data->polygons[i].vertexes[j] - 1].z);
    }

  }
  glEnd();
}
