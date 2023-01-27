#include <iostream>

const char stageData[] = "\
########\n\
# ..   #\n\
# oo   #\n\
#      #\n\
########";

const int stageWidth = 8;
const int stageHeight = 5;

enum class Object {
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_COAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,
	
	OBJ_UNKNOWN,
};


void initialize(Object*, int, int, const char*);
void getInput(const Object*, int, int);
void update(Object*, char, int, int);
bool checkClear(const Object*, int, int);


int main() {

	// 게임 보드 생성
	Object* state = new Object[stageWidth * stageHeight];
	// 스테이지 초기화
	initialize(state, stageWidth, stageHeight, stageData);

	while (true) {
		//
	}

	delete[] state;
	state = nullptr;

	return 0;
}


void initialize(Object* state, int w, int h, const char* stageData){

}

void getInput(const Object* state, int w, int h){
}

void update(Object* state, char input, int w, int h){
}

bool checkClear(const Object* state, int w, int h){
	return false;
}




