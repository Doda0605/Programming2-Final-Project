#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

#ifndef MAX
#define MAX 100
#endif

char* gameIntro(void) {
	printf("==================================GAME\INTRO============================\n");

	printf("\t  Hello! And welcome to 'Who Wants to Be a Millionaire?'\n");
	printf("\tI'm David Budim and I will be your host of the game today.\n");
	printf("\t       We have a very special guest coming along.\n");
	printf("\t        Let's take a look on this young fella\n");

	printf("======================================\================================\n");

	char username[17];

	printf("Enter what you would like to be called (16 characters max): ");
	scanf("%16s", username);

	return username;
}

int menu(const char* const fileName) {
	printf("====================");
	printf("Who Wants To Be a Millionaire?");
	printf("====================\n");
	printf("\t\t\t1. Start new game\n");
	printf("\t\t\t2. Leaderboard\n");
	printf("\t\t\t3. Rules\n");
	printf("\t\t\t4. Exit game\n");
	printf("======================================\================================\n");
	
	int condition = 0, score = 0, leaderboardField[100], size = sizeof(PLAYER);
	char username[17];
	PLAYER newScore;
	PLAYER* pokPLAYER = NULL;
	pokPLAYER = dynamicMemoryAllocation(10, size);
	if (pokPLAYER == NULL) {
		return 1;
	}
	scanf("%d", &condition);

	switch (condition) {
	case 1:
		strcpy(username, gameIntro());
		score = game();
		loadLeaderboard(pokPLAYER);
		newScore = newPlayerScore(score, username);
		pokPLAYER = comparingToLeaderboard(pokPLAYER, newScore);
		updateLeaderboard(pokPLAYER);
		break;
	case 2:
		loadLeaderboard(pokPLAYER);
		printField(pokPLAYER);
		break;
	case 3:
		rules();
		break;
	case 4:
		free(pokPLAYER);
		condition = exitProgram();
		break;
	default:
		condition = 0;
	}
	
	return condition;
}

int exitProgram(void) {
	printf("====================");
	printf("Thank you for playing my game!");
	printf("===================\n");
	return 0;
}

/*int choosingCategory(void) {
	int category = 0;

	printf("====================");
	printf("Choose your category:");
	printf("====================\n");
	printf("\t\t\t1. Music\n");
	printf("\t\t\t2. Film\n");
	printf("\t\t\t3. History\n");
	printf("\t\t\t4. Sport\n");
	printf("\t\t\t5. Science\n");
	printf("\t\t\t6. Famous people\n");
	printf("\t\t\t7. General knowledge\n");
	printf("\t\t\t8. Geography\n");
	printf("======================================\========================\n");

	do{
		printf("I choose category number ");
		scanf("%d", &category);
	} while (category < 1 || category > 8);

	return category;
}*/

int game(void) {
	int level = 1, gameScenario = 0, money;

	int LifelineUsesAndHp[4] = { 1, 1, 1, 1 };

	while ((LifelineUsesAndHp[0] == 1) && (level <= 15)) {
		
		gameScenario = loadingQuestions(level);
		loadingAnswers(level, gameScenario);
		LifelineUsesAndHp[0] = loadingSolutions(level, gameScenario, LifelineUsesAndHp);

		if (LifelineUsesAndHp[0] != 0) {
			printf("Congratulations, you answered correctly!\n");
			level++;
		}
		else {
			printf("GAME OVER!");
		}
	}

	money = calculateMoney(level);

	return money;
}

int loadingQuestions(int level) {
	
	int gameScenario = 0;

	gameScenario = (rand() % (5 - 1 + 1)) + 1;
	
	FILE* fp = NULL;
	
	char buf[15];
	snprintf(buf, 15, "%dQuestion.txt", level); // puts string into buffer
	//printf("%s\n", buf); // outputs so you can see it
	
	fp = fopen(buf, "r");
	if (fp == NULL) {
		printf("File can't be opened.\n");
		exit(-1);
	}

	QUESTION* headNode = createSLList(fp);
	if (headNode == NULL) {
		return 1;
	}

	for (int i = 0; i < 4; i++) {
		headNode = insertNewNodeSLList(headNode, fp);
	}

	fclose(fp);

	QUESTION* targetNode = searchSLList(headNode, gameScenario);
	if (targetNode == NULL) {
		return 1;
	}

	printLineNodeSLList(&headNode, targetNode, level);
	deleteNodeSLList(&headNode, targetNode);
	headNode = deleteWholeSLList(headNode);

	return gameScenario;
}

