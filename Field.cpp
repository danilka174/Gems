#include "Field.h"

Field::Field(float fieldWindowHeight, float fieldWindowWidth, unsigned numberGemsInRow, unsigned numberGemsInColumn) { //конструктор поля
	height = fieldWindowHeight;
	width = fieldWindowWidth;
	gemsInRow = numberGemsInRow;
	gemsInColumn = numberGemsInColumn;

	sf::Texture texture;

	for (unsigned i = 0; i < 2; i++) {
		texture.loadFromFile("Textures/" + std::to_string(i) + ".png");
		textureMatrix.push_back(texture);
	}

	GenerateField();
}

bool Field::GemIsEmpty(unsigned i, unsigned j) { //пустой ли гем
	return gemsMatrix[i][j].IsEmpty();
}

void Field::SetNewGemsColor(std::vector <std::array <unsigned, 2>> paintedGems, unsigned i, unsigned j) { //изменение цвета гема
	for (unsigned k = 0; k < paintedGems.size(); k++)
		gemsMatrix[paintedGems[k][0]][paintedGems[k][1]].SetColor(gemsMatrix[i][j].GetColor());
}

void Field::GenerateField(void) { //заполнение поля
	float gemHeight = height / gemsInColumn, gemWidth = width / gemsInRow;

	for (unsigned i = 0; i < gemsInColumn; i++) {
		std::vector <Gem> temp;

		for (unsigned j = 0; j < gemsInRow; j++)
			temp.push_back({ gemHeight, gemWidth, colorsMatrix[rand() % colorsMatrix.size()] });

		gemsMatrix.push_back(temp);
		temp.~vector();
	}
}

void Field::DrawField(std::shared_ptr <sf::RenderWindow> window, float offsetHeight, float offsetWidth) { //вывод поля с гемсами
	float posX, posY;

	for (unsigned i = 0; i < gemsInColumn; i++)
		for (unsigned j = 0; j < gemsInRow; j++) {
			posX = j * width / gemsInRow + offsetWidth;
			posY = i * height / gemsInColumn + offsetHeight;
			gemsMatrix[i][j].DrawGem(window, posX, posY);
		}
}

sf::Texture* Field::GetTexture(unsigned type) { //получить текстуру под бонус
	return &textureMatrix[type];
}

unsigned Field::GetGemsInRow(void) { //получить количество гемов в ряду
	return gemsInRow;
}

unsigned Field::GetGemsInColumn(void) { //получить количество гемов в столбце
	return gemsInColumn;
}

std::vector <std::array <unsigned, 2>> Field::GetReiterationVector(void) { //получить вектор с комбинациями гемов
	return reiteration;
}

float Field::GetFieldHeight(void) { //получить высоту поля
	return height;
}

float Field::GetFieldWidth(void) { //получить ширину поля
	return width;
}

void Field::SetHighlight(unsigned x, unsigned y, sf::Color newColor) { //обводка гема
	if (!gemsMatrix[y][x].IsEmpty())
		gemsMatrix[y][x].SetOutline(newColor);
}

void Field::GemsSwap(unsigned x1, unsigned y1, unsigned x2, unsigned y2) { //поменять местами два гема
	sf::Color color1, color2;
	color1 = gemsMatrix[y1][x1].GetColor();
	color2 = gemsMatrix[y2][x2].GetColor();

	gemsMatrix[y1][x1].SetColor(color2);
	gemsMatrix[y2][x2].SetColor(color1);
}

bool Field::GemsDrop(void) { //падение верхних гемов
	bool drop = false;

	for (unsigned i = 0; i < gemsInColumn - 1; i++)
		for (unsigned j = 0; j < gemsInRow; j++)
			if ((!gemsMatrix[i][j].IsEmpty()) && (gemsMatrix[i + 1][j].IsEmpty()))
			{
				gemsMatrix[i + 1][j].SetColor(gemsMatrix[i][j].GetColor());
				gemsMatrix[i][j].SetEmpty();
				drop = true;
			}
	return drop;
}

void Field::FieldRefilling(void) { //заполнение остатка
	for (unsigned i = 0; i < gemsInColumn; i++)
		for (unsigned j = 0; j < gemsInRow; j++)
			if (gemsMatrix[i][j].IsEmpty())
				gemsMatrix[i][j].SetColor(colorsMatrix[rand() % colorsMatrix.size()]);
}

bool Field::NeighborIsGood(unsigned i, unsigned j, unsigned neighborI, unsigned neighborJ) { //вспомогательная функция нахождения соседей
	bool colorIsGood = false, notChoosed = false;

	if (gemsMatrix[i][j].GetColor() == gemsMatrix[neighborI][neighborJ].GetColor())
		colorIsGood = true;
	if (!AlreadyChoose(neighborI, neighborJ, reiteration))
		notChoosed = true;

	return colorIsGood & notChoosed;
}

void Field::CheckField(unsigned i, unsigned j) { //найти соседей гема с тем же цветов
	if (gemsMatrix[i][j].IsEmpty()) //пропустить пустые гемы
		return;

	if ((i != 0) && (NeighborIsGood(i, j, i - 1, j))) //верхний
	{
		reiteration.push_back({ i - 1, j });
		CheckField(i - 1, j);
	}

	if ((j != gemsInRow - 1) && (NeighborIsGood(i, j, i, j + 1))) //правый
	{
		reiteration.push_back({ i, j + 1 });
		CheckField(i, j + 1);
	}

	if ((i != gemsInColumn - 1) && (NeighborIsGood(i, j, i + 1, j))) //нижний
	{
		reiteration.push_back({ i + 1, j });
		CheckField(i + 1, j);
	}

	if ((j != 0) && (NeighborIsGood(i, j, i, j - 1))) //левый
	{
		reiteration.push_back({ i, j - 1 });
		CheckField(i, j - 1);
	}
}

bool Field::AlreadyChoose(unsigned i, unsigned j, std::vector <std::array <unsigned, 2>> checkVector) { //не позволяет добавить уже проверенный элемент
	for (unsigned k = 0; k < checkVector.size(); k++)
		if ((i == checkVector[k][0]) && (j == checkVector[k][1]))
			return true;
	return false;
}

unsigned Field::DeleteChoosedGems(std::vector <std::array <unsigned, 2>> forDeletion) { //удаление комбо
	for (unsigned k = 0; k < forDeletion.size(); k++)
		gemsMatrix[forDeletion[k][0]][forDeletion[k][1]].SetEmpty();

	return (unsigned)forDeletion.size();
}

unsigned Field::FindGemsReiteration(void) { //найти и удалить гемы повтор
	reiteration.clear();

	for (unsigned i = 0; i < gemsInColumn; i++)
	{
		for (unsigned j = 0; j < gemsInRow; j++)
		{
			reiteration.push_back({ i, j });
			CheckField(i, j);

			if (reiteration.size() > 2)
				break;
			else
				if (!reiteration.empty())
					reiteration.clear();
		}

		if (reiteration.size() > 2)
			break;
	}

	return DeleteChoosedGems(reiteration);
}