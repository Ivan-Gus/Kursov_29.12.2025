#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#define _USE_MATH_DEFINES 
#define _CRT_SECURE_NO_DEPRECATE

double f1(double x);
double f2(double x);
double f3(double x);
// Основная функция вычисления f(x)
void monoton_f(double start, double end, double point);
double n_f(double x);
double proizvod(double x, double h);
double find_x_y(double Y, double tochn, double start, double end);
void tabl(FILE *file,double start, double toch,int count);

int main() {
	setlocale(LC_CTYPE, "RUS");
	int c;
	double x,toch;
	double start, shag,y,end;
	int count;
	double prov,res;
	FILE* file;
	double h = 1e-6;
	do {
		printf("Кусочная функция\n\n");
		printf("1. Значение функции в точке \n2. Таблица значений \n3. Определение монотонности \n4. Поиск X по Y \n5. Производная в точке \n0. Выход \n");
		printf("Введите команду: \n");
		scanf("%d", &c);
		switch (c) {
		case 1:
			printf("Введите x: ");
			scanf("%lf", &x);
			printf("f(%.2f) = %.6f\n", x, n_f(x));
			break;
		case 2:
			printf("Введите начало интервала: ");
			scanf("%lf", &start);
			printf("Введите точность: ");
			scanf("%lf", &toch);
			printf("Количество отсчетов: ");
			scanf("%d", &count);
			char fname[]="tabl.txt";
			if ((file = fopen(fname, "wt")) == NULL) {
				printf("Ошибка открытия файла для записи");
				return 0;
			}
			tabl(file, start, toch, count);
			fclose(file);
			printf("Текстовый файл успешно создан.\n");
			break;
		case 3:
			printf("Введите начало интервала: ");
			scanf("%lf", &start);
			printf("Введите конец интервала: ");
			scanf("%lf", &end);
			printf("Введите точность поиска: ");
			scanf("%lf", &toch);
			monoton_f(start, end, toch);
			break;
		case 4:
			printf("Введите Y: ");
			scanf("%lf", &y);
			printf("Введите точность поиска: ");
			scanf("%lf", &toch);
			res = find_x_y(y, toch, -1000, 1000);
			printf("Результат x = % .5f\n", res);

			break;
		case 5:
			printf("Введите x: ");
			scanf("%lf", &x);
			prov = proizvod(x, h);
			printf("f'(%.6f) = %.6f\n", x, prov);
			break;
		}
	} while (c != 0); 
	return 0;
}

double find_x_y(double Y, double tochn, double start, double end) {
	double x, fx, best_x = start;
	double min = fabs(n_f(start) - Y);
	for (x = start; x <= end; x += tochn) {
		fx = n_f(x);
		double diff = fabs(fx - Y);
		if (diff < min) {
			min = diff;
			best_x = x;
		}
		
	}
	return best_x;
}

double f1(double x) {
	return (tan(x * x) - 1) / x;
}

double f2(double x) {
	return (x * exp(-x / 2)) / log(2 + x * x);
}

double f3(double x) {
	return pow(1 + x * x * x, 0.25);
}

double n_f(double x) {
	if (x < -2) {
		return f1(x);
	}
	else if (x >= -2 & x < 3) {
		return f2(x);
	}
	else if (x >= 3) {
		return f3(x);
	}
	else {
		return NAN; // Неопределено
	}
}

double proizvod(double x, double h) {
	
	return (n_f(x + h) - n_f(x)) / h;
}

void monoton_f(double start, double end, double tochn) {
	if (start >= end) {
		printf("Ошибка в заданных параметрах.\n");
		return 0;
	}
	double per = n_f(start);
	int inc = 1, dec = 1;
	for (double x = start; x <= end; x += tochn) {
		double y = n_f(x);
		if (y < per) inc = 0;
		if (y > per) dec = 0;
		per = y;
	}
	if (inc && dec) printf("Постоянна на [%.1f, %.1f]\n", start, end);
	else if (inc)   printf("Возрастает на [%.1f, %.1f]\n", start, end);
	else if (dec)   printf("Убывает на [%.1f, %.1f]\n", start, end);
	else           printf("Не монотонна на [%.1f, %.1f]\n", start, end);
}

void tabl(FILE *file,double start, double toch, int count) {
	if (file == NULL) {
		fprintf(stderr, "Ошибка: неверный указатель на поток\n");
		return 0;
	}
	fprintf(file,"\n|============================|\n");
	fprintf(file,"|  Таблица значений функции  |\n");
	fprintf(file,"|============================|\n");
	fprintf(file,"|      x      |     f(x)     | \n");
	fprintf(file,"|============================|\n");
	for (int i = 0; i < count; i++) {
		double y;
		y = n_f(start);
		fprintf(file,"| %10.6f  | %12.8f |\n", start, y);
		fprintf(file,"|============================|\n");
		start += toch;
	}
}