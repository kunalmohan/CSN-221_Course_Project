#include "cacheGeneric.h"

void SIM(void);
void Simul(CACHE *, int, int );
void memory_access( int, int );
void enforceinclusion( CACHE*);

void SIM(void){
  int i,K;
  
  for (i = 0; i < NbSimul; i++) {
    
    for (K = 0; K < Index; K++)
      Simul(&TWOSKEWLRU[i], ADDR[K], MEMTYPE[K]);
  
  }
  Index = 0;
}

void Simul(CACHE *cache, int add, int memtype){
  
  CACHEINT *	cacheint;


  if (memtype == mem_inst) {
    cacheint = &(cache->cacheinst);
  } else {
    cacheint = &(cache->cachedata);
  }

  (*cacheint).ACCESS++;
  if ((*cacheint).Is_In_Cache(add, cacheint, memtype)) {
    cacheint->Hit++;
  } else {
    cacheint->Miss++;
    ((*cacheint).Allocate(add, cacheint, memtype));
#ifdef SIML2
    cacheint = &(cache->cacheL2);
    (*cacheint).ACCESS++;
    add = add >> (LOGLINE - 4);
    if ((*cacheint).Is_In_Cache(add, cacheint, memtype)) {
      cacheint->Hit++;
    } else {
      cacheint->Miss++;
      ((*cacheint).Allocate(add, cacheint, memtype));
      enforceinclusion(cache);
    }
#endif
  }
}



#ifdef SIML2
void enforceinclusion(CACHE* cache)
{
  int             i;
  EXCLUDE = EXCLUDE << LOGLINE - 4;
  for (i = 0; i < (1 << LOGLINE - 4) - 1; i++) {
    if ((*cache).cacheinst.Cache[EXCLUDE & 511] == EXCLUDE)
      (*cache).cacheinst.Cache[EXCLUDE & 511] = -1;
    if ((*cache).cachedata.Cache[EXCLUDE & 511] == EXCLUDE)
      (*cache).cachedata.Cache[EXCLUDE & 511] = -1;
    EXCLUDE++;
  }
}
#endif

void memory_access(int memtype, int addr){

#ifdef SIML2
  addr = addr >> 4;
#else
  addr = addr >> LOGLINE;
#endif
  nbref++;
  if (nbref & 0xffff == 0){
    printf(" (%d) ", nbref);
  }
  if (memtype == mem_inst){
    nbinst++;
    if (lastinst == addr) return;
    lastinst = addr;
  } else {
    if (lastdata == addr) return;
    lastdata = addr;
  }
  
  MEMTYPE[Index] = memtype;
  ADDR[Index] = addr;
  Index++;
  if (Index != 16384)
    return;
  SIM();
}