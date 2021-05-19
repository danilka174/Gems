#include "Bonus.h"

Bonus::Bonus(unsigned newX, unsigned newY, unsigned newType) { //конструктор бонуса
	x = newX;
	y = newY;
	type = newType;
}

void Bonus::DrawBonus(std::shared_ptr <sf::RenderWindow> window, std::shared_ptr<Field> field, float offsetHeight, float offsetWidth) { //печать текстуры бонуса
	float width = field->GetFieldWidth() / field->GetGemsInRow();
	float height = field->GetFieldHeight() / field->GetGemsInColumn();

	float posX = x * width + offsetWidth;
	float posY = y * height + offsetHeight;

	sf::RectangleShape shape(sf::Vector2f(width, height));
	shape.setPosition(posX, posY);
	shape.setTexture(field->GetTexture(type));
	window->draw(shape);
}

unsigned Bomb::Trigger(std::shared_ptr<Field> field) { // "взрыв" бомбы
	std::vector <std::array<unsigned, 2>> bombedGems;
	bombedGems.push_back({ y, x });

	for (unsigned i = 0; i < bombedNumber - 1; i++)
		bombedGems.push_back({ rand() % field->GetGemsInColumn(), rand() % field->GetGemsInRow() });

	return field->DeleteChoosedGems(bombedGems);
}

bool Painter::GemIsBad(std::shared_ptr<Field> field, unsigned x, unsigned y, std::vector <std::array<unsigned, 2>> paintedGems) { //проверка на возможность быть перекрашенным
	bool empty = false, choosing = false;

	if ((x < field->GetGemsInRow()) & (y < field->GetGemsInColumn()))
	{
		empty = field->GemIsEmpty(y, x);
		choosing = field->AlreadyChoose(y, x, paintedGems);

		return empty || choosing;
	}
	else
		return true;
}

unsigned Painter::Trigger(std::shared_ptr<Field> field) { //перекрашивание
	std::vector <std::array<unsigned, 2>> paintedGems;

	unsigned randomX, randomY;

	for (unsigned i = 0; i < paintedNumber; i++)
	{
		do
		{
			randomX = x + (rand() % (paintedRadius - 1) + 2) * (int)pow(-1, rand() % 2);
			randomY = y + (rand() % (paintedRadius - 1) + 2) * (int)pow(-1, rand() % 2);

		} while (GemIsBad(field, randomX, randomY, paintedGems));

		paintedGems.push_back({ randomY, randomX });
	}

	field->SetNewGemsColor(paintedGems, y, x);
	return 0;
}