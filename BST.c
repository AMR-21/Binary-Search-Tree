#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#define MAX_SIZE 100

typedef struct
{
    char *data;
    struct Node *left, *right, *parent;
} Node;

Node *newNode(char *str)
{
    Node *n = malloc(sizeof(Node));
    n->left = n->right = NULL;
    n->data = malloc(strlen(str) + 1);
    strcpy(n->data, str);
    return n;
}

Node *insert(Node *root, char *str)
{
    if (!root)
        return newNode(str);
    else if (strcasecmp(root->data, str) < 0)
        root->right = insert(root->right, str);
    else if (strcasecmp(root->data, str) > 0)
        root->left = insert(root->left, str);
    return root;
}

Node *search(Node *root, char *str)
{
    if (!strcasecmp(root->data, str))
        return root;
    else if (strcasecmp(root->data, str) < 0)
    {
        if (!root->right)
            return root;
        search(root->right, str);
    }
    else if (strcasecmp(root->data, str) > 0)
    {
        if (!root->left)
            return root;
        search(root->left, str);
    }
}

int size(Node *root)
{
    if (!root)
        return 0;
    else
        return 1 + size(root->left) + size(root->right);
}

int max(int x, int y)
{
    return x > y ? x : y;
}

int height(Node *root)
{
    if (!root)
        return -1;
    else
        return 1 + max(height(root->left), height(root->right));
}

Node *readInTree(char *filename)
{
    Node *root = NULL;
    char data[MAX_SIZE];
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        printf("File can not be opened\n");
        return;
    }
    while (!feof(f))
    {
        fscanf(f, "%s", data);
        root = insert(root, data);
    }
    return root;
}

void getSucPre(Node *root, Node **pre, Node **suc, char *str)
{
    if (root == NULL)
        return;
    if (!strcasecmp(root->data, str))
    {
        if (root->left != NULL)
        {
            Node *temp = root->left;
            while (temp->right)
            {
                temp = temp->right;
            }
            *pre = temp;
        }
        if (root->right != NULL)
        {
            Node *temp = root->right;
            while (temp->left)
            {
                temp = temp->left;
            }
            *suc = temp;
        }
        return;
    }
    if (strcasecmp(root->data, str) > 0)
    {
        *suc = root;
        getSucPre(root->left, pre, suc, str);
    }
    else
    {
        *pre = root;
        getSucPre(root->right, pre, suc, str);
    }
}
Node *findMin(Node *root)
{
    if (!root)
        return NULL;
    else if (root->left)
        return findMin(root->left);
    else
        return root;
}
Node *getsuc(Node *root, int key)
{
    Node *suc = NULL;
    Node *temp = root;
    while (temp)
    {
        if (temp->key > key)
        {
            suc = temp;
            temp = temp->left;
        }
        else if (temp->key < key)
        {
            temp = temp->right;
        }
        else
        {
            if (temp->right)
                return findMin(temp->right);
            else
                return suc;
        }
    }
    return suc;
}

void validate(Node *root, char *str)
{
    char *token = strtok(str, " ");
    while (token)
    {
        Node *n = search(root, token);
        if (!strcasecmp(n->data, token))
        {
            printf("%s - CORRECT\n", token);
        }
        else
        {
            printf("%s - Incorrect, Suggestions: %s ", token, n->data);
            Node *successor = getsuc(root, n->data);
            //getSucPre(root, &predecessor, &successor, n->data);
            printf("%s\n", successor->data);
        }
        token = strtok(NULL, " ");
    }
}

int main()
{
    Node *root = readInTree("EN-US-Dictionary.txt");
    char sentence[256];
    printf("\nDictionary loaded Successfully...!\n");
    printf("..................................\n");
    printf("Size = %d\n", size(root));
    printf("..................................\n");
    printf("Height = %d\n", height(root));
    printf("..................................\n");
    printf("Enter a sentence:\n");
    while (fgets(sentence, 256, stdin) != NULL)
    {
        sentence[strlen(sentence) - 1] = '\0';
        validate(root, sentence);
        printf("\nEnter a sentence:\n");
    }
    return 0;
}