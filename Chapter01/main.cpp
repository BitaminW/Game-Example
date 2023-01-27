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
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,
	
	OBJ_UNKNOWN,
};


void initialize(Object*, int, int, const char*);
void draw(const Object*, int, int);
void update(Object*, char, int, int);
bool checkClear(const Object*, int, int);
void gameExit(Object*);


int main() {

	// 게임 보드 생성
	Object* state = new Object[stageWidth * stageHeight];
	// 스테이지 초기화
	initialize(state, stageWidth, stageHeight, stageData);

	while (true) {
		draw(state, stageWidth, stageHeight);

		if (checkClear(state, stageWidth, stageHeight)) {
			break;
		}
		// 입력값 취득
		std::cout << "a:left s:right w:up z:down. command?" << "\n";
		char input;
		std::cin >> input;

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
}

bool checkClear(const Object* state, int width, int height){
	return false;
}

// 게임 종료 후
void gameExit(Object* state) {
	std::cout << "당신이 이겼습니다!!." << "\n";
	delete[] state;
	state = nullptr;
}

