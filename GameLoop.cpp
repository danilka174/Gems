#include "GameLoop.h"

#define TARGET_SCORE 100
#define CHANCE_OF_BONUS 5

GameLoop::GameLoop(void) { //конструктор
	userWindowHeight = 600;
	userWindowWidth = 500;
	fieldWindowHeight = 480;
	fieldWindowWidth = 480;
	numberGemsInRow = 5;
	numberGemsInColumn = 6;
	offsetHeight = 0;
	offsetWidth = 0;

	score = 0;

	drop = false;
	secondClick = false;
	swap = false;
	firstswap = false;

	gem1X = 0;
	gem1Y = 0;
	gem2X = 0;
	gem2Y = 0;

	window = std::make_shared<sf::RenderWindow>(sf::VideoMode(userWindowWidth, userWindowHeight), "Gems");
	window->setFramerateLimit(3); //скорость обновления кадров

	field = std::make_shared<Field>((float)fieldWindowHeight, (float)fieldWindowWidth, numberGemsInRow, numberGemsInColumn);
}

void GameLoop::FieldOffset(void) { //смещение окошка поля по центру окна
	float minOffset;
	offsetHeight = (float)(userWindowHeight - fieldWindowHeight);
	offsetWidth = (float)(userWindowWidth - fieldWindowWidth);
	minOffset = offsetWidth / 2;
	offsetWidth = minOffset;
	offsetHeight -= offsetWidth;
}

void GameLoop::BonusCoordinates(unsigned& bonusX, unsigned& bonusY, unsigned k) { //генерация позиции возникновения бонуса
	unsigned randomOffsetX, randomOffsetY;

	do
	{
		randomOffsetX = (rand() % 3 + 1) * (int)pow(-1, rand() % 2);
		randomOffsetY = (rand() % 3 + 1) * (int)pow(-1, rand() % 2);

		bonusX = field->GetReiterationVector()[k][1] + randomOffsetX;
		bonusY = field->GetReiterationVector()[k][0] + randomOffsetY;

	} while ((bonusX >= field->GetGemsInRow()) || (bonusY >= field->GetGemsInColumn()) || (field->GemIsEmpty(bonusY, bonusX)));
}

void GameLoop::SpawnBonus(void) { //генерация вида бонуса 
	unsigned bonusX, bonusY;
	for (unsigned k = 0; k < (field->GetReiterationVector()).size(); k++)
		if (rand() % 100 < CHANCE_OF_BONUS)
		{
			BonusCoordinates(bonusX, bonusY, k);

			switch (rand() % 2)
			{
			case 0:
				bonusesMatrix.push_back(std::make_shared<Bomb>(bonusX, bonusY));
				break;
			case 1:
				bonusesMatrix.push_back(std::make_shared<Painter>(bonusX, bonusY));
				break;
			}
		}
}

void GameLoop::GemsDeletion(void) { //удаление комбинации гемсов
	unsigned addition = 0;
	if (!drop) {
		addition = field->FindGemsReiteration(); //сколько удалили
		if ((swap) & (addition == 0))
			field->GemsSwap(gem2X, gem2Y, gem1X, gem1Y); 
		score += addition;
	}

	swap = false;

	drop = field->GemsDrop();
	if (!drop) {
		field->FieldRefilling();
		SpawnBonus();
	}
}

void GameLoop::BonusTrigger(void) { //использование бонуса
	for (unsigned k = 0; k < bonusesMatrix.size(); k++)
		score += bonusesMatrix[k]->Trigger(field); //добавление счета

	if (bonusesMatrix.size() > 0) {
		drop = true; //идет падение
		bonusesMatrix.clear();
	}
}

bool GemsIsGood(unsigned gem1X, unsigned gem1Y, unsigned gem2X, unsigned gem2Y) { //проверка соседства гемсов
	bool radiusCheck = false;
	bool diagonalCheck = false;

	if ((abs((int)gem2X - (int)gem1X) <= 1) && (abs((int)gem2Y - (int)gem1Y) <= 1))
		radiusCheck = true;

	if (!((abs((int)gem2X - (int)gem1X) == 1) && (abs((int)gem2Y - (int)gem1Y) == 1)))
		diagonalCheck = true;

	return (radiusCheck & diagonalCheck);
}