void loadingAnswers(int level, int gameScenario) {

	FILE* fp = NULL;

	char buf[15];
	snprintf(buf, 15, "%dAnswer.txt", level);

	fp = fopen(buf, "r");
	if (fp == NULL) {
		printf("File can't be opened.\n");
		exit(-1);
	}

	ANSWER* headNode = createSLListAnswer(fp);
	if (headNode == NULL) {
		return 1;
	}

	for (int i = 0; i < 4; i++) {
		headNode = insertNewNodeSLListAnswer(headNode, fp);
	}

	fclose(fp);

	ANSWER* targetNode = searchSLList(headNode, gameScenario);
	if (targetNode == NULL) {
		return 1;
	}

	printLineNodeSLListAnswer(&headNode, targetNode);
	deleteNodeSLListAnswer(&headNode, targetNode);

	headNode = deleteWholeSLListAnswer(headNode);

}

int* loadingSolutions(int level, int gameScenario, int* LifelineUsesAndHp) {

	FILE* fp = NULL;

	char buf[15];
	snprintf(buf, 15, "%dSolution.txt", level);

	fp = fopen(buf, "r");
	if (fp == NULL) {
		printf("File can't be opened.\n");
		exit(-1);
	}

	SOLUTION* headNode = createSLListSolution(fp);
	if (headNode == NULL) {
		return 1;
	}

	for (int i = 0; i < 4; i++) {
		headNode = insertNewNodeSLListSolution(headNode, fp);
	}

	fclose(fp);

	SOLUTION* targetNode = searchSLList(headNode, gameScenario);
	if (targetNode == NULL) {
		return 1;
	}

	compareToSolution(&headNode, targetNode, LifelineUsesAndHp, level, gameScenario);
	deleteNodeSLListSolution(&headNode, targetNode);

	headNode = deleteWholeSLListSolution(headNode);

	return LifelineUsesAndHp[0];
}

QUESTION* createSLList(FILE* p) {

	QUESTION* headNode = (QUESTION*)calloc(1, sizeof(QUESTION));
	if (headNode == NULL) {
		perror("Creating headNode.");
		return NULL;
	}
	else {
		fscanf(p, "%d", &headNode->id);


		headNode->next = NULL;

		fscanf(p, "%c", &headNode->question);
		input(&headNode->question, p);
		if (headNode->question == NULL) {
			exit(-1);
		}
	}
	return headNode;
}

void input(char** pointer, FILE* p) {
	char helperField[200] = { 0 };
	int length = 0;
	fscanf(p, "%[^\n]s", helperField);
	length = strlen(helperField);
	*pointer = (char*)calloc(length + 1, sizeof(char));
	if (*pointer == NULL) {
		return;
	}
	strcpy(*pointer, helperField);
}

QUESTION* insertNewNodeSLList(QUESTION* headNode, FILE* p) {

	QUESTION* newHeadNode = (QUESTION*)calloc(1, sizeof(QUESTION));
	if (newHeadNode == NULL) {
		perror("Creating newHeadNode.");
		return headNode;
	}
	else {
		fscanf(p, "%d", &newHeadNode->id);
		newHeadNode->next = headNode;
		fscanf(p, "%c", &newHeadNode->question);
		input(&newHeadNode->question, p);
		if (newHeadNode->question == NULL) {
			exit(-1);
		}
		return newHeadNode;
	}
}

int traverseSLList(QUESTION* traverseNode) {
	int counter = 0;
	if (traverseNode == NULL) {
		exit(-1);
	}
	else {
		while (traverseNode) {
			counter++;
			printf("id %d\tquestion: %s\n", traverseNode->id, traverseNode->question);
			traverseNode = traverseNode->next;
		}
	}
	return counter;
}

QUESTION* searchSLList(QUESTION* traverseNode, int criteria) {
	while (traverseNode) {
		if (traverseNode->id == criteria) {
			return traverseNode;
		}
		else {
			traverseNode = traverseNode->next;
		}
	}
	return NULL;
}

void printLineNodeSLList(QUESTION** headNode, QUESTION* targetNode, int level) {

	printf("=========================QUESTION NUMBER %d=======\=====================\n", level);
	printf("%s\n", targetNode->question);
	printf("======================================\================================\n");

}

