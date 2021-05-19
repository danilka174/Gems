#include "GameLoop.h"

int main(void) {

	GameLoop Game;

	Game.FieldOffset(); //задаем позицию поля в окошке
	Game.Start(); //игра работает

	return 0;
}