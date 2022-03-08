//
// Created by Bartek on 06.03.2022.
//

#include <iostream>
#include <math.h>
#include <fstream>

double silnia(int i) {
  if (i > 1)
	return i * silnia(i - 1);
  else
	return 1;
}

double funkcjaFX(double x) {
  return (1 - exp(-x)) / x;
}

// funkcje 1 - exp(-x) rozwijamy w szereg Taylora aby do obliczenia wyniku nie korzystac z odejmowania
double funkcjaUlepszona(double x) {
  double result=1;
  int i = 1;
  double wyraz=1;
  while(fabs(wyraz)+1.0>1.0) {
	wyraz = wyraz * (-x / ((double)i+1.));
	result += wyraz;
	i++;
  }
  return result;
}

int main() {
  std::ifstream plikFunkcja, plikX, plikLog;
  std::ofstream daneWynikowe, danePoprawione;
  double wartoscPlik = 0, wartoscObliczona = 0, wartoscLog10 = 0, argument = 0, blad = 0, bladLog = 0;

  /**
   * Plik zawieraja wartosci pobrane ze strony
   * sourceFunction.txt zawiera zbior wartosci zadanej funkcji
   * sourceX.txt zawiera zbior argumentow
   * sourceLog.txt zawiera zbior wartosci funkcji log_10
   */
  plikFunkcja.open("sourceFunction.txt");
  plikX.open("sourceX.txt");
  plikLog.open("sourceLog.txt");
  daneWynikowe.open("daneWynikowe.txt");
  danePoprawione.open("danePoprawione.txt");

  std::cout << "|      Argument Funkcji      |      Blad Wzgledny      |      log10(blad)      |\n";

  while (!plikFunkcja.eof()) {
	// Pobranie wartosci z plikow
	plikFunkcja >> wartoscPlik;
	plikX >> argument;
	plikLog >> wartoscLog10;

	// Obliczenie wartosci funkcji oraz oblicznie bledu wzglednego
	wartoscObliczona = funkcjaFX(argument);
	blad = abs((wartoscObliczona - wartoscPlik) / wartoscPlik);

	// Obliczenie zaleznosci logarytmu dziesietnego z bezgledna wartoscia bledu wzglednego
	bladLog = log10(blad);

	// Zapisanie wyniku do pliku
	daneWynikowe << bladLog << " " << wartoscLog10 << "\n";

	//std::cout << "|         " << argument << "        |            " << blad << "            |            " <<
	//		  bladLog << "          |\n";


	std::cout << wartoscObliczona << " | ";


	//std::cout << "******************************************************\n";

	if(!wartoscObliczona == 0.00138038)
		std::cout << "";
	// Obliczenie wartosci funkcji oraz oblicznie bledu wzglednego
	//wartoscObliczona = funkcjaUlepszona(argument);
	wartoscObliczona = funkcjaUlepszona(argument);
	blad = abs((wartoscObliczona - wartoscPlik) / wartoscPlik);

	// Obliczenie zaleznosci logarytmu dziesietnego z bezgledna wartoscia bledu wzglednego
	bladLog = log10(blad);

	// Zapisanie wyniku do pliku
	danePoprawione << bladLog << " " << wartoscLog10 << "\n";

	//std::cout << "|         " << argument << "        |            " << blad << "            |            " <<
	//		  bladLog << "          |\n";

	std::cout << wartoscObliczona << "\n";
  }

  plikFunkcja.close();
  plikX.close();
  plikLog.close();
  daneWynikowe.close();
  danePoprawione.close();

  return 0;
}