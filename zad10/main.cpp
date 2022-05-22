//
// Created by Bartosz Fiejdasz on 21/05/2022.
//

#include <iostream>
#include <fstream>
#include <cmath>

double funkcjaBezposrednia(double dt, double tk, double yk) {
  return yk + dt * (-((10.0 * tk * tk + 20.0) / (tk * tk + 1.0)) * (yk - 1.0));
}
double funkcjaAnalityczna(double tk) { return 1 - exp(-10.0 * (tk + atan(tk))); }
double funkcjaPosrednia(double dt, double tk, double y_prev) {
  double y, temp;
  temp = (10.0 * (tk + dt) * (tk + dt) + 20.0) / ((tk + dt) * (tk + dt) + 1.0);
  y = (y_prev + dt * temp) / (1 + dt * temp);
  return y;
}
double funkcjaPosredniaBlad(double dt) {
  double y, diff, tk = 0, maxDiff = 0.0, y_poprzednie = 0.0;
  while (tk < 1.0) {
    y = funkcjaPosrednia(dt, tk, y_poprzednie);
    diff = fabs(funkcjaAnalityczna(tk) - y_poprzednie);
    y_poprzednie = y;
    if (diff > maxDiff) maxDiff = diff;
    tk += dt;
  }
  return maxDiff;
}

double funkcjaBezposredniaBlad(double dt) {
  double y, maxDiff = 0.0, diff, tk = 0, y_poprzednie = 0.0;
  while (tk < 1.0) {
    y = funkcjaBezposrednia(dt, tk, y_poprzednie);
    diff = fabs(funkcjaAnalityczna(tk) - y_poprzednie);
    y_poprzednie = y;
    if (diff > maxDiff)
      maxDiff = diff;
    tk += dt;
  }
  return maxDiff;
}
double funkcjaTrapezow(double dt, double tk, double y_poprzedni) {
  double y, tempf, tempf1, i;
  while (i < tk) {
    tempf = ((10.0 * i * i + 20.0) / (i * i + 1.0)); //część f(tk,yk)
    tempf1 = (10.0 * (i + dt) * (i + dt) + 20.0) / ((i + dt) * (i + dt) + 1.0);
    y = ((-dt / 2.0) * (tempf * (y_poprzedni - 1.0) - tempf1) + y_poprzedni) / (1.0
        + (dt / 2.0) * tempf1);
    i += dt;
  }
  return y;
}
double funkcjaTrapezowBlad(double dt) {
  double diff, tk = 0, y, maxDiff = 0.0, y_poprzedni = 0.0;
  while (tk < 1.0) {
    y = funkcjaTrapezow(dt, tk, y_poprzedni);
    diff = fabs(funkcjaAnalityczna(tk) - y_poprzedni);
    y_poprzedni = y;
    if (diff > maxDiff)
      maxDiff = diff;
    tk += dt;
  }
  return maxDiff;
}

int main() {
  std::fstream diffBezposrednia, diffPosrednia, diffTrapezow, resultsBezposredniaSTB, resultsPosrednia, resultsTrapezow,
      resultsAnalitycznie, resultsBezposredniaNST; //tworzenie plików
  diffBezposrednia.open("diffBezposrednia.txt", std::fstream::out); // otwieramy do zapisu
  diffPosrednia.open("diffPosrednia.txt", std::fstream::out);
  diffTrapezow.open("diffTrapezow.txt", std::fstream::out);
  resultsBezposredniaSTB.open("resultsBezposredniaSTB.txt", std::fstream::out);
  resultsPosrednia.open("resultsPosrednia.txt", std::fstream::out);
  resultsTrapezow.open("resultsTrapezow.txt", std::fstream::out);
  resultsAnalitycznie.open("resultsAnalitycznie.txt", std::fstream::out);
  resultsBezposredniaNST.open("resultsBezposredniaNST.txt", std::fstream::out);
  resultsBezposredniaSTB << std::scientific; //ustawienie precyzji
  resultsPosrednia << std::scientific;
  resultsTrapezow << std::scientific;
  resultsAnalitycznie << std::scientific;
  resultsBezposredniaNST << std::scientific;
  diffBezposrednia << std::scientific;
  diffPosrednia << std::scientific;
  diffTrapezow << std::scientific;
  double dt = 0.005, y_nastepne, yn = 0, tk = 0, blad, y_poprzednie;
//analityczna
  while (tk < 5.0) {
    y_nastepne = funkcjaAnalityczna(tk);
    resultsAnalitycznie << tk << " " << y_nastepne << "\n";
    tk += dt;
  }
  tk = 0;
//bezpośrednia - stabilna
  while (tk < 1.0) {
    y_nastepne = funkcjaBezposrednia(dt, tk, yn);
    yn = y_nastepne;
    resultsBezposredniaSTB << tk << " " << y_nastepne << "\n";
    tk += dt;
  }
  dt = 0.2, yn = 0, tk = 0;
//bezpośrednia - niestabilna
  while (tk < 5.0) {
    y_nastepne = funkcjaBezposrednia(dt, tk, yn);
    resultsBezposredniaNST << tk << " " << y_nastepne << "\n";
    yn = y_nastepne;
    tk += dt;
  }
  dt = 0.005, tk = 0;
  y_poprzednie = 0.0;
//pośrednia
  while (tk < 1.0) {
    y_nastepne = funkcjaPosrednia(dt, tk, y_poprzednie);
    y_poprzednie = y_nastepne;
    resultsPosrednia << tk << " " << y_nastepne << "\n";
    tk += dt;
  }
  tk = 0;
//trapezów
  y_poprzednie = 0.0;
  while (tk < 1.0) {
    y_nastepne = funkcjaTrapezow(dt, tk, y_poprzednie);
    y_poprzednie = y_nastepne;
    resultsTrapezow << tk << " " << y_nastepne << "\n";
    tk += dt;
  }
// liczymy błędy maksymalne
  dt = 0.1;
  while (dt > 1e-14) {
    blad = log10(funkcjaBezposredniaBlad(dt));
    diffBezposrednia << log10(dt) << " " << blad << "\n";
    blad = log10(funkcjaPosredniaBlad(dt));
    diffPosrednia << log10(dt) << " " << blad << "\n";
    blad = log10(funkcjaTrapezowBlad(dt));
    diffTrapezow << log10(dt) << " " << blad << "\n";
    dt = dt / 1.12;
  }
  resultsBezposredniaSTB.close(); //zamykamy pliki
  resultsPosrednia.close();
  resultsAnalitycznie.close();
  resultsBezposredniaNST.close();
  resultsTrapezow.close();
  diffBezposrednia.close();
  diffPosrednia.close();
  diffTrapezow.close();
}