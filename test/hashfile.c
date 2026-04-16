#include <stdio.h>
#include "unity.h"
#include "hashfile.h"
#include "pessoa.h"
#include "quadra.h"

HashFile hash;

void setUp(void) {

}

void tearDown(void) {
    //destruirHashFile(hash);
}

void test_tamanho_struct(void) {
    Pessoa p = criaPessoa("12345678901", "Joaoooooooooooo", "Silva", 'M', "01012000");
    Quadra q = criaQuadra("123456780000", 10.0, 20.0, 30.0, 40.0, "sw", "fill", "stroke");

    printf("Tamanho real da struct Pessoa: %zu bytes\n", tamanhoPessoa());
    printf("Tamanho real da struct Quadra: %zu bytes\n", tamanhoQuadra());

    destruirPessoa(p);
    destruirQuadra(q);
}

void test_inicializacao_hash_d2(void) {
    printf("Iniciando teste de inicialização do hashfile...\n");
    hash = criarHashFile("output/hashfile.dat", tamanhoQuadra(), 2048);
    hash = lerHashFile("output/hashfile.dat");
    TEST_ASSERT_NOT_NULL(hash);
}

void test_inserir_item_hash(void) {
    hash = lerHashFile("output/hashfile.dat");
    Quadra q1 = criaQuadra("123456780000", 10.0, 20.0, 30.0, 40.0, "sw", "fill", "stroke");
    Quadra q2 = criaQuadra("123456780001", 10.0, 20.0, 30.0, 40.0, "sw", "fill", "stroke");
    Quadra q3 = criaQuadra("123456780002", 10.0, 20.0, 30.0, 40.0, "sw", "fill", "stroke");
    int res1 = adicionarHashItem(&hash, q1);
    int res2 = adicionarHashItem(&hash, q2);
    int res3 = adicionarHashItem(&hash, q3);
    printHashFileInfo(hash);
    TEST_ASSERT_NOT_NULL(hash);
}
// void test_buscar_item_existente_hash(void) {
//     hash = criarHashFile("hashfile.dat", 2);
//     int item1 = 1234;
//     int res1 = adicionarHashItem(&hash, &item1);
//     TEST_ASSERT_EQUAL_INT(1, res1); // Supondo que 1 indica sucesso
//     HashItem encontrado = buscarHashItem(hash, item1);
//     TEST_ASSERT_NOT_NULL(encontrado);
// }

// void test_buscar_item_inexistente_hash(void) {
//     hash = criarHashFile("hashfile.dat", 2);
//     int item1 = 1234;
//     int res1 = adicionarHashItem(&hash, &item1);
//     TEST_ASSERT_EQUAL_INT(1, res1); // Supondo que 1 indica sucesso
//     int item2 = 5678;
//     HashItem naoencontrado = buscarHashItem(hash, item2);
//     TEST_ASSERT_NULL(naoencontrado);
// }


// simple test runner
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_tamanho_struct);
    RUN_TEST(test_inicializacao_hash_d2);
    RUN_TEST(test_inserir_item_hash);
    // RUN_TEST(test_buscar_item_existente_hash);
    // RUN_TEST(test_buscar_item_inexistente_hash);
    return UNITY_END();
}