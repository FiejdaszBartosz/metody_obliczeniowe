//
// Created by Bartosz Fiejdasz on 24/05/2022.
//

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "calerf.h"
#include "calerf.cpp"

using namespace std;

const int D = 1;
const double T_MIN = 0.0;
const double T_MAX = 2.0;
const double A = 6.0 * sqrt(D * T_MAX);
const double X_MAX = A;
const double X_MIN = -A;
const double LAMBDA_BEZPOSREDNIE = 0.4;
const double LAMBDA_POSREDNIE = 1.0;
const double H = 0.1;

# define wykres1

/**
 * Tworzy wektor o rozmiarze n
 * @param n rozmiar wektora
 * @return wskaznik do wektora
 */
double *utworzWektor(int n) {
  return new double[n];
}

/**
 * Tworzy macierz n na m
 * @param n rozmiar n
 * @param m rozmiar m
 * @return wskaznik do macierzy
 */
double **utworzMacierz(int n, int m) {
  double **matrix = new double *[n];

  for (int i = 0; i < n; i++)
    matrix[i] = new double[m];

  return matrix;
}

/**
 * Dealokuje wektor
 * @param wektor wskaznik do wektora ktory ma zostac dealokowany
 */
void usunWektor(double *wektor) {
  delete[] wektor;
}

/**
 * Dealokuje macierz
 * @param macierz  wskaznik do macierzy ktora ma zostac zdealokowana
 * @param n rozmiar n
 */
void usunMacierz(double **macierz, int n) {
  for (int i = 0; i < n; i++)
    delete[] macierz[i];

  delete[] macierz;
}

/**
 * Zapisuje podany wektor do pliku
 * @param wektor
 * @param n
 * @param nazwa_pliku
 */
void zapiszWektor(double *wektor, int n, string nazwa_pliku) {
  fstream file(nazwa_pliku.c_str(), ios::out);

  if (file.is_open()) {
    for (int i = 0; i < n; i++) {
      file << wektor[i] << endl;
    }
  }
}

/**
 * Zapisuje podany wektor do pliku
 * @param wektor
 * @param n
 * @param nazwa_pliku
 */
void zapiszDwaWektory(double *wektor1, double *wektor2, int n, string nazwa_pliku) {
  fstream file(nazwa_pliku.c_str(), ios::out);

  if (file.is_open()) {
    for (int i = 0; i < n; i++) {
      file << wektor1[i] << "\t" << wektor2[i] << endl;
    }
  }
}

/**
 * Zapisuje podana macierz do pliku
 * @param matrix podana macierz
 * @param n rozmiar n
 * @param m rozmiar m
 * @param nazwa_pliku nazwa pliku do ktorego ma zostac zapisana macierz
 */
void zapiszMacierz(double **matrix, int n, int m, string nazwa_pliku) {
  fstream file(nazwa_pliku.c_str(), ios::out);

  if (file.is_open()) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        file << matrix[i][j] << ";";
      }
      file << "\n";
    }
  }

  file.close();
}

/**
 *
 * @param vector
 * @param n
 * @return
 */
double normaMax(double *vector, int n) {
  double max = fabs(vector[0]);
  for (int i = 1; i < n; i++) {
    if (max < fabs(vector[i])) {
      max = vector[i];
    }
  }
  return max;
}

/**
 * Funkcja realizujaca algorytm thomasa
 * @param l wektor l
 * @param d wektor d
 * @param u wektor u
 * @param b wektor b
 * @param x wektor x
 * @param m romiar m
 */
void algorytmThomasa(double *l, double *d, double *u, double *b, double *x, int m) {
  for (int i = 2; i < m; ++i) {
    d[i] = d[i] - (l[i - 1] / d[i - 1]) * u[i - 1];
    b[i] = b[i] - (l[i - 1] / d[i - 1]) * b[i - 1];
  }
  x[m - 1] = b[m - 1] / d[m - 1];
  for (int i = m - 2; i >= 0; --i) {
    x[i] = (b[i] - u[i] * x[i + 1]) / d[i];
  }
}

/**
 * Uzupelnia macierz o warunek poczatkowy
 * @param macierz dana macierz
 * @param n rozmiar n
 * @param m rozmiar m
 */
void warunekPoczatkowy(double **macierz, int n, int m) {
  double x = X_MIN;
  for (int i = 0; i < m; i++) {
    if (x < 0) {
      macierz[0][i] = 1.0;
    } else {
      macierz[0][i] = 0.0;
    }
    x += H;
  }
}

