#include <iostream>
#include <string>
#include "Arifmetic.h"


using namespace std;

int start_skobka_index; // индекс начальной скобки
int end_skobka_index;	// индекс конечной скобки

int start_num_index;	// индекс начала числа слева
int end_num_index;		// индекс конца числа справа

char* main_arr;			// исходная строка

// поиск скобок в исходной строке
void PoiskSkobka(char* arr);
// выделение подстроки, которая находится в скобках
void NewStroka(char* arr, int a, int b);
// узнаем приоритет знаков
void PrirZnak(char* arr, char* old_arr);
// отделяем числа слева и справа от знака
void PoiskChislo(char* arr, int index);
// конверируем из char* в double
void SubNum(char* arr, int index);
// производим арифметическое действие
void ResultNum(char* arr, int index, double a, double b);
// заменяем подстроку на итоговый результат подсчетов
void ResultStroka(char* arr, char* new_arr);
// заменяем строку на новую с посчитанными значениями в скобках
void ResultStrokaSkobka(char* arr, char* new_arr, int a, int b);
// проверка на наличие еще скобок или знаков
bool Proverka(char* res);

int main()
{
	string s;
	cin >> s;
	main_arr = (char*)s.c_str();

	PoiskSkobka(main_arr);
    
}
// 1
void PoiskSkobka(char* arr) {
		start_skobka_index = 0; // начало скобки
		end_skobka_index = 0; // конец скобки
		int s = 0;
		// поиск скобок
		for (int i = 0; i < strlen(arr); i++) {
			if (arr[i] == '(') {
				start_skobka_index = i;
				s++;
			}
			if (arr[i] == ')') {
				end_skobka_index = i;
				s++;
			}
		}
		if (s == 0) PrirZnak(arr, main_arr);
		else NewStroka(arr, ++start_skobka_index, end_skobka_index);
}

// 2
void NewStroka(char* arr, int a, int b) {
	char* new_arr = new char[b - a];
	strncpy_s(new_arr, b, arr + a, b - a); // копирование подстроки в скобках (без скобок)
	PrirZnak(new_arr, main_arr);
}
// 3
void PrirZnak(char* arr, char* old_arr) {
	bool flag = true;
	int index = 0;
	for (int i = 0; i < strlen(arr); i++) {
		if (arr[i] == '/' || arr[i] == '*') {
			flag = false;
			index = i;
			break;
		}
	}
	if (flag) {
		for (int i = 0; i < strlen(arr); i++) {
			if (arr[i] == '-' || arr[i] == '+') {
				flag = false;
				index = i;
				break;
			}
		}
	}
	if (!flag) PoiskChislo(arr, index);
	else ResultStrokaSkobka(old_arr, arr, --start_skobka_index, end_skobka_index);

}
// 4
void PoiskChislo(char* arr, int index) {
	start_num_index = 0;
	end_num_index = 0;
	if (arr[0] == '-')
		ResultStrokaSkobka(arr, arr, 0, strlen(arr));
	else {
		//число слева
		for (int i = index - 1; i >= 0; i--) {
			if (isdigit(arr[i]) && i >= 0 || arr[i] == '.') {
				start_num_index = i;
			}
			else
			{
				break;
			}
		}
		//число справа
		for (int i = index + 1; i < strlen(arr); i++) {
			if (isdigit(arr[i]) && i >= 0 || arr[i] == '.') {
				end_num_index = i;
			}
			else {
				break;
			}
		}
		SubNum(arr, index);
	}

}
// 5
void SubNum(char* arr, int index) {
	int sizeA = index - start_num_index;
	int sizeB = end_num_index - index;
	char* firstNum = new char[sizeA];
	char* secondNum = new char[sizeB];
	strncpy_s(firstNum, sizeA + 1, arr + start_num_index, sizeA);
	strncpy_s(secondNum, sizeB + 1, arr + index+1, sizeB);
	double left = atof(firstNum);										
	double right = atof(secondNum);										
	ResultNum(arr, index, left, right);
}
// 6
void ResultNum(char* arr, int index, double a, double b) {				
	double result = NULL;
	switch (arr[index])
	{
	case '+':
		result = Plus(a, b);
		break;
	case '-':
		result = Minus(a, b);
		break;
	case '*':
		result = Mult(a, b);
		break;
	case '/':
		result = Div(a, b);
		break;
	default:
		break;
	}
	result = round(result * 100) / 100;
	std::string f = std::to_string(result);
	char* n = (char*)f.c_str();
	ResultStroka(arr, n);

}
// 7
void ResultStroka(char* arr, char* new_arr) {
	int size = strlen(arr) - (end_num_index - start_num_index) + strlen(new_arr);
	char* res_arr = new char[size];
	strncpy_s(res_arr, size, arr, start_num_index);
	strncat_s(res_arr, size, new_arr, strlen(new_arr));
	strncat_s(res_arr, size, arr + end_num_index + 1, strlen(arr));
	PrirZnak(res_arr, main_arr);

}
// 8
void ResultStrokaSkobka(char* arr, char* new_arr, int a, int b) {
	
	int size = strlen(arr) - (b - a) + strlen(new_arr);
		
	char* res_arr = new char[size];
		
	if(a>=0) strncpy_s(res_arr, size, arr, a);
	else strncpy_s(res_arr, 1, "", 0);
	strncat_s(res_arr, size + 1, new_arr, strlen(new_arr));
	if (b> 0 && arr[0] != '-') strncat_s(res_arr, size, arr + b + 1, strlen(arr));
	main_arr = res_arr;
	if (Proverka(res_arr) && arr[0] != '-') PoiskSkobka(res_arr);
	else printf("%0.2f", atof(res_arr));
		
}
// 9
bool Proverka(char* res) {
	for (int i = 0; i < strlen(res); i++) {
		if (res[i] == 42 || res[i] == 43 || res[i] == 45 || res[i] == 47)
			return true;
	}
	return false;
}
