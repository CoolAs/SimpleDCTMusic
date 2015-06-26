#include <stdio.h>
#include <math.h>
//This can not be changed unless you change the dct functions
#define WINDOW_SIZE 32
int error(const char* string)
{
  printf("%s\n",string);
  return -1;
}
void edct(const float x[], float X[])
{
  for (int k = 0; k < 32; ++k)
  {
    float sum = 0;
    float s = (k == 0) ? sqrt(.5) : 1;
    for (int n = 0; n < 32; ++n)
    {
      sum += s* x[n] * cos(M_PI * (n + .5) * k / 32);
    }
    // This line will need to be changed to change window_size
    X[k] = sum / 4;
  }
}
void ddct(const float X[], float x[])
{
  for (int n = 0; n < 32; ++n)
  {
    float sum = 0;
    for (int k = 0; k < 32; ++k)
    {
      float s = (k == 0) ? sqrt(.5) : 1;
      sum += s * X[k] * cos(M_PI * (n + .5) * k / 32);
    }
    // This line will need to be changed to change window_size
    x[n] = sum / 4;
  }
}
int enc(int argc, char* argv[]);
int dec(int argc, char* argv[]);
int num;
char control;

int main(int argc, char* argv[])
{
  num = 0;
  if (argc != 4 && argc != 5)
  {
    printf("Usage: %s <e/d> <infile> <outfile>\n",argv[0]);
    return -1;
  }
  if (argv[1][0]=='e') return enc(argc,argv);
  else if (argv[1][0]=='d') return dec(argc,argv);
  else     printf("Usage: %s <e/d> <infile> <outfile>\n",argv[0]);
}

char Z_getchar(FILE* in)
{
  if (num == 0)
  {
    control = fgetc(in);
    num = 8;
  }
  --num;
  int mask = 1 << num;
  if (control&mask)
  {
    return fgetc(in);
  }
  else return 0;
}
int dec(int argc, char* argv[])
{
  FILE* in;
  FILE* out;
  in = fopen(argv[2],"r");
  out = fopen(argv[3],"w");
  if (in == NULL) return error("Could not open input file");
  if (out == NULL) return error("Could not open output file");
 num = 0;
 int iarr[WINDOW_SIZE*2];
 float darr[WINDOW_SIZE*2];
 float doutl[WINDOW_SIZE];
 float doutr[WINDOW_SIZE];
 int iout[WINDOW_SIZE*4];
 unsigned char buf[WINDOW_SIZE*4];
 while (!feof(in)){
  //Load the window
  for (int i = 0; i < WINDOW_SIZE;++i)
  {
    darr[i] = ((float)Z_getchar(in))/16.;
    darr[i+WINDOW_SIZE] = ((float)Z_getchar(in))/16.;
  }
  //Get the DCT
  ddct(darr,doutl);
  float* darrr = darr + WINDOW_SIZE;
  ddct(darrr,doutr);
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
    fputc(iout[i]&0xFF,out);
    fputc((iout[i]&0xFF00)/256,out);
  }
 }
 fclose(in);
 fclose(out);
}
int enc(int argc, char* argv[])
{
  FILE* in;
  FILE* out;
  in = fopen(argv[2],"r");
  out = fopen(argv[3],"w");
  if (in == NULL) return error("Could not open input file");
  if (out == NULL) return error("Could not open output file");
 unsigned char buf[WINDOW_SIZE*4];
 int iarr[WINDOW_SIZE*2];
 float darr[WINDOW_SIZE*2];
 float doutl[WINDOW_SIZE];
 float doutr[WINDOW_SIZE];
 int iout[WINDOW_SIZE*2];
 while (!feof(in)){
  //Load the window
  for (int i = 0; i<WINDOW_SIZE*4;++i)
  {
    buf[i]=fgetc(in);
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
  edct(darr,doutl);
  float* darrr = darr + WINDOW_SIZE;
  edct(darrr,doutr);
  //Convert the DCT to ints
  for (int i = 0; i < WINDOW_SIZE; ++i)
  {
    iout[i*2] = doutl[i]*16;
    iout[i*2+1] = doutr[i]*16;
  }  
  //Now print the array
  for (int i = 0; i < WINDOW_SIZE*2;i+=8)
  {
    char control = 0;
    if (iout[i]&0xFF) control|=128;
    if (iout[i+1]&0xFF) control|=64;
    if (iout[i+2]&0xFF) control|=32;
    if (iout[i+3]&0xFF) control|=16;
    if (iout[i+4]&0xFF) control|=8;
    if (iout[i+5]&0xFF) control|=4;
    if (iout[i+6]&0xFF) control|=2;
    if (iout[i+7]&0xFF) control|=1;


    fputc(control,out);
    for (int j = 0; j < 8; ++j)
    {
      if (iout[i+j]&0xFF) fputc(iout[i+j]&0xFF,out);
    }
  }
 }
 fclose(in);
 fclose(out);
}
