PROJ_NAME=ted
ALUNO=
LIBS=
OBJETOS=$(SOURCES:.c=.o)

CC = gcc
CFLAGS = -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Iinclude
LDFLAGS = -O0
SOURCES = main.c $(wildcard src/*.c)

# Unity testing framework settings
UNITY_DIR = unity/src
UNITY_INC = -I$(UNITY_DIR)
TEST_SOURCES = test/test_calc.c
TEST_TARGET = run_tests

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME) $(LDFLAGS) $(OBJETOS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJETOS) $(PROJ_NAME)

run: $(PROJ_NAME)
	./$(PROJ_NAME)

# build and run unit tests
$(TEST_TARGET): src/calc.c $(TEST_SOURCES) $(UNITY_DIR)/unity.c
	$(CC) $(CFLAGS) $(UNITY_INC) -o $@ src/calc.c $(TEST_SOURCES) $(UNITY_DIR)/unity.c

.PHONY: all clean run $(TEST_TARGET)