
#ifndef _PATH_FUNCTIONS_H
#define _PATH_FUNCTIONS_H

#include <string.h>

#define MAX_PATH_LENGTH 256

typedef struct Path {
	char location[MAX_PATH_LENGTH];
} Path;

void initializePath(Path* path) {
	if(path) {
		char empty[MAX_PATH_LENGTH] = { 0 };
		strncpy(path->location, empty, MAX_PATH_LENGTH);
	}
}

void copyPath(Path* dest, Path* src) {
	if(dest && src) {
		strncpy(dest->location, src->location, MAX_PATH_LENGTH);
	}
}

void pushPath(Path* path, char data) {
	if(path) {
		int len = strlen(path->location);
		if(len < MAX_PATH_LENGTH - 1) {
			path->location[len] = data;
		}
	}
}

char popPath(Path* path) {
	if(path) {
		int len = strlen(path->location);
		if(len > 0) {
			char result = path->location[len - 1];
			path->location[len - 1] = 0;
			return result;
		}
	}
	return 0;
}

#endif