void deleteNodeSLList(QUESTION** headNode, QUESTION* targetNode) {
	if (*headNode == targetNode) {
		*headNode = (*headNode)->next;
		free(targetNode->question);
		free(targetNode);
		//printf("Node freed: %p\n", targetNode);
	}
	else {
		QUESTION* traverseNode = *headNode;
		while (traverseNode->next) {
			if (traverseNode->next == targetNode) {
				traverseNode->next = targetNode->next;
				free(targetNode->question);
				free(targetNode);
				//printf("Node freed: %p\n", targetNode);
				break;
			}
			traverseNode = traverseNode->next;
		}
	}
}

QUESTION* deleteWholeSLList(QUESTION* traverseNode) {
	QUESTION* deleteNode = NULL;
	while (traverseNode) {
		deleteNode = traverseNode;
		traverseNode = traverseNode->next;
		free(deleteNode->question);
		free(deleteNode);
		//printf("Node freed: %p\n", deleteNode);
	}
	return NULL;
}

ANSWER* createSLListAnswer(FILE* p) {

	ANSWER* headNode = (ANSWER*)calloc(1, sizeof(ANSWER));
	if (headNode == NULL) {
		perror("Creating headNode.");
		return NULL;
	}
	else {
		fscanf(p, "%d", &headNode->id);


		headNode->next = NULL;

		fscanf(p, "%c", &headNode->answer);
		input(&headNode->answer, p);
		if (headNode->answer == NULL) {
			exit(-1);
		}
	}
	return headNode;
}

ANSWER* insertNewNodeSLListAnswer(ANSWER* headNode, FILE* p) {

	ANSWER* newHeadNode = (ANSWER*)calloc(1, sizeof(ANSWER));
	if (newHeadNode == NULL) {
		perror("Creating newHeadNode.");
		return headNode;
	}
	else {
		fscanf(p, "%d", &newHeadNode->id);
		newHeadNode->next = headNode;
		fscanf(p, "%c", &newHeadNode->answer);
		input(&newHeadNode->answer, p);
		if (newHeadNode->answer == NULL) {
			exit(-1);
		}
		return newHeadNode;
	}
}

int traverseSLListAnswer(ANSWER* traverseNode) {
	int counter = 0;
	if (traverseNode == NULL) {
		exit(-1);
	}
	else {
		while (traverseNode) {
			counter++;
			printf("id %d\tanswer: %s\n", traverseNode->id, traverseNode->answer);
			traverseNode = traverseNode->next;
		}
	}
	return counter;
}

void printLineNodeSLListAnswer(ANSWER** headNode, ANSWER* targetNode) {

	printf("======================================\================================\n");
	printf("%s\n", targetNode->answer);
	printf("======================================\================================\n");

}

void deleteNodeSLListAnswer(ANSWER** headNode, ANSWER* targetNode) {
	if (*headNode == targetNode) {
		*headNode = (*headNode)->next;
		free(targetNode->answer);
		free(targetNode);
		//printf("Node freed: %p\n", targetNode);
	}
	else {
		QUESTION* traverseNode = *headNode;
		while (traverseNode->next) {
			if (traverseNode->next == targetNode) {
				traverseNode->next = targetNode->next;
				free(targetNode->answer);
				free(targetNode);
				//printf("Node freed: %p\n", targetNode);
				break;
			}
			traverseNode = traverseNode->next;
		}
	}
}

ANSWER* deleteWholeSLListAnswer(ANSWER* traverseNode) {
	ANSWER* deleteNode = NULL;
	while (traverseNode) {
		deleteNode = traverseNode;
		traverseNode = traverseNode->next;
		free(deleteNode->answer);
		free(deleteNode);
		//printf("Node freed: %p\n", deleteNode);
	}
	return NULL;
}

SOLUTION* createSLListSolution(FILE* p) {

	SOLUTION* headNode = (SOLUTION*)calloc(1, sizeof(SOLUTION));
	if (headNode == NULL) {
		perror("Creating headNode.");
		return NULL;
	}
	else {
		fscanf(p, "%d", &headNode->id);


		headNode->next = NULL;

		fscanf(p, "%c", &headNode->solution);
		input(&headNode->solution, p);
		if (headNode->solution == NULL) {
			exit(-1);
		}
	}
	return headNode;
}

