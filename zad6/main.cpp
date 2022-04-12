//
// Created by Bartosz Fiejdasz on 09/04/2022.
//

#include <iostream>

#define N 6

// W funkcja zastosowalem wzory z wykladu
// A * x = b

/**
 * Oblicza wartosci eta
 * @param u wektor wartosci u (wartosci nad d)
 * @param d wektor wartosci d (diagonala)
 * @param l wektor wartosci l (wartosci pod d)
 * @param eta nowe wartosci na diagonali
 * @param rozmiar rozmiar macierzy
 */
void rozwiazA(double *u, double *d, double *l, double *eta, int rozmiar) {
  // Wykonanie pierwszego kroku dla A
  eta[0] = d[0];

  // Dla pozostalych
  for (int i = 1; i < rozmiar + 1; i++)
    eta[i] = d[i] - l[i] * u[i - 1] / eta[i - 1];
}

/**
 * Obliczenie rozwiazania
 * @param u wektor wartosci u (wartosci nad d)
 * @param x wektor z rozwiazaniem
 * @param eta nowe wartosci na diagonali
 * @param r nowe wartosci dla wektora b
 * @param rozmiar rozmiar macierzy
 */
void obliczWynik(double *u, double *x, double *eta, double *r, int rozmiar) {
  // Obliczenie Xn
  x[rozmiar] = r[rozmiar] / eta[rozmiar];

  // Dla pozostalych
  for (int i = rozmiar - 1; i >= 0; i--)
    x[i] = (r[i] - u[i] * x[i + 1]) / eta[i];
}
/**
 * wyliczenie wektora r
 * @param u wektor wartosci u (wartosci nad d)
 * @param l wektor wartosci l (wartosci pod d)
 * @param b wektor b
 * @param x wektor z rozwiazaniem
 * @param eta nowe wartosci na diagonali
 * @param r nowe wartosci dla wektora b
 * @param rozmiar
 */
void rozwiazB(double *u, double *l, double *b, double *x, double *eta, double *r, int rozmiar) {
  // Wykonanie pierwszego kroku dla A
  r[0] = b[0];

  // Wykonanie pierwszego kroku dla B
  for (int i = 1; i < rozmiar + 1; i++)
    r[i] = b[i] - l[i] * r[i - 1] / eta[i - 1];

  obliczWynik(u, x, eta, r, rozmiar);
}

/**
 * uzupelnia wektory podanymi wartosciami
 * @param u wektor u
 * @param d wektor d
 * @param l wektor l
 * @param b wektor b
 */
void uzupelnijWektor(double *u, double *d, double *l, double *b) {
  u[0] = 1.0 / 2.0;
  u[1] = 1.0 / 4.0;
  u[2] = 1.0 / 6.0;
  u[3] = 1.0 / 8.0;
  u[4] = 1.0 / 10.0;

  d[0] = 10.0;
  d[1] = 20.0;
  d[2] = 30.0;
  d[3] = 30.0;
  d[4] = 20.0;
  d[5] = 10.0;

  l[0] = 0.0;   // W algorytmie zaczynamy od l2 wiec dopisuje jeden element
  l[1] = 1.0 / 3.0;
  l[2] = 1.0 / 5.0;
  l[3] = 1.0 / 7.0;
  l[4] = 1.0 / 9.0;
  l[5] = 1.0 / 11.0;

  b[0] = 31.0;
  b[1] = 165.0 / 4.0;
  b[2] = 917.0 / 30.0;
  b[3] = 851.0 / 28.0;
  b[4] = 3637.0 / 90.0;
  b[5] = 332.0 / 11.0;
}

void wyswietlWektor(double *wektor, int rozmiar) {
  std::cout << "\n";
  for (int i = 0; i < rozmiar; ++i)
    std::cout << wektor[i] << "\n";
  std::cout << "\n";
}

void usunWektor(double *wektor) { delete[] wektor; }

int main() {
  double *u = new double[N - 1];
  double *d = new double[N];
  double *l = new double[N - 1];
  double *b = new double[N];
  double *x = new double[N];
  double *eta = new double[N];
  double *r = new double[N];

  uzupelnijWektor(u, d, l, b);
  rozwiazA(u, d, l, eta, N - 1);
  rozwiazB(u, l, b, x, eta, r, N - 1);

  std::cout << "ALGORYTM THOMASA\n Wektor eta:";
  wyswietlWektor(eta, N);
  std::cout << "Wektor r:";
  wyswietlWektor(r, N);
  std::cout << "Wektor x - rozwiazanie:";
  wyswietlWektor(x, N);

  usunWektor(l);
  usunWektor(d);
  usunWektor(u);
  usunWektor(b);
  usunWektor(x);

  return 0;
}
