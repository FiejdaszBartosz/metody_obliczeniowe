//
// Created by Bartosz Fiejdasz on 21/05/2022.
//

#include <iostream>
#include <fstream>
#include <math.h>

double rozwiazanieAnalityczne(double t); //funkcja do rozwiazywania równania analitycznie
double bezposredniaEulera(double deltaT,
                          double tmax); //funkcja do rozwiazywania równania za pomocą metody bezpośredniej Eulera
double bezposredniaEuleraBlad(double deltaT, int b); //funkcja do obliczania błędu dla metody bezpośredniej Eulera
double posredniaEulera(double deltaT, double t); //funkcja do rozwiazywania równania za pomocą metody bpośredniej Eulera
double posredniaEuleraBlad(double deltaT, int ilosc); //funkcja do obliczania błędu dla metody pośredniej Eulera
double trapezow(double deltaT, double t); //funkcja do rozwiazywania równania za pomocą metody trapezów
double trapezowBlad(double deltaT, int ilosc); //funkcja do obliczania błędu dla metody trapezów

/**
 * Oblicza rozwiazanie analityczne od argumentu t
 * @param t zmienna t
 * @return wynik
 */
double rozwiazanieAnalityczne(double t) {
  return 1 - exp(-10.0 * (t + atan(t)));
}

/**
 * Realizacja bezposredniej metody Eulera. Wzor obliczony wczesniej na kartce
 * @param deltaT
 * @param t zmienna t
 * @return przyblizenie
 */
double bezposredniaEulera(double deltaT, double t) {
  double yk = 0.0; // Warunek poczatkowy

  for (double tk = 0.0; tk < t; tk += deltaT) {
    yk = yk + deltaT * (-((10.0 * tk * tk + 20.0) / (tk * tk + 1.0)) * (yk - 1.0));
  }

  return yk;
}

/**
 * Realizacja bezposredniej metody Eulera ze zwroceniem bledu
 * @param deltaT krok
 * @param ilosc liczba iteracji
 * @return najwieksza roznica pomiedzy yk a wynikiem analitycznym
 */
double bezposredniaEuleraBlad(double deltaT, int ilosc) {
  double yk = 0.0;
  double blad = 0.0;
  double tk = deltaT;
  double wynikAnalityczny;
  double roznica = 0.0;

  for (int i = 0; i < ilosc; i++) {
    wynikAnalityczny = rozwiazanieAnalityczne(tk);

    yk = (-((10.0 * tk * tk + 20.0) / (tk * tk + 1.0)) * (yk - 1.0)) * deltaT + yk;

    roznica = fabs(wynikAnalityczny - yk);
    if (roznica > blad) //znajdujemy największy błąd
      blad = roznica;

    tk += deltaT;
  }

  return blad;
}

/**
 * Realizacja posredniej metody Eulera. Wzor obliczony wczesniej na kartce
 * @param deltaT
 * @param t zmienna t
 * @return przyblizenie
 */
double posredniaEulera(double deltaT, double t) {
  double yk = 0.0;
  double temp;
  for (double i = 0.0; i < t; i += deltaT) {
    temp = (10.0 * (i + deltaT) * (i + deltaT) + 20.0) / ((i + deltaT) * (i + deltaT) + 1.0);
    yk = (yk + deltaT * temp) / (1 + deltaT * temp);
  }
  return yk;
}

/**
 * Realizacja bezposredniej metody Eulera ze zwroceniem bledu
 * @param deltaT krok
 * @param ilosc liczba iteracji
 * @return najwieksza roznica pomiedzy yk a wynikiem analitycznym
 */
double posredniaEuleraBlad(double deltaT, int ilosc) {
  double yk = 0.0;
  double blad = 0.0;
  double tk = deltaT;
  double wAnalityczna;
  double wspolczynnik;
  double roznica = 0.0;

  for (int i = 0; i < ilosc; i++) {
    wAnalityczna = rozwiazanieAnalityczne(tk);

    wspolczynnik = (10.0 * (tk + deltaT) * (tk + deltaT) + 20.0) / ((tk + deltaT) * (tk + deltaT) + 1.0);
    yk = (yk + deltaT * wspolczynnik) / (1 + deltaT * wspolczynnik);

    roznica = fabs(wAnalityczna - yk);
    if (roznica > blad)
      blad = roznica;

    tk += deltaT;
  }
  return blad;
}

