#include "Gem.h"

Gem::Gem(float gemHeight, float gemWidth, sf::Color gemColor) { //конструктор гема
	height = gemHeight; width = gemWidth;
	color = gemColor;
	outline = sf::Color::Black;
}

bool Gem::IsEmpty(void) { //проверка на пустоту
	if (color == sf::Color::Black)
		return true;
	return false;
}

void Gem::SetEmpty(void) { //делает гем пустым
	color = sf::Color::Black;
}

sf::Color Gem::GetColor(void) { //возвращает цвет гема
	return color;
}

void Gem::SetColor(sf::Color newColor) { //задать цвет гема
	color = newColor;
}

void Gem::SetOutline(sf::Color newColor) { //задать цвет обводки
	outline = newColor;
}


void Gem::DrawGem(std::shared_ptr <sf::RenderWindow> window, float x, float y) { //нарисовать гем на поле
	sf::RectangleShape shape(sf::Vector2f(width, height));

	shape.setFillColor(color);
	shape.setPosition(x, y);
	shape.setOutlineThickness((height + width) / -40);
	shape.setOutlineColor(outline);

	window->draw(shape);
}