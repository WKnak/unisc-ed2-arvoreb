/* UNISC 2018/2
 * Trabalho para a disciplina de Estrutura de Dados II 
 * Árvore B 
 * WILLIAM KNAK
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#define GRAU 5   // máximo de apontamentos
#define MAX_ELEMENTOS GRAU-1

COORD coord = {0,0};

typedef enum { false, true } bool;

typedef int t_posicao;

typedef struct tipo_no {
    int nro_chaves;
    int chaves[GRAU];                   // máximo de chaves = GRAU - 1; (tem 1 de folga)
    struct tipo_no *aponta[GRAU+1];     // máximo de apontamentos = GRAU;  (tem 1 de folga)
    int x, y;                           // coordenadas para facilitar impressão
} tipo_no;

tipo_no *raiz;

void inicializaArvore() {
    raiz = malloc(sizeof(tipo_no));
    memset(raiz, 0, sizeof(tipo_no));
}

/**
 * Inserir uma chave na folha
 */
t_posicao inserirChaveNaFolha(tipo_no *folha, const int chave) {

    int i, j, posicao;

    posicao = folha->nro_chaves;

    if(folha->nro_chaves>0) {

        for(i=0; i<folha->nro_chaves; i++) {
            // localiza a posição para incluir a chave de forma ordenada
            if(chave < folha->chaves[i]) {
                // encontrou a posicao
                posicao = i;
                for(j=folha->nro_chaves; j>i; j--) {
                    // desloca as chaves para abrir espaço
                    folha->chaves[j] = folha->chaves[j-1];
                    // desloca os apontamentos para abrir espaço
                    folha->aponta[j+1] = folha->aponta[j];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        // WILLIAM KNAK UNISC 2018/2
                }
                break;
            }
        }
    }

    folha->chaves[posicao] = chave;
    folha->nro_chaves++;
    return posicao;
}

tipo_no* novoNodo() {
    tipo_no *novo = malloc(sizeof(tipo_no));
    memset(novo, 0, sizeof(tipo_no));
    return novo;
}

bool chaveExiste(int chave) {

    int i;
    tipo_no *aux = raiz;
    while(aux != NULL) {
        for(i=0; i < aux->nro_chaves; i++) {
            // tenta navegar pela esquerda
            if(chave < aux->chaves[i]) {
                aux = aux->aponta[i];
                if(aux==NULL) return false;
                i = 0;
                continue;
            }
            // chave igual, retornar true
            if(aux->chaves[i] == chave) return true;
        }
        // terminou o laço e não achou pela esquerda, tenta pela direita
        aux = aux->aponta[aux->nro_chaves];
    }
    return false;

}

void dividir(tipo_no *pai, tipo_no *filho) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        // WILLIAM KNAK UNISC 2018/2

    if( (pai != NULL) && (pai->nro_chaves < GRAU)) {
        if( pai == filho) {
            // nodo pai nao tem mais espaco
            tipo_no *pai = novoNodo();
        }
        // nodo pai consegue acomodar o filho do meio
        int itemDoMeio = floor((filho->nro_chaves)/2);
        int posicao = inserirChaveNaFolha(pai, filho->chaves[itemDoMeio]);

        tipo_no *dir = novoNodo();
        tipo_no *esq = novoNodo();

        int i, ini, fim;

        ini = 0; fim = itemDoMeio;
        for(i=ini; i<fim; i++) {
            // remonta os apontamentos pela esquerda, do novo NODO ESQUERDO
            esq->chaves[esq->nro_chaves] = filho->chaves[i];
            esq->aponta[esq->nro_chaves] = filho->aponta[i];
            esq->nro_chaves++;
        }
        // remonta o apontamento pela DIREITA, do novo NODO ESQUERDO
        esq->aponta[esq->nro_chaves] = filho->aponta[fim];

        ini = itemDoMeio+1; fim = filho->nro_chaves;
        for(i=ini; i<fim; i++) {
            // remonta os apontamentos pela ESQUERDA, do novo NODO DIREITO
            dir->chaves[dir->nro_chaves] = filho->chaves[i];
            dir->aponta[dir->nro_chaves] = filho->aponta[i];
            dir->nro_chaves++;
        }
        // remonta o apontamento pela DIREITA, do novo NODO DIREITO
        dir->aponta[dir->nro_chaves] = filho->aponta[fim]; //  dir->aponta[1]->chaves[0]

        pai->aponta[posicao] = esq;
        pai->aponta[posicao+1] = dir;

        free(filho);

    } else if(pai != NULL) {

        // nodo pai não tem mais espaço
        dividir(pai, pai);

    } else {

        // nodo pai não existe (raiz)
        tipo_no *novoPai = novoNodo();
        dividir(novoPai, filho);
        pai = novoPai;
        raiz = novoPai;

    }
}