/**
 * Uzupelnia macierz o warunek brzegowy
 * @param macierz dana macierz
 * @param n rozmiar n
 * @param m rozmiar m
 */
void warunekBrzegowy(double **macierz, int n, int m) {
  for (int i = 0; i < n; i++) {
    macierz[i][0] = 1.0;
    macierz[i][m - 1] = 0.0;
  }
}

/**
 * Rozwiazanie analityczne zadanego rowanania rozniczkowego
 * @param x zmienna x
 * @param t zmienan t
 * @return wartosc funkcji f(x,t)
 */
double rozwiazanieAnalityczne(double x, double t) {
  return 0.5 * calerf::ERFC_L((x) / (2.0 * sqrt(D * t)));
}

/**
 * Zwraca macierz rozwiazana z wykorzystaniem rozwiazania analitycznego
 * @param n romiar n
 * @param m rozmiar m
 * @param h krok h
 * @param dt przyrost t
 * @return rozwiazanie
 */
double **rozwiazAnalitycznie(int n, int m, double h, double dt) {
  double **rozwiazanie = utworzMacierz(n, m);
  double x = X_MIN;
  double t = T_MIN;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      rozwiazanie[i][j] = rozwiazanieAnalityczne(x, t);
      x += h;
    }
    x = X_MIN;
    t += dt;
  }
  return rozwiazanie;
}

/**
 * Implementacja algorytmu na klasyczna metode bezposrednia
 * @param macierz macierz na ktorej ma byc wykonana metoda
 * @param n rozmiar n
 * @param m rozmiar m
 */
void kmb(double **macierz, int n, int m) {
  for (int i = 1; i < n; i++) {
    for (int j = 1; j < m - 1; j++) {
      macierz[i][j] =
          macierz[i - 1][j]
              + LAMBDA_BEZPOSREDNIE * (macierz[i - 1][j - 1] - (2 * macierz[i - 1][j]) + macierz[i - 1][j + 1]);
    }
  }
}

/**
 * Zwraca rozwiazanie przy uzyciu klasycznej metody bezposredniej, uzuopelnia macierz o warunek poczatkowy i brzegowy
 * @param n rozmiar n
 * @param m rozmiar m
 * @return macierz z rozwiazaniem
 */
double **kmbRozwiazanie(int n, int m) {
  double **rozwiazanie = utworzMacierz(n, m);
  warunekPoczatkowy(rozwiazanie, n, m);
  warunekBrzegowy(rozwiazanie, n, m);
  kmb(rozwiazanie, n, m);
  return rozwiazanie;
}

/**
 * Implementacja metody Laasonen, uklad rownan rozwiazany przy pomocy algorytmu thomasa
 * @param macirz zadana macierz
 * @param n rozmiar n
 * @param m rozmiar m
 */
void mlThomas(double **macirz, int n, int m) {
  double L_LAMBDA = 1.0 + 2.0 * LAMBDA_POSREDNIE;
  double *l = utworzWektor(m);
  double *d = utworzWektor(m);
  double *u = utworzWektor(m);
  double *b = utworzWektor(m);
  double *x = utworzWektor(m);

  for (int k = 1; k < n; k++) {
    l[0] = 0.0;
    d[0] = 1.0;
    u[0] = 0.0;
    b[0] = macirz[k - 1][0];

    for (int i = 1; i < m - 1; i++) {
      l[i] = LAMBDA_POSREDNIE;
      d[i] = -L_LAMBDA;
      u[i] = LAMBDA_POSREDNIE;
      b[i] = -macirz[k - 1][i];
    }

    l[m - 1] = 0.0;
    d[m - 1] = 1.0;
    u[m - 1] = 0.0;
    b[m - 1] = 0.0;

    algorytmThomasa(l, d, u, b, x, m);

    for (int i = 1; i < m - 1; i++) {
      macirz[k][i] = x[i];
    }
  }
  usunWektor(l);
  usunWektor(d);
  usunWektor(u);
  usunWektor(b);
  usunWektor(x);
}

/**
 * Wyliczenie wektora residuum
 * @param macierz macierz wejsciowa
 * @param b wektor b
 * @param x wektor x
 * @param m rozmiar m
 * @return wektor residualny
 */
