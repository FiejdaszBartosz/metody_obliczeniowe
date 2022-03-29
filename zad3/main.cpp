//
// Created by Bartosz Fiejdasz on 15/03/2022.
//

#include <iostream>
#include <cmath>

// Funkcje z zadania wraz z obliczonym fi oraz pochodna z fi
double funkcja1(double x) {
  double pom = 1.0;

  pom = sin(x / 4.0);

  return pom * pom - x;
}

double pochodnaFunkcji1(double x) {
  return (1.0 / 4.0) * (sin(x / 2.0)) - 1.0;
}

double fi1(double x) {
  double pom = 1.0;

  pom = sin(x / 4.0);

  return pom * pom;
}

double pochodnaFi1(double x) {
  return 0.25 * sin(x / 2.0);
}

double funkcja2(double x) {
  return tan(2.0 * x) - x - 1.0;
}

double pochodnaFunkcji2(double x) {
  return -1.0 + 2.0 / (cos(2.0 * x) * cos(2.0 * x));
}

double fi2(double x) {
  return tan(2.0 * x) - 1.0;
}

double pochodnaFi2(double x) {
  double pom = 1.0;

  pom = cos(2.0 * x);

  return 2.0 / (pom * pom);
}

typedef double (*funkcja)(double);

/**
 * @brief Metoda Picarda do rozwiazywania algebraicznych równań nielinowych
 * @param fPoczatkowa funkcja dla której będziemy szukać rowzwiązania
 * @param fi funkcja Fi czyli przekształcenie funkcji do postaci x = ...
 * @param pochodnaFi pochodna z funkcji Fi
 * @param x punkt początkowy
 * @param maxIteracje maksymalna liczba iteracji
 * @param tolX zadana tolerancja błedu
 * @param tolF zadana tolerancja reziduum
 * @return 1 w przypadku powodzenia
 * @return -1 w przypadku niepowodzenia
 */
double picard(funkcja fPoczatkowa, funkcja fi, funkcja pochodnaFi, double x, double maxIteracje,
              double tolX, double tolF) {

  // sprawdzenie kryterium zbieznosci dla metody Picarda
  if (fabs(pochodnaFi(x)) >= 1.0) {
    std::cout << "!!! Funkcja rozbierzna !!!\nDla tej funkcji nie da sie uzyc metody Picarda";
    return -1;
  }

  double estymator = 0.0, residuum = 0.0, przybizenie = x;
  int i = 1;
  bool kontynuuj = true;

  std::cout << "Iteracja\t Wartosc przyblizona\t\t residuum\t\t\t estymator bledu\n";

  while (kontynuuj) {
    std::cout << i << "\t";

    // Obliczenie kolejnego przyblizenia wedlug wzoru
    przybizenie = fi(przybizenie);
    std::cout << przybizenie << "\t\t";

    estymator = fabs(przybizenie - x);

    // Nowe przyblizenie staje sie x
    x = przybizenie;
    residuum = fPoczatkowa(x);

    std::cout << residuum << "\t\t" << estymator << "\n";

    // Warunki przerwania
    if ((fabs(residuum) <= tolF) && (estymator <= tolX) || (i >= maxIteracje)) {
      kontynuuj = false;
    }

    i++;
  }

  return 1;
}

/**
 * @brief Metoda bisekcji do rozwiazywania algebraicznych równań nielinowych
 * @param fPoczatkowa funkcja dla której będziemy szukać rowzwiązania
 * @param a lewy koniec przedziału
 * @param b prawy koniec przedziału
 * @param maxIteracje maksymalna liczba iteracji
 * @param tolX zadana tolerancja błedu
 * @param tolF zadana tolerancja reziduum
 * @return 1 w przypadku powodzenia
 * @return -1 w przypadku niepowodzenia
 */
double bisekcja(funkcja fPoczatkowa, double a, double b, double maxIteracje, double tolX, double tolF) {
  // Sprawdzenie czy wartości funkcji dla podanych wartości a oraz b są różnych znaków
  if ((fPoczatkowa(a) > 0 && fPoczatkowa(b) > 0) || (fPoczatkowa(a) < 0 && fPoczatkowa(b) < 0)) {
    std::cout << "Funkcja dla podanego przedzialu nie zmienai znaku\n";
    return -1;
  }

  double x = 0.0, estymator = 0.0, residuum = 0.0;
  int i = 0;
  bool kontynuuj = true;

  std::cout << "Iteracja\t\t a\t\t\t b\t\t\t x\t\t\t residuum\t\t estymator bledu\n";

  while (kontynuuj) {
    // Obliczenie środka przedziału
    x = (a + b) / 2.0;
    estymator = fabs((b - a) / 2.0);
    residuum = fPoczatkowa(x);

    std::cout << i << "\t\t" << a << "\t\t\t" << b << "\t\t\t" << x << "\t\t\t" << residuum
              << "\t\t" << estymator << "\n";

    // Wybranie nowego punktu skrajnego
    if ((fPoczatkowa(a) < 0 && fPoczatkowa(x) > 0) || (fPoczatkowa(a) > 0 && fPoczatkowa(x) < 0))
      b = x;
    else
      a = x;

    // Warunki przerwania
    if ((fabs(residuum) <= tolF) && (estymator <= tolX) || (i >= maxIteracje)) {
      kontynuuj = false;
    }

    i++;
  }

  return 1;
}

