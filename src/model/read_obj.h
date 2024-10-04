#ifndef READOBJ_H_
#define READOBJ_H_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

typedef struct vector3D {
  double x, y, z;
} vector3D;

typedef struct facets {
  unsigned int vertexCount;
  unsigned int* vertexes;
} polygon_t;

typedef struct data_t {
  unsigned int vertexesCount;
  unsigned int polygonsCount;
  vector3D* matrix;
  polygon_t* polygons;
} data_t;

void initData(data_t* data);

void freeData(data_t* data);

void allocMatrix(data_t* data);

void allocPolygons(data_t* data);

void allocPolygonVertexes(data_t* data, int facetIndex);

void getCountOfVertexesAndFacets(const char* filename, data_t* data);

void getVertexesAndFacets(const char* filename, data_t* data);

void readObjFile(const char* filename, data_t* data);
char* mystrdup(const char* s);

void replaceAllPointsWithCommas(char* str);

#endif