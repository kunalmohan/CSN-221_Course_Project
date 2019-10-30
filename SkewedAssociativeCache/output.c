#include "cacheGeneric.h"
 
void DUMPRES(void);
void printCACHE( FILE* , CACHE* );

 
void DUMPRES(void)
{
  FILE           *fx;
  int             i, Taille;
    
  fx = fopen(ResFile, "a");

  fprintf(fx, "#instructions : %d  || #data : %d \n", nbinst, nbref - nbinst);

    fprintf(fx, "\tnbinst: number of instructions\n");

#ifdef SIML2
    fprintf(fx, "\tL2Miss: number of miss of the second level cache\n");
    fprintf(fx,"\tthe first level of cache is fixed to 8K, 16bytes line size with a Direct Map replacement policy.");

#else
    fprintf(fx, "\tdataMiss: number of miss of the data cache\n\tinstMiss:number of miss of the instruction cache\n");

#endif

  fprintf(fx, "\n\n");
  Taille = CacheSize;
  for (i = 0; i < 3; i++) {

    fprintf(fx, "\n# %d \n", Taille);

    fprintf(fx, "strategy\tNbBank\tnbinst\t");

#ifdef SIML2
    fprintf(fx, "L2Miss\t100*L2Miss/nbinst\n\n");
#else
    fprintf(fx, "dataMiss\tinstMiss\t100*nbMiss/nbinst\t\n\n");

#endif

    fprintf(fx, "SkewLRU   \t2\t");   printCACHE(fx, &TWOSKEWLRU[i]);

    Taille = Taille * 2;
    fprintf(fx, "\n");
    
  }
  
  fclose(fx);
}

void printCACHE(FILE *fx, CACHE *cache)
{
 fprintf(fx, "%d\t",nbinst);
 
#ifdef SIML2
    fprintf(fx, "%d\t%f",(*cache).cacheL2.Miss,100. * (float) ((*cache).cacheL2.Miss) / ((float) nbinst));

#else
  fprintf(fx, "%d\t\t%d\t\t%f\t", 
	  (*cache).cachedata.Miss, 
	  (*cache).cacheinst.Miss, 
	  100.* ((float) ((*cache).cachedata.Miss + (*cache).cacheinst.Miss)) / ((float) nbinst));

#endif
    fprintf(fx, "\n");
  
}





