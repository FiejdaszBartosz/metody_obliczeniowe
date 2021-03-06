//
// Created by Bartek on 06.03.2022.
//

#include <iostream>
#include <math.h>
#include <fstream>

// podstawowa rozszerzona
#define podstawowa

// funkcja z zadania
double funkcjaFX(double x) {
  return (1 - exp(-x)) / x;
}

// funkcje 1 - exp(-x) rozwijamy w szereg Taylora
// zapobiega odejmowaniu podobnych licz co by powodowalo utrate cyfr znaczacych
double funkcjaUlepszona(double x) {
  double wynik = 1;
  double znak = -1;
  double krok = 1;

  // ograniczenie i wynika z tego ze obliczamy za jego pomoca silnie
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
  daneWynikowe.open("daneWynikowe.txt");
  danePoprawione.open("danePoprawione.txt");
  plikDane.open("dane.txt");

  std::cout << "|      Argument Funkcji      |     Blad Wzgledny     |    log10(blad)    |"
               "   ~Blad Wzgledny    |   ~log10(blad)    |\n";

  while (!plikDane.eof()) {
	// Pobranie wartosci z plikow
    plikDane >> wartoscLog10;
    plikDane >> argument;
    plikDane >> wartoscPlik;

#ifdef podstawowa
	// Obliczenie wartosci funkcji oraz oblicznie bledu wzglednego
	wartoscObliczona = funkcjaFX(argument);
	blad = abs((wartoscObliczona - wartoscPlik) / wartoscPlik);

	// Obliczenie zaleznosci logarytmu dziesietnego z bezgledna wartoscia bledu wzglednego
	bladLog = log10(blad);

	// Zapisanie wyniku do pliku
	daneWynikowe << wartoscLog10 << " " << bladLog << "\n";

    std::cout << "|          ";
    std::cout << argument << "         |         ";
    std::cout << blad << "         |         ";
    std::cout << bladLog << "         |         ";

	// Obliczenie wartosci funkcji oraz oblicznie bledu wzglednego z szeregu taylora
	// Korzystamy z rozwiniecia w szereg aby uniknac odejmowanie dwoch podbnych liczb
	wartoscObliczona = funkcjaUlepszona(argument);
	blad = abs((wartoscObliczona - wartoscPlik) / wartoscPlik);

	// Obliczenie zaleznosci logarytmu dziesietnego z bezgledna wartoscia bledu wzglednego
	bladLog = log10(blad);

	// Zapisanie wyniku do pliku
	danePoprawione << wartoscLog10 << " " << bladLog << "\n";

    std::cout << blad << "         |         ";
    std::cout << bladLog << "         |\n";
#endif

#ifdef rozszerzona
	if(wartoscLog10 < -0.5){
	  wartoscObliczona = funkcjaUlepszona(argument);
	  blad = abs((wartoscObliczona - wartoscPlik) / wartoscPlik);
	  bladLog = log10(blad);
	  danePoprawione << wartoscLog10 << " " << bladLog << "\n";
	} else {
	  wartoscObliczona = funkcjaFX(argument);
	  blad = abs((wartoscObliczona - wartoscPlik) / wartoscPlik);
	  bladLog = log10(blad);
	  danePoprawione << wartoscLog10 << " " << bladLog << "\n";
	}

#endif
  }

  daneWynikowe.close();
  danePoprawione.close();
  plikDane.close();

  return 0;
}