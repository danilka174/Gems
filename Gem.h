#pragma once

#include <SFML/Graphics.hpp>

class Gem { //класс гемсов
private:
	float height, width; //размеры гема
	sf::Color color; //цвет гема
	sf::Color outline; //обводка гема

public:
	Gem(float gemHeight, float gemWidth, sf::Color gemColor); //конструктор гема
	~Gem(void) {} //деструктор гема

	bool IsEmpty(void); //проверяет гем на пустоту

	void SetEmpty(void); //делает гем пустым
	void SetColor(sf::Color newColor); //задает новый цвет гема
	void SetOutline(sf::Color newColor); //задает новый цвет обводки

	sf::Color GetColor(void); //возвращает цвет гема

	void DrawGem(std::shared_ptr <sf::RenderWindow> window, float x, float y); //рисует гем на поле
}; 