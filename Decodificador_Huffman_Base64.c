/*--------------------------------------------------------------
 * PROCESSAMENTO DE IMAGEM -                                               
 * Atividade 3 - Decodificador Huffman - Base64
 * Aluno: Lucas Bertoni Scarton
 *--------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAXSYMBS 256
#define MAXNODES 511
#define CREATOR "# CREATOR: Lucas Bertoni Scarton - 2020\n"

typedef int *imagem;

typedef struct no_ no;
typedef struct bancoDados_ bancoDados;

struct no_ {
    int valor;
    no *prox;
};

struct bancoDados_ {
    char parametro;
    int cod[6];
};

bancoDados dado[64] = {
    {'A', {0, 0, 0, 0, 0, 0}},
    {'B', {0, 0, 0, 0, 0, 1}},
    {'C', {0, 0, 0, 0, 1, 0}},
    {'D', {0, 0, 0, 0, 1, 1}},
    {'E', {0, 0, 0, 1, 0, 0}},
    {'F', {0, 0, 0, 1, 0, 1}},
    {'G', {0, 0, 0, 1, 1, 0}},
    {'H', {0, 0, 0, 1, 1, 1}},
    {'I', {0, 0, 1, 0, 0, 0}},
    {'J', {0, 0, 1, 0, 0, 1}},
    {'K', {0, 0, 1, 0, 1, 0}},
    {'L', {0, 0, 1, 0, 1, 1}},
    {'M', {0, 0, 1, 1, 0, 0}},
    {'N', {0, 0, 1, 1, 0, 1}},
    {'O', {0, 0, 1, 1, 1, 0}},
    {'P', {0, 0, 1, 1, 1, 1}},
    {'Q', {0, 1, 0, 0, 0, 0}},
    {'R', {0, 1, 0, 0, 0, 1}},
    {'S', {0, 1, 0, 0, 1, 0}},
    {'T', {0, 1, 0, 0, 1, 1}},
    {'U', {0, 1, 0, 1, 0, 0}},
    {'V', {0, 1, 0, 1, 0, 1}},
    {'W', {0, 1, 0, 1, 1, 0}},
    {'X', {0, 1, 0, 1, 1, 1}},
    {'Y', {0, 1, 1, 0, 0, 0}},
    {'Z', {0, 1, 1, 0, 0, 1}},
    {'a', {0, 1, 1, 0, 1, 0}},
    {'b', {0, 1, 1, 0, 1, 1}},
    {'c', {0, 1, 1, 1, 0, 0}},
    {'d', {0, 1, 1, 1, 0, 1}},
    {'e', {0, 1, 1, 1, 1, 0}},
    {'f', {0, 1, 1, 1, 1, 1}},
    {'g', {1, 0, 0, 0, 0, 0}},
    {'h', {1, 0, 0, 0, 0, 1}},
    {'i', {1, 0, 0, 0, 1, 0}},
    {'j', {1, 0, 0, 0, 1, 1}},
    {'k', {1, 0, 0, 1, 0, 0}},
    {'l', {1, 0, 0, 1, 0, 1}},
    {'m', {1, 0, 0, 1, 1, 0}},
    {'n', {1, 0, 0, 1, 1, 1}},
    {'o', {1, 0, 1, 0, 0, 0}},
    {'p', {1, 0, 1, 0, 0, 1}},
    {'q', {1, 0, 1, 0, 1, 0}},
    {'r', {1, 0, 1, 0, 1, 1}},
    {'s', {1, 0, 1, 1, 0, 0}},
    {'t', {1, 0, 1, 1, 0, 1}},
    {'u', {1, 0, 1, 1, 1, 0}},
    {'v', {1, 0, 1, 1, 1, 1}},
    {'w', {1, 1, 0, 0, 0, 0}},
    {'x', {1, 1, 0, 0, 0, 1}},
    {'y', {1, 1, 0, 0, 1, 0}},
    {'z', {1, 1, 0, 0, 1, 1}},
    {'0', {1, 1, 0, 1, 0, 0}},
    {'1', {1, 1, 0, 1, 0, 1}},
    {'2', {1, 1, 0, 1, 1, 0}},
    {'3', {1, 1, 0, 1, 1, 1}},
    {'4', {1, 1, 1, 0, 0, 0}},
    {'5', {1, 1, 1, 0, 0, 1}},
    {'6', {1, 1, 1, 0, 1, 0}},
    {'7', {1, 1, 1, 0, 1, 1}},
    {'8', {1, 1, 1, 1, 0, 0}},
    {'9', {1, 1, 1, 1, 0, 1}},
    {'+', {1, 1, 1, 1, 1, 0}},
    {'/', {1, 1, 1, 1, 1, 1}},
};

typedef struct {
    int freq;
    int father;
    int left;
    int right;
    int value;
} nodetype;

nodetype node[MAXNODES];

int pqmindelete(int *rootnodes) {
    int aux;
    aux = *rootnodes;
    *rootnodes = node[*rootnodes].father;
    return aux;
}

int pqinsert(int *rootnodes, int i) {
    int k, ant, aux;
    aux = *rootnodes;
    ant = -1;
    while (aux != -1 && node[aux].freq <= node[i].freq) {
        ant = aux;
        aux = node[aux].father;
    }
    if (ant == -1) {
        node[i].father = *rootnodes;
        *rootnodes = i;
    } else {
        node[i].father = node[ant].father;
        node[ant].father = i;
    }
}

/*--------------------------------------------------------------
 * buildTree
 * Funcao que constroi a arvore de huffman
 * Parametros:
 *    h  - vetor de histograma (frequencia de cada pixel 0..mn+1)
 *    mn - maximo nivel de cinza
 * Retorna a raiz da árvore construida (2 * mn)
 *--------------------------------------------------------------*/
