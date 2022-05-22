//
// Created by Bartosz Fiejdasz on 17/05/2022.
//

#include <iostream>
#include <cmath>
#include <fstream>

// Odczytanie wspolczynnikow z zadanego rownania rozniczkowego

// Z zadanego równania różniczkowego
const double P = 1.0, Q = 0.0, R = -4.0;

// Z warunków brzegowych
const double ALFA = 0.0, BETA = 1.0, GAMMA = -1.0;
const double FI = 0.0, PSI = 1.0, TETA = 0.0;

// Zadany przedzial
const double POCZATEK_PRZEDZIALU = 0.0, KONIEC_PRZEDZIALU = 1.0;

/**
 * Rozwiazanie analityczne
 * @param x zmienna x
 * @return rozwizanie dla zmiennej x
 */
double rownanieAnalityczne(double x) {
  return (exp(2.0 - 2.0 * x) - 4.0 * exp(4.0 - x * 2.0) + 4.0 * exp(x * 2.0) - exp(2.0 + 2.0 * x) - x + x * exp(4.0))
      / (4.0 - 4 * exp(4.0));
}

/**
 * Realizacja algorytmu Thomasa, zmienne podpisane jak na wykladzie
 * @param l wektor l
 * @param d wektor d
 * @param u wektor u
 * @param b wektor b
 * @param x wektor wynikowy x
 * @param N rozmiar wektorow
 */
void thomas(double *l, double *d, double *u, double *b, double *x, int N) {
  double *bKopia = new double[N];
  double *dKopia = new double[N];

  // Realizuje algorytm Thomasa zgodnie z wykladem
  dKopia[0] = d[0];
  bKopia[0] = b[0];

  // wyliczenie ni
  for (int i = 1; i < N; i++) {
    dKopia[i] = d[i] - l[i - 1] * (u[i - 1] / dKopia[i - 1]);
  }

  // wyliczenie r
  for (int i = 1; i < N; i++) {
    bKopia[i] = b[i] - l[i - 1] * bKopia[i - 1] / dKopia[i - 1];
  }

  // Obliczanie rozwiazania
  x[N - 1] = bKopia[N - 1] / dKopia[N - 1];

  // Obliczenie pozostalych x
  for (int i = N - 2; i >= 0; i--) {
    x[i] = (bKopia[i] - u[i] * x[i + 1]) / dKopia[i];
  }

  delete[] bKopia;
  delete[] dKopia;
}

/**
 * Znajduje najwiekszy blad i zwraca jego index (Norma maximum)
 * @param blad wektor bledu
 * @param N rozmiar
 * @return index
 */
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
  double *l, *d, *u, *b, *x, *blad, x0 = POCZATEK_PRZEDZIALU, xn = POCZATEK_PRZEDZIALU;
  std::fstream numerow, analitycznie;
  numerow.open("wynikiNumerow.txt", std::ios_base::app);
  analitycznie.open("wynikiAnalitycznie.txt", std::ios_base::app);
  analitycznie << std::scientific;
  numerow << std::scientific;
  std::cout.precision(10);
  l = new double[N];
  d = new double[N];
  u = new double[N];
  b = new double[N];
  x = new double[N];
  blad = new double[N];

  // Realizuje algorytm zgodnie ze wzorami z wykladu

  // Z warunku brzegowego
  u[0] = ALFA / h;
  d[0] = BETA - ALFA / h;
  b[0] = -GAMMA;

  // Wyznaczenie srodkowych wyrazow w zapisie wektorowym
  for (int i = 1; i < N - 1; i++) {
    l[i - 1] = P / (h * h) + R / 12.0;
    d[i] = (-2.0 * P) / (h * h) + R * (10.0 / 12.0);
    u[i] = P / (h * h) + R / 12.0;
    b[i] = (x0 + i * h - h) / 12.0 + (10.0 / 12.0) * (x0 + i * h) + (x0 + i * h + h) / 12.0;
  }

  // Z warunku brzegowego
  l[N - 2] = -FI / h;
  d[N - 1] = -FI / h + PSI;
  b[N - 1] = -TETA;


  // Rozwiazanie macierzy trojdiagonalnej algorytmem thomasa
  thomas(l, d, u, b, x, N);

  // Obliczenie bledu między algotytmem numerowa, a rozwiązaniem analitycznym
  for (int i = 0; i < N; i++) {
    blad[i] = fabs(x[i] - rownanieAnalityczne(xn));
    xn += h;
  }

  int naj = znajdzNajwiekszyBlad(blad, N); //znajdujemy największy błąd

  if (N == 1002) {
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

/**
 * Realizacja trzypunktowej dyskretyzacji konwencjonalnej
 * @param h krok
 * @param N rozmiar
 * @return blad
 */
double konwencjonalnaTrzypunktowa(double h, int N) //funkcja realizująca dyskretyzację konwencjonalną trzypunktową
{
  double *l, *d, *u, *b, *x, *blad, x0 = POCZATEK_PRZEDZIALU, xn = POCZATEK_PRZEDZIALU;
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

  // Z warunku brzegowego
  u[0] = ALFA / h;
  d[0] = BETA - ALFA / h;
  b[0] = -GAMMA;

  // Wyznaczenie srodkowych wyrazow w zapisie wektorowym
  for (int i = 1; i < N - 1; i++) {
    l[i - 1] = P / (h * h) - Q / (2.0 * h);
    d[i] = (-2.0 * P) / (h * h) + R;
    u[i] = P / (h * h) + Q / (2.0 * h);
    b[i] = (x0 + i * h);
  }

  // Z warunku brzegowego
  l[N - 2] = -FI / h;
  d[N - 1] = -FI / h + PSI;
  b[N - 1] = -TETA;

  thomas(l, d, u, b, x, N);

  for (int i = 0; i < N; i++) {
    blad[i] = fabs(x[i] - rownanieAnalityczne(xn));
    xn += h;
  }

  int naj = znajdzNajwiekszyBlad(blad, N);
  if (N == 1002) {
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

int main() //funkcja główna
{
  double h, bladNum, bladKonw;
  int N; //ilość iteracji

  std::fstream bledy, rzedy;
  bledy.open("bledy.txt", std::fstream::out);
  bledy << std::scientific;
  std::cout.precision(10);

  for (N = 2; N < 1000000; N += 100) {
    h = (KONIEC_PRZEDZIALU - POCZATEK_PRZEDZIALU) / (N - 1);
    bladKonw = konwencjonalnaTrzypunktowa(h, N);
    bladNum = Numerowa(h, N);
    bledy << log10(h) << "\t" << log10(bladKonw) << "\t" << log10(bladNum) << "\n";
  }
  bledy.close();
  return 0;
}