/**
 * @brief Metoda Newtona do rozwiazywania algebraicznych równań nielinowych
 * @param fPoczatkowa funkcja dla której będziemy szukać rowzwiązania
 * @param fPochodna pochodna z funkcji
 * @param x punkt początkowy
 * @param maxIteracje maksymalna liczba iteracji
 * @param tolX zadana tolerancja błedu
 * @param tolF zadana tolerancja reziduum
 * @return 1 w przypadku powodzenia
 */
double newton(funkcja fPoczatkowa, funkcja fPochodna, double x, double maxIteracje, double tolX, double tolF) {
  double x0 = x, x1 = 0.0, estymator = 0.0, residuum = 0.0;
  int i = 0;
  bool kontynuuj = true;

  std::cout << "Iteracja\t\t x1\t\t\t residuum\t\t estymator bledu\n";

  while (kontynuuj) {
    // Obliczenie wartosci x1 ze wzoru
    x1 = x0 - (fPoczatkowa(x0) / fPochodna(x0));
    estymator = fabs(x0 - x1);
    x0 = x1;
    residuum = fPoczatkowa(x0);

    std::cout << i << "\t\t" << x1 << "\t\t\t" << residuum << "\t\t" << estymator << "\n";


    // Warunki przerwania
    if ((fabs(residuum) <= tolF) && (estymator <= tolX) || (i >= maxIteracje)) {
      kontynuuj = false;
    }

    i++;
  }

  return 1;
}

/**
 * @brief Metoda siecznych do rozwiazywania algebraicznych równań nielinowych
 * @param fPoczatkowa funkcja dla której będziemy szukać rowzwiązania
 * @param x0 punkt
 * @param x1 punkt x0 + delta x
 * @param maxIteracje maksymalna liczba iteracji
 * @param tolX zadana tolerancja błedu
 * @param tolF zadana tolerancja reziduum
 * @return 1 w przypadku powodzenia
 */
double siecznych(funkcja fPoczatkowa, double x0, double x1, double maxIteracje, double tolX, double tolF) {
  double x2 = 0.0, estymator = 0.0, residuum = 0.0;
  int i = 0;
  bool kontynuuj = true;

  std::cout << "Iteracja\t\t x1\t\t\t residuum\t\t estymator bledu\n";

  while (kontynuuj) {
    x2 = x1 - fPoczatkowa(x1) / ((fPoczatkowa(x1) - fPoczatkowa(x0)) / (x1 - x0));
    estymator = fabs(x2 - x1);
    residuum = fPoczatkowa(x2);

    std::cout << i << "\t\t" << x1 << "\t\t\t" << residuum << "\t\t" << estymator << "\n";

    // Ustawienie nowych punktów
    x0 = x1;
    x1 = x2;

    // Warunki przerwania
    if ((fabs(residuum) <= tolF) && (estymator <= tolX) || (i >= maxIteracje)) {
      kontynuuj = false;
    }

    i++;
  }

  return 1;
}

int main() {
  const int iteracje = 100;
  const double blad = 1e-15;

  std::cout << "***************** Pierwsze rownanie *****************\n";

  std::cout << "Metoda Picarda:\n";
  picard(funkcja1, fi1, pochodnaFi1, 0.3, iteracje, blad, blad);

  std::cout << "\n\nMetoda Bisekcji:\n";
  bisekcja(funkcja1, -0.6, 1, iteracje, blad, blad);

  std::cout << "\n\nMetoda Newtona:\n";
  newton(funkcja1, pochodnaFunkcji1, 0.5, iteracje, blad, blad);

  std::cout << "\n\nMetoda Siecznych:\n";
  siecznych(funkcja1, -0.6, 1, iteracje, blad, blad);

  std::cout << "\n\n***************** Drugie rownanie *****************\n";

  std::cout << "Metoda Picarda:\n";
  picard(funkcja2, fi2, pochodnaFi2, 1, iteracje, blad, blad);

  std::cout << "\n\nMetoda Bisekcji:\n";
  bisekcja(funkcja2, 0.3, 0.6, iteracje, blad, blad);

  std::cout << "\n\nMetoda Newtona:\n";
  newton(funkcja2, pochodnaFunkcji2, 0.4, iteracje, blad, blad);

  std::cout << "\n\nMetoda Siecznych:\n";
  siecznych(funkcja2, 0.1, 0.6, iteracje, blad, blad);


  return 0;
}