//
// Created by Bartosz Fiejdasz on 29/03/2022.
//

#include <iostream>
#include <cmath>

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
  for (int i = n - 1; i >= 0; --i) {
    delete[] pMacierz[i];

    delete pMacierz;
  }
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

  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      pMacierz[i][j] = pMacierz[i][j];
}

/**
 * Wyswietla podana macierz
 * @param pMacierz wskaznik do macierzy
 * @param n rozmiar macierzy
 * @param index indeks w macierzy
 */
void wyswietlMacierz(double **pMacierz, int n, int *index) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j)
      std::cout << "  " << pMacierz[index[i]][j] << "\t";
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
  int wiersz;
  for (int i = j; i < n; i++) {
    if (std::fabs(pMacierz[index[i]][j]) < std::fabs(pMacierz[index[i + 1]][j]))
      wiersz = index[i + 1];
    else
      wiersz = index[i];
  }

  return wiersz;
}

void gauss(double **pMacierz, int *index) {
  int wiersz;
  double v;
  for (int k = 0; k < 3; k++) {
    if (pMacierz[index[k]][index[k]] == 0.0) {
      wiersz = elementPodstawowy(pMacierz, index[k], 3, index);
      // Zapisanie zmian w tablicy indeksow
      index[wiersz] = index[k];
      index[k] = wiersz;
    }
    for (int i = k + 1; i < 4; i++) {
      v = pMacierz[index[i]][k];
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
  for (int i = 0; i <= n; i++) {
    for (int j = 0; j < i; j++)
      suma += macierzL[index[i]][j] * wektorB[index[j]];

    wektorB[index[i]] = (wektorB[index[i]] - suma) / 1.0;     //Na glownej przekatnej sa 1

    suma = 0.0;
  }
}

void wyznaczU(double **macierzU, double *wektorB, int *index, int n) {
  double suma = 0.0;
  for (int i = n; i >= 0; i--) {
    for (int j = i + 1; j <= n; j++)
      suma += macierzU[index[i]][j] * wektorB[index[j]];

    wektorB[index[i]] = (wektorB[index[i]] - suma) / (macierzU[index[i]][i]);

    suma = 0.0;
  }
}

int main() {

}