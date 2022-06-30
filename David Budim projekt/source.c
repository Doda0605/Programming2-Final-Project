#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "header.h"

int main(void) {

	srand((unsigned)time(NULL));

	int condition = 1;
	char fileName[] = {"questions.txt"};

	FILE* fp = NULL;
	fp = fopen(fileName, "w");
	if (fp == NULL) {
		printf("The file can't be opened.\n");
	}
	else {
		fclose(fp);
	}
	while (condition) {
		condition = menu(fileName);
	}
	
	return 0;
}