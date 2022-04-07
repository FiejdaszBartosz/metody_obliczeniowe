//
// Created by Bartosz Fiejdasz on 29/03/2022.
//

#include <iostream>

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
void wyswietlMacierz(double **pMacierz, int n, int* index) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j)
      std::cout << "  " << pMacierz[index[i]][j] << "\t";
    std::cout << "\n";
  }
}

int main() {

}