SOLUTION* insertNewNodeSLListSolution(SOLUTION* headNode, FILE* p) {

	SOLUTION* newHeadNode = (SOLUTION*)calloc(1, sizeof(SOLUTION));
	if (newHeadNode == NULL) {
		perror("Creating newHeadNode.");
		return headNode;
	}
	else {
		fscanf(p, "%d", &newHeadNode->id);
		newHeadNode->next = headNode;
		fscanf(p, "%c", &newHeadNode->solution);
		input(&newHeadNode->solution, p);
		if (newHeadNode->solution == NULL) {
			exit(-1);
		}
		return newHeadNode;
	}
}

int traverseSLListSolution(SOLUTION* traverseNode) {
	int counter = 0;
	if (traverseNode == NULL) {
		exit(-1);
	}
	else {
		while (traverseNode) {
			counter++;
			printf("id %d\tsolution: %s\n", traverseNode->id, traverseNode->solution);
			traverseNode = traverseNode->next;
		}
	}
	return counter;
}

void printLineNodeSLListSolution(SOLUTION** headNode, SOLUTION* targetNode) {

	printf("======================================\================================\n");
	printf("ODGOVORI: %s\n", targetNode->solution);
	printf("======================================\================================\n");

}

void compareToSolution(SOLUTION** headNode, SOLUTION* targetNode, int* LifelineUsesAndHp, int level, int gameScenario) {

	LifelineUsesAndHp[0] = 0;
	char userAnswer, confirmation = 'N', solution;
	solution = targetNode->solution[0];

	printf("============================YOUR ANSWER================================\n");	
	
	while (confirmation == 'N' || confirmation == 'n') {

		if (LifelineUsesAndHp[1] == 1) {
			printf("'E' - 50/50");
		}

		if (LifelineUsesAndHp[2] == 1) {
			printf("\t'F' - CALL");
		}

		if (LifelineUsesAndHp[3] == 1) {
			printf("\t'G' - VOTE");
		}

		do {
			printf("\nYour answer is: ");
			scanf(" %c", &userAnswer);
		} while ((userAnswer < 'a' || userAnswer > 'g') && (userAnswer < 'A' || userAnswer > 'G'));

		if (userAnswer >= 'a' && userAnswer <= 'g') {
				userAnswer = userAnswer - 32;
		}

		do {
			printf("Is %c your final answer? (Y/N) ", userAnswer);
			scanf(" %c", &confirmation);
		} while (confirmation != 'y' && confirmation != 'Y' && confirmation != 'n' && confirmation != 'N');
	
		if ((userAnswer >= 'E') && (userAnswer <= 'G')) {

			switch (userAnswer) {
			case 'E':
				FiftyFiftyLifeline(LifelineUsesAndHp, solution);
				confirmation = 'n';
				break;
			case 'F':
				CallLifeline(LifelineUsesAndHp, solution);
				confirmation = 'n';
				break;
			case 'G':
				VoteLifeline(LifelineUsesAndHp, solution);
				confirmation = 'n';
				break;
			default:
				break;
			}
		}
	}

	if (solution == userAnswer) {
		LifelineUsesAndHp[0] = 1;
	}
	
	printf("======================================\================================\n");
}

void deleteNodeSLListSolution(SOLUTION** headNode, SOLUTION* targetNode) {
	if (*headNode == targetNode) {
		*headNode = (*headNode)->next;
		free(targetNode->solution);
		free(targetNode);
		//printf("Node freed: %p\n", targetNode);
	}
	else {
		QUESTION* traverseNode = *headNode;
		while (traverseNode->next) {
			if (traverseNode->next == targetNode) {
				traverseNode->next = targetNode->next;
				free(targetNode->solution);
				free(targetNode);
				//printf("Node freed: %p\n", targetNode);
				break;
			}
			traverseNode = traverseNode->next;
		}
	}
}

SOLUTION* deleteWholeSLListSolution(SOLUTION* traverseNode) {
	SOLUTION* deleteNode = NULL;
	while (traverseNode) {
		deleteNode = traverseNode;
		traverseNode = traverseNode->next;
		free(deleteNode->solution);
		free(deleteNode);
		//printf("Node freed: %p\n", deleteNode);
	}
	return NULL;
}

void loadLeaderboard(PLAYER* pokPLAYER) {

	FILE* fp = NULL;

	fp = fopen("leaderboard.txt", "r");
	if (fp == NULL) {
		printf("File can't be opened.\n");
		exit(-1);
	}

	for (int i = 0; i < 10; i++){
		(fscanf(fp, "%16s %d", pokPLAYER[i].name, &pokPLAYER[i].points));
	}
	fclose(fp);
}

