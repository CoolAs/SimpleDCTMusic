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
 unsigned char buf[WINDOW_SIZE*4];
 int iarr[WINDOW_SIZE*2];
 double darr[WINDOW_SIZE*2];
 double doutl[WINDOW_SIZE];
 double doutr[WINDOW_SIZE];
 int iout[WINDOW_SIZE*2];
 while (!feof(stdin)){
  //Load the window
  for (int i = 0; i<WINDOW_SIZE*4;++i)
  {
    buf[i]=getchar();
  }
  //Convert the window to ints
  for (int i = 0; i <WINDOW_SIZE;++i)
  {
    //Get the 16bits
    int l = buf[i*4];
    l += (buf[i*4+1]&0x7F)*256;
    //Fix the sign bit -- TODO, account for two's complement, instead of ones complement
    if (buf[i*4+1]&0x80) l = (0x7FFF - l)*-1;
    //Store
    iarr[i] = l;
  
    int r;
    r = 0;
    r |= buf[i*4+2];
    r |= (buf[i*4+3]&0x7F)*256;
    if (buf[i*4+3]&0x80)
    {
      r = (0x7FFF - r)*-1;
    }
    iarr[i+WINDOW_SIZE] = r;
  }
  //Convert the ints to doubles
  for (int i = 0; i < WINDOW_SIZE*2;++i)
  {
    darr[i] = ((double)iarr[i])/32768.;
  }
  //Get the DCT
  dct_ii(WINDOW_SIZE,darr,doutl);
  double* darrr = darr + WINDOW_SIZE;
  dct_ii(WINDOW_SIZE,darrr,doutr);
  //Convert the DCT to ints
  for (int i = 0; i < WINDOW_SIZE; ++i)
  {
    iout[i*2] = doutl[i]*8;
    iout[i*2+1] = doutr[i]*8;
  }  
  //Now print the array
  for (int i = 0; i < WINDOW_SIZE*2;++i)
  {
    putchar(iout[i]&0xFF);
  }
 }
}
