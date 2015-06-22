#include <stdio.h>
#include <math.h>
#define WINDOW_SIZE 256
void dct_ii(int N, const double x[], double X[]) {
  for (int k = 0; k < N; ++k) {
    double sum = 0.;
    double s = (k == 0) ? sqrt(.5) : 1.;
    for (int n = 0; n < N; ++n) {
      sum += s * x[n] * cos(M_PI * (n + .5) * k / N);
    }
    X[k] = sum * sqrt(2. / N);
  }
}

void idct(int N, const double X[], double x[]) {
  for (int n = 0; n < N; ++n) {
    double sum = 0.;
    for (int k = 0; k < N; ++k) {
      double s = (k == 0) ? sqrt(.5) : 1.;
      sum += s * X[k] * cos(M_PI * (n + .5) * k / N);
    }
    x[n] = sum * sqrt(2. / N);
  }
}


int main()
{

 int iarr[WINDOW_SIZE*2];
 double darr[WINDOW_SIZE*2];
 double doutl[WINDOW_SIZE];
 double doutr[WINDOW_SIZE];
 int iout[WINDOW_SIZE*4];
 unsigned char buf[WINDOW_SIZE*4];
 while (!feof(stdin)){
  //Load the window
  for (int i = 0; i<WINDOW_SIZE;++i)
  {
    signed char c = getchar();
    iarr[i]=c;
    c = getchar();
    iarr[i+WINDOW_SIZE]=c;
  }
  //Convert the ints to doubles
  for (int i = 0; i < WINDOW_SIZE*2;++i)
  {
    darr[i] = ((double)iarr[i])/128.;
  }
  //Get the DCT
  idct(WINDOW_SIZE,darr,doutl);
  double* darrr = darr + WINDOW_SIZE;
  idct(WINDOW_SIZE,darrr,doutr);
  //Convert the DCT to ints
  for (int i = 0; i < WINDOW_SIZE; ++i)
  {
    iout[i*2] = doutl[i]*32768;
    iout[i*2+1] = doutr[i]*32768;
  }  
  //Now print the array
  for (int i = 0; i < WINDOW_SIZE*2;++i)
  {
    putchar(iout[i]&0xFF);
    putchar((iout[i]&0xFF00)/256);
  }
 }
}
