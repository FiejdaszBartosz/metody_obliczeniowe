//
// Created by Bartek on 06.03.2022.
//

#include <iostream>
#include <math.h>
#include <fstream>
#include <iomanip>

double funkcjaFX(double x) {
  return (1 - exp(-x)) / x;
}

// funkcje 1 - exp(-x) rozwijamy w szereg Taylora
double funkcjaUlepszona(double x) {
  double wynik = 1;
  double znak = -1;
  double krok = 1;

  for (int i = 2; i < 20; ++i){
    krok = krok * (x / i);
    wynik += znak * krok;
    znak = -znak;
  }

  return wynik;
}

int main() {
  std::ifstream plikDane;
  std::ofstream daneWynikowe, danePoprawione;
  double wartoscPlik = 0, wartoscObliczona = 0, wartoscLog10 = 0, argument = 0, blad = 0, bladLog = 0;
  int pom = 0;

  // Plik zawieraja wartosci pobrane ze strony
  danePoprawione.open("danePoprawione.txt");
  plikDane.open("dane.txt");

  std::cout << "|      Argument Funkcji      |      Blad Wzgledny      |      log10(blad)      |"
               "     ~Blad Wzgledny      |     ~log10(blad)      |\n";

  while (!plikDane.eof()) {
	// Pobranie wartosci z plikow
    plikDane >> wartoscLog10;
    plikDane >> argument;
    plikDane >> wartoscPlik;

	// Obliczenie wartosci funkcji oraz oblicznie bledu wzglednego
	wartoscObliczona = funkcjaFX(argument);
	blad = abs((wartoscObliczona - wartoscPlik) / wartoscPlik);

	// Obliczenie zaleznosci logarytmu dziesietnego z bezgledna wartoscia bledu wzglednego
	bladLog = log10(blad);

	// Zapisanie wyniku do pliku
	daneWynikowe << bladLog << " " << wartoscLog10 << "\n";

    std::cout << "|          ";
    //std::cout.width(15);
    std::cout << argument << "         |         ";
    std::cout << blad << "         |         ";
    std::cout << bladLog << "         |         ";

	// Obliczenie wartosci funkcji oraz oblicznie bledu wzglednego
	wartoscObliczona = funkcjaUlepszona(argument);
	blad = abs((wartoscObliczona - wartoscPlik) / wartoscPlik);

	// Obliczenie zaleznosci logarytmu dziesietnego z bezgledna wartoscia bledu wzglednego
	bladLog = log10(blad);

	// Zapisanie wyniku do pliku
	danePoprawione << bladLog << " " << wartoscLog10 << "\n";

    std::cout << blad << "         |         ";
    std::cout << bladLog << "         |\n";

  }

  daneWynikowe.close();
  danePoprawione.close();
  plikDane.close();

  return 0;
}