void GameLoop::SelectFirstGem(float xInMatrix, float yInMatrix, float offsetX, float offsetY) { //первый клик по одному гему
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { 
		sf::Vector2i localPosition1 = sf::Mouse::getPosition(*window);

		float x = localPosition1.x / xInMatrix;
		float y = localPosition1.y / yInMatrix;

		gem1X = (unsigned)(x - offsetX);
		gem1Y = (unsigned)(y - offsetY);

		field->SetHighlight(gem1X, gem1Y, sf::Color::White);
		secondClick = true;
	}
}

void GameLoop::CancelClick(void) { //отмена выделения
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		field->SetHighlight(gem1X, gem1Y, sf::Color::Black);
		secondClick = false;
	}
}

void GameLoop::GemsPermutation(void) { //изменение цветов двух соответсвующих гемсов
	if (GemsIsGood(gem1X, gem1Y, gem2X, gem2Y)) {
		field->GemsSwap(gem1X, gem1Y, gem2X, gem2Y);
		field->SetHighlight(gem1X, gem1Y, sf::Color::Black);
		field->SetHighlight(gem2X, gem2Y, sf::Color::Black);

		swap = true;
		firstswap = true;
	}
}

void GameLoop::SelectSecondGem(float xInMatrix, float yInMatrix, float offsetX, float offsetY) { //второй клик уже по другому гему и смена цветов между двумя гемами
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i localPosition2 = sf::Mouse::getPosition(*window);

		float x = localPosition2.x / xInMatrix;
		float y = localPosition2.y / yInMatrix;

		gem2X = (unsigned)(x - offsetX);
		gem2Y = (unsigned)(y - offsetY);

		GemsPermutation();
		secondClick = false;
	}
	else
		CancelClick();
}

void GameLoop::Swap(void) { //выделение одного гема или смена двух выделенных пользователем гемсов (просто изменение цветов, без удаления)
	float xInMatrix = fieldWindowWidth / (float)numberGemsInRow;
	float yInMatrix = fieldWindowHeight / (float)numberGemsInColumn;

	float offsetX = offsetWidth / xInMatrix;
	float offsetY = offsetHeight / yInMatrix;

	if (!drop)
		if (!secondClick)
			SelectFirstGem(xInMatrix, yInMatrix, offsetX, offsetY);
		else
			SelectSecondGem(xInMatrix, yInMatrix, offsetX, offsetY);
}

void GameLoop::DrawGameLoop(void) { //вывод гемсов и показ бонусов на поле
	field->DrawField(window, offsetHeight, offsetWidth);

	for (unsigned k = 0; k < bonusesMatrix.size(); k++)
		bonusesMatrix[k]->DrawBonus(window, field, offsetHeight, offsetWidth);
}

void GameLoop::Win(void) { //вывод победной картинки
	sf::Texture texture;
	texture.loadFromFile("Textures/win.png");

	sf::RectangleShape shape(sf::Vector2f((float)fieldWindowWidth, (float)fieldWindowHeight));
	shape.setPosition(offsetWidth, offsetHeight);
	shape.setTexture(&texture);

	window->draw(shape);
	window->display();

	sf::Clock timer;
	while (timer.getElapsedTime().asSeconds() < 5);

	window->close();
}

void GameLoop::CreateText(void) { //показ баллов
	sf::Font font;
	font.loadFromFile("Textures/font.otf");

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(90);
	text.setPosition(offsetWidth, offsetHeight / 25);

	if (!firstswap)
		score = 0;
	text.setString("Your score:   " + std::to_string(score));
	window->draw(text);
}

void GameLoop::Start(void) { //геймплей
	while (window->isOpen()) {
		sf::Event event; 
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();
		}

		window->clear();

		BonusTrigger(); // взрыв имеющегося бонуса
		GemsDeletion(); //удаление комбо
		Swap(); //выделение одного гема или смена двух выделенных пользователем гемсов (просто изменение цветов, без удаления)
		DrawGameLoop(); //вывод поля с гемсами и бонусами
		CreateText(); //текст со счетом

		if (score >= TARGET_SCORE) {
			Win();
			break;
		}

		window->display();
	}
}