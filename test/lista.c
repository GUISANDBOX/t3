#include "unity.h"
#include "Lista.h"

Lista lista;

void setUp(void) {
    printf("Setup: Criando lista...\n");
    lista = crialista();
}

void tearDown(void) {
    limpaLista(&lista);
    printf("Teardown: Lista limpa.\n");
}

void test_inicializacao_lista(void) {
    TEST_ASSERT_NULL(lista);
    TEST_ASSERT_TRUE(listavazia(lista));
}

void test_insert_item(void) {
    adicionar(&lista, "abcd", 0);
    TEST_ASSERT_NOT_NULL(lista);
    TEST_ASSERT_FALSE(listavazia(lista));
}

// simple test runner
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_inicializacao_lista);
    RUN_TEST(test_insert_item);
    return UNITY_END();
}