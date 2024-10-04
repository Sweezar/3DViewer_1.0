#include "read_obj.h"

void initData(data_t* data) {
  data->vertexesCount = 0;
  data->polygonsCount = 0;
  data->matrix = NULL;
  data->polygons = NULL;
}

void freeData(data_t* data) {
  if (data->matrix) {
    free(data->matrix);
    data->matrix = NULL;
  }
  if (data->polygons) {
    for (unsigned int i = 0; i < data->polygonsCount; i++) {
      free(data->polygons[i].vertexes);
    }
    free(data->polygons);
    data->polygons = NULL;
  }
}

void allocMatrix(data_t* data) {
  data->matrix = (vector3D*)calloc(data->vertexesCount, sizeof(vector3D));
}

void allocPolygons(data_t* data) {
  data->polygons = (polygon_t*)calloc(data->polygonsCount, sizeof(polygon_t));
}

void allocPolygonVertexes(data_t* data, int facetIndex) {
  data->polygons[facetIndex].vertexes = (unsigned int*)calloc(
      data->polygons[facetIndex].vertexCount, sizeof(unsigned int));
}

void getCountOfVertexesAndFacets(const char* filename, data_t* data) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Ошибка при открытии файла '%s'\n", filename);
    return;
  }

  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = 0;
    // Читаем вершины
    if (strncmp(line, "v ", 2) == 0) {
      data->vertexesCount++;
    }

    // Читаем полигоны
    if (strncmp(line, "f ", 2) == 0) {
      data->polygonsCount++;
    }
  }

  fclose(file);
}

void replaceAllPointsWithCommas(char* str) {
  char* ptr = str;
  while (*ptr != '\0') {
    if (*ptr == '.') {
      *ptr = ',';
    }
    ptr++;
  }
}

void getVertexesAndFacets(const char* filename, data_t* data) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Ошибка при открытии файла '%s'\n", filename);
    return;
  }

  char line[MAX_LINE_LENGTH];
  int v = 0;
  int f = 0;

  while (fgets(line, sizeof(line), file)) {
    // Заполнение матрицы координатами вершин
    if (strncmp(line, "v ", 2) == 0) {
#ifdef __linux__
      replaceAllPointsWithCommas(line);
#endif
      sscanf(line, "v %lf %lf %lf", &(data->matrix[v].x), &(data->matrix[v].y),
             &(data->matrix[v].z));
      v++;
    }

    if (strncmp(line, "f ", 2) == 0) {
      // Считываем количество вершин у полигона
      int vertexCount = 0;
      char* copyLine = mystrdup(line);
      copyLine[strlen(line)] = '\0';
      char* token = strtok(copyLine, " ");
      while (token != NULL) {
        if (strcmp(token, "f") != 0 && isdigit(token[0])) {
          vertexCount++;
        }
        token = strtok(NULL, " ");
      }
      free(copyLine);
      copyLine = NULL;

      // Нужно выделить память для вершин полигона
      data->polygons[f].vertexCount = vertexCount;
      allocPolygonVertexes(data, f);
      // Читаем вершины полигона
      char* next_token = strtok(line, " ");
      int i = 0;
      while (next_token != NULL) {
        if (strcmp(next_token, "f") != 0 && isdigit(next_token[0])) {
          int number = atoi(next_token);
          data->polygons[f].vertexes[i] = number;
          i++;
        }
        next_token = strtok(NULL, " ");
      }

      f++;
    }
  }

  fclose(file);
}

void readObjFile(const char* filename, data_t* data) {
  initData(data);
  getCountOfVertexesAndFacets(filename, data);
  allocMatrix(data);
  allocPolygons(data);
  getVertexesAndFacets(filename, data);
}

char* mystrdup(const char* s) {
  char* p = malloc(strlen(s) + 1);
  if (p) strcpy(p, s);
  return p;
}
