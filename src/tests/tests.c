#include "tests.h"

#include <check.h>
#include <math.h>

#include "../model/affine.h"
#include "../model/read_obj.h"

// Тест перемещения по оси X
START_TEST(test_moveX) {
  data_t data;
  vector3D vertices[3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  data.matrix = vertices;
  data.vertexesCount = 3;

  moveX(&data, 2.0);

  ck_assert_double_eq(data.matrix[0].x, 3.0);
  ck_assert_double_eq(data.matrix[1].x, 6.0);
  ck_assert_double_eq(data.matrix[2].x, 9.0);
}
END_TEST

// Тест перемещения по оси Y
START_TEST(test_moveY) {
  data_t data;
  vector3D vertices[3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  data.matrix = vertices;
  data.vertexesCount = 3;

  moveY(&data, -1.0);

  ck_assert_double_eq(data.matrix[0].y, 1.0);
  ck_assert_double_eq(data.matrix[1].y, 4.0);
  ck_assert_double_eq(data.matrix[2].y, 7.0);
}
END_TEST

// Тест перемещения по оси Z
START_TEST(test_moveZ) {
  data_t data;
  vector3D vertices[3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  data.matrix = vertices;
  data.vertexesCount = 3;

  moveZ(&data, 5.0);

  ck_assert_double_eq(data.matrix[0].z, 8.0);
  ck_assert_double_eq(data.matrix[1].z, 11.0);
  ck_assert_double_eq(data.matrix[2].z, 14.0);
}
END_TEST

// Тест вращения вокруг оси OX
START_TEST(test_rotateOX) {
  data_t data;
  vector3D vertices[1] = {{0, 1, 0}};
  data.matrix = vertices;
  data.vertexesCount = 1;

  rotateOX(&data, angleInRadians(90));

  ck_assert(fabs(data.matrix[0].y) < 1);
  ck_assert(fabs(data.matrix[0].z) > -1e-6);
}
END_TEST

// Тест вращения вокруг оси OY
START_TEST(test_rotateOY) {
  data_t data;
  vector3D vertices[1] = {{1, 0, 0}};
  data.matrix = vertices;
  data.vertexesCount = 1;

  rotateOY(&data, angleInRadians(90));

  ck_assert(fabs(data.matrix[0].x) < 1);
  ck_assert(fabs(data.matrix[0].z) > 1e-6);
}
END_TEST

// Тест вращения вокруг оси OZ
START_TEST(test_rotateOZ) {
  data_t data;
  vector3D vertices[1] = {{1, 1, 0}};
  data.matrix = vertices;
  data.vertexesCount = 1;

  rotateOZ(&data, angleInRadians(90));

  ck_assert(fabs(data.matrix[0].x) > 1);
  ck_assert(fabs(data.matrix[0].y) < 1);
}
END_TEST

// Тест масштабирования
START_TEST(test_scale) {
  data_t data;
  vector3D vertices[3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  data.matrix = vertices;
  data.vertexesCount = 3;

  scale(&data, 2.0);

  ck_assert_double_eq(data.matrix[0].x, 2.0);
  ck_assert_double_eq(data.matrix[0].y, 4.0);
  ck_assert_double_eq(data.matrix[0].z, 6.0);
}
END_TEST

// Тест масштабирования до нуля
START_TEST(test_scale_zero) {
  data_t data;
  vector3D vertices[3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  data.matrix = vertices;
  data.vertexesCount = 3;

  scale(&data, 0);

  ck_assert_double_eq(data.matrix[0].x, 1);
  ck_assert_double_eq(data.matrix[0].y, 2);
  ck_assert_double_eq(data.matrix[0].z, 3);
}
END_TEST

// Тест инициализации данных
START_TEST(test_initData) {
  data_t data;
  initData(&data);

  ck_assert_int_eq(data.vertexesCount, 0);
  ck_assert_int_eq(data.polygonsCount, 0);
  ck_assert_ptr_eq(data.matrix, NULL);
  ck_assert_ptr_eq(data.polygons, NULL);
}
END_TEST

// Тест выделения матрицы
START_TEST(test_allocMatrix) {
  data_t data;
  initData(&data);

  data.vertexesCount = 5;  // Установим количество вершин
  allocMatrix(&data);

  ck_assert_ptr_ne(data.matrix, NULL);  // Проверяем, что память выделена
  ck_assert_int_eq(data.vertexesCount, 5);  // Проверяем количество вершин
}
END_TEST

// Тест выделения полигонов
START_TEST(test_allocPolygons) {
  data_t data;
  initData(&data);

  data.polygonsCount = 3;  // Установим количество полигонов
  allocPolygons(&data);

  ck_assert_ptr_ne(data.polygons, NULL);  // Проверяем, что память выделена
  ck_assert_int_eq(data.polygonsCount, 3);  // Проверяем количество полигонов
}
END_TEST

// Тест получения количества вершин и фасетов из файла
START_TEST(test_getCountOfVertexesAndFacets) {
  data_t data;
  initData(&data);

  // Создаем временный файл для тестирования
  const char *filename = "test.obj";
  FILE *file = fopen(filename, "w");

  fprintf(file, "v 1.0 1.0 1.0\n");
  fprintf(file, "v 2.0 2.0 2.0\n");
  fprintf(file, "f 1 2\n");

  fclose(file);

  getCountOfVertexesAndFacets(filename, &data);

  ck_assert_int_eq(data.vertexesCount, 2);  // Должно быть две вершины
  ck_assert_int_eq(data.polygonsCount, 1);  // Должно быть один полигон

  // Удаляем временный файл
  remove(filename);
}
END_TEST

// Тест получения вершин и фасетов из файла
START_TEST(test_getVertexesAndFacets) {
  data_t data;
  initData(&data);

  // Создаем временный файл для тестирования
  const char *filename = "test.obj";
  FILE *file = fopen(filename, "w");
  readObjFile(filename, &data);

  fprintf(file, "v 1.0 1.0 1.0\n");
  fprintf(file, "v 2.0 2.0 2.0\n");
  fprintf(file, "f 1 2\n");

  fclose(file);

  getCountOfVertexesAndFacets(filename, &data);

  allocMatrix(&data);
  allocPolygons(&data);

  getVertexesAndFacets(filename, &data);

  // Проверка первой вершины
  ck_assert_double_eq(data.matrix[0].x, 1.0);
  ck_assert_double_eq(data.matrix[0].y, 1.0);
  ck_assert_double_eq(data.matrix[0].z, 1.0);

  // Проверка второй вершины
  ck_assert_double_eq(data.matrix[1].x, 2.0);
  ck_assert_double_eq(data.matrix[1].y, 2.0);
  ck_assert_double_eq(data.matrix[1].z, 2.0);

  // Проверка количества вершин в первом полигоне
  ck_assert_int_eq(data.polygons[0].vertexCount, 2);
  ck_assert_int_eq(data.polygons[0].vertexes[0], 1);
  ck_assert_int_eq(data.polygons[0].vertexes[1], 2);

  // Удаляем временный файл и освобождаем память
  remove(filename);
}
END_TEST

// Тест освобождения данных
START_TEST(test_freeData) {
  data_t data;
  data.vertexesCount = 3;
  data.polygonsCount = 2;

  // Выделяем память для матрицы вершин
  data.matrix = malloc(data.vertexesCount * sizeof(vector3D));
  for (unsigned int i = 0; i < data.vertexesCount; i++) {
    data.matrix[i] =
        (vector3D){i + 1, i + 2, i + 3};  // Инициализация значениями
  }

  // Выделяем память для полигонов
  data.polygons = malloc(data.polygonsCount * sizeof(polygon_t));
  for (unsigned int i = 0; i < data.polygonsCount; i++) {
    data.polygons[i].vertexCount = 2;
    data.polygons[i].vertexes =
        malloc(data.polygons[i].vertexCount * sizeof(unsigned int));
    // Инициализация значениями
    for (unsigned int j = 0; j < data.polygons[i].vertexCount; j++) {
      data.polygons[i].vertexes[j] = j + 1;
    }
  }

  // Освобождаем память с помощью freeData
  freeData(&data);

  // Проверяем, что указатели теперь равны NULL
  ck_assert_ptr_eq(data.matrix, NULL);
  ck_assert_ptr_eq(data.polygons, NULL);
}
END_TEST

// Тест для функции replaceAllPointsWithCommas
START_TEST(test_replaceAllPointsWithCommas) {
  char str1[] = "Hello.World";
  char str2[] = "NoPointsHere";
  char str3[] = "Multiple.Points.In.String";
  char str4[] = "";

  replaceAllPointsWithCommas(str1);
  replaceAllPointsWithCommas(str2);
  replaceAllPointsWithCommas(str3);
  replaceAllPointsWithCommas(str4);

  ck_assert_str_eq(str1, "Hello,World");
  ck_assert_str_eq(str2, "NoPointsHere");
  ck_assert_str_eq(str3, "Multiple,Points,In,String");
  ck_assert_str_eq(str4, "");
}
END_TEST

// Функция для регистрации тестов
Suite *tests_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Affine Transformations");

  /* Core test case */
  tc_core = tcase_create("Core");

  // Добавляем тесты в тестовый случай
  tcase_add_test(tc_core, test_moveX);
  tcase_add_test(tc_core, test_moveY);
  tcase_add_test(tc_core, test_moveZ);
  tcase_add_test(tc_core, test_rotateOX);
  tcase_add_test(tc_core, test_rotateOY);
  tcase_add_test(tc_core, test_rotateOZ);
  tcase_add_test(tc_core, test_scale);
  tcase_add_test(tc_core, test_scale_zero);
  tcase_add_test(tc_core, test_initData);
  tcase_add_test(tc_core, test_allocMatrix);
  tcase_add_test(tc_core, test_allocPolygons);
  tcase_add_test(tc_core, test_getCountOfVertexesAndFacets);
  tcase_add_test(tc_core, test_getVertexesAndFacets);
  tcase_add_test(tc_core, test_freeData);
  tcase_add_test(tc_core, test_replaceAllPointsWithCommas);

  // Добавляем тестовый случай в набор тестов

  suite_add_tcase(s, tc_core);
  tcase_set_timeout(tc_core, 10);  // Устанавливаем таймаут в 10 секунд

  return s;
}

int main(void) {
  int number_failed;

  Suite *s;
  SRunner *sr;

  s = tests_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}