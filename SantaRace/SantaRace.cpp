#include <bangtal.h>

SceneID scene;

ObjectID startButton, endButton, playButton, santa, present;

bool presentDrop = false;

int santaX = 0, santaY = 500;

TimerID timer1;


void startGame() {
	hideObject(startButton);
	hideObject(endButton);

	showObject(playButton);

	setTimer(timer1, 10.f);
	startTimer(timer1);
}

void endGame(bool succeess) {
	if (succeess) {
		showMessage("선물 배달 성공~");
	}
	else {
		showMessage("선물 배달 실패!!");
	}

	hideObject(playButton);

	setObjectImage(startButton, "Images/restart.png");
	showObject(startButton);
	showObject(endButton);

	santaX = 0;
	locateObject(santa, scene, santaX, santaY);

	stopTimer(timer1);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == endButton) {
		endGame();
	}
	else if (object == startButton) {
		startGame();
	}
	else if (object == playButton) {
		santaX = santaX + 30;
		locateObject(santa, scene, santaX, santaY);
		locateObject(present, scene, santaX + 30, santaY + 10);

		if (santaX > 1280) {
			endGame(true);
		}
	}
}

void timerCallback(TimerID timer) {
	if (timer == timer1)
		endGame(false);
}


int main() {
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

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

	present = createObject("Images/present.png");
	locateObject(present, scene, santaX + 30, santaY + 10);
	scaleObject(present, 0.05f);
	showObject(present);

	playButton = createObject("Images/play.png");
	locateObject(playButton, scene, 610, 30);

	timer1 = createTimer(10.f);
	showTimer(timer1);

	startGame(scene);
}