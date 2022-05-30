//
// Created by Bartosz Fiejdasz on 28/05/2022.
//

#include <iostream>
#include <math.h>
#include <fstream>
#include "calerf.cpp"
using namespace std;

const double T_MAX = 1., T_MIN = 0, b = 0.1, D = 1;
const double A_MIN = -10, A_MAX = 10;
const double I_MAX = 50;
const double TOLX = 1e-15, TOLY = 1e-15;
const double LAMBDA_BEZ = 0.4, LAMBDA_POS = 1.;

double analitycznie(double x, double t) { return (1/2) * exp((D*t)/(b * b) - x/b) * calerf::ERFC_L(2 * D * t / b - x) / 2 * sqrt(D) * t;}

void warunki_poczatkowe(double **u, double h, int N){
  double x = A_MIN;
  for(int i = 0; i <= N; i++){
    if(x < 0)
      u[i][0] = 0;
    else
      u[i][0] = exp(-x/b);
    x += h;
  }
}

void kmb(double **u, int N, int M){
  for (int k = 0; k < M; k++){
    u[0][k+1] = 0;
    u[N][k+1] = 0;
    for(int i = 1; i < N; i++){
      u[i][k+1] = u[i][k] + LAMBDA_BEZ * (u[i - 1][k] - 2 * u[i][k] + u[i + 1][k]);
    }
  }
}

void ThomasMatrix(double *d, double *l, double *u, int n){
  for(int i = 1; i < n; i++)
    d[i] = d[i] - l[i] * u[i - 1] / d[i - 1];
}

void Thomas(double *d, double *l, double *u, double *b, double **x, int k, int n){
  for (int i = 1; i < n; ++i) {
    b[i] = b[i] - l[i] * b[i - 1] / d[i - 1];
  }
  x[n - 1][k] = b[n - 1] / d[n - 1];
  for (int i = n - 2; i >= 0; i--) {
    x[i][k] = (b[i] - u[i] * x[i + 1][k]) / d[i];
  }
}


int wyborElemPodst(double **macierz, int n, int N, int* index){
  int ret;
  for(int i = n; i<N-1; i++){
    if(fabs(macierz[index[i]][n]) < fabs(macierz[index[i+1]][n])){
      ret = index[i+1];
    }
    else{
      ret = index[i];
    }
  }
  return ret;
}


void gauss(double **macierz, int *index, int N){
  int row;
  double v;
  for(int k = 0; k < N-1; k++){
    if(macierz[index[k]][k] == 0.0){
      row = wyborElemPodst(macierz,index[k], N, index);
      index[row] = index[k];
      index[k] = row;
    }
    for(int i = k+1; i<N;i++){
      v=macierz[index[i]][k];
      for (int j = k+1; j < N; ++j) {
        macierz[index[i]][j] = macierz[index[i]][j] - macierz[index[k]][j] * (v/macierz[index[k]][k]);
        macierz[index[i]][k] = v/macierz[index[k]][k];
      }
    }
  }
}


void macierzL(double **L, double *b, int *index, int N){
  double sum = 0.0;
  for (int i = 0; i <= N-1; i++) {
    for (int j = 0; j < i; j++) {
      sum = sum+L[index[i]][j]*b[index[j]];
    }
    b[index[i]] = (b[index[i]]-sum)/1.0;
    sum=0.0;
  }
}


void macierzU(double **U, double *b, int *index, int N) {
  double sum = 0.0;
  for (int i = 3; i >= 0; i--) {
    for (int j = i + 1; j <= N-1; j++) {
      sum = sum + U[index[i]][j] * b[index[j]];
    }
    b[index[i]] = (b[index[i]] - sum) / U[index[i]][i];
    sum = 0.0;
  }
}

void crank_nicolson_thomas(double **y, int N, int M){
  double *d = new double[N + 1];
  double* l = new double[N + 1];
  double* u = new double[N + 1];
  double* b = new double[N + 1];

  l[0] = 0.;
  d[0] = 1.;
  u[0] = 0.;
  for (int i = 1; i < N; i++) {
    l[i] = LAMBDA_POS / 2.;
    d[i] = -(1. + LAMBDA_POS);
    u[i] = LAMBDA_POS / 2.;
  }
  l[N] = 0.;
  d[N] = 1.;
  u[N] = 0.;
  ThomasMatrix(d, l, u, N);

  for (int i = 0; i < M; ++i) {
    b[0] = 1.;
    for (int j = 1; j < N; ++j) {
      b[j] = -(y[j-1][i] * LAMBDA_POS / 2. + y[j][i] * (1. - LAMBDA_POS) + y[j+1][i] * LAMBDA_POS / 2.);
    }
    b[N] = 0.;
    Thomas(d, l, u, b, y, i+1, N+1);
  }
  delete [] d, l, y, b;
}

