#pragma once

#include <array>

#include "Gem.h"

class Field { //класс, который содержит гемсы в матрице
private:
	float height, width; //размер поля
	unsigned gemsInRow, gemsInColumn; //количество гемсов в строках и столбцах

	std::vector <std::vector <Gem>> gemsMatrix; //матрица гемсов
	std::array <sf::Color, 6> colorsMatrix = { sf::Color::Blue, sf::Color::Green, sf::Color::Red, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan }; //матрица цветов гемсов
	std::vector <std::array <unsigned, 2>> reiteration; //вектор координат одного цвета
	std::vector <sf::Texture> textureMatrix; //вектор текстур

public:
	Field(float fieldWindowHeight, float fieldWindowWidth, unsigned numberGemsInRow, unsigned numberGemsInColumn); //конструктор поля
	~Field(void) {} //деструктор поля

	void GenerateField(void); //заполнение поля
	void DrawField(std::shared_ptr <sf::RenderWindow> window, float offsetHeight, float offsetWidth); //вывод поля с гемсами

	unsigned GetGemsInRow(void); //получить число гемсов в строке
	unsigned GetGemsInColumn(void); //получить число гемсов в ряду
	float GetFieldHeight(void);  //получить высоту поля
	float GetFieldWidth(void); //получить ширину поля

	void CheckField(unsigned i, unsigned j); //найти соседей гема с тем же цветов
	bool NeighborIsGood(unsigned i, unsigned j, unsigned neighborI, unsigned neighborJ); //вспомогательная функция нахождения соседей
	bool AlreadyChoose(unsigned i, unsigned j, std::vector <std::array <unsigned, 2>> checkVector); //не позволяет добавить уже проверенный элемент
	unsigned DeleteChoosedGems(std::vector <std::array <unsigned, 2>> forDeletion); //удаление комбо
	unsigned FindGemsReiteration(void); //найти и удалить гемы повтор

	bool GemsDrop(void); //падение верхних гемов
	void FieldRefilling(void); //заполнение остатка

	void SetHighlight(unsigned x, unsigned y, sf::Color newColor); //обводка гема
	void GemsSwap(unsigned x1, unsigned y1, unsigned x2, unsigned y2); //поменять местами два гема

	void SetNewGemsColor(std::vector <std::array <unsigned, 2>> paintedGems, unsigned i, unsigned j); //задать новый цвет гема

	sf::Texture* GetTexture(unsigned type); //текстура под бонус
	bool GemIsEmpty(unsigned i, unsigned j); //пустой ли гем
	std::vector <std::array <unsigned, 2>> GetReiterationVector(void); //получить вектор с комбинациями гемов
};