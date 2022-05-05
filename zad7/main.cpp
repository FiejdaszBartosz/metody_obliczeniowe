//
// Created by Bartosz Fiejdasz on 12/04/2022.
//

#include <iostream>
#include <cmath>

#define TOLF 10e-10 //ustawienie tolerancji residuum
#define TOLX 10e-10 //ustawienie tolerancji estymatora
#define ITERACJE 50 //ustawienie liczby iteracji

void uzupelnijDane(double **pA, double *pB, double *pX) {
  pA[0][0] = 10.0;
  pA[0][1] = 1.0;
  pA[0][2] = 2.0;
  pA[1][0] = 1.0;
  pA[1][1] = 20.0;
  pA[1][2] = 5.0;
  pA[2][0] = 3.0;
  pA[2][1] = 4.0;
  pA[2][2] = 30.0;

  pB[0] = 8.0;
  pB[1] = -4.0;
  pB[2] = -27.0;

  pX[0] = 2.0;
  pX[1] = 2.0;
  pX[2] = 2.0;
}

void wypiszMacierz(double **A, double n) //funkcja do wypisywania macierzy
{
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++)
      std::cout << A[i][j] << " ";

    std::cout << "\n";
  }
  std::cout << "\n";
}
void wypiszWektor(double *wektor, double n) //funkcja do wyisywania wektorów
{
  for (int i = 0; i < n; i++)
    std::cout << wektor[i] << "  ";
}

double maximum(double *tab, int n) //funkcja znajdująca największą wartość w wektorze/tablicy
{
  double a = tab[0];
  for (int i = 1; i < n; i++)
    if (tab[i] > a)
      a = tab[i];
  return a;
}

void metodaJacobiego(double **pA, double *pB, double *pX, int n) {
  int licznik = 0;
  double estymator[n], residuum[n];
  double est = 1.0, res = 1.0;
  double przyblizenie[n], suma;
  bool kontynuuj = true;

  std::cout << "Iteracja \t x1 \t\t x2 \t\t x3 \t\t x4 \t\t Reziduum: \t Epsilon: \n";

  while (kontynuuj) {
    for (int i = 0; i < n; ++i) {
      suma = 0.0;
      for (int j = 0; j < n; ++j) {
        if (i != j)                     // Pomijamy elemnty na przekątnej
          suma += pX[j] * pA[i][j];     // (L + U) * x
      }

      // Xn = M * Xn-1 + c
      // Xn = -D ^ -1 * (L + U) * Xn-1 + D ^ -1 * b
      przyblizenie[i] = (pB[i] - suma) / pA[i][i];
    }

    for (int i = 0; i < n; ++i) {
      residuum[i] = 0.0;
      estymator[i] = fabs(przyblizenie[i] - pX[i]);

      for (int j = 0; j < n; ++j)
        residuum[i] += pA[i][j] * pX[j];

      residuum[i] = fabs(residuum[i] - pB[i]);    // A * x - b = 0

      pX[i] = przyblizenie[i];    // Przypisanie nowego przyblizenia
    }

    est = maximum(estymator, n);
    res = maximum(estymator, n);

    std::cout << licznik << "\t";
    wypiszWektor(pX, n);
    std::cout << "\t" << res << "\t" << est << "\n";

    if ((res <= TOLF && est <= TOLX) || (licznik >= ITERACJE)) {
      kontynuuj = false;
    }

    licznik++;
  }
}

void metodaGaussaSeidela(double **pA, double *pB, double *pX, int n) {
  int licznik = 0;
  double estymator[n], residuum[n];
  double est = 1.0, res = 1.0;
  double przyblizenie[n], suma = 0.0;
  bool kontynuuj = true;

  std::cout << "Iteracja \t x1 \t\t x2 \t\t x3 \t\t x4 \t\t Reziduum: \t Epsilon: \n";

  while (kontynuuj) {
    for (int i = 0; i < n; i++) {
      suma = 0.0;
      for (int j = 0; j < n; j++) {
        if (i != j) {
          suma += pX[j] * pA[i][j];
        }
      }

      przyblizenie[i] = pX[i];
      pX[i] = (pB[i] - suma) / pA[i][i];
      estymator[i] = fabs(przyblizenie[i] - pX[i]);
    }

    for (int i = 0; i < n; ++i) {
      residuum[i] = 0.0;

      for (int j = 0; j < n; ++j)
        residuum[i] += pA[i][j] * pX[j];

      residuum[i] = fabs(residuum[i] - pB[i]);    // A * x - b = 0
    }

    est = maximum(estymator, n);
    res = maximum(estymator, n);

    std::cout << licznik << "\t";
    wypiszWektor(pX, n);
    std::cout << "\t" << res << "\t" << est << "\n";

    if ((res <= TOLF && est <= TOLX) || (licznik >= ITERACJE)) {
      kontynuuj = false;
    }

    licznik++;
  }
}

void metodaSor(double **pA, double *pB, double *pX, double omega, int n) {
  int licznik = 0;
  double estymator[n], residuum[n];
  double est = 1.0, res = 1.0;
  double przyblizenie[n], suma = 0.0;
  bool kontynuuj = true;

  std::cout << "Iteracja \t x1 \t\t x2 \t\t x3 \t\t x4 \t\t Reziduum: \t Epsilon: \n";

  while (kontynuuj) {
    for (int i = 0; i < n; i++) {
      suma = 0.0;
      for (int j = 0; j < n; j++) {
        if (i != j) {
          suma += pX[j] * pA[i][j];
        }
      }

      przyblizenie[i] = pX[i];
      pX[i] = (1.0 - omega) * pX[i] + (omega * (pB[i] - suma) / pA[i][i]);    //wyliczenia ze wzorów
      estymator[i] = fabs(przyblizenie[i] - pX[i]);
    }

    for (int i = 0; i < n; i++)   //wyliczanie residuum
    {
      residuum[i] = 0;
      for (int j = 0; j < n; j++)
        residuum[i] = residuum[i] + pA[i][j] * pX[j];

      residuum[i] = fabs(residuum[i] - pB[i]);
    }

    est = maximum(estymator, n);
    res = maximum(estymator, n);

    std::cout << licznik << "\t";
    wypiszWektor(pX, n);
    std::cout << "\t" << res << "\t" << est << "\n";

    if ((res <= TOLF && est <= TOLX) || (licznik >= ITERACJE)) {
      kontynuuj = false;
    }

    licznik++;
  }
}

int main() {
  int n = 3;
  double **A, *b, *x;
  A = new double *[n];

  for (int i = 0; i < n; i++)
    A[i] = new double[n];

  b = new double[n];
  x = new double[n];

  uzupelnijDane(A, b, x);

  std::cout << "Macierz A:\n";
  wypiszMacierz(A, n);
  std::cout << "\n";
  std::cout << " Wektor b:\n";
  wypiszWektor(b, n);
  std::cout << " Wektor x:\n";
  wypiszWektor(x, n);

  std::cout << "Metoda Jacobiego" << "\n";
  metodaJacobiego(A, b, x, n);
  std::cout << "\n";

  uzupelnijDane(A, b, x);

  std::cout << "Metoda Gaussa-Seidela" << "\n";
  metodaGaussaSeidela(A, b, x, n);
  std::cout << "\n";

  uzupelnijDane(A, b, x);

  std::cout << "Metoda SOR" << "\n";
  double omega = 1.0 / 2.0;
  metodaSor(A, b, x, omega, n);
  std::cout << "\n";

  for (int i = 0; i < n; i++)
    delete[] A[i];

  delete[] A;
  delete[] b;
  delete[] x;
}