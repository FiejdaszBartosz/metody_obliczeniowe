//
// Created by Bartosz Fiejdasz on 29/03/2022.
//

#include <iostream>
#include <cmath>
#include <iomanip>

#define N 4

/**
 * Tworzy macierz kwadratowa o podanym rozmiarze
 * @param n rozmiar
 * @return wskaznik do utworzonej macierzy
 */
double **stworzMacierz(int n) {
  double **macierz = new double *[n];
  for (int i = 0; i < n; i++)
    macierz[i] = new double[n];
  return macierz;
}

/**
 * Usuwa macierz na ktora wskazuje wskaznik
 * @param pMacierz wskaznik do macierzy
 * @param n rozmiar macierzy
 */
void usunMacierz(double **pMacierz, int n) {
  for (int i = 0; i < n; i++)
    delete[] pMacierz[i];

  delete[] pMacierz;
}

/**
 * Uzupelnia macierz parametrami zadania
 * @param pMacierz wskaznik do ktorego przypiszmy macierz
 * @param n wymiar macierzy
 */
void uzupelnijMacierz(double **pMacierz, int n) {
  double macierzZadana[4][4] =
      {{1.0, -20.0, 30.0, -4.0},
       {2.0, -40.0, -6.0, 50.0},
       {9.0, -180.0, 11.0, -12.0},
       {-16.0, 15.0, -140.0, 13.0}};

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      pMacierz[i][j] = macierzZadana[i][j];
}
/**
 * Wyswietla podany wektor
 * @param b wskaznik do wektora
 * @param n rozmiar wektora
 */
void wyswietlWektor(double *b, int n) {
  for (int i = 0; i < n; i++)
    std::cout << b[i] << "\n";
}

/**
 * Wyswietla podana macierz
 * @param pMacierz wskaznik do macierzy
 * @param n rozmiar macierzy
 * @param index indeks w macierzy
 */
void wyswietlMacierz(double **pMacierz, int n, int *index) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++)
      std::cout << std::setw(8) << pMacierz[index[i]][j] << "\t";
    std::cout << "\n";
  }
}

/**
 * Realizuje czesciowy wybor elementu podstawowego. Potrzebna aby usiknac dzielenia przez zero
 * @param pMacierz wskaznik do macierzy
 * @param n rozmiar macierzy
 * @param j pozycja w index
 * @param index
 * @return wiersz po zamianie
 */
int elementPodstawowy(double **pMacierz, int n, int j, int *index) {
  int numerWiersza;
  // Szuka wartosci najwiekszej w kolumnie ponizej komorki [j][j]
  for (int i = j; i < n; i++) {
    if (std::fabs(pMacierz[index[i]][j]) < std::fabs(pMacierz[index[i + 1]][j]))
      numerWiersza = index[i + 1];
    else
      numerWiersza = index[i];
  }

  return numerWiersza;
}

void gauss(double **pMacierz, int *index) {
  int wiersz;
  double v;   // Element ponizej diagonali
  // poruszanie sie po diagonali
  for (int k = 0; k < 3; k++) {
    if (pMacierz[index[k]][index[k]] == 0.0) {
      wiersz = elementPodstawowy(pMacierz, 3, index[k], index);
      // Zapisanie zmian w tablicy indeksow
      index[wiersz] = index[k];
      index[k] = wiersz;
    }
    // Zejscie w dol po kolumnie
    for (int i = k + 1; i < 4; i++) {
      v = pMacierz[index[i]][k];
      // Poruszanie sie w prawo po wierszu
      for (int j = k + 1; j < 4; j++) {
        // Obliczanie wartosci dla macierzy U
        pMacierz[index[i]][j] = pMacierz[index[i]][j] - pMacierz[index[k]][j] * (v / pMacierz[index[k]][k]);
        // Zapisanie macierzy L
        // wspolczynniki przez ktore mnozymy kolejne wiersze (A[i][k] / A[k][k])
        pMacierz[index[i]][k] = v / pMacierz[index[k]][k];
      }
    }
  }
}

/*
 * Aby roziwazac rownanie:
 * A * x = b
 * Korzystamy z tego ze:
 * A = L * U
 * Oraz tworzymy uklad rowanan:
 * L * y = b
 * y = U * x
 */

/**
 * Wyznaczenie y potrzebnego do rozwiazania powyzszego ukladu rownan
 * @param macierzL maczierz L (trojkatna dolna)
 * @param wektorB wektor b
 * @param index tablica indexow
 * @param n rozmiar macierzy
 */
void wyznaczY(double **macierzL, double *wektorB, int *index, int n) {
  double suma = 0.0;
  // Poruszanie sie po kolumnie, zaczyna od lewego gornego
  for (int i = 0; i <= n; i++) {
    // Poruszanie sie wierszu w prawo
    for (int j = 0; j < i; j++)
      suma += macierzL[index[i]][j] * wektorB[index[j]];

    wektorB[index[i]] = (wektorB[index[i]] - suma) / 1.0;     //Na glownej przekatnej sa 1

    suma = 0.0;
  }
}

/**
 * Wyznaczenie x potrzebnego do rozwiazania powyzszego ukladu rownan
 * @param macierzU maczierz U (trojkatna gorna)
 * @param wektorB wektor b
 * @param index tablica indexow
 * @param n rozmiar macierzy
 */
void wyznaczX(double **macierzU, double *wektorB, int *index, int n) {
  double suma = 0.0;
  // Zaczyna od prawego dolnego rogu
  for (int i = n; i >= 0; i--) {
    for (int j = i + 1; j <= n; j++)
      suma += macierzU[index[i]][j] * wektorB[index[j]];

    wektorB[index[i]] = (wektorB[index[i]] - suma) / (macierzU[index[i]][i]);

    suma = 0.0;
  }
}

int main() {
  double **matrix = stworzMacierz(N);
  double b[4] = {35.0, 104.0, -366.0, -354.0};
  int index[4] = {0, 1, 2, 3};

  uzupelnijMacierz(matrix, N);

  std::cout << "Dana w zadaniu macierz A:\n";
  wyswietlMacierz(matrix, N, index);

  gauss(matrix, index);
  std::cout << "\n\nMacierz po dekompozycji LU:\n";
  wyswietlMacierz(matrix, N, index);

  std::cout << "\n\nRoziwiazanie ukladu rownan Ax = b:\n";
  wyznaczY(matrix, b, index, N - 1);
  std::cout << "\n\nWektor y:\n";
  wyswietlWektor(b, N);
  std::cout << "\n\n";

  wyznaczX(matrix, b, index, N - 1);
  std::cout << "\n\nWektor x (Ux = y):\n";
  wyswietlWektor(b, N);

  usunMacierz(matrix, N);
  return 0;
}