//
// Created by Bartosz Fiejdasz on 09/04/2022.
//

#include <iostream>

#define N 6

void rozwiazA(double *u, double *d, double *l, double *eta, int rozmiar) {
  // Wykonanie pierwszego kroku dla A
  eta[0] = d[0];

  // Dla pozostalych
  for (int i = 1; i < rozmiar + 1; i++)
    eta[i] = d[i] - l[i - 1] * u[i - 1] / eta[i - 1];
}

void obliczWynik(double *u, double *x, double *eta, double *r, int rozmiar) {
  // Obliczenie Xn
  x[rozmiar] = r[rozmiar] / eta[rozmiar];

  // Dla pozostalych
  for (int i = rozmiar - 1; i >= 0; i--)
    x[i] = (r[i] - u[i] * x[i + 1]) / eta[i];
}

void rozwiazB(double *u, double *l, double *b, double *x, double *eta, double *r, int rozmiar) {
  // Wykonanie pierwszego kroku dla A
  r[0] = b[0];

  // Wykonanie pierwszego kroku dla B
  for (int i = 1; i < rozmiar + 1; i++)
    r[i] = b[i] - l[i - 1] * r[i - 1] / eta[i - 1];

  obliczWynik(u, x, eta, r, rozmiar);
}

void uzupelnijWektor(double *u, double *d, double *l, double *b) {
  u[0] = 0.5;
  u[1] = 0.25;
  u[2] = 1.0 / 6.0;
  u[3] = 0.125;
  u[4] = 0.01;

  d[0] = 10.0;
  d[1] = 20.0;
  d[2] = 30.0;
  d[3] = 30.0;
  d[4] = 20.0;
  d[5] = 10.0;

  l[0] = 1.0 / 3.0;
  l[1] = 0.2;
  l[2] = 1.0 / 7.0;
  l[3] = 1.0 / 9.0;
  l[4] = 1.0 / 11.0;

  b[0] = 31.0;
  b[1] = 41.25;
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
