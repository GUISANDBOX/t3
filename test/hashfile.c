#include "unity.h"
#include "hashfile.h"

HashFile hash;

void setUp(void) {

}

void tearDown(void) {
    destruirHashFile(&hash);
}

void test_inicializacao_hash_d2(void) {
    hash = criarHashFile("hashfile.dat", 2);
    int bucketsLength = getBucketsLength(hash);
    TEST_ASSERT_EQUAL_INT(4, bucketsLength); // Espera-se 4 buckets para profundidade 2
    TEST_ASSERT_NOT_NULL(hash);
}

void test_inserir_item_hash(void) {
    hash = criarHashFile("hashfile.dat", 2);
    char *item = "teste";
    int res = adicionarHashItem(&hash, item);
    TEST_ASSERT_EQUAL_INT(1, res); // Supondo que 1 indica sucesso
}

void test_buscar_item_existente_hash(void) {
    hash = criarHashFile("hashfile.dat", 2);
    int item1 = 1234;
    int res1 = adicionarHashItem(&hash, &item1);
    TEST_ASSERT_EQUAL_INT(1, res1); // Supondo que 1 indica sucesso
    HashItem encontrado = buscarHashItem(hash, item1);
    TEST_ASSERT_NOT_NULL(encontrado);
}

void test_buscar_item_inexistente_hash(void) {
    hash = criarHashFile("hashfile.dat", 2);
    int item1 = 1234;
    int res1 = adicionarHashItem(&hash, &item1);
    TEST_ASSERT_EQUAL_INT(1, res1); // Supondo que 1 indica sucesso
    int item2 = 5678;
    HashItem naoencontrado = buscarHashItem(hash, item2);
    TEST_ASSERT_NULL(naoencontrado);
}


// simple test runner
int main(void) {
    UNITY_BEGIN();
    // RUN_TEST(test_inicializacao_hash_d2);
    // RUN_TEST(test_inserir_item_hash);
    // RUN_TEST(test_buscar_item_existente_hash);
    // RUN_TEST(test_buscar_item_inexistente_hash);
    return UNITY_END();
}