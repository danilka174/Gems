#pragma once

#include "Field.h"

class Bonus { //абстрактный класс для бонусов
protected:
	unsigned x, y; //позиция бонуса
	unsigned type; //тип бонуса: "0" - бомба или "1" - кисточка

public:
	Bonus(unsigned newX, unsigned newY, unsigned newType); //конструктор бонуса
	~Bonus(void) {} //деструктор бонуса

	virtual unsigned Trigger(std::shared_ptr<Field> field) = 0; //виртуальная функция для использования бонуса
	void DrawBonus(std::shared_ptr <sf::RenderWindow> window, std::shared_ptr<Field> field, float offsetHeight, float offsetWidth); //печать текстуры бонуса
};

class Bomb :public Bonus { //наследственный класс бонуса - бомба, которая уничтожает пять гемсов
private:
	unsigned bombedNumber = 5; //количество гемсов для уничтожения

public:
	Bomb(unsigned newX, unsigned newY) :Bonus(newX, newY, 0) {} //конструктор для бомбы
	unsigned Trigger(std::shared_ptr<Field> field) override; //"взрыв" бомбы
	~Bomb(void) {} //деструктор для бомбы
};

class Painter :public Bonus { //наследственный класс бонуса - кисточка, которая перекрашивает два гемса в цвет того, на какой попала кисть
private:
	unsigned paintedNumber = 2; //количество гемсов, которые покрасят
	unsigned paintedRadius = 3; //радиус покраски

public:
	Painter(unsigned newX, unsigned newY) :Bonus(newX, newY, 1) {} //конструктор для кисточки
	unsigned Trigger(std::shared_ptr<Field> field) override; //окрашивание
	bool GemIsBad(std::shared_ptr<Field> field, unsigned x, unsigned y, std::vector <std::array<unsigned, 2>> paintedGems); //проверка гемса на возможность быть перекрашенным

	~Painter(void) {} //деструктор для кисточки
};