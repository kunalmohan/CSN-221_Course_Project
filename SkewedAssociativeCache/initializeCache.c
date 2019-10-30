#include "cacheGeneric.h"


void INITCACHE(int, char**);
void initCACHE(CACHE*, int, int, void (*) () , int (*) ());
void INITCACHEINT(CACHEINT*, int, int, void (*) (), int (*) ());
char* allocation( int );

extern void (*Mapfct) ();
extern void Mapfct1(int, CACHEINT*, int);
extern void Mapfct2(int, CACHEINT*, int);
extern void Mapfct3(int, CACHEINT*, int);

extern void (*Allocate) ();
extern int (*Is_In_Cache) ();

extern void AllocateSKEWLRU ();
extern int Is_In_CacheSKEWLRU ();



void INITCACHE(int argc, char** argv){
  int            i;
  int		 NbLines;	
  int            LINE;		
  int            Map;

  if(argc != 5){
    printf("Usage: NAME NbMapfct CacheSize LogLine ResFile < SimFile\n");
    printf("\tNAME: CACHESKEW or CACHESKEWL2\n\tNbMapfct: Number of the mapping function\n\tCacheSize: Size in bytes of the cache\n\tLogLine: Log base 2 of line size\n\tResFile: Result File\n\tSimFile: Simulation File\n"); 
    exit(0);
  };

  Map = atoi(argv[1]);
  CacheSize = atoi(argv[2]);
  LOGLINE = atoi(argv[3]);	
  ResFile = argv[4];

  switch (Map) {

  case 1: Mapfct = Mapfct1; break;
  case 2: Mapfct = Mapfct2; break;
  case 3: Mapfct = Mapfct3; break;
  default: printf("Error: Unknown mapping fonction\n"); exit(0);
  
  }

  LINE = 1;
  for (i = 0; i < LOGLINE; i++){
    LINE = 2 * LINE;
  }
  NbLines = CacheSize / LINE;

  initMappingfunction();
  AdBank = (int*)malloc(sizeof(int)*4);

  for (i = 0; i < NbSimul; i++) {

    initCACHE(&TWOSKEWLRU[i], NbLines, 2, AllocateSKEWLRU, Is_In_CacheSKEWLRU);

    
    NbLines = NbLines * 2;
  }

}

void initCACHE(CACHE *cache, int NbLines, int NbBank, void (*Allo) (), int (*Is_In) ()){
#ifdef SIML2
  INITCACHEINT(&(*cache).cacheL2, NbLines, NbBank, Allo, Is_In);
#else
  INITCACHEINT(&(*cache).cacheinst, NbLines, NbBank, Allo, Is_In);
  INITCACHEINT(&(*cache).cachedata, NbLines, NbBank, Allo, Is_In);

#endif
}


void INITCACHEINT(CACHEINT *cache, int NbLines, int NbBank, void (*Allo) (), int (*Is_In) ())
{
  int             i;
  int             t;
  
  (*cache).Size = NbLines;
  (*cache).Number_of_Bank = NbBank;
  (*cache).Size_Bank = NbLines / NbBank;

  (*cache).Logbank = 0;
  t = 1;
  while (t < (*cache).Size_Bank) {
    (*cache).Logbank++;
    t = t * 2;
  }
  
  (*cache).Cache = (unsigned int *) allocation(NbLines + 27);
  (*cache).Cache_DATE = (int *) allocation(NbLines + 39);
  (*cache).Cache_Prio = (int *) allocation(2 * NbLines + 67);
  
  for (i = 0; i < NbLines; i++) {
    (*cache).Cache[i] = FreeAddr;
    (*cache).Cache_DATE[i] = -1;
    (*cache).Cache_Prio[i] = 0;
  }
  (*cache).Allocate = Allo;
  (*cache).Is_In_Cache = Is_In;
}

char* allocation(int size)
{
  char           *pt;
  pt = malloc(sizeof(int) * size);
  if (!pt) {
    fprintf(stderr, "Not enough Memory\n");
    exit(1);
  }
  return (pt);
}
    






