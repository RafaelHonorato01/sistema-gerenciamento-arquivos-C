#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_FILHOS 10

typedef struct Nodo
{
    char nome[100];
    bool diretorio;
    struct Nodo *filho[NUM_FILHOS];
    int countFilho;
    struct Nodo *irmao;
} Nodo;

Nodo *gerarNodo(char *nome, bool diretorio, Nodo *irmao)
{
    Nodo *novoNodo = (Nodo *)malloc(sizeof(Nodo));
    strcpy(novoNodo->nome, nome);
    novoNodo->diretorio = diretorio;
    novoNodo->countFilho = 0;
    novoNodo->irmao = irmao;
    return novoNodo;
}

void exibirPastaAtual(Nodo *caminho)
{
    if (caminho->irmao != NULL)
    {
        exibirPastaAtual(caminho->irmao);
        printf("-%s", caminho->nome);
    }
}

void sortfilho(Nodo* caminho) {
    for (int i = 0; i < caminho->countFilho - 1; i++) {
        for (int j = 0; j < caminho->countFilho - i - 1; j++) {
            if (strcasecmp(caminho->filho[j]->nome, caminho->filho[j + 1]->nome) > 0) {
                Nodo* temp = caminho->filho[j];
                caminho->filho[j] = caminho->filho[j + 1];
                caminho->filho[j + 1] = temp;
            }
        }
    }
}

void ls(Nodo *caminho)
{
    sortfilho(caminho);

    for (int i = 0; i < caminho->countFilho; i++)
    {
        printf("%s", caminho->filho[i]->nome);
        if (caminho->filho[i]->diretorio)
        {
            printf("-");
        }
        printf("\n");
    }
}

void gerarArquivo(Nodo *caminho, char *filenome)
{
    if (caminho->countFilho < NUM_FILHOS)
    {
        Nodo *novoArquivo = gerarNodo(filenome, false, caminho);
        caminho->filho[caminho->countFilho++] = novoArquivo;
    }
    else
    {
        printf("comando invalido\n");
    }
}

void gerarPasta(Nodo *caminho, char *nomePasta)
{
    if (caminho->countFilho < NUM_FILHOS)
    {
        Nodo *novaPasta = gerarNodo(nomePasta, true, caminho);
        caminho->filho[caminho->countFilho++] = novaPasta;
    }
    else
    {
        printf("comando invalido\n");
    }
}

Nodo *buscarDiretorio(Nodo *caminho, char *nomePasta)
{
    if (strcmp(nomePasta, "..") == 0)
    {
        if (caminho->irmao != NULL)
        {
            return caminho->irmao;
        }
        else
        {
            printf("comando invalido\n");
        }
    }
    else
    {
        for (int i = 0; i < caminho->countFilho; i++)
        {
            if (caminho->filho[i]->diretorio && strcmp(caminho->filho[i]->nome, nomePasta) == 0)
            {
                return caminho->filho[i];
            }
        }
        printf("comando invalido\n");
    }
    return caminho;
}

void excluirNodo(Nodo *caminho, char *nome)
{
    int x = -1;
    for (int i = 0; i < caminho->countFilho; i++)
    {
        if (strcmp(caminho->filho[i]->nome, nome) == 0)
        {
            x = i;
            break;
        }
    }

    if (x == -1)
    {
        printf("comando invalido\n");
        return;
    }

    Nodo *deletar = caminho->filho[x];

    if (deletar->diretorio)
    {
        for (int i = 0; i < deletar->countFilho; i++)
        {
            free(deletar->filho[i]);
        }
    }

    for (int i = x; i < caminho->countFilho - 1; i++)
    {
        caminho->filho[i] = caminho->filho[i + 1];
    }

    caminho->countFilho--;

    free(deletar);
}

void encerrar()
{
    printf("sistema encerrado\n");
    exit(0);
}

int main()
{
    Nodo *raiz = gerarNodo("raiz", true, NULL);
    Nodo *caminho = raiz;
    char acao[100], arg[100];

    while (true)
    {
        exibirPastaAtual(caminho);
        printf("->");
        scanf("%s", acao);

        if (strcmp(acao, "ls") == 0)
        {
            ls(caminho);
        }
        else if (strcmp(acao, "ma") == 0)
        {
            scanf("%s", arg);
            gerarArquivo(caminho, arg);
        }
        else if (strcmp(acao, "mp") == 0)
        {
            scanf("%s", arg);
            gerarPasta(caminho, arg);
        }
        else if (strcmp(acao, "cd") == 0)
        {
            scanf("%s", arg);
            caminho = buscarDiretorio(caminho, arg);
        }
        else if (strcmp(acao, "rm") == 0)
        {
            scanf("%s", arg);
            excluirNodo(caminho, arg);
        }
        else if (strcmp(acao, "ex") == 0)
        {
            encerrar();
        }
        else
        {
            printf("comando invalido\n");
        }
    }
    return 0;
}