double *residuum(double **macierz, double *b, double *x, int m) {
  double sum = 0.0;
  double *wynik = new double[m];
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      sum += macierz[i][j] * x[j];
    }
    wynik[i] = sum - b[i];
    sum = 0.0;
  }
  return wynik;
}

/**
 * Oblicza estymator
 * @param xNowe
 * @param xPoprzednie
 * @param m rozmiar m
 * @return zwraca estymator
 */
double estymator(double *xNowe, double *xPoprzednie, int m) {
  double max = 0.0;
  double *p = new double[m];

  for (int i = 0; i < m; i++)
    p[i] = xNowe[i] - xPoprzednie[i];

  if (fabs(p[0]) > fabs(p[1]))
    max = fabs(p[0]);
  else
    max = fabs(p[1]);

  for (int i = 0; i < m; i++) {
    if (fabs(p[i]) > max)
      max = fabs(p[i]);
  }

  delete[] p;
  return max;
}

/**
 * Implementacja metody jacobiego
 * @param macierz zadana macierz
 * @param b wektor b
 * @param x wektor x
 * @param n rozmiar n
 * @param m rozmiar m
 */
void jacobi(double **macierz, double *b, double *x, int n, int m) {
  double TOL = 1e-16;
  double *xNowy = utworzWektor(m);
  double sum = 0.0;

  for (int iter = 0; iter < 100; iter++) {
    for (int i = 0; i < m; i++) {
      sum = 0.0;
      for (int j = 0; j < m; j++) {
        if (j != i) {
          sum += (macierz[i][j] * x[j]);
        }
      }
      xNowy[i] = (-sum + b[i]) / macierz[i][i];
    }
    for (int i = 0; i < m; i++) {
      x[i] = xNowy[i];
    }
    if ((fabs((normaMax(residuum(macierz, b, x, m), m))) < TOL) && (fabs((estymator(xNowy, x, m))) < TOL)) break;
  }
}

/**
 * Implementacja metody Laasonem, rozwiazanie algebraiczne przy pomocy metody jacobiego
 * @param macierz
 * @param n
 * @param m
 */
void mlJacobi(double **macierz, int n, int m) {
  double tempLabda = 1.0 + 2.0 * LAMBDA_POSREDNIE;
  double *b = new double[m];
  double *wynik = new double[m];
  double **tempMacierz = new double *[m];

  for (int i = 0; i < m; i++) {
    b[i] = 0.0;
    wynik[i] = 0.0;
  }

  for (int i = 0; i < m; i++)
    tempMacierz[i] = new double[m];

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++)
      tempMacierz[i][j] = 0.0;
  }

  for (int k = 1; k < n; k++) {
    tempMacierz[0][0] = 1.0;
    b[0] = macierz[k - 1][0];

    for (int i = 1; i < m - 1; i++) {
      tempMacierz[i][i] = -tempLabda;
      tempMacierz[i][i + 1] = LAMBDA_POSREDNIE;
      tempMacierz[i][i - 1] = LAMBDA_POSREDNIE;
      b[i] = -macierz[k - 1][i];
    }

    b[m - 1] = 0.0;
    tempMacierz[m - 1][m - 1] = 1.0;

    jacobi(tempMacierz, b, wynik, n, m);

    for (int i = 1; i < m - 1; i++)
      macierz[k][i] = wynik[i];
  }
}

double **mlThomasRozwiazanie(int n, int m) {
  double **laasonen_thomas = utworzMacierz(n, m);
  warunekPoczatkowy(laasonen_thomas, n, m);
  warunekBrzegowy(laasonen_thomas, n, m);
  mlThomas(laasonen_thomas, n, m);
  return laasonen_thomas;
}

/**
 * Zwraca rozwiazanie przy uzyciu metody posredniej Lassonem, uzuopelnia macierz o warunek poczatkowy i brzegowy
 * @param n rozmiar n
 * @param m rozmiar m
 * @return macierz z rozwiazaniem
 */
double **mlJacobiRozwiazanie(int n, int m) {
  double **rozwiazanie = utworzMacierz(n, m);

  warunekPoczatkowy(rozwiazanie, n, m);
  warunekBrzegowy(rozwiazanie, n, m);
  mlJacobi(rozwiazanie, n, m);

  return rozwiazanie;
}

/**
 * Oblicza blad pomiedzy dokladnym a przyblizonym rozwiazaniem
 * @param dokladne dokladne rozwiazanie
 * @param przyblizenie przyblizone rozwiazanie
 * @param blad macierz na bledy
 * @param n rozmiar n
 * @param m romiar m
 * @return macierz zawierajaca obliczone bledy
 */
