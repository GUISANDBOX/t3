#include "unity.h"
#include "calc.h"

void test_calcular_fatorial_simples(void) {
    TEST_ASSERT_EQUAL_INT(120, calcular_fatorial(5));
}

void test_calcular_fatorial_negativo(void) {
    TEST_ASSERT_EQUAL_INT(-1, calcular_fatorial(-1));
}

void test_desconto_para_menor_de_idade(void) {
    float valor = 100.0;
    float percentual = 15.0;
    int idade = 17;
    float resultado = calcular_desconto(valor, percentual, idade);
    TEST_ASSERT_FLOAT_WITHIN(0.0, 85.0, resultado); // Espera-se um desconto de 10%
}

void test_desconto_para_idade_normal(void) {
    float valor = 100.0;
    float percentual = 15.0;
    int idade = 30;
    float resultado = calcular_desconto(valor, percentual, idade);
    TEST_ASSERT_FLOAT_WITHIN(0.0, 100.0, resultado); // Espera-se sem desconto
}

void test_desconto_para_aposentado(void) {
    float valor = 100.0;
    float percentual = 15.0;
    int idade = 70;
    float resultado = calcular_desconto(valor, percentual, idade);
    TEST_ASSERT_FLOAT_WITHIN(0.0, 85.0, resultado); // Espera-se sem desconto
}

void test_desconto_para_dezoito(void) {
    float valor = 100.0;
    float percentual = 15.0;
    int idade = 18;
    float resultado = calcular_desconto(valor, percentual, idade);
    TEST_ASSERT_FLOAT_WITHIN(0.0, 100.0, resultado); // Espera-se um desconto de 10%
}

void test_desconto_inicio_aposentadoria(void) {
    float valor = 100.0;
    float percentual = 15.0;
    int idade = 65;
    float resultado = calcular_desconto(valor, percentual, idade);
    TEST_ASSERT_FLOAT_WITHIN(0.0, 85.0, resultado); // Espera-se um desconto de 10%
}

void test_valor_nulo(void) {
    void *ptr = NULL;
    TEST_ASSERT_NULL(ptr);
    // int z = 0;
    // TEST_ASSERT_NULL(z);
}

void test_desconto_valor_real(void) {
    float valor = 99.99;
    float percentual = 25.0;
    int idade = 15;
    float resultado = calcular_desconto(valor, percentual, idade);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 74.99, resultado); // Espera-se um desconto de 10%
}

// Unity requires these even if empty
void setUp(void) {}
void tearDown(void) {}

// simple test runner
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_calcular_fatorial_simples);
    RUN_TEST(test_desconto_para_menor_de_idade);
    RUN_TEST(test_desconto_para_idade_normal);
    RUN_TEST(test_desconto_para_aposentado);
    RUN_TEST(test_desconto_para_dezoito);
    RUN_TEST(test_desconto_inicio_aposentadoria);
    RUN_TEST(test_valor_nulo);
    RUN_TEST(test_desconto_valor_real);
    return UNITY_END();
}