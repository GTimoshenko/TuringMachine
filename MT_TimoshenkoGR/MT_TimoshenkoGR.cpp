#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <Windows.h>
using namespace std;

//Achtung123

//--- Achtung!
// hConsole - переменная, с помощью которой мы получаем дескриптор консоли, позволяющий изменять формат ввода/вывода.
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

/* Структура table хранит в себе 
* char command - символ(>, <, .), как должна двигаться головка машины
* char pastingSymbol - символ, на который будет заменен текущий
* int condition - состояние, в которое перейдет машина
*/
typedef struct table {
	char command;
	char pastingSymbol;
	int condition;
}table;

/* Функция readConsole получает на вход
* string a - строка, считываемая из консоли
* Далее функция обрабатывает эту строку, проверяет на наличие не соответствующих условию символов.
* Возвращает переменную типа int inputIsCorrect, которая принимает значение 0, если строка не подходит,
* значение 1, если строка - подходящая.
* Проверяет строку на валидность.
*/
bool readConsole(string a) {
	int inputIsCorrect = 1;

	for (int i = 0; i < a.size(); i++) {
		if (tolower(a.at(i)) != 'a' && tolower(a.at(i)) != 'b' && tolower(a.at(i)) != 'c') {
			inputIsCorrect = 0;
			break;
		}
	}

	return inputIsCorrect;
}

/*Функция readInput получает на вход string fileName - название файла, в котором описаны состояния машины.
* Возвращает map<char, vector<table>> inputTable - map, в которой ключом являются символы алфавита машины,
* vector<table> - вектор структур table, хранящий для каждого состояния данные из структуры.
* Функция заполняет этот map.
*/
map <char, vector<table>> readInput(string fileName) {
	ifstream fin(fileName);
	int alphabetSize, numOfConditions;

	fin >> alphabetSize >> numOfConditions;

	map <char, vector<table>> inputTable;

	vector<char> keySymbol(alphabetSize);

	table currStruct;

	for (int i = 0; i < alphabetSize; i++) {
		fin >> keySymbol[i];
	}

	for (int i = 0; i < alphabetSize; i++) {
		inputTable[keySymbol[i]].push_back(currStruct);
		for (int j = 0; j < numOfConditions; j++) {
			fin >> currStruct.condition >> currStruct.pastingSymbol >> currStruct.command;
			inputTable[keySymbol[i]].push_back(currStruct);
		}
	}

	return inputTable;
}

/*Функция compMachineStep получает на вход:
* map<char, vector<table>>& condTable - адрес map<char, vector<table>>, хранящей считанную таблицу состояний машины
* string& inputStr - адрес переменной, хранящей считанную строку.
* int& currCondition - адрес переменной, хранящей нынешнее состояние машины.
* int& currPosition - адрес переменной, хранящей индекс символа, обрабатывающегося в данный момент.
* Функция делает выполняет шаг по таблице переходов машины.
* Ничего не возвращает.
*/
void compMachineStep(map<char, vector<table>>& condTable, string& inputStr, int& currCondition, int& currPosition) {
	char prevSymbol = inputStr[currPosition];

	inputStr[currPosition] = condTable[inputStr[currPosition]][currCondition].pastingSymbol;

	if (inputStr[currPosition] != 'X' && currPosition == inputStr.size() - 1) {
		inputStr = inputStr + "X";
	}
	if (inputStr[currPosition] != 'X' && currPosition == 0) {
		inputStr = "X" + inputStr;
		currPosition++;
	}

	if (condTable[prevSymbol][currCondition].command == '>') {
		currPosition++;
	}
	else if (condTable[prevSymbol][currCondition].command == '<') {
		currPosition--;
	}
	
	currCondition = condTable[prevSymbol][currCondition].condition;
}

/* Функция printCurrSymbolPointer получает на вход:
* string& inputString - адрес переменной, хранящей считанную строку. 
* int& currPosition - адрес переменной, хранящей индекс символа, обрабатывающегося в данный момент.
* int& isFirstTime - адрес переменной, хранящей 0 или 1 в зависимости от того, в первый раз ли выводится строка или нет.
* Ничего не возвращает
*/
void printCurrSymbolPointer(string& inputString, int& currPosition, int& isFirstTime) {
	if (!isFirstTime) {
		cout << endl << "Начальная конфигурация: ";
		isFirstTime = 1;
	}
	for (int i = 0; i < inputString.size(); i++) {
		if (i == currPosition) {
			SetConsoleTextAttribute(hConsole, 4); //--- Red color
			cout << inputString[i];
			SetConsoleTextAttribute(hConsole, 7); //--- Default color
		}
		else cout << inputString[i];
	}
}

int main() {
	setlocale(LC_ALL, "Russian");

	string inputString;

	map <char, vector<table>> condTable;
	int inputIsCorrect, stop = 0;
	int currCond = 1, currPos = 1, startCond = 0, isFirstTime = 0;

	while (!stop) {
		cout << "Введите строку: ";
		cin >> inputString;
		inputIsCorrect = readConsole(inputString);
		inputString = "X" + inputString + "X";
		if (!inputIsCorrect) {
			cout << "Введена некорректная строка." << endl;
		}
		else {
			condTable = readInput("INPUT.txt");
			currCond = 1;
			while (currCond != 0) {
				printCurrSymbolPointer(inputString, currPos, isFirstTime);
				compMachineStep(condTable, inputString, currCond, currPos);
				cout << endl << "Текущее состояние: " << currCond << "." << endl << endl;
			}
		}
		cout << endl << "Машина завершила работу." << endl << "Введите:" 
			 << endl << "1 - Если хотите ввести строку заново."
			 << endl << "2 - Если хотите выйти из программы." << endl;

		int ans;
		cin >> ans; cout << endl;

		if (ans == 1) continue;
		else if (ans == 2)
		{
			cout << "Программа закончила работу." << endl;
			stop = 1;
		}
		else {
			cout << "Такой комманды не существует!" << endl;
			cout << "Программа закончила работу." << endl;
			stop = 1;
		}
	}
	return 0;
}