int buildTree(int *h, int mn) {
    int i, k, p, p1, p2, rootnodes = -1, root, father;
    for (i = 0; i < mn + 1; i++) {
        node[i].freq = h[i];
        node[i].left = -1;
        node[i].right = -1;
        node[i].value = i;
        pqinsert(&rootnodes, i);
    }
    for (p = mn + 1; p < 2 * (mn + 1) - 1; p++) {
        p1 = pqmindelete(&rootnodes);
        p2 = pqmindelete(&rootnodes);
        node[p1].father = p;
        node[p2].father = p;
        node[p].freq = node[p1].freq + node[p2].freq;
        node[p].left = p1;
        node[p].right = p2;
        pqinsert(&rootnodes, p);
    }
    return pqmindelete(&rootnodes);
}

/*--------------------------------------------------------------
 * drawLinks, createDot
 * Rotinas que geram uma visualização da árvore de huffman
 * Para produzir uma imagem, deve-se ter graphViz instalado
 * e digitar em linha de comando:
 *    dot -Tpng tree.dot -o tree.png
 *--------------------------------------------------------------*/
void drawLinks(FILE *dot, int root) {
    if (root != -1) {
        int hasLeft = node[root].left != -1;
        int hasRight = node[root].right != -1;
        if (!hasLeft && !hasRight)
            fprintf(dot, "\tn%d [shape = record, label=\"%d|%d\"]\n", root, root, node[root].freq);
        else
            fprintf(dot, "\tn%d [shape = circle, label=\"%d\"]\n", root, node[root].freq);
        drawLinks(dot, node[root].left);
        if (hasLeft)
            fprintf(dot, "\tn%d -> n%d [label=0]\n", root, node[root].left);
        drawLinks(dot, node[root].right);
        if (hasRight)
            fprintf(dot, "\tn%d -> n%d [label=1]\n", root, node[root].right);
    }
}

void error(char *s) {
    puts(s);
    exit(10);
}

void createDot(int root) {
    int i;
    FILE *dot;
    dot = fopen("tree.dot", "wt");
    if (!dot)
        error("Erro na criação do arquivo .DOT");
    fprintf(dot, "digraph {\n");
    fprintf(dot, "\tnode [fontename=\"Arial\"];\n");
    drawLinks(dot, root);
    fprintf(dot, "}\n");
    fclose(dot);
}



//Alocar memória para imagem PGM
int aloca_memo(imagem *Img, int nl, int nc) {
    *Img = (int *) malloc(nl * nc * sizeof (int));
    if (*Img)
        return TRUE;
    else
        return FALSE;
}

//Desalocar memória para imagem PGM
int desaloca_memo(imagem *Img) {
    free(*Img);
}

//Gravar imagem em PGM
void grava_imagem_pgm(imagem Img, char *nome, int nl, int nc, int mn) {
    int i, j, x, k, valores_por_linha;
    FILE *arq;
    if ((arq = fopen(nome, "wt")) == NULL) {
        printf("Erro na CRIACAO do arquivo <%s>\n\n", nome);
    } else {
        fputs("P2\n", arq);
        fputs(CREATOR, arq);
        fprintf(arq, "%d  %d\n", nc, nl);
        fprintf(arq, "%d\n", mn);
        valores_por_linha = 16;
        for (i = 0, k = 0; i < nl; i++)
            for (j = 0; j < nc; j++) {
                x = *(Img + i * nc + j);
                fprintf(arq, "%3d ", x);
                k++;
                if (k > valores_por_linha) {
                    fprintf(arq, "\n");
                    k = 0;
                }
            }
    }
    fclose(arq);
}