void* dynamicMemoryAllocation(int number, int size) {

	return calloc(number, size);
}

void printField(PLAYER* pokPLAYER) {
	
	printf("===============================LEADERBOARD=============================");

	int i = 0;
	
	recursivePrint(i, pokPLAYER);
	printf("\n======================================\================================\n");
	/*for (int i = 0; i < 10; i++)
	{
		printf("\n\t\t\t%d.\t%s\t%d$", i + 1, pokPLAYER[i].name, pokPLAYER[i].points);
	}
	printf("\n======================================\================================\n");*/
}

void recursivePrint(int i, PLAYER* pokPLAYER) {

	if (i < 10) {
		printf("\n\t\t\t%d.\t%s\t%d$", i + 1, pokPLAYER[i].name, pokPLAYER[i].points);
		recursivePrint(i + 1, pokPLAYER);
	}

}

PLAYER newPlayerScore(int score, char* username) {

	int moneyToTakeHome = 0;

	PLAYER pokNewScore;
	strcpy(pokNewScore.name, username);
	pokNewScore.points = score;

	moneyToTakeHome = calculateMoneyToTakeHome(score);

	printf("\n======================================\================================\n");
	printf("Your final score was:\n%s\t%d$", pokNewScore.name, pokNewScore.points);
	printf("\nYou are going home with:\n%d$", moneyToTakeHome);
	printf("\n======================================\================================\n");

	return pokNewScore;
}

PLAYER* comparingToLeaderboard(PLAYER* pokPLAYER, PLAYER newPlayerScore) {

	int tempScore;
	char tempUsername[17];

	if (newPlayerScore.points > ((pokPLAYER + 9)->points)) {

		pokPLAYER[9] = newPlayerScore;

		for (int i = 8; i >= 0; i--) {

			if ((pokPLAYER[i + 1].points) >(pokPLAYER[i].points)) {
				tempScore = pokPLAYER[i + 1].points;
				strcpy(tempUsername, pokPLAYER[i + 1].name);
				pokPLAYER[i + 1] = pokPLAYER[i];
				strcpy(pokPLAYER[i].name, tempUsername);
				pokPLAYER[i].points = tempScore;
			}
		}
	}

	return pokPLAYER;
}

int calculateMoney(int level) {

	int money;

	switch (level) {
	case 1:
		money = 100;
		break;
	case 2:
		money = 200;
		break;
	case 3:
		money = 300;
		break;
	case 4:
		money = 500;
		break;
	case 5:
		money = 1000;
		break;
	case 6:
		money = 2000;
		break;
	case 7:
		money = 4000;
		break;
	case 8:
		money = 8000;
		break;
	case 9:
		money = 16000;
		break;
	case 10:
		money = 32000;
		break;
	case 11:
		money = 64000;
		break;
	case 12:
		money = 125000;
		break;
	case 13:
		money = 250000;
		break;
	case 14:
		money = 500000;
		break;
	case 15:
		money = 1000000;
		break;
	default:
		money = 0;
	}

	return money;
}

int calculateMoneyToTakeHome(int score) {

	int money;

	switch (score) {
	case 100:
		money = 0;
		break;
	case 200:
		money = 0;
		break;
	case 300:
		money = 0;
		break;
	case 500:
		money = 0;
		break;
	case 1000:
		money = 1000;
		break;
	case 2000:
		money = 1000;
		break;
	case 4000:
		money = 1000;
		break;
	case 8000:
		money = 1000;
		break;
	case 16000:
		money = 1000;
		break;
	case 32000:
		money = 32000;
		break;
	case 64000:
		money = 32000;
		break;
	case 125000:
		money = 32000;
		break;
	case 250000:
		money = 32000;
		break;
	case 500000:
		money = 32000;
		break;
	case 1000000:
		money = 1000000;
		break;
	default:
		money = 0;
	}

	return money;
}

void updateLeaderboard(PLAYER* pokPLAYER) {

	FILE* fp = NULL;

	fp = fopen("leaderboard.txt", "w");
	if (fp == NULL) {
		printf("File can't be opened.\n");
		exit(-1);
	}

	rewind(fp);

	for (int i = 0; i < 10; i++)
	{
		char buffer[50];
		sprintf(buffer, "%s %d\n", pokPLAYER[i].name, pokPLAYER[i].points);
		fwrite(buffer, strlen(buffer), 1, fp);
	}

	fclose(fp);
}