void crank_nicolson_LU(double **y, int N, int M){
  int *index = new int[N];
  double *b = new double[N+1];
  double **A = new double*[N+1];

  for (int i = 0; i < N+1; i++) {
    A[i] = new double[N+1];
  }


  for(int i = 0; i < N; i++){
    index[i] = i;
  }

  for(int i = 0; i <= N; i++){
    for(int j = 0; j <= N; j++){
      if(i == j)
        A[i][j] = -(1. + LAMBDA_POS);
      else if (j == i - 1 || j == i + 1)
        A[i][j] = LAMBDA_POS / 2.;
      else
        A[i][j] = 0;
    }
  }
  A[0][0] = 0.;
  A[0][1] = 0.;
  A[N][N - 1] = 0.;
  A[N][N] = 0.;

  gauss(A, index, N);

  for (int i = 0; i < M; ++i) {
    b[0] = 1.;
    for (int j = 1; j < N; ++j) {
      b[j] = -(y[j-1][i] * LAMBDA_POS / 2. + y[j][i] * (1. - LAMBDA_POS) + y[j+1][i] * LAMBDA_POS / 2.);
    }
    b[N] = 0.;
    macierzL(A, b, index, N);
    macierzU(A, b, index, N);

    for(int j = 1; j < N; ++j){
      y[i][j] = b[j];
    }
  }

  for (int i = 0; i < N + 1; i++) {
    delete[] A[i];
  }
  delete[] A, b;
}

double h_error(double **y, int m, double t, double h, int n){
  double xi, e_max, tmp;
  xi = A_MIN;
  e_max = -1.;
  for(int i = 0; i <= n; i++){
    tmp = abs(y[i][m] - analitycznie(xi, t));
    if (tmp > e_max)
      e_max = tmp;
    xi += h;
  }
  return e_max;
}

void save_t_error(double** y, double dt, double h, int n, int m, const char* name){
  ofstream file;
  file.open(name, ofstream::out);
  double x, t, e_max, tmp;
  t = 0.;
  for(int i = 0; i <= m; i++){
    file << t << " " << h_error(y, i, t, h, n) << endl;
    t += dt;
  }
  file.close();
}

void save_sol(double** y, double h, int j, int N, const char* name) {
  ofstream file;
  file.open(name, ofstream::out);
  double x = A_MIN;
  for (int i = 0; i < N; i++) {
    file << x << " " << y[i][j] << endl;
    x += h;
  }
  file.close();
}

int main() {
  double dt, h, x, **y;
  int N, M, i, n_step = 50;
  ofstream errors_h_KMB, errors_h_MCN_LU, errors_h_MCN_T, results_analytical;

  errors_h_KMB.open("errors_h_KMB.txt", ofstream::out);
  errors_h_MCN_T.open("errors_h_MCN.txt", ofstream::out);
  errors_h_MCN_LU.open("errors_h_MCN_LU.txt", ofstream::out);
  results_analytical.open("results_analytical.txt", ofstream::out);

  if(errors_h_KMB && errors_h_MCN_T && errors_h_MCN_LU && results_analytical){
    for(N = 50; N < 500; N+= n_step){
      h = abs(A_MIN - A_MAX) / N;
      //KMB
      dt = h * h * LAMBDA_BEZ / D;
      M = int(T_MAX / dt + 0.5);
      y = new double* [N+1];
      for(i = 0; i <= N; i++)
        y[i] = new double[M + 1];

      cout << "Obliczenia dla h: " << h << " dt:" << dt << " N: " << N
           << " M:" << M << " t_max: " << int(M * dt + 0.5) << " b:" << int(N * h + 0.5) << endl;
      warunki_poczatkowe(y, h, N);
      kmb(y, N, M);
      errors_h_KMB << log10(h) << " " << log10(h_error(y, M, T_MAX, h, N)) << endl;

      if (N == 250){
        x = A_MIN;
        for(i = 0; i <= N; i++){
          results_analytical << x << " " << analitycznie(x, 2) << endl;
          x += h;
        }

        save_sol(y, h, M, N, "results_KMB.txt");

        save_t_error(y, dt, h, N, M, "errors_t_KMB.txt");
      }
      //CN Thomas
      dt = h * h * LAMBDA_POS / D;
      M = int(T_MAX / dt + 0.5);
      y = new double* [N+1];
      for(i = 0; i <= N; i++){
        y[i] = new double [M + 1];
      }
      cout << "Obliczenia dla h: " << h << " dt:" << dt << " N: " << N
           << " M:" << M << " t_max: " << int(M * dt + 0.5) << " b:" << int(N * h + 0.5) << endl;
      warunki_poczatkowe(y, h, N);
      crank_nicolson_thomas(y, N, M);
      errors_h_MCN_T << log10(h) << " " << log10(h_error(y, M, T_MAX, h, N)) << endl;

      if (N == 250){
        save_sol(y, h, M, N, "results_MCN_T.txt");
        save_t_error(y, dt, h, N, M, "errors_t_MCN_T.txt");
      }

      //CN LU
      dt = h * h * LAMBDA_POS / D;
      M = int(T_MAX / dt + 0.5);
      y = new double* [N+1];
      for(i = 0; i <= N; i++){
        y[i] = new double [M + 1];
      }
      cout << "Obliczenia dla h: " << h << " dt:" << dt << " N: " << N
           << " M:" << M << " t_max: " << int(M * dt + 0.5) << " b:" << int(N * h + 0.5) << endl;
      warunki_poczatkowe(y, h, N);
      crank_nicolson_LU(y, N, M);
      errors_h_MCN_LU << log10(h) << " " << log10(h_error(y, M, T_MAX, h, N)) << endl;

      if(N == 250){
        save_sol(y, h, M, N, "results_MCN_LU.txt");
        save_t_error(y, dt, h, N, M, "errors_t_MCN_LU.txt");
      }
      errors_h_KMB.close();
      errors_h_MCN_T.close();
      errors_h_MCN_LU.close();
      results_analytical.close();
      for (i = 0; i <= N-n_step; i++) {
        delete[] y[i];
      }
      delete[] y;
    }
  }

  return 0;
}
