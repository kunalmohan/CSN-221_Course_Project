#define _main_
#include <stdio.h>
#include "cacheGeneric.h"
 
main(int argc, char** argv)
{
  int	memtype;
  int	addr;
  

  INITCACHE(argc, &argv[0]);
	
  while (scanf("%d %x", &memtype, &addr)>=0) {
    memory_access(memtype, addr);
  };
  SIM();

  DUMPRES();
  
}

 
