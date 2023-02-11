#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <conio.h>

// pair 좌표
#define X first
#define Y second

// 게임판
const char stageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";

const int stageWidth = 8;
const int stageHeight = 5;

// 커서 숨김
void CursorView()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //커서 굵기 (1 ~ 100)
	cursorInfo.bVisible = FALSE; //커서 Visible TRUE(보임) FALSE(숨김)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}


enum class Object : unsigned int {
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,
	
	OBJ_UNKNOWN,
};


void initialize(Object*, int, int, const char*);
void draw(const Object*, int, int);
void update(Object*, char, int, int);
bool checkClear(const Object*, int, int);
bool checkPlayer(Object*, int);
void gameExit(Object*);


int main() {

	// 게임 보드 생성
	Object* state = new Object[stageWidth * stageHeight];
	// 스테이지 초기화
	initialize(state, stageWidth, stageHeight, stageData);
	CursorView();		// 커서 숨김

	while (true) {
		draw(state, stageWidth, stageHeight);

		if (checkClear(state, stageWidth, stageHeight)) {
			break;
		}
		// 입력값 취득
		std::cout << "a:left d:right w:up s:down. command?" << "\n";
		char input;
		// std::cin >> input;
		
		
		input = static_cast<char>(_getch()); // 키입력

		system("cls");
		update(state, input, stageWidth, stageHeight);

	}

	gameExit(state);

	return 0;
}


void initialize(Object* state, int width, int height, const char* stageData){
	const char* d = stageData;
	int x = 0, y = 0;
	
	// NULL 문자가 아닐동안
	while (*d != '\0') {
		Object t;
		switch (*d) {
		case '#': t = Object::OBJ_WALL; break;
		case ' ': t = Object::OBJ_SPACE; break;
		case 'o': t = Object::OBJ_BLOCK; break;
		case 'O': t = Object::OBJ_BLOCK_ON_GOAL; break;
		case '.': t = Object::OBJ_GOAL; break;
		case 'p': t = Object::OBJ_MAN; break;
		case 'P': t = Object::OBJ_MAN_ON_GOAL; break;
		case '\n':    // 다음 행으로
			x = 0;	  // x는 왼쪽 끝으로 되돌리기
			++y;	  // y는 한 단계 아래로 
			t = Object::OBJ_UNKNOWN; break; // 데이터는 없음

		default: t = Object::OBJ_UNKNOWN; break; // 부정 데이터
		}
		++d;
		// 모르는 문자일 경우 무시한다.
		if (t != Object::OBJ_UNKNOWN) {
			state[y * width + x] = t;   // 쓰기
			++x;
		}
	}
}

void draw(const Object* state, int width, int height){
	// Object 순서
	const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' };
	
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Object o = state[y * width + x];
			std::cout << font[static_cast<int>(o)]; // enum class 순서에 맞게 출력 
		}
		std::cout << std::endl;
	}
}

void update(Object* state, char input, int width, int height){
	int dx = 0;
	int dy = 0;
	switch (input) {
		case'a': 
			dx = -1; 
			break;     // 왼쪽
		case'd': 
			dx = 1; 
			break;		 // 오른쪽
		case'w': 
			dy = -1; 
			break;	 // 위
		case's': 
			dy = 1; 
			break;		 // 아래
	}

	// 플레이어 좌표 검색
	int i;
	for (i = 0; i < width * height; ++i) {
		// 목적지에 도착한 플레이어 or 보통 상태 플레이어인 경우
		if (checkPlayer(state, i)) {
			break;
		}
	}
	int x = i % width;
	int y = i / width;

	int tx = x + dx;
	int ty = y + dy;

	if (tx < 0 || ty < 0 || tx >= width || dy >= height)
		return;

	int p = y * width + x;
	int tp = ty * width + tx;
	if (state[tp] == Object::OBJ_SPACE || state[tp] == Object::OBJ_GOAL)
	{
		state[tp] = (state[tp] == Object::OBJ_GOAL) ? Object::OBJ_MAN_ON_GOAL : Object::OBJ_MAN;
		state[p] = (state[p] == Object::OBJ_MAN_ON_GOAL) ? Object::OBJ_GOAL : Object::OBJ_SPACE;
	}
	else if (state[tp] == Object::OBJ_BLOCK || state[tp] == Object::OBJ_BLOCK_ON_GOAL)
	{
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= width || ty2 >= height)
		{
			return;
		}

		int tp2 = ty2 * width + tx2;
		if (state[tp2] == Object::OBJ_SPACE || state[tp2] == Object::OBJ_GOAL)
		{
			state[tp2] = (state[tp2] == Object::OBJ_GOAL) ? Object::OBJ_BLOCK_ON_GOAL : Object::OBJ_BLOCK;
			state[tp] = (state[tp] == Object::OBJ_BLOCK_ON_GOAL) ? Object::OBJ_MAN_ON_GOAL : Object::OBJ_MAN;
			state[p] = (state[p] == Object::OBJ_MAN_ON_GOAL) ? Object::OBJ_GOAL : Object::OBJ_SPACE;
		}
	}
}

bool checkClear(const Object* state, int width, int height){
	for (int i = 0; i < width * height; i++) {
		if (state[i] == Object::OBJ_BLOCK)
			return false;
	}
	return true;
}

// 플레이어 상태확인 
bool checkPlayer(Object* state, int index) {
	return (state[index] == Object::OBJ_MAN) || (state[index] == Object::OBJ_MAN_ON_GOAL);
}

// 게임 종료 후
void gameExit(Object* state) {
	std::cout << "당신이 이겼습니다!!." << "\n";
	delete[] state;
	state = nullptr;
}




