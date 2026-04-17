#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hashfile.h"
#include "item.h"
#include "comandosgeo.h"
#include "comandospm.h"
#include "comandosqry.h"
#include "quadra.h"
#include "pessoa.h"

#define PATH_LEN 250
#define FILE_NAME_LEN 100
#define MSG_LEN 1000

void trataPath(char *path, int tamMax, char* arg){
 int argLen = strlen(arg);
 assert(argLen<tamMax);
 if(arg[argLen-1]=='/'){
 arg[argLen-1]='\0';
 }
 strcpy(path,arg);
}

void trataNomeArquivo(char *path, int tamMax, char* arg){
 sprintf(path,"%s",arg);
}

char *getNomeArquivoSemExtensao(const char *caminhoCompleto) {
    static char nomeArquivoSemExtensao[FILE_NAME_LEN];

    // 1. Achar o último separador de diretório ('/' ou '\')
    const char *barra = strrchr(caminhoCompleto, '/');
#ifdef _WIN32
    const char *barra2 = strrchr(caminhoCompleto, '\\');
    if (barra2 && (!barra || barra2 > barra))
        barra = barra2;
#endif

    // Se houver diretório, o nome começa depois da barra
    const char *nome = (barra) ? barra + 1 : caminhoCompleto;

    // 2. Procurar o último ponto após o nome (para extensão)
    const char *ponto = strrchr(nome, '.');

    size_t len;
    if (ponto && ponto > nome)
        len = ponto - nome;  // comprimento até o ponto
    else
        len = strlen(nome);  // sem extensão

    if (len >= FILE_NAME_LEN)
        len = FILE_NAME_LEN - 1;  // evitar overflow

    strncpy(nomeArquivoSemExtensao, nome, len);
    nomeArquivoSemExtensao[len] = '\0';

    return nomeArquivoSemExtensao;
}

int main(int argc, char *argv[]) {
    char dir[PATH_LEN], bed[PATH_LEN], arq[FILE_NAME_LEN], arqquery[FILE_NAME_LEN], arqpm[FILE_NAME_LEN], dirsaidaqry[PATH_LEN], dirsaida[PATH_LEN], dirsaidabase[PATH_LEN];
    char *fullNameArq;

    int z;
    int temquery = 0;
    int tempm = 0;
    int temf = 0;
    int temo = 0;
    int teme = 0;
    char comando[3];
    char a;
    int i=1;
    float x, y, x1, x2, y1, y2, r, w, h;
    char corb[100], corp[100], cor[100], txto[100], fFamily[100], fWeight[100], fSize[100];
    
    while (i < argc){
         if (strcmp(argv[i],"-e")==0){
             i++;
             /* se i >= argc: ERRO-falta parametro */
             trataPath(dir,PATH_LEN, argv[i]);
             strcpy(bed, dir);
             teme = 1;
         }
         else if (strcmp(argv[i],"-f") == 0){
             i++;
             /* se i >= argc: ERRO-falta parametro */
             trataNomeArquivo(arq,FILE_NAME_LEN,argv[i]);
             printf("LIDO ARQ F: %s\n", arq);
             temf = 1;
         }
         else if (strcmp(argv[i],"-o") == 0){
             i++;
             /* se i >= argc: ERRO-falta parametro */
             trataNomeArquivo(dirsaida,PATH_LEN,argv[i]);
             strcpy(dirsaidabase, dirsaida);
             temo = 1;
         }
         else if (strcmp(argv[i],"-q") == 0){
             i++;
             /* se i >= argc: ERRO-falta parametro */
             trataNomeArquivo(arqquery,FILE_NAME_LEN,argv[i]);
             temquery = 1;
         }
         else if (strcmp(argv[i],"-pm") == 0){
             i++;
             /* se i >= argc: ERRO-falta parametro */
             trataNomeArquivo(arqpm,FILE_NAME_LEN,argv[i]);
             tempm = 1;
         }
         i++;
     } //while

    if (!temf || !temo) {
        printf("Parâmetros -f e -o são obrigatórios.\n");
        return 1;
    }

    if (!teme) {
        strcpy(dir, ".");
        strcpy(bed, ".");
    }

    strcat(dir,"/");strcat(dir,arq);
    // TODO - fazer com que o nome do arquivo svg seja o nome do arquivo geo com extensão .svg
    char arquivoSVGArena[FILE_NAME_LEN];
    strcpy(arquivoSVGArena, getNomeArquivoSemExtensao(arq));
    strcat(arquivoSVGArena, ".svg");
    strcat(dirsaida,"/");
    strcat(dirsaida,arquivoSVGArena);

    FILE *arqgeo = fopen(dir, "r"); // exemplo.geo e retg-decres.geo
    FILE *arqnovo = fopen(dirsaida, "w+");

    if (!arqgeo) {
        printf("Erro ao abrir o arquivo GEO  %s\n", dir);
        return 1;
    }
    
    HashFile hashQuadra = criarHashFile("hashquadra.hf", tamanhoQuadra(), 2048);
    processaGeo(arqgeo, hashQuadra);

    HashFile hashPessoa = criarHashFile("hashpessoa.hf", tamanhoPessoa(), 2048);
    if (tempm) {
        char dirpm[PATH_LEN];
        strcpy(dirpm, bed);
        strcat(dirpm, "/");
        strcat(dirpm, arqpm);
        FILE *arqpm_file = fopen(dirpm, "r");
        if (!arqpm_file) {
            printf("Erro ao abrir o arquivo PM %s\n", dirpm);
            return 1;
        }
        processaPm(arqpm_file, hashPessoa);
        fclose(arqpm_file);
    }

    printHashFileInfo(hashQuadra);
    printHashFileInfo(hashPessoa);

    fclose(arqgeo);
    fclose(arqnovo);
    printf("Arquivo SVG criado!\n");

    if (!temquery) {
        printf("Nenhum arquivo QRY fornecido. Encerrando.\n");
        return 0;
    }

    strcat(bed,"/");strcat(bed,arqquery);

    char arquivoSVGquery[FILE_NAME_LEN];
    char arquivoTXTquery[FILE_NAME_LEN];
    char dirsaidabaseaux[PATH_LEN];
    strcpy(dirsaidabaseaux, dirsaidabase);
    strcpy(arquivoSVGquery, getNomeArquivoSemExtensao(arq));
    strcat(arquivoSVGquery, "-");
    strcat(arquivoSVGquery, getNomeArquivoSemExtensao(arqquery));
    strcat(arquivoSVGquery, ".svg");

    strcpy(arquivoTXTquery, getNomeArquivoSemExtensao(arquivoSVGquery));
    strcat(arquivoTXTquery, ".txt");

    strcat(dirsaidabase, "/");
    strcat(dirsaidabase, arquivoSVGquery);

    strcat(dirsaidabaseaux, "/");
    strcat(dirsaidabaseaux, arquivoTXTquery);

    FILE *fileq = fopen(bed, "r");
    if (!fileq) {
        printf("OCORREU UM ERRO AO LER O QUERY\n");
        return 1;
    }
    printf("Criando SVG %s \n", dirsaidabase);
    printf("Criando TXT %s \n", dirsaidabaseaux);
    FILE *filesaidaquery = fopen(dirsaidabase, "w+");
    FILE *filesaidatxt = fopen(dirsaidabaseaux, "w+");
    processaQry(fileq, hashPessoa, hashQuadra, filesaidatxt);

    destruirHashFile(&hashQuadra);
    destruirHashFile(&hashPessoa);
    
    fclose(fileq);
    fclose(filesaidaquery);
    fclose(filesaidatxt);
    printf("Sucesso!\n");

    return 0;
}