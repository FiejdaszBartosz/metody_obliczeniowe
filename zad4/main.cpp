//
// Created by Bartosz Fiejdasz on 20/03/2022.
//

#include <iostream>
#include <math.h>

// Wartosci stale do przerwania metody iteracyjnej
const double TOLF = 1e-16;
const double TOLX = 1e-16;
const int MAXITERACJE = 30;

/**
 * @brief wybranie max z 3 liczb
 * @param a 1 liczba do porownania
 * @param b 2 liczba do porownania
 * @param c 3 liczba do porownania
 * @return najwieksza wartosc z trzech podach liczb
 */
double max(double a, double b, double c) {
  double maximum = a;

  if (b > maximum)
    maximum = b;

  if(c > maximum)
    maximum = c;

  return maximum;
}

// Wzory funkcji wziete z zadania 1
double funkcja1(double x, double y, double z) {
  return x * x + y * y + z * z - 2.0;
}

double funkcja2(double x, double y, double z) {
  return x * x + y * y - 1.0;
}

double funkcja3(double x, double y, double z) {
  return x * x - y;
}

// Obliczenie wektora abc, wzory obliczone wczesniej na kartce
double obliczA(double x, double y, double z) {
  return (x * x - y * y - 1.0 + 2.0 * x * x * y) / (2.0 * x * (1.0 + 2.0 * y));
}

double obliczB(double x, double y, double z) {
  return (y * y + y - 1.0) / (1.0 + 2.0 * y);
}

double obliczC(double x, double y, double z) {
  return (z * z + 2.0 * z * z * y - 2.0 * y - 1.0) / (2.0 * z * (1.0 + 2.0 * y));
}

/**
 * @brief Sprawdzenie czy podane wartosci naleza do dziedziny funkcji
 * @param x
 * @param y
 * @param z
 * @return true - jesli podane punkty naleza do dziedziny funkcji
 * @return false - jesli podane punkty nie naleza do dziedziny funkcji
 */
bool sprawdzZmienne(double x, double y, double z) {
  if (x == 0.0) {
	std::cout << "Nieprawidlowy x\n";
	return false;
  } else if (z == 0.0) {
	std::cout << "Nieprawidlowy z\n";
	return false;
  } else if (y == -1.0 / 2.0 || 4.0 * y * x + 2.0 * x == 0.0) {
	std::cout << "Nieprawidlowy x lub y\n";
	return false;
  } else
	return true;
}

/**
 * @brief Funkcja realizujaca uogolnioną metodę Newtona rozwiązywania układu trzech algebraicznych rownań nieliniowych
 * @param x, y, z - wartosci poczatkowe
 */
void algorytmNewtona(double x, double y, double z) {
  double estymator = 0.0, reziduum = 0.0, wartosciFunkcji[3], wektorABC[3];
  int i = 1;
  bool kontynuuj = true;

  while (kontynuuj) {
	// Wyzerowanie wektora przy kazdej iteracji
	for (int j = 0; j < 3; ++j)
	  wektorABC[j] = 0;

	if (!sprawdzZmienne(x, y, z))
	  break;

    // Wyznaczenie wektora abc (wektor delta)
    wektorABC[0] = obliczA(x, y, z);
    wektorABC[1] = obliczB(x, y, z);
    wektorABC[2] = obliczC(x, y, z);

    // Wyznaczenie nowego przyblizenia
    x = x - wektorABC[0];
    y = y - wektorABC[1];
    z = z - wektorABC[2];

    // Przypisanie do funkcji nowych wartosci
    wartosciFunkcji[0] = funkcja1(x, y, z);
    wartosciFunkcji[1] = funkcja2(x, y, z);
    wartosciFunkcji[2] = funkcja3(x, y, z);

    // Wyznaczenie estymatora
    estymator = max(fabs(wektorABC[0]), fabs(wektorABC[1]), fabs(wektorABC[2]));

    // Wyznaczenie reziduum
    reziduum = max(fabs(wartosciFunkcji[0]), fabs(wartosciFunkcji[1]), fabs(wartosciFunkcji[2]));

    std::cout << "Iteracja "<< i << "\nx: " << x << "\ny: " << y << "\nz: " << z << "\nestymator: " << estymator
              << "\nreziduum: " << reziduum << "\n\n";

	// Warunki przerwania
	if ((fabs(reziduum) <= TOLF) || (estymator <= TOLX) || (i >= MAXITERACJE)) {
	  kontynuuj = false;
	}

	i++;
  }
}

int main() {
  algorytmNewtona(5.0, 3.0, 5.0);

  return 0;
}