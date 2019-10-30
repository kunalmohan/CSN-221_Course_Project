#ifndef lib_file

#define lib_file

#define  NbSimul 3 

#ifdef _WIN32
#define random rand
#endif

#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
  
  int           Hit;
  int           Miss;
  int           Size;    
  int           Logbank; 
  int           Number_of_Bank; 
  int           Size_Bank;
  int	          DATE;    
  int           ACCESS;  
  
  unsigned int	*Cache; 
  int	*Cache_DATE; 
  int	*Cache_Prio; 
  
  void (*Allocate) ();
  int (*Is_In_Cache) ();
     
}CACHEINT;
  


typedef struct {
  
  CACHEINT	cacheL2;
  CACHEINT	cachedata;
  CACHEINT	cacheinst; 
  
}CACHE;


#define FreeAddr -1
#define mem_inst 2 
#define mem_write 1
#define mem_read 0


#ifdef _main_

int	CacheSize;

int	LOGLINE;

void (*Mapfct) ();

char *ResFile;

unsigned int	ADDR[16384];

unsigned int	MEMTYPE[16384];
 
int	Index = 0;

int	nbref = 0;

int	nbinst = 0;

int	lastinst; 
int	lastdata; 
int *	AdBank;

int	DEUP[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768 };

int    	SCRAMBLE[32768][16]; 
 
#ifdef SIML2
int    	EXCLUDE = 0;
#endif

TWOSKEWLRU[NbSimul];

#else

extern	int	*AdBank;  
extern	int	nbref;
extern	int	nbinst;
extern	int	lastinst;
extern	int	lastdata;
extern	int	CacheSize;
extern	int	LOGLINE;
extern	char	*ResFile;
extern	unsigned int	MEMTYPE[32768];
extern	unsigned int	ADDR[32768];
extern	int	Index;
extern	int	DEUP[16];
extern	int	SCRAMBLE[32768][16];
   
#ifdef SIML2
extern int	EXCLUDE;
#endif

extern	CACHE	TWOSKEWLRU[NbSimul]; 

#endif
#endif