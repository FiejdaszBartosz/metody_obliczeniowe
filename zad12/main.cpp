//
// Created by Bartosz Fiejdasz on 05/06/2022.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

using namespace std;

#define dx 1e-2
#define N 11
#define A -1
#define B 1

double X[N];

/**
 * zadana funkcaja
 * @param x zmienna x
 * @return wartosc f(x)
 */
double funkcja_dokladna(double x) {
  return 1.0 / (1.0 + 10.0 * x * x * x * x * x * x);
}

/**
 * Oblicza wezlow rownoodleglych
 */
void wezly_rownooddalone() {
  for (int i = 0; i < N; ++i) {
    X[i] = A + i * ((B - A) / (N - 1.0));
  }
}

/**
 * Oblicza wezlow czebyszewa. Wedlug wzorow z wykladu
 */
void wezly_czebyszewa() {
  double temp;
  for (int i = 0; i < N; ++i) {
    temp = cos(((2.0 * i + 1.0) / (2.0 * N + 2.0)) * M_PI);
    X[i] = (B + A) / 2.0 + ((B - A) / 2.0) * temp;
  }
}

/**
 * Implementacja bazyw newtona do konstrukcji wielomianu interpolacyjnego
 * @param wykres_nazwa zanwa wykresu
 */
void baza_newtona_wyniki(const string &wykres_nazwa) {
  double c[N][N];
  double wartosci, krok;
  fstream plik;

  // Zasosowanie algorytmu z wykladu
  // Uzupelnienie 2 kolumny
  for (int i = 0; i < N; ++i) {
    c[i][0] = funkcja_dokladna(X[i]);
  }

  // Uzupelnienie pozostalych kolumn
  for (int i = 1; i < N; ++i)
    for (int j = 0; j < N - i; ++j)
      c[j][i] = (c[j + 1][i - 1] - c[j][i - 1]) / (X[i + j] - X[j]);

  plik.open(string("wyniki_") + wykres_nazwa + (".txt"), fstream::out);

  // Obliczanie wartosci dla wielomianu interpolacyjnego
  for (krok = A; krok <= B; krok += dx) {
    wartosci = c[0][N - 1];
    for (int i = N - 1; i > 0; --i) {
      wartosci *= (krok - X[i - 1]);
      wartosci += c[0][i - 1];
    }

    plik << krok << "\t" << wartosci << endl;
  }

  plik.close();
}
void funkcja_dokladna_wyniki() {
  fstream result_Dokladne;
  result_Dokladne.open("wyniki_dokladne.txt", fstream::out);
  result_Dokladne << scientific;

  for (double ix = A; ix <= B; ix += dx) {
    result_Dokladne << ix << "\t" << funkcja_dokladna(ix) << endl;
  }

  result_Dokladne.close();
}

int main() {
  wezly_rownooddalone();
  baza_newtona_wyniki("rownoodlegle");
  wezly_czebyszewa();
  baza_newtona_wyniki("czebyszew");
  funkcja_dokladna_wyniki();
}

// plot "wyniki_rownoodlegle.txt" title "Rownoodlegle", "wyniki_czebyszew.txt" title "Czebyszew" ,"wyniki_dokladne.txt"  w lines title "Dokladne"