
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "PathFunctions.h"

typedef unsigned char ValueType;

typedef struct Node Node;
struct Node {
	ValueType value;
	Node* left;
	Node* right;
	Path path;
};

Node* createNode(ValueType value, Node* left, Node* right) {
	Node* newNode = (Node*) calloc(1, sizeof(Node));
	newNode->value = value;
	
	newNode->left = left;
	newNode->right = right;
	
	return newNode;
}

void freeNode(Node* node) {
	if(node) {
		freeNode(node->left);
		freeNode(node->right);
		free(node);
	}
}

#define MAX_QUEUE_SIZE 512
typedef struct Queue Queue;
struct Queue {
	Node *buffer[MAX_QUEUE_SIZE];
	int head;
	int tail;
	int size;
};

Queue* createQueue() {
	Queue* newQueue = (Queue*) calloc(1, sizeof(Queue));
	for(int i = 0; i < MAX_QUEUE_SIZE; ++i) {
		newQueue->buffer[i] = NULL;
	}
	newQueue->head = 0;
	newQueue->tail = 0;
	newQueue->size = 0;
	return newQueue;
}

Node* enqueue_node(Queue* queue, Node* newNode) {
	if(queue) {
		if(queue->size < MAX_QUEUE_SIZE) {
			queue->buffer[queue->tail] = newNode;
			queue->tail = (queue->tail + 1) % MAX_QUEUE_SIZE;
			++queue->size;
			
			return newNode;
		}
	}
	return NULL;
}

Node* dequeue_node(Queue* queue) {
	if(queue) {
		if(queue->size > 0) {
			queue->tail = (queue->tail - 1) % MAX_QUEUE_SIZE;
			Node* result = queue->buffer[queue->tail];
			
			--queue->size;
			return result;
		}
	}
	return NULL;
}

void freeQueue(Queue* queue) {
	free(queue);
}

Node* readTreeFromFile(char* inputFileName) {
	FILE* inputFile = fopen(inputFileName, "r");
	
	if(inputFile) {
		Queue* nodes = createQueue();
		
		char data;
		while((data = fgetc(inputFile)) && !feof(inputFile)) {
			if(data == '1') {
				data = fgetc(inputFile);
				enqueue_node(nodes, createNode(data, NULL, NULL));
			} else {
				Node* a = dequeue_node(nodes);
				Node* b = dequeue_node(nodes);
				
				Node* combined = createNode(0, a, b);
				
				enqueue_node(nodes, combined);
			}
		}
		Node* lastNode = dequeue_node(nodes);
		Node* rootNode = lastNode->left;
		
		free(lastNode);
		freeQueue(nodes);
		
		fclose(inputFile);
		return rootNode;
	}
	return NULL;
}

bool isLeaf(Node* node) {
	return node && node->left == NULL && node->right == NULL;
}
void printHuffmanCodes(Node* huffmanTreeRoot, Path* location) {
	if(huffmanTreeRoot) {
		pushPath(location, '0');
		printHuffmanCodes(huffmanTreeRoot->right, location);
		popPath(location);
		
		pushPath(location, '1');
		printHuffmanCodes(huffmanTreeRoot->left, location);
		popPath(location);
		
		if(isLeaf(huffmanTreeRoot)) {
			copyPath(&(huffmanTreeRoot->path), location);
			printf("%c %d %s\n", huffmanTreeRoot->value, (int)(huffmanTreeRoot->value), location->location);
		}
	}
}

void performActionOnLeaf(Node* huffmanTreeRoot, Path* location,
						void (*action)(Node*, void* args), void* args) {
	if(huffmanTreeRoot) {
		pushPath(location, '0');
		performActionOnLeaf(huffmanTreeRoot->right,
			location, action, args);
		popPath(location);
		
		pushPath(location, '1');
		performActionOnLeaf(huffmanTreeRoot->left,
			location, action, args);
		popPath(location);
		
		if(isLeaf(huffmanTreeRoot)) {
			copyPath(&(huffmanTreeRoot->path), location);
			action(huffmanTreeRoot, args);
		}
	}
}

int application(int argc, char* argv[]) {
	if(argc < 2) {
		printf("Files not specified in argument!\n");
		return EXIT_FAILURE;
	}
	
	char* inputFileName = argv[1];
	Node* huffmanTreeRoot = readTreeFromFile(inputFileName);
	
	if(huffmanTreeRoot != NULL) {
		Path locationBuffer;
		initializePath(&locationBuffer);
		
		printHuffmanCodes(huffmanTreeRoot, &locationBuffer);
	
		freeNode(huffmanTreeRoot);
	}
	
	return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
	return application(argc, argv);
}

