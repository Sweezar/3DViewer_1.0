#include "affine.h"

#include <math.h>

void moveX(data_t *data, double x) {
  for (unsigned int i = 0; i < data->vertexesCount; i++) {
    data->matrix[i].x += x;
  }
}

void moveY(data_t *data, double y) {
  for (unsigned int i = 0; i < data->vertexesCount; i++) {
    data->matrix[i].y += y;
  }
}

void moveZ(data_t *data, double z) {
  for (unsigned int i = 0; i < data->vertexesCount; i++) {
    data->matrix[i].z += z;
  }
}

double angleInRadians(double angle) { return angle * M_PI / 180; }

void rotateOX(data_t *data, double angle) {
  angle = angleInRadians(angle);
  for (unsigned int i = 0; i < data->vertexesCount; i++) {
    double y = data->matrix[i].y;
    double z = data->matrix[i].z;

    data->matrix[i].y = y * cos(angle) + z * sin(angle);
    data->matrix[i].z = z * cos(angle) - y * sin(angle);
  }
}

void rotateOY(data_t *data, double angle) {
  angle = angleInRadians(angle);
  for (unsigned int i = 0; i < data->vertexesCount; i++) {
    double x = data->matrix[i].x;
    double z = data->matrix[i].z;

    data->matrix[i].x = x * cos(angle) + z * sin(angle);
    data->matrix[i].z = z * cos(angle) - x * sin(angle);
  }
}

void rotateOZ(data_t *data, double angle) {
  angle = angleInRadians(angle);
  for (unsigned int i = 0; i < data->vertexesCount; i++) {
    double x = data->matrix[i].x;
    double y = data->matrix[i].y;

    data->matrix[i].x = x * cos(angle) + y * sin(angle);
    data->matrix[i].y = y * cos(angle) - x * sin(angle);
  }
}

void scale(data_t *data, double s) {
  if (s == 0) {
    return;
  }
  for (unsigned int i = 0; i < data->vertexesCount; i++) {
    if (s > 0) {
      data->matrix[i].x *= s;
      data->matrix[i].y *= s;
      data->matrix[i].z *= s;
    }
  }
}
