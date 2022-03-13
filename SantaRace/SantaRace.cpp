#include <bangtal.h>
#include <string>
using namespace std;

SceneID scene;

ObjectID startButton, endButton, santa;
ObjectID arrowUI[5], presents[20];

int santaX = 0, santaY = 550;
int presentScore=0, arrowNum=0;
int arrow[5];

TimerID presentTimer,deliveryTimer;

bool isPlaying=false, presentDirect=true;

//화살표 이미지 변경
void changeArrowUi(int i, int arrow) {
	arrowNum = 0;
	switch (arrow)
	{
	case 1:
		setObjectImage(arrowUI[i], "Images/up.png");
		break;
	case 2:
		setObjectImage(arrowUI[i], "Images/right.png");
		break;
	case 3:
		setObjectImage(arrowUI[i], "Images/down.png");
		break;
	case 4:
		setObjectImage(arrowUI[i], "Images/left.png");
	}
	showObject(arrowUI[i]);
}

//화살표 초기화
void changeArrow() {
	printf("\n");
	for (int i = 0; i < 5; i++) {
		arrow[i] = rand() % 4 + 1;
		changeArrowUi(i, arrow[i]);
		printf("%d", arrow[i]);
	}
}

//입력한 키와 화살표가 맞는지
bool checkArrow(KeyCode code) {
	printf("\narrowNum:%d, num:%d, KeyCode:", arrowNum, arrow[arrowNum]);
	switch (code)
	{
	case KeyCode::KEY_UP_ARROW:
		printf("1");
		if (arrow[arrowNum] == 1) {
			return true;
		}
		break;
	case KeyCode::KEY_RIGHT_ARROW:
		printf("2");
		if (arrow[arrowNum] == 2) {
			return true;
		}
		break;
	case KeyCode::KEY_DOWN_ARROW:
		printf("3");
		if (arrow[arrowNum] == 3) {
			return true;
		}
		break;
	case KeyCode::KEY_LEFT_ARROW:
		printf("4");
		if (arrow[arrowNum] == 4) {
			return true;
		}
		break;
	}
	return false;
	
}

//선물 개수 관리
void presentMg(int num) {
	//맞추면 하나 추가
	if (num > 0) { 
		showObject(presents[presentScore]);
		presentScore += num;
		santaX += 60;
	}
	//틀리면 하나 마이너스
	else { 
		if (presentScore > 0) {
			presentScore += num;
			hideObject(presents[presentScore]);
			santaX -= 60;
		}
	}
	locateObject(santa, scene, santaX, santaY);
}

void startGame() {
	hideObject(startButton);
	hideObject(endButton);

	presentScore = 0;

	setTimer(presentTimer, 20.f);
	startTimer(presentTimer);
}

void finishGame() {
	isPlaying = false;
	string str = to_string(presentScore);
	str += "개\n선물 배달 성공~";
	const char* cstr = str.c_str();
	showMessage(cstr);

	for (int i = 0; i < 5; i++) {
		hideObject(arrowUI[i]);
	}
	for (int i = 0; i < presentScore; i++) {
		hideObject(presents[i]);
	}
	setObjectImage(startButton, "Images/restart.png");
	showObject(startButton);
	showObject(endButton);

	santaX = 0;
	locateObject(santa, scene, santaX, santaY);

	stopTimer(presentTimer);
}

void keyboardCallback(KeyCode code, KeyState state) {
	
	if (state == KeyState::KEY_PRESSED&&isPlaying) {
		if (checkArrow(code)) {
			printf("correct");
			arrowNum++;
			if (arrowNum > 4) {
				presentMg(1);
				changeArrow();
			}
		}
		else {
			printf("false");
			presentMg(-1);
			changeArrow();
		}
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == endButton) {
		endGame();
	}
	else if (object == startButton) {
		isPlaying = true;
		changeArrow();
		startGame();
	}
}

void timerCallback(TimerID timer) {
	if (timer == presentTimer)
		finishGame();
}

int main() {
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboardCallback);

	scene = createScene("Santa Race", "Images/background.png");

	startButton = createObject("Images/start.png");
	locateObject(startButton, scene, 590, 70);
	showObject(startButton);

	endButton = createObject("Images/end.png");
	locateObject(endButton, scene, 590, 20);
	showObject(endButton);

	santa = createObject("Images/santa.png");
	locateObject(santa, scene, santaX, santaY);
	showObject(santa);


	for (int i = 0; i < 5; i++) {
		arrowUI[i] = createObject("Images/right.png");
		locateObject(arrowUI[i], scene, 460 + i * 50, 30);
	}

	for (int i = 0; i < 20; i++) {
		presents[i] = createObject("Images/present.png");
		scaleObject(presents[i],0.1f);
		locateObject(presents[i], scene, 0 + i * 60, 560);
	}
	presentTimer = createTimer(20.f);

	showTimer(presentTimer);

	startGame(scene);

}