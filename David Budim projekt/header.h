#ifndef HEADER_H
#define HEADER_H

typedef struct question {
	char* question;
	int id;
	struct question* next;
}QUESTION;

typedef struct answer {
	char* answer;
	int id;
	struct answer* next;
}ANSWER;

typedef struct solution {
	char* solution;
	int id;
	struct solution* next;
}SOLUTION;

typedef struct player {
	char name[16];
	int points;
}PLAYER;

char* gameIntro(void);
int menu(const char* const fileName);
void readLeaderboard(void);
int exitProgram(void);
int game(void);
int loadingQuestions(int level);
void loadingAnswers(int level, int gameScenario);
int* loadingSolutions(int level, int gameScenario, int* LifelineUsesAndHp);
QUESTION* createSLList(FILE* p);
void input(char** pointer, FILE* p);
QUESTION* insertNewNodeSLList(QUESTION* headNode, FILE* p);
int traverseSLList(QUESTION* traverseNode);
QUESTION* searchSLList(QUESTION* traverseNode, int criteria);
void printLineNodeSLList(QUESTION** headNode, QUESTION* targetNode, int level);
void deleteNodeSLList(QUESTION** headNode, QUESTION* targetNode);
QUESTION* deleteWholeSLList(QUESTION* traverseNode);
ANSWER* createSLListAnswer(FILE* p);
ANSWER* insertNewNodeSLListAnswer(ANSWER* headNode, FILE* p);
int traverseSLListAnswer(ANSWER* traverseNode);
void printLineNodeSLListAnswer(ANSWER** headNode, ANSWER* targetNode);
void deleteNodeSLListAnswer(ANSWER** headNode, ANSWER* targetNode);
ANSWER* deleteWholeSLListAnswer(ANSWER* traverseNode);
SOLUTION* createSLListSolution(FILE* p);
SOLUTION* insertNewNodeSLListSolution(SOLUTION* headNode, FILE* p);
int traverseSLListSolution(SOLUTION* traverseNode);
void printLineNodeSLListSolution(SOLUTION** headNode, SOLUTION* targetNode);
void compareToSolution(SOLUTION** headNode, SOLUTION* targetNode);
void deleteNodeSLListSolution(SOLUTION** headNode, SOLUTION* targetNode);
SOLUTION* deleteWholeSLListSolution(SOLUTION* traverseNode);
void* dynamicMemoryAllocation(int number, int size);
void loadLeaderboard(PLAYER* pokPLAYER);
void printField(PLAYER* pokPLAYER);
int calculateMoney(int level);
PLAYER newPlayerScore(int score, char* username);
PLAYER* comparingToLeaderboard(PLAYER* pokPLAYER, PLAYER newPlayerScore);
void updateLeaderboard(PLAYER* pokPLAYER);
void recursivePrint(int i, PLAYER* pokPLAYER);
int calculateMoneyToTakeHome(int level);
void rules(void);
void FiftyFiftyLifeline(int* LifelineUsesAndHp, char solution);
char randomAnswer(char solution);
char randomAnswerGenerator(int randomNumber);
void CallLifeline(int* LifelineUsesAndHp, char solution);
void VoteLifeline(int* LifelineUsesAndHp, char solution);

#endif //HEADER_H