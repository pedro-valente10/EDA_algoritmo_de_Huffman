// Pedro Henrique Padilha Valente - 2510382

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 256

// Estrutura para o nó da árvore de Huffman
struct MinHeapNode 
{
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// Estrutura para a fila de prioridade (Min Heap)
struct MinHeap
 {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

// Tabela para armazenar os códigos gerados
char codes[256][MAX_TREE_HT];
int code_lengths[256];

// Função para criar um novo nó
struct MinHeapNode* newNode(char data, unsigned freq)
 {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// Função para criar um Min Heap
struct MinHeap* createMinHeap(unsigned capacity)
 {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// Troca dois nós do Min Heap
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
 {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Mantém a propriedade do Min Heap
void minHeapify(struct MinHeap* minHeap, int idx) 
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Verifica se o tamanho é 1
int isSizeOne(struct MinHeap* minHeap) 
{
    return (minHeap->size == 1);
}

// Extrai o nó de menor frequência
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
 {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// Insere um novo nó no Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) 
{
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// Constrói o Min Heap
void buildMinHeap(struct MinHeap* minHeap)
{
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// Verifica se o nó é uma folha
int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

// Cria e constrói o Min Heap a partir de caracteres e frequências
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size)
{
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

// A função principal que constrói a árvore de Huffman
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) 
{
    struct MinHeapNode *left, *right, *top;
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq); // '$' é um caractere interno
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

// Salva os códigos na tabela global e os imprime
void storeCodes(struct MinHeapNode* root, int arr[], int top) 
{
    if (root->left) {
        arr[top] = 0;
        storeCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        storeCodes(root->right, arr, top + 1);
    }
    if (isLeaf(root)) {
        printf(" '%c'\t\t%d\t\t", root->data, top);
        for (int i = 0; i < top; ++i) {
            printf("%d", arr[i]);
            codes[(unsigned char)root->data][i] = arr[i] + '0';
        }
        codes[(unsigned char)root->data][top] = '\0';
        code_lengths[(unsigned char)root->data] = top;
        printf("\n");
    }
}

// Descompacta a string codificada usando a árvore
void decodeFile(struct MinHeapNode* root, char* encodedString) 
{
    struct MinHeapNode* curr = root;
    for (int i = 0; encodedString[i] != '\0'; i++) {
        if (encodedString[i] == '0')
            curr = curr->left;
        else
            curr = curr->right;

        if (isLeaf(curr)) {
            printf("%c", curr->data);
            curr = root;
        }
    }
    printf("\n");
}

int main() 
{
    // Frase de teste fornecida no roteiro do trabalho
    char text[] = "AS ESTRUTURAS DE DADOS SAO FUNDAMENTAIS PARA A ORGANIZACAO E A MANIPULACAO EFICIENTE DAS INFORMACOES";
    
    int freqMap[256] = {0};
    int n = strlen(text);

    // Conta a frequência de cada caractere
    for (int i = 0; i < n; i++) {
        freqMap[(unsigned char)text[i]]++;
    }

    // Calcula quantos caracteres únicos existem
    int uniqueChars = 0;
    for (int i = 0; i < 256; i++) {
        if (freqMap[i] > 0) uniqueChars++;
    }

    char data[256];
    int freq[256];
    int idx = 0;

    printf("=== TABELA DE OCORRENCIAS ===\n");
    printf("Caractere\tFrequencia\n");
    for (int i = 0; i < 256; i++) {
        if (freqMap[i] > 0) {
            data[idx] = (char)i;
            freq[idx] = freqMap[i];
            printf(" '%c'\t\t%d\n", data[idx], freq[idx]);
            idx++;
        }
    }
    printf("--------------------------------------------------\n\n");

    // Constrói a árvore de Huffman
    struct MinHeapNode* root = buildHuffmanTree(data, freq, uniqueChars);

    int arr[MAX_TREE_HT], top = 0;
    
    // Imprime a tabela de códigos e tamanhos (exigência do relatório)
    printf("=== TABELA DE CODIGOS BINARIOS ===\n");
    printf("Caractere\tTamanho(bits)\tCodigo\n");
    storeCodes(root, arr, top);
    printf("--------------------------------------------------\n\n");

    // Compactação (Geração da string de bits)
    printf("=== TEXTO COMPACTADO ===\n");
    char encodedString[2048] = ""; 
    for (int i = 0; i < n; i++) {
        strcat(encodedString, codes[(unsigned char)text[i]]);
    }
    printf("%s\n", encodedString);
    printf("Total de bits na string original (8 bits por char): %d bits\n", n * 8);
    printf("Total de bits na string compactada: %lu bits\n", strlen(encodedString));
    printf("--------------------------------------------------\n\n");

    // Descompactação
    printf("=== TEXTO DESCOMPACTADO ===\n");
    decodeFile(root, encodedString);
    printf("--------------------------------------------------\n");

    return 0;
}
