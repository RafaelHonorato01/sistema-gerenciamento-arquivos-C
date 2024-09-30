#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHILDREN 10 // Grau máximo da árvore B

typedef struct Node {
    char name[100];               // Nome do arquivo/pasta
    bool isDirectory;             // Indica se é diretório (true) ou arquivo (false)
    struct Node* children[MAX_CHILDREN]; // Filhos do diretório
    int childCount;               // Quantidade de filhos
    struct Node* parent;          // Pai do nó
} Node;

// Função para criar um novo nó (arquivo ou pasta)
Node* createNode(char* name, bool isDirectory, Node* parent) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    strcpy(newNode->name, name);
    newNode->isDirectory = isDirectory;
    newNode->childCount = 0;
    newNode->parent = parent;
    return newNode;
}

void showCurrentPath(Node* current) {
    if (current->parent != NULL) {
        showCurrentPath(current->parent);
        printf("-%s", current->name);
    }
}

void showPath(Node* current) {
    printf("->");
    showCurrentPath(current);
    printf("->\n");
}

void ls(Node* current) {
    for (int i = 0; i < current->childCount; i++) {
        printf("%s", current->children[i]->name);
        if (current->children[i]->isDirectory) {
            printf("-");
        }
        printf("\n");
    }
}

void createFile(Node* current, char* fileName) {
    if (current->childCount < MAX_CHILDREN) {
        Node* newFile = createNode(fileName, false, current);
        current->children[current->childCount++] = newFile;
    } else {
        printf("Não é possível criar mais arquivos nesta pasta.\n");
    }
}

void createFolder(Node* current, char* folderName) {
    if (current->childCount < MAX_CHILDREN) {
        Node* newFolder = createNode(folderName, true, current);
        current->children[current->childCount++] = newFolder;
    } else {
        printf("Não é possível criar mais pastas nesta pasta.\n");
    }
}

Node* changeDirectory(Node* current, char* folderName) {
    if (strcmp(folderName, "..") == 0) {
        if (current->parent != NULL) {
            return current->parent;
        } else {
            printf("Já está na pasta raiz.\n");
        }
    } else {
        for (int i = 0; i < current->childCount; i++) {
            if (current->children[i]->isDirectory && strcmp(current->children[i]->name, folderName) == 0) {
                return current->children[i];
            }
        }
        printf("Pasta não encontrada.\n");
    }
    return current;
}

void removeNode(Node* current, char* name) {
    int found = -1;
    for (int i = 0; i < current->childCount; i++) {
        if (strcmp(current->children[i]->name, name) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Arquivo ou pasta não encontrado.\n");
        return;
    }

    Node* toDelete = current->children[found];

    if (toDelete->isDirectory && toDelete->childCount > 0) {
        printf("A pasta contém arquivos, exclua todos os arquivos primeiro.\n");
    } else {
        for (int i = found; i < current->childCount - 1; i++) {
            current->children[i] = current->children[i + 1];
        }
        current->childCount--;
        free(toDelete);
        printf("Removido com sucesso.\n");
    }
}

void exitProgram() {
    printf("Sistema encerrado.\n");
    exit(0);
}

int main() {
    Node* root = createNode("root", true, NULL);
    Node* current = root;
    char command[100], arg[100];

    while (true) {
        showPath(current);
        printf("Digite um comando: ");
        scanf("%s", command);

        if (strcmp(command, "ls") == 0) {
            ls(current);
        } else if (strcmp(command, "ma") == 0) {
            scanf("%s", arg);
            createFile(current, arg);
        } else if (strcmp(command, "mp") == 0) {
            scanf("%s", arg);
            createFolder(current, arg);
        } else if (strcmp(command, "cd") == 0) {
            scanf("%s", arg);
            current = changeDirectory(current, arg);
        } else if (strcmp(command, "rm") == 0) {
            scanf("%s", arg);
            removeNode(current, arg);
        } else if (strcmp(command, "ex") == 0) {
            exitProgram();
        } else {
            printf("Comando inválido.\n");
        }
    }
    return 0;
}