/**
 * Realizacja metody trapezow. Wzor obliczony wczesniej na kartce
 * @param deltaT
 * @param t zmienna t
 * @return przyblizenie
 */
double trapezow(double deltaT, double t) {
  double yk = 0.0, temp1, temp2;

  for (double i = 0.0; i < t; i += deltaT) {
    temp1 = ((10.0 * i * i + 20.0) / (i * i + 1.0));
    temp2 = (10.0 * (i + deltaT) * (i + deltaT) + 20.0) / ((i + deltaT) * (i + deltaT) + 1.0);

    yk = ((-deltaT / 2.0) * (temp1 * (yk - 1.0) - temp2) + yk) / (1.0 + (deltaT / 2.0) * temp2);
  }

  return yk;
}

/**
 * Realizacja metody trapezow ze zwroceniem bledu
 * @param deltaT krok
 * @param ilosc liczba iteracji
 * @return najwieksza roznica pomiedzy yk a wynikiem analitycznym
 */
double trapezowBlad(double deltaT, int ilosc) {
  double blad = 0.0;
  double t = deltaT;
  double yk = 0.0;
  double wAnalityczna;
  double temp1, temp2;
  double roznica = 0.0;

  for (int i = 0; i < ilosc; i++) {
    wAnalityczna = rozwiazanieAnalityczne(t);

    temp1 = ((10.0 * t * t + 20.0) / (t * t + 1.0));
    temp2 = (10.0 * (t + deltaT) * (t + deltaT) + 20.0) / ((t + deltaT) * (t + deltaT) + 1.0);

    yk = ((-deltaT / 2.0) * (temp1 * (yk - 1.0) - temp2) + yk) / (1.0 + (deltaT / 2.0) * temp2);

    roznica = fabs(wAnalityczna - yk);
    if (roznica > blad)
      blad = roznica;

    t += deltaT;
  }

  return blad;
}

int main() {
  std::fstream bledy, wyniki, wyniki2;

  bledy.open("bledy.txt", std::fstream::out);
  wyniki.open("wyniki.txt", std::fstream::out);
  wyniki2.open("wyniki2.txt", std::fstream::out);

  wyniki << std::scientific;
  wyniki2 << std::scientific;
  bledy << std::scientific;
  std::cout.precision(10);

  double h, analitycznieWynik, besWynik, bensWynik, peWynik, tWynik;
  int N = 5000000; //maksymalna ilosć iteracji

  // Obliczanie bledow z metody
  h = 0.1;
  for (h; h > 1e-16; h = h / 2) {
    besWynik = log10(bezposredniaEuleraBlad(h, N)); //stabilna bezpośrednia Eulera
    peWynik = log10(posredniaEuleraBlad(h, N));
    tWynik = log10(trapezowBlad(h, N));
    bledy << log10(h) << "\t" << besWynik << "\t" << peWynik << "\t" << tWynik << "\n";
  }

  h = 0.01;
  for (double t = 0; t < 5; t += 0.01) //pętla wyników do rysunku zależności y od zmiennej niezależnej t
  {
    analitycznieWynik = rozwiazanieAnalityczne(t);
    peWynik = posredniaEulera(h, t);
    tWynik = trapezow(h, t);
    besWynik = bezposredniaEulera(h, t);
    wyniki << t << "\t" << analitycznieWynik << "\t" << peWynik << "\t" << tWynik << "\t" << besWynik << "\n";
  }

  h = 0.15;
  for (double t = 0; t < 5; t += 0.15) {
    bensWynik = bezposredniaEulera(h, t); //niestabilna bezpośrednia Eulera
    wyniki2 << t << "\t" << rozwiazanieAnalityczne(t) << "\t" << bensWynik << "\n";
  }
  wyniki2.close();
  bledy.close();
  wyniki.close();
}