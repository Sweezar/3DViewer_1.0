#ifndef AFFINE_H_
#define AFFINE_H_

#include "math.h"
#include "read_obj.h"

void moveX(data_t* data, double x);
void moveY(data_t* data, double y);
void moveZ(data_t* data, double z);
double angleInRadians(double angle);

void rotateOX(data_t* data, double angle);
void rotateOY(data_t* data, double angle);
void rotateOZ(data_t* data, double angle);

void scale(data_t* data, double s);

#endif  // AFFINE_H_