double **obliczBlad(double **dokladne, double **przyblizenie, int n, int m) {
  double **blad = utworzMacierz(n, m);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      blad[i][j] = fabs(przyblizenie[i][j] - dokladne[i][j]);
    }
  }

  return blad;
}

/**
 * Oblicza maksymalny blad
 * @param blad macierz z bledami
 * @param n rozmiar n
 * @param m rozmiar m
 * @return najwiekszy blad
 */
double *maxBlad(double **blad, int n, int m) {
  double *wynik = utworzWektor(n);

  for (int i = 0; i < n; i++)
    wynik[i] = normaMax(blad[i], m);

  return wynik;
}

/**
 * Oblicza dt
 * @param lambda wartosc lambda
 * @param h wartosc h
 * @param d wartosc d
 * @return dt
 */
double obliczDT(double lambda, double h, double d) {
  return (lambda * h * h) / d;
}

/**
 * Zwraca wektor z kolejnym wartoscami x += H
 * @param dt wartosc dt
 * @param n rozmiar n
 * @param m rozmair m
 * @return wektor wynikowy
 */
double *obliczOdstepyH(double dt, int n, int m) {
  double *wynik = utworzWektor(m);
  double x = X_MIN;

  for (int i = 0; i < m; i++) {
    wynik[i] = x;
    x += H;
  }

  return wynik;
}

/**
 * Zwraca wektor z kolejnym wartoscami x += dt
 * @param dt wartosc dt
 * @param n rozmiar n
 * @param m rozmair m
 * @return wektor wynikowy
 */
double *obliczOdstepyDT(double dt, int n, int m) {
  double *wynik = utworzWektor(n);
  double t = T_MIN;

  for (int i = 0; i < n; i++) {
    wynik[i] = t;
    t += dt;
  }

  return wynik;
}