//Inserindo numa lista
void insere(no **inicio, no **fim, int cod) {

    if (*inicio == NULL) {
        *inicio = malloc(sizeof (no));
        (*inicio)->valor = cod;
        (*inicio)->prox = NULL;
        *fim = *inicio;
    } else {
        no *n;
        n = malloc(sizeof (no));
        n->valor = cod;
        (*fim)->prox = n;
        n->prox = NULL;
        *fim = (*fim)->prox;
        //printf("%d", (*fim)->valor);
    }
}

//Consultando tabela e descriptografando
int descrip(char parametro, int* cod) {
    int i, aux = 0;
    for (i = 0; i < 64; i++) {
        if (dado[i].parametro == parametro) {
            cod[0] = dado[i].cod[0];
            cod[1] = dado[i].cod[1];
            cod[2] = dado[i].cod[2];
            cod[3] = dado[i].cod[3];
            cod[4] = dado[i].cod[4];
            cod[5] = dado[i].cod[5];
            aux = 1;
        }
    }
    return aux;
}

void hb64_pgm(char *nome) {
    int i, j, nc, nl, mxc;
    char vetor[100];
    FILE *arq;

    //Abertura do arquivo
    if ((arq = fopen(nome, "r")) == NULL)
        printf("Falha na abertura do arquivo <%s>\n\n", nome);

    //Formato do arquivo
    fgets(vetor, 80, arq);
    if (!strstr(vetor, "hb64"))
        printf("Falha no formato do arquivo <%s>\n\n", nome);

    //Dimensões da imagem
    fgets(vetor, 80, arq);
    do {
        fgets(vetor, 80, arq);
    } while (strchr(vetor, '#'));
    sscanf(vetor, "%d %d", &nc, &nl);
    fscanf(arq, "%d", &mxc);

    //Histograma
    int hist[mxc + 1];
    for (i = 0; i < mxc + 1; i++) {
        fscanf(arq, "%d", &hist[i]);
    }

    //Descriptografando e colocando numa Lista;
    char caractere;
    no * inicio;
    no * fim;
    inicio = NULL;
    fim = NULL;
    int cod[6];

    while ((caractere = getc(arq)) != EOF) {
        if (descrip(caractere, cod)) {
            for (i = 0; i < 6; i++) {
                insere(&inicio, &fim, cod[i]);
            }
        }
    }

    //Conferindo árvore
    int raiz, raizAux, esq, dir;
    imagem out;
    aloca_memo(&out, nl, nc);
    i = 0;
    j = 0;

    raiz = buildTree(hist, mxc);
    raizAux = raiz;
    no *lista = inicio;

    while (lista->prox != NULL) {

        if (lista->valor == 0) {
            raizAux = node[raizAux].left;
            if (node[raizAux].left == -1 && node[raizAux].right == -1) {
                out[i * nc + j] = node[raizAux].value;
                if (j == (nc - 1)) {
                    j = 0;
                    i++;
                } else {
                    j++;
                }
                raizAux = raiz;
            }
        } else {
            raizAux = node[raizAux].right;
            if (node[raizAux].left == -1 && node[raizAux].right == -1) {
                out[i * nc + j] = node[raizAux].value;
                if (j == (nc - 1)) {
                    j = 0;
                    i++;
                } else {
                    j++;
                }
                raizAux = raiz;
            }
        }
        lista = lista->prox;
    }

    //Gravando imagem PGM
    strcat(nome, "-PGM.pgm");
    grava_imagem_pgm(out, nome, nl, nc, mxc);

    //Encerrando
    fclose(arq);
    desaloca_memo(&out);
}

void msg(char *s) {
    printf("\nConversor Huffman-Base64 para PGM");
    printf("\n---------------------------------");
    printf("\nUSO.:  %s  <IMG.HB64>", s);
    printf("\nONDE:\n");
    printf("    <IMG.HB64>  Arquivo da imagem em formato HB64\n\n");
}

/*+------------------------------------------------------+
  |        P R O G R A M A    P R I N C I P A L          |
  +------------------------------------------------------+*/
int main(int argc, char *argv[]) {
    int nc, nl, mn;
    char nome[100] = "./teste.pgm.hb64";
    if (argc < 2)
        msg(argv[0]);
    else
        strcpy(nome, argv[1]);
    printf("\nDecodificador HB64\n");
    hb64_pgm(nome);
    return 0;
}