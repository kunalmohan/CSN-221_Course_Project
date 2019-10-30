#include "cacheGeneric.h"
 
void  Mapfct2(int, CACHEINT*, int);


int H(int Y, int shift, int Mask){
  int y1, yn, result;

  y1 = Y >> (shift - 1);
  yn = Y & 0x01;

  result = ((Y * 2) + (yn ^ y1)) & Mask;
  return(result);

}


int G(int Y, int shift, int Mask)  {
  int yk, y1, yn, result;
  
  y1 = (Y & 0x02) >> 1;
  yn = (y1 ^ Y) & 1;
  result =  (Y >> 1) + (yn << (shift-1));
  
  return(result);

}

void  Mapfct2( int addr, CACHEINT *cache, int  memtype )
{
  
  int    A1, A2;
  int    mask,Log;
  

  mask = (*cache).Size_Bank - 1;
  Log = (*cache).Logbank;

  A1 = addr & mask;
  A2 = (addr >> Log) & mask;

  AdBank[0] = (H(A1, Log, mask) ^ G(A2, Log, mask) ^ A2);
  AdBank[1] = (H(A1, Log, mask) ^ G(A2, Log, mask) ^ A1) + (*cache).Size_Bank;
  AdBank[2] = (G(A1, Log, mask) ^ H(A2, Log, mask) ^ A2) + (2 * (*cache).Size_Bank);
  AdBank[3] = (G(A1, Log, mask) ^ H(A2, Log, mask) ^ A1) + (3 * (*cache).Size_Bank);

}





















































