int main() {
  double dt = obliczDT(LAMBDA_BEZPOSREDNIE, H, D);
  int n = ((T_MAX - T_MIN) / dt);
  int m = ((X_MAX - X_MIN) / H);
  double **rozwiazanieAnalityczne;
  double **rozwiazanieKmb;
  double **rozwiazanieLaasonenThomas;
  double **rozwiazanieLaasonenJacobi;
  double **macierzBledy;
  double *wektorBledy;
  double *odstepDT;
  double *odstepX;

  // Rozwiazanie analityczne
  rozwiazanieAnalityczne = rozwiazAnalitycznie(n, m, H, dt);
  zapiszMacierz(rozwiazanieAnalityczne, n, m, "rozwiazanieAnalityczne.csv");

  // KMB
  rozwiazanieKmb = kmbRozwiazanie(n, m);
  zapiszMacierz(rozwiazanieKmb, n, m, "rozwiazanieKmb.csv");

  macierzBledy = obliczBlad(rozwiazanieAnalityczne, rozwiazanieKmb, n, m);
  wektorBledy = maxBlad(macierzBledy, n, m);
  zapiszWektor(wektorBledy, n, "maxErrKmb.csv");
  zapiszMacierz(macierzBledy, n, m, "errMacierzKmb.csv");

  odstepX = obliczOdstepyH(dt, n, m);
  odstepDT = obliczOdstepyDT(dt, n, m);
  zapiszWektor(odstepDT, n, "odstepyCzasoweKmb.csv");
  zapiszWektor(odstepX, n, "odstepyXKmb.csv");

  // Metoda Posrednia Laasonem + Algorytm Thomasa
  dt = obliczDT(LAMBDA_POSREDNIE, H, D);
  n = ((T_MAX - T_MIN) / dt);
  m = ((X_MAX - X_MIN) / H);

  rozwiazanieLaasonenThomas = mlThomasRozwiazanie(n, m);
  zapiszMacierz(rozwiazanieLaasonenThomas, n, m, "laasonenThomasRozwiazanie.csv");

  macierzBledy = obliczBlad(rozwiazanieAnalityczne, rozwiazanieLaasonenThomas, n, m);
  wektorBledy = maxBlad(macierzBledy, n, m);
  zapiszWektor(wektorBledy, n, "maxErrLaasonenThomas.csv");
  zapiszMacierz(macierzBledy, n, m, "errLaasonenThomas.csv");

  odstepX = obliczOdstepyH(dt, n, m);
  odstepDT = obliczOdstepyDT(dt, n, m);
  zapiszWektor(odstepX, n, "odstepyXLaasonenThomas.csv");
  zapiszWektor(odstepDT, n, "odstepyCzasoweLaasonenThomas.csv");

  // Metoda Posrednia Laasonem + Metoda Iteracyjna Jacobiego
  dt = obliczDT(LAMBDA_POSREDNIE, H, D);
  n = ((T_MAX - T_MIN) / dt);
  m = ((X_MAX - X_MIN) / H);
  rozwiazanieLaasonenJacobi = mlJacobiRozwiazanie(n, m);
  zapiszMacierz(rozwiazanieLaasonenThomas, n, m, "laasonenJacobiRozwiazanie.csv");

  macierzBledy = obliczBlad(rozwiazanieAnalityczne, rozwiazanieLaasonenJacobi, n, m);
  wektorBledy = maxBlad(macierzBledy, n, m);
  zapiszWektor(wektorBledy, n, "maxErrLaasonenJacobi.csv");
  zapiszMacierz(macierzBledy, n, m, "errLaasonenJacobi.csv");

  odstepX = obliczOdstepyH(dt, n, m);
  odstepDT = obliczOdstepyDT(dt, n, m);
  zapiszWektor(odstepX, n, "odstepyXLaasonenThomas.csv");
  zapiszWektor(odstepDT, n, "odstepyCzasoweLaasonenJacobi.csv");

#ifdef wykres1
  double h, deltaT, maxErr;
  int k = 1000, j = 0;
  double *wykres1_kmb = utworzWektor(k);
  double *wykres1_LT = utworzWektor(k);
  double *wykres1_LJ = utworzWektor(k);
  double *wykres1_kroki = utworzWektor(k);


  for (int i = 40; i < 400; i += 40) {
    h = (X_MAX - X_MIN) / i;
    dt = obliczDT(LAMBDA_BEZPOSREDNIE, h, D);
    n = ((T_MAX - T_MIN) / dt);
    m = ((X_MAX - X_MIN) / h);

    rozwiazanieAnalityczne = rozwiazAnalitycznie(n, m, h, dt);

    rozwiazanieKmb = kmbRozwiazanie(n, m);
    macierzBledy = obliczBlad(rozwiazanieAnalityczne, rozwiazanieKmb, n, m);
    wektorBledy = maxBlad(macierzBledy, n, m);
    wykres1_kmb[j] = log10(fabs(normaMax(wektorBledy, n)));

    dt = obliczDT(LAMBDA_POSREDNIE, h, D);
    n = ((T_MAX - T_MIN) / dt);

    rozwiazanieLaasonenThomas = mlThomasRozwiazanie(n, m);
    macierzBledy = obliczBlad(rozwiazanieAnalityczne, rozwiazanieLaasonenThomas, n, m);
    wektorBledy = maxBlad(macierzBledy, n, m);
    wykres1_LT[j] = log10(fabs(wektorBledy[n - 1]));

    rozwiazanieLaasonenJacobi = mlJacobiRozwiazanie(n, m);
    macierzBledy = obliczBlad(rozwiazanieAnalityczne, rozwiazanieLaasonenJacobi, n, m);
    wektorBledy = maxBlad(macierzBledy, n, m);
    wykres1_LJ[j] = log10(fabs(wektorBledy[n - 1]));

    wykres1_kroki[j] = log10(h);
    j++;
  }

  zapiszDwaWektory( wykres1_kroki, wykres1_kmb, k, "wykres1_1.csv");
  zapiszDwaWektory(wykres1_kroki, wykres1_LT, k, "wykres1_2.csv");
  zapiszDwaWektory(wykres1_kroki, wykres1_LJ, k, "wykres1_3.csv");
#endif

  usunMacierz(rozwiazanieAnalityczne, n);
  usunMacierz(rozwiazanieLaasonenThomas, n);
  usunMacierz(rozwiazanieLaasonenJacobi, n);
  usunMacierz(macierzBledy, n);
  usunWektor(wektorBledy);
  usunWektor(odstepX);
  usunWektor(odstepDT);

#ifdef wykres1
  usunWektor(wykres1_kmb);
  usunWektor(wykres1_LT);
  usunWektor(wykres1_LJ);
  usunWektor(wykres1_kroki);
#endif
  return 0;
}