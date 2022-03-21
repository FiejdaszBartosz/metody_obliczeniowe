//
// Created by Bartosz Fiejdasz on 20/03/2022.
//

#include <iostream>
#include <math.h>

const double TOLF = 1e-8;
const double TOLX = 1e-8;
const int MAXITERACJE = 30;

double funkcja1(double x, double y, double z) {
  return x * x + y * y + z * z - 2.0;
}

double funkcja2(double x, double y, double z) {
  return x * x + y * y - 1.0;
}

double funkcja3(double x, double y, double z) {
  return x * x - y;
}

bool sprawdzZmienne(double x, double y, double z) {
  if (x == 0) {
	std::cout << "Nieprawidlowy x\n";
	return false;
  } else if (z == 0) {
	std::cout << "Nieprawidlowy y\n";
	return false;
  } else if (y == -1 / 2 || 4 * y * x + 2 * x == 0) {
	std::cout << "Nieprawidlowy z\n";
	return false;
  } else
	return true;
}

/**
 * @brief Funkcja tworzy jakobian. Zostal on wyznaczony wczesniej na kartce
 * @param x
 * @param y
 * @param z
 * @return wskaznik do jakobianu 3x3
 */
double **uzupelnijJakobian(double x, double y, double z) {
  // Storzenie pomocniczej macierzy 3x3
  double **temp = new double *[3];
  for (int i = 0; i < 3; ++i) {
	temp[i] = new double[3];
  }

  temp[0][0] = 2.0 * x;
  temp[0][1] = 2.0 * y;
  temp[0][2] = 2.0 * z;
  temp[1][0] = 2.0 * x;
  temp[1][1] = 2.0 * y;
  temp[1][2] = 0.0;
  temp[2][0] = 2.0 * x;
  temp[2][1] = -1.0;
  temp[2][2] = 0.0;

  return temp;
}

void algorytmNewtona(double x, double y, double z, double **macierz) {
  double estymator = 0.0, reziduum = 0.0, wartosciFunkcji[3], wektorABC[3];
  int i = 1;
  bool kontynuuj = true;

  wartosciFunkcji[0] = funkcja1(x, y, z);
  wartosciFunkcji[1] = funkcja2(x, y, z);
  wartosciFunkcji[2] = funkcja3(x, y, z);

  while (kontynuuj) {
	// Wyzerowanie wektora przy kazdej iteracji
	for (int j = 0; j < 3; ++j)
	  wektorABC[j] = 0;

	if (!sprawdzZmienne(x, y, z))
	  break;

	// Wyznaczenie Jakobianu
	macierz = uzupelnijJakobian(x, y, z);

	// Warunki przerwania
	if ((fabs(reziduum) <= TOLF) || (estymator <= TOLX) || (i >= MAXITERACJE)) {
	  kontynuuj = false;
	}

	i++;
  }
}