/**
 * Localiza o Nodo/Folha apropriado para incluir a chave (recursividade)
 */
void inserirRecursivo(tipo_no *paiRef, tipo_no *no, const int chave) {

    bool isFolha = (no->aponta[0] == NULL);
    bool precisaDividir = false;
    bool achouPelaEsquerda = false;

    if(isFolha) {
        // FOLHA -> não possui apontamentos, pode incluir aqui
        inserirChaveNaFolha(no, chave);
        precisaDividir = (no->nro_chaves > MAX_ELEMENTOS);
        if(precisaDividir) {
            dividir(paiRef, no); //paiRef->chaves[0]
        }
        return;
    } else {
        // ... continua procurando uma folha adequada
        int i;
        for(i=0; i < no->nro_chaves; i++) {
            // tenta ir pela esquerda
            if(chave < no->chaves[i]) {
                achouPelaEsquerda = true;
                inserirRecursivo(no, no->aponta[i], chave);
                precisaDividir = (no->nro_chaves > MAX_ELEMENTOS);
                if(precisaDividir) dividir(paiRef, no);
                break;
            }
        }
        // se não achou na esquerda, vai pela direita
        if(!achouPelaEsquerda) {
            int ultimo = no->nro_chaves;
            inserirRecursivo(no, no->aponta[ultimo], chave);
            precisaDividir = (no->nro_chaves > MAX_ELEMENTOS);
            if(precisaDividir) dividir(paiRef, no);
        }
    }
}

void gotoxy(const int x, const int y)
{
    coord.X = ((x-1) * 3) + ((x-1) * 2);
    coord.Y = y*4;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        // WILLIAM KNAK UNISC 2018/2
}

/**
 * Função para inserir uma chave na árvore
 */
void inserir(const int chave) {

    int i;
    if(!chaveExiste(chave)) {
        inserirRecursivo(NULL, raiz, chave);
    }
}

/**
 *  calcula uma posição X/Y para imprimir
 */
int calcularPosicao(tipo_no *nodo, int x, int y) {

    int i;

    bool isFolha = (nodo->aponta[0] == NULL);
    if(!isFolha) {
        for(i=0; i <= nodo->nro_chaves; i++) {

            nodo->x = x;
            nodo->y = y;

            x = calcularPosicao(nodo->aponta[i], x, y+1); //raiz->aponta[1]->x

        }
    } else {
        x += nodo->nro_chaves;
        nodo->x = x;
        nodo->y = y;
    }
    return x;
}

/**
 * Imprimir o nodo atual (recursividade);
 * X e Y são posições centrais no topo da tela
 */
void imprimirNodo(tipo_no *nodo) {

    int j;

    bool isFolha = (nodo->aponta[0] == NULL);

    gotoxy(nodo->x, nodo->y);
    printf("[");
    for(j=0; j < nodo->nro_chaves; j++) {
        if(j>0) printf("|");
        //printf("%d", nodo->x);
        printf("%d", nodo->chaves[j]);
    }
    printf("] ");

    if(!isFolha) {

        for(j=0; j <= nodo->nro_chaves; j++) {
            imprimirNodo(nodo->aponta[j]);
        }
    }

}

/**
 * Imprimir toda a árvore
 */
void imprimirArvore() {

    //system('cls');
    calcularPosicao(raiz, 1, 1);
    imprimirNodo(raiz);
    gotoxy(0, 20); printf("\n");

}

int main() {

    inicializaArvore();

    /*
    inserir(310);
    inserir(550);

    inserir(750);

    inserir(499);
    inserir(777);
    inserir(100);

    inserir(101);
    inserir(498);

    inserir(99);
    inserir(124);


    inserir(311);
    inserir(309);
    inserir(480);
    inserir(721);*/


    int i;
    int r;
    printf("aleatorios: ");

    for (i=0; i<50; i++) {
        r = (rand() % 89) + 10;
        if(chaveExiste(r)) continue;
        printf("%d -> ", r);
        inserir(r);
    }
    printf("\n\n");

    /*
    inserir(311);
    inserir(309);
    inserir(480);
    inserir(721);
    inserir(722); */

    imprimirArvore();

    return 0;
}