void rules(void) {

	printf("================================RULES=================================\n");
	printf("The contestant must answer 15 multiple-choice questions \ncorrectly in a row to win the jackpot.");
	printf("\nThe contestant may quit at any time and keep their \nearnings. For each question, they are shown");
	printf("\nthe question and four possible answers in advance.");
	printf("\nIf they do decide to offer an answer, it must be \ncorrect to stay in the game.");

	printf("\n\nIf at any stage they answer incorrectly, they fall \nback to the last guarantee point - either 1,000$ or 32,000$\nand their game is over. For example, a contestant failing on\nquestion 13 would win 32,000$.\nAnswering incorrectly before reaching the first guarantee\npoint (1,000$) loses everything. ");
	printf("\n\nAt any point, the contestant may use up one (or more) \nof their three lifelines. These are:");
	printf("\n\n50:50 \n\ttwo of the three incorrect answers are removed.\n\tOriginally, these answers were chosen in advance by the\n\tquestion-setters (and so would invariably\n\t be the two you knew it couldn't be), but this was later\n\tchanged to a random selection.");
	printf("\n\nPhone-A-friend \n\tthe contestants may speak to a friend \n\tor relative on the phone for 30 seconds to discuss the\n\tquestion.");
	printf("\n\nAsk the audience \n\tthe audience votes with their keypads on their choice of\n\tanswer.");
	printf("\n\nEach lifeline may only be used once during a contestant's entire game.");
	printf("\n======================================\================================\n");
}

void FiftyFiftyLifeline(int* LifelineUsesAndHp, char solution) {

	char randomAnswerFinal;
	randomAnswerFinal = randomAnswer(solution);

	if (LifelineUsesAndHp[1] == 0) {
		printf("================================50/50=================================\n");
		printf("You used your 50/50 lifeline!\n");
		printf("======================================\================================\n");
	}

	if (LifelineUsesAndHp[1] == 1) {
		printf("================================50/50=================================\n");
		printf("You have used your 50/50 lifeline!");
		printf("\nThe correct answer is either %c) or %c)\n", solution, randomAnswerFinal);
		printf("======================================\================================\n");
		LifelineUsesAndHp[1] = 0;
	}
}

char randomAnswer(char solution) {

	char randomAnswer = 'A';
	int randomNumber = (rand() % (4 - 1 + 1)) + 1;

	while (randomAnswer == solution) {
		randomAnswer = randomAnswerGenerator(randomNumber);
	}

	return randomAnswer;
}

char randomAnswerGenerator(int randomNumber) {

	char randomAnswer = 'A';

	switch (randomNumber) {
	case '1':
		randomAnswer = 'A';
		return randomAnswer;
		break;
	case '2':
		randomAnswer = 'B';
		return randomAnswer;
		break;
	case '3':
		randomAnswer = 'C';
		return randomAnswer;
		break;
	case '4':
		randomAnswer = 'D';
		return randomAnswer;
		break;
	default:
		return randomAnswer;
		break;
	}
}

void CallLifeline(int* LifelineUsesAndHp, char solution) {

	if (LifelineUsesAndHp[2] == 0) {
		printf("================================CALL===================================\n");
		printf("You used your CALL lifeline!\n");
		printf("======================================\================================\n");
	}

	if (LifelineUsesAndHp[2] == 1) {
		printf("================================CALL===================================\n");
		printf("You have used your CALL lifeline!");
		printf("\nThe person you called thinks that correct answer is most likely %c)\n", solution);
		printf("======================================\================================\n");
		LifelineUsesAndHp[2] = 0;
	}
}

void VoteLifeline(int* LifelineUsesAndHp, char solution) {

	int randomCrowdNumber = (rand() % (100 - 60 + 1)) + 60;

	if (LifelineUsesAndHp[3] == 0) {
		printf("================================VOTE===================================\n");
		printf("You used your VOTE lifeline!\n");
		printf("======================================\================================\n");
	}

	if (LifelineUsesAndHp[3] == 1) {
		printf("================================VOTE===================================\n");
		printf("You have used your VOTE lifeline!");
		printf("\nThe %d percent of the crowd voted for answer %c)\n", randomCrowdNumber, solution);
		printf("======================================\================================\n");
		LifelineUsesAndHp[3] = 0;
	}
}