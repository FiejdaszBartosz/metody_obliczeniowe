//
// Created by Bartosz Fiejdasz on 17/05/2022.
//

#include <iostream>
#include <cmath>
#include <fstream>

const double p = 1.0, q = 0.0, r = -4.0; //współczynniki w równaniu
const double alfa = 0.0, beta = 1.0, gamma = -1.0; //współczynniki z warunków brzegowych
const double fi = 0.0, psi = 1.0, teta = 0.0; //psi dowolne
const double poczatekPrzedzialu = 0.0, koniecPrzedzialu = 1.0; //granice przedziału

double rownanieAnalityczne(double x) {    // wyliczone analitycznie
  return (exp(2.0 - 2.0 * x) - 4 * exp(4.0 - x * 2.0) + 4.0 * exp(x * 2.0) - exp(2.0 + 2.0 * x) - x + x * exp(4.0))
      / (4.0 - 4 * exp(4.0));
}

void thomas(double *l, double *d, double *u, double *b, double *x, int N) {
  // Urzywam kopi aby uniknac zmienniania oryginalnych danych
  double *bKopia = new double[N];
  double *dKopia = new double[N];

  // Realizuje algorytm Thomasa zgodnie z wykladem
  dKopia[0] = d[0];
  bKopia[0] = b[0];

  for (int i = 1; i < N; i++) {
    dKopia[i] = d[i] - l[i - 1] * (u[i - 1] / dKopia[i - 1]);
  }

  for (int i = 1; i < N; i++) {
    bKopia[i] = b[i] - l[i - 1] * bKopia[i - 1] / dKopia[i - 1];
  }

  x[N - 1] = bKopia[N - 1] / dKopia[N - 1];   // Obliczenie x n-1

  for (int i = N - 2; i >= 0; i--) {    // Obliczenie pozostalych x
    x[i] = (bKopia[i] - u[i] * x[i + 1]) / dKopia[i];
  }

  delete[] bKopia;
  delete[] dKopia;
}

int znajdzNajwiekszyBlad(double *blad, int N) {
  double maksymalny = fabs(blad[0]);    // Inicjalizujemy blad pierwsza wartoscia wektora
  int index = 0;

  for (int i = 0; i < N; i++)
    if (fabs(blad[i]) > maksymalny) {
      maksymalny = fabs(blad[i]);
      index = i;
    }

  return index;
}
/**
 * Funkcja realizująca dyskretyzację Numerowa
 * @param h krok
 * @param N ilosc iteracji
 * @return blad
 */
double Numerowa(double h, int N) {
  double *l, *d, *u, *b, *x, *blad, x0 = poczatekPrzedzialu, xn = poczatekPrzedzialu;
  std::fstream numerow, analitycznie;
  numerow.open("wynikiNumerow.txt", std::ios_base::app);
  analitycznie.open("wynikiAnalitycznie.txt", std::ios_base::app);
  analitycznie << std::scientific;
  numerow << std::scientific;
  std::cout.precision(10);
  l = new double[N]; //tworzymy wektory
  d = new double[N];
  u = new double[N];
  b = new double[N];
  x = new double[N];
  blad = new double[N];

  // Realizuje algorytm zgodnie ze wzorami z wykladu

  u[0] = alfa / h;
  d[0] = beta - alfa / h;
  b[0] = -gamma;

  for (int i = 1; i < N - 1; i++) {
    l[i - 1] = p / (h * h) + r / 12.0;
    d[i] = (-2.0 * p) / (h * h) + r * (10.0 / 12.0);
    u[i] = p / (h * h) + r / 12.0;
    b[i] = (x0 + i * h - h) / 12.0 + (10.0 / 12.0) * (x0 + i * h) + (x0 + i * h + h) / 12.0;
  }

  l[N - 2] = -fi / h; //końcowe wyrazy mają także specjalne dane
  d[N - 1] = -fi / h + psi;
  b[N - 1] = -teta;

  thomas(l, d, u, b, x, N); // Algorytm Thomasa

  // Obliczenie bledu między algotytmem numerowa, a rozwiązaniem analitycznym
  for (int i = 0; i < N; i++) {
    blad[i] = fabs(x[i] - rownanieAnalityczne(xn));
    xn += h;
  }
  int naj = znajdzNajwiekszyBlad(blad, N); //znajdujemy największy błąd

  if (N == 162) //losowa liczba
  {
    for (int i = 0; i < N; i++) {
      numerow << x0 << "\t" << x[i] << "\t\n";
      analitycznie << x0 << "\t" << rownanieAnalityczne(x0) << "\n";
      x0 += h;
    }
  }

  delete[] l;
  delete[] d;
  delete[] u;
  delete[] x;
  delete[] b;

  analitycznie.close();
  numerow.close();

  return blad[naj];
}

double konwencjonalnaTrzypunktowa(double h, int N) //funkcja realizująca dyskretyzację konwencjonalną trzypunktową
{
  double *l, *d, *u, *b, *x, *blad, x0 = poczatekPrzedzialu, xn = poczatekPrzedzialu;
  std::fstream konwencjonalnie;
  konwencjonalnie.open("wynikiKonwencjonalnie.txt", std::ios_base::app);
  konwencjonalnie << std::scientific;
  std::cout.precision(10);
  l = new double[N];
  d = new double[N];
  u = new double[N];
  b = new double[N];
  x = new double[N];
  blad = new double[N];

  u[0] = alfa / h;
  d[0] = beta - alfa / h;
  b[0] = -gamma;

  for (int i = 1; i < N - 1; i++) {
    l[i - 1] = p / (h * h) - q / (2.0 * h);
    d[i] = (-2.0 * p) / (h * h) + r;
    u[i] = p / (h * h) + q / (2.0 * h);
    b[i] = (x0 + i * h);
  }

  l[N - 2] = -fi / h;
  d[N - 1] = -fi / h + psi;
  b[N - 1] = -teta;

  thomas(l, d, u, b, x, N);

  for (int i = 0; i < N; i++) {
    blad[i] = fabs(x[i] - rownanieAnalityczne(xn));
    xn += h;
  }

  int naj = znajdzNajwiekszyBlad(blad, N);
  if (N == 162) //losowa liczba
  {
    for (int i = 0; i < N; i++) {
      konwencjonalnie << x0 << "\t" << x[i] << "\n";
      x0 += h;
    }
  }

  delete[] l;
  delete[] d;
  delete[] u;
  delete[] x;
  delete[] b;

  return blad[naj];
}

int main() {
  double h, bladNum, bladKonw;
  int i;

  std::fstream bledy, rzedy;
  bledy.open("bledy.txt", std::fstream::out);
  bledy << std::scientific; //ustawienie precyzji
  std::cout.precision(10);

  for (i = 2; i < 30000; i += 80) {
    h = (koniecPrzedzialu - poczatekPrzedzialu) / (i - 1);
    bladKonw = (konwencjonalnaTrzypunktowa(h, i));
    bladNum = (Numerowa(h, i));
    bledy << log10(h) << "\t" << log10(bladKonw) << "\t" << log10(bladNum) << "\n";
  }
  bledy.close();
  return 0;
}