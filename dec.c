#include <stdio.h>
#include <math.h>
#define WINDOW_SIZE 64

int num;
char control;
char Z_getchar()
{
  if (num == 0)
  {
    control = getchar();
    num = 8;
  }
  --num;
  int mask = 1 << num;
  if (control&mask)
  {
    return getchar();
  }
  else return 0;
}
int main()
{
 num = 0;
 int iarr[WINDOW_SIZE*2];
 float darr[WINDOW_SIZE*2];
 float doutl[WINDOW_SIZE];
 float doutr[WINDOW_SIZE];
 int iout[WINDOW_SIZE*4];
 unsigned char buf[WINDOW_SIZE*4];
 while (!feof(stdin)){
  //Load the window
  for (int i = 0; i<WINDOW_SIZE;++i)
  {
    signed char c = Z_getchar();
    iarr[i]=c;
    c = Z_getchar();
    iarr[i+WINDOW_SIZE]=c;
  }
  //Convert the ints to floats
  for (int i = 0; i < WINDOW_SIZE*2;++i)
  {
    darr[i] = ((float)iarr[i])/16.;
  }
  //Get the DCT
  idct(WINDOW_SIZE,darr,doutl);
  float* darrr = darr + WINDOW_SIZE;
  idct(WINDOW_SIZE,darrr,doutr);
  //Convert the DCT to ints
  for (int i = 0; i < WINDOW_SIZE; ++i)
  {
    if (doutl[i]<-1) doutl[i]=-1;
    else if (doutl[i]>0.99999) doutl[i]=0.99999;
    if (doutr[i]<-1) doutr[i]=-1;
    else if (doutr[i]>0.99999) doutr[i]=0.99999;
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
