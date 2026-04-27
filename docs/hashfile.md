# Documentação do HashFile

## Problema Original
Anteriormente, a implementação do `HashFile` utilizava um único arquivo para armazenar as seguintes estruturas:
1. O **Cabeçalho** (`Header`)
2. A **Tabela Hash / Diretório** (Ponteiros para as posições de memória dos buckets)
3. Os **Buckets** (As tabelas e dados reais contendo os registros)

Como no Hashing Estendido a tabela de diretórios precisa dobrar de tamanho ao expandir a profundidade global (`globalDepth`), o fato de tudo estar num só arquivo gerava um problema grave de fragmentação: toda vez que a tabela precisava aumentar, um novo espaço no final do arquivo precisava ser realocado (`fseek` para o final e `fwrite`), misturando a representação de diretórios novos com dados dos buckets antigos e criando "buracos" no disco, prejudicando tanto o desempenho quanto a integridade em arquivos longos.

## Solução Arquitetural
A solução para esse problema foi implementar a **separação lógica e física da estrutura de dados**. Para cada hashfile criado, o sistema agora gera e gerencia dois arquivos associados.

1. **Arquivo de Cabeçalho/Metadados (`[nome].hfc`)**
   Este arquivo contém exclusivamente a estrutura de cabeçalho (`Header`) e a tabela de diretório daquele Hash. Quando a tabela de diretório sofre expansão pelo aumento de volume de dados, basta reescrevê-la contiguamente nesse arquivo a partir da mesma posição (offset igual a `sizeof(Header)`), evitando fragmentações e isolando os metadados.

2. **Arquivo Principal / Tabelas de Dados (`[nome]`)**
   Este é o arquivo principal que preserva a mesma nomenclatura de criação, e armazena **apenas as tabelas de registro (Buckets)**. A alocação de novos buckets sempre ocorrerá organicamente através da expansão no final (via `append` lógico) do arquivo, resultando em um armazenamento muito mais rápido, escalável e fácil para iterar os dados armazenados.

## Transparência na Criação dos Arquivos (API)
Apesar da separação criar dois arquivos físicos no disco, a assinatura da API foi preservada para manter a simplicidade para quem for utilizar o componente. Ou seja, na `main.c`, o usuário continua enviando apenas um único parâmetro de nome:

```c
HashFile hashQuadra = criarHashFile("hashquadra.hf", tamanhoQuadra(), 16384);
```

Isso é possível pois, internamente (no arquivo `src/hashfile.c`), a função processa de forma autônoma o nome do segundo arquivo substituindo a extensão original pela nova extensão `.hfc`:
```c
char hdrName[256];
strcpy(hdrName, nome);
char *dot = strrchr(hdrName, '.');
if (dot) {
    strcpy(dot, ".hfc");
} else {
    strcat(hdrName, ".hfc");
}
```
Dessa forma:
- O arquivo `hashquadra.hf` original é instanciado como o arquivo de tabelas (Buckets).
- O arquivo derivado `hashquadra.hfc` é criado automaticamente para cuidar do Header e Diretório.
Essa transparência ocorre de forma idêntica tanto para o método `criarHashFile` quanto para o `lerHashFile`, não sendo necessário mudar a lógica na `main.c`.

## Atualização de Memória (`sHashFile`)
Para aplicar o suporte aos dois arquivos, a estrutura original em C foi adaptada em `hashfile.c` para comportar ponteiros de arquivos individualizados:

```c
typedef struct {
    Header header;
    long *directory;
    FILE *file;      // Ponteiro do arquivo que guarda os buckets
    FILE *hdrFile;   // Ponteiro do arquivo exclusivo de cabeçalho/diretórios
} sHashFile;
```

## Benefícios
- **Menor Complexidade no Disco**: A exclusão do gerenciamento de offsets contínuos da tabela hash do meio das páginas/buckets reduziu as complexidades de operações dinâmicas.
- **Isolamento e Segurança**: Em caso de corrupção ou necessidade de reparo, o arquivo que armazena os dados brutos (`[nome]`) não tem seu formato binário modificado por metadados e diretórios dinâmicos da hash table.
- **Escalabilidade**: A tabela agora pode escalar infinitamente no `.hfc` sempre em contiguidade sem sofrer os problemas inerentes ao sistema original.
