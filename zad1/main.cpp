#include <iostream>

int main() {
  int licznikBitowDouble = 0;

  // pomDouble dowolna wartosc wieksza od 1 aby sie wykonal while
  double precyzjaDouble = 1.0, pomDouble = 1.1;

  while(pomDouble > 1.0) {
	/**
	 * dokonujemy dzielenia przez dwa aby 1 bitowa przemieszczala sie po mantysie
	 * 1.000...
	 * 0.100...
	 * 0.010...
	 */
	precyzjaDouble *= 0.5;
	// kiedy mantysa dojdzie do postacie 0.000...0 dojdzie do przerwania pętli while
	pomDouble = 1.0 + precyzjaDouble;
	// nie zwiekszamy licznika dla ostatniego przejscia petli
	if(pomDouble != 1.0)
		licznikBitowDouble++;
  }
  // iloczyn maszynowy = precyzjaDouble * 2
  // precyzja jest bliska zero wiec jest zdenormalizowana
  // wiec po wyjsciu z petli trzeba to pomnozyc razy 2
  std::cout << "--------------Double--------------\n";
  std::cout << "Iloczyn maszynowy wynosi: " << 2 * precyzjaDouble << "\n" << "Liczba bitow mantysy: " << licznikBitowDouble;





  int licznikBitowFloat = 0;
  /**
   * pomDouble dowolna wartosc wieksza od 1 aby sie wykonal while
   */
  float precyzjaFloat = 1.0f, pomFloat = 1.1f;

  while(pomFloat > 1.0f) {
	/**
	 * dokonujemy dzielenia przez dwa aby 1 bitowa przemieszczala sie po mantysie
	 * 1.000...
	 * 0.100...
	 * 0.010...
	 */
	precyzjaFloat *= 0.5f;
	// kiedy mantysa dojdzie do postacie 0.000...0 dojdzie do przerwania pętli while
	pomFloat = 1.0f + precyzjaFloat;
	// nie zwiekszamy licznika dla ostatniego przejscia petli
	if(pomFloat != 1.0f)
	  licznikBitowFloat++;
  }
  // iloczyn maszynowy = precyzjaDouble * 2
  std::cout << "\n--------------FLOAT--------------\n";
  std::cout << "Iloczyn maszynowy wynosi: " << 2.0f * precyzjaFloat << "\n" << "Liczba bitow mantysy: " << licznikBitowFloat;
}
