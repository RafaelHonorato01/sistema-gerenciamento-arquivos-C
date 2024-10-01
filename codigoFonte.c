#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHILDREN 10

typedef struct Node
{
    char name[100];
    bool isDirectory;
    struct Node *children[MAX_CHILDREN];
    int childCount;
    struct Node *parent;
} Node;

Node *createNode(char *name, bool isDirectory, Node *parent)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    strcpy(newNode->name, name);
    newNode->isDirectory = isDirectory;
    newNode->childCount = 0;
    newNode->parent = parent;
    return newNode;
}

void showCurrentPath(Node *current)
{
    if (current->parent != NULL)
    {
        showCurrentPath(current->parent);
        printf("-%s", current->name);
    }
}

void sortChildren(Node* current) {
    for (int i = 0; i < current->childCount - 1; i++) {
        for (int j = 0; j < current->childCount - i - 1; j++) {
            if (strcasecmp(current->children[j]->name, current->children[j + 1]->name) > 0) {
                Node* temp = current->children[j];
                current->children[j] = current->children[j + 1];
                current->children[j + 1] = temp;
            }
        }
    }
}

void ls(Node *current)
{
    sortChildren(current);

    for (int i = 0; i < current->childCount; i++)
    {
        printf("%s", current->children[i]->name);
        if (current->children[i]->isDirectory)
        {
            printf("-");
        }
        printf("\n");
    }
}

void createFile(Node *current, char *fileName)
{
    if (current->childCount < MAX_CHILDREN)
    {
        Node *newFile = createNode(fileName, false, current);
        current->children[current->childCount++] = newFile;
    }
    else
    {
        printf("comando invalido\n");
    }
}

void createFolder(Node *current, char *folderName)
{
    if (current->childCount < MAX_CHILDREN)
    {
        Node *newFolder = createNode(folderName, true, current);
        current->children[current->childCount++] = newFolder;
    }
    else
    {
        printf("comando invalido\n");
    }
}

Node *changeDirectory(Node *current, char *folderName)
{
    if (strcmp(folderName, "..") == 0)
    {
        if (current->parent != NULL)
        {
            return current->parent;
        }
        else
        {
            printf("comando invalido\n");
        }
    }
    else
    {
        for (int i = 0; i < current->childCount; i++)
        {
            if (current->children[i]->isDirectory && strcmp(current->children[i]->name, folderName) == 0)
            {
                return current->children[i];
            }
        }
        printf("comando invalido\n");
    }
    return current;
}

void removeNode(Node *current, char *name)
{
    int found = -1;
    for (int i = 0; i < current->childCount; i++)
    {
        if (strcmp(current->children[i]->name, name) == 0)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("comando invalido\n");
        return;
    }

    Node *toDelete = current->children[found];

    if (toDelete->isDirectory)
    {
        for (int i = 0; i < toDelete->childCount; i++)
        {
            free(toDelete->children[i]);
        }
    }

    for (int i = found; i < current->childCount - 1; i++)
    {
        current->children[i] = current->children[i + 1];
    }

    current->childCount--;

    free(toDelete);
}

void exitProgram()
{
    printf("sistema encerrado\n");
    exit(0);
}

int main()
{
    Node *root = createNode("root", true, NULL);
    Node *current = root;
    char command[100], arg[100];

    while (true)
    {
        showCurrentPath(current);
        printf("->");
        scanf("%s", command);

        if (strcmp(command, "ls") == 0)
        {
            ls(current);
        }
        else if (strcmp(command, "ma") == 0)
        {
            scanf("%s", arg);
            createFile(current, arg);
        }
        else if (strcmp(command, "mp") == 0)
        {
            scanf("%s", arg);
            createFolder(current, arg);
        }
        else if (strcmp(command, "cd") == 0)
        {
            scanf("%s", arg);
            current = changeDirectory(current, arg);
        }
        else if (strcmp(command, "rm") == 0)
        {
            scanf("%s", arg);
            removeNode(current, arg);
        }
        else if (strcmp(command, "ex") == 0)
        {
            exitProgram();
        }
        else
        {
            printf("comando invalido\n");
        }
    }
    return 0;
}