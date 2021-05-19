#pragma once

#include "Field.h"
#include "Bonus.h"

class GameLoop { //implements the entire gameplay class
private:
	unsigned userWindowHeight, userWindowWidth; //размер окна
	unsigned fieldWindowHeight, fieldWindowWidth; //размер поля
	unsigned numberGemsInRow, numberGemsInColumn; //количество гемсов в строку и колонку
	unsigned score; //счет
	float offsetHeight, offsetWidth; //смещение

	bool drop; //продолжается ли падение
	bool secondClick; //выбран ли второй гем 
	bool swap, firstswap; //если идет обмен

	unsigned gem1X, gem1Y, gem2X, gem2Y; //координаты двух гемов

	std::shared_ptr <sf::RenderWindow> window; //указатель на окошко
	std::shared_ptr <Field> field; //указатель на поле
	std::vector<std::shared_ptr<Bonus>> bonusesMatrix; //бонусы

public:
	GameLoop(void); //конструктор
	~GameLoop(void) {} //деструктор 

	void FieldOffset(void); //обозначает смещение окошка
	void Start(void); //геймплей

	void SpawnBonus(void); //какой бонус выпадет
	void BonusCoordinates(unsigned& bonusX, unsigned& bonusY, unsigned k); //генерация координат бонуса
	void BonusTrigger(void); //использование бонуса
	void GemsDeletion(void); //удаление комбо

	void SelectFirstGem(float xInMatrix, float yInMatrix, float offsetX, float offsetY); //первый клик по одному гему
	void SelectSecondGem(float xInMatrix, float yInMatrix, float offsetX, float offsetY); //второй клик уже по другому гему
	void CancelClick(void); //отмена клика
	void GemsPermutation(void); //изменение цветов двух соответствующих гемсов
	void Swap(void); //выделение одного гема или смена двух выделенных пользователем гемсов (просто изменение цветов, без удаления)

	void DrawGameLoop(void); //вывод гемсов и показ бонусов на поле
	void CreateText(void); //счет

	void Win(void); //победа
}; 