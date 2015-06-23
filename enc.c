#include <stdio.h>
#include <math.h>
#define WINDOW_SIZE 32

int lc;
int num;
void RLE_putchar(char c)
{
  if (num == -1)
  {
   lc = c;
   num = 1;
  }
  else if (lc == c)
  {
    if (num < 255) ++num;
    else
    {
      putchar(num);
      putchar(lc);
      num = 1;
    }   
  }
  else{
    putchar(num);
    putchar(lc);
    lc = c;
    num = 1;
  }

}

int main()
{
 num = -1;
 unsigned char buf[WINDOW_SIZE*4];
 int iarr[WINDOW_SIZE*2];
 float darr[WINDOW_SIZE*2];
 float doutl[WINDOW_SIZE];
 float doutr[WINDOW_SIZE];
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
    if (buf[i*4+1]&0x80) l = (0x8000 - l)*-1;
    //Store
    iarr[i] = l;
  
    int r;
    r = 0;
    r |= buf[i*4+2];
    r |= (buf[i*4+3]&0x7F)*256;
    if (buf[i*4+3]&0x80)
    {
      r = (0x8000 - r)*-1;
    }
    iarr[i+WINDOW_SIZE] = r;
  }
  //Convert the ints to floats
  for (int i = 0; i < WINDOW_SIZE*2;++i)
  {
    darr[i] = ((float)iarr[i])/32768.;
  }
  //Get the DCT
  dct_ii(WINDOW_SIZE,darr,doutl);
  float* darrr = darr + WINDOW_SIZE;
  dct_ii(WINDOW_SIZE,darrr,doutr);
  //Convert the DCT to ints
  for (int i = 0; i < WINDOW_SIZE; ++i)
  {
    iout[i*2] = doutl[i]*16;
    iout[i*2+1] = doutr[i]*16;
  }  
  //Now print the array
  for (int i = 0; i < WINDOW_SIZE*2;++i)
  {
    putchar(iout[i]&0xFF);
  }
 }
}
