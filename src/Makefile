# all, install, uninstall, clean, dvi, dist, test, gcov_report.
CSTD = -std=c11
CPPSTD = -std=c++17 -lstdc++
CFLAGS = -Wall -Wextra -Werror

TARGET := 3DViewer

MODEL_FILES = model/*.c
MODEL_HEADERS = model/*.h
MODEL_TEST_FILES = tests/*.c
MODEL_TEST_HEADERS = tests/*.h

VIEW_FILES = view/*.cpp
VIEW_HEADERS = view/*.h
VIEW_UI = view/*.ui
 
ALL_FILES = $(MODEL_FILES) $(MODEL_HEADERS) $(MODEL_TEST_FILES) $(MODEL_TEST_HEADERS) $(VIEW_FILES) $(VIEW_HEADERS)

CMAKE_BUILD_DIR = view/build

PARALLEL_JOBS = 8

GCOVFLAGS = -fprofile-arcs -ftest-coverage 
DIST_DIR = archive

.PHONY: test

UNAME_S = $(shell uname)
USER = $(shell whoami)

ifeq ($(UNAME_S),Darwin)
OPEN_CMD = open
ADD_LDFLAGS = -lgtest
MACDIR = $(TARGET).app
endif

ifeq ($(UNAME_S),Linux)
OPEN_CMD = xdg-open
ADD_LDFLAGS = -lgtest -lm -lsubunit
MACDIR = $(TARGET)
endif

all: install

install:
	@echo "Установка десктопного приложения..."
	@mkdir -p build
	@mkdir -p $(CMAKE_BUILD_DIR)
	@cmake -B $(CMAKE_BUILD_DIR) -S view || { echo "Ошибка CMake"; exit 1; }
	@cmake --build $(CMAKE_BUILD_DIR) --target $(TARGET) --parallel $(PARALLEL_JOBS) || { echo "Сборка завершилась с ошибкой"; exit 1; }
	@if [ "$(CMAKE_BUILD_DIR)/$(MACDIR)" ]; then \
		echo "Копирование $(TARGET) в build/$(TARGET)"; \
		cp -R $(CMAKE_BUILD_DIR)/$(MACDIR) build/$(MACDIR); \
		echo "Копирование $(TARGET) завершено"; \
	else \
		echo "Целевой объект $(TARGET) не найден в директории сборки."; \
		exit 1; \
	fi
	@rm -rf $(CMAKE_BUILD_DIR)

uninstall:
	rm -rf build

clean:
	rm -f $(TARGET)_test_file
# && rm -f *.txt
	rm -rf build
	rm -rf ${DIST_DIR}.tar.gz
	make clean_gcov
	rm -f test.info
	rm -f $(TARGET)_test_file
	rm -f *.log

dvi:
	$(OPEN_CMD) doc/index.html

dist:
	mkdir -p ${DIST_DIR}
	cp -rf ./ ${DIST_DIR}/$(TARGET)/
	cp -rf Makefile ${DIST_DIR}/
	tar -czvf ${DIST_DIR}.tar.gz ${DIST_DIR}
	rm -rf ${DIST_DIR}

test:
	gcc -g $(CFLAGS) $(CSTD) $(MODEL_FILES) $(MODEL_TEST_FILES) -o $(TARGET)_test_file -lcheck $(ADD_LDFLAGS) 
	./$(TARGET)_test_file

gcov_report: clean
	mkdir -p gcov
	gcc $(CFLAGS) $(CSTD) $(GCOVFLAGS) $(MODEL_FILES) $(MODEL_TEST_FILES) -o gcov_test -lcheck $(ADD_LDFLAGS)
	./gcov_test
	mv gcov_test *.gcno *.gcda ./gcov
	lcov --capture --directory ./gcov --output-file ./gcov/coverage.info
	genhtml -o ./gcov/report ./gcov/coverage.info
	$(OPEN_CMD) ./gcov/report/index.html

clang_format:
# cp ../materials/linters/.clang-format .
	clang-format -i --style=Google $(ALL_FILES)
# rm -f .clang-format

clang_check:
# cp ../materials/linters/.clang-format .
	clang-format -n --style=Google $(ALL_FILES)
# rm -f .clang-format

leaks_test: clean test
	leaks -atExit -- ./$(TARGET)_test_file

valgrind_test: clean test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./$(TARGET)_test_file

clean_gcov:
	rm -rf gcov*
