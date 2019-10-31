#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
using namespace std;

#define MM 64
#define cache 1
#define victim 512
#define blocksize 8

#define read 0
#define write 1
#define no_blocks_l1 128
#define no_blocks_vic 64

long binary_to_decimal(string add){
	long sum=0;
	int digit;
	int i=15;
	while(i>-1){
		digit = add[i]-48;
		sum = sum + (digit*pow(2,i));
		i--;
	}
	return sum;
}

int min(long arr[], long n){
	long j, min=10000000000, k;
	for(j=0;j<n;j++){
		if(arr[j]<min)
		k = j;
	}
	return k;
}


int main(){
	long dmcache[no_blocks_l1] = {0};
	long viccache[no_blocks_vic] = {0};
	long lruc[no_blocks_vic] = {0};
	long countr = 0, countw = 0;
	long op;
	string add;
	long read_miss_l1 = 0;
	long read_miss_vic = 0;
	long read_hit_l1 = 0;
	long read_hit_vic = 0;
	long write_miss_l1 = 0;
	long write_miss_vic = 0;
	long write_hit_l1 = 0;
	long write_hit_vic = 0;
	long queries = 0;

	#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin); 	
    freopen("output.txt", "w", stdout);
	#endif

	long no_queries;
	cin>>no_queries;

	while(no_queries--){

		cin>>op>>add;
		queries++;

		long lineno = binary_to_decimal(add.substr(6,12));
		long tag = binary_to_decimal(add.substr(0,5));
		long mod = lineno%no_blocks_l1;
		if(op == 0){
			if(dmcache[mod] == tag){
				read_hit_l1++;
			}
			else{
				read_miss_l1++;
				if(dmcache[mod] == 0){
				dmcache[mod] = tag;
			}
				else{
					int flag = 0;
					for(long i=0;i<no_blocks_vic;i++){
						if(viccache[i]==tag){
							read_hit_vic++;
							flag = 1;
							long temp = viccache[i];
							viccache[i] = dmcache[mod];
							dmcache[mod] = temp;
							countr++;
							lruc[i]=countr;
						}
					}
					if(flag == 0){
						read_miss_vic++;
						long lru_index = min(lruc, no_blocks_vic);
						viccache[lru_index] = dmcache[mod];
						dmcache[mod] = tag;	
					}
				}
			}
		}
	
		if(op == 1){
			if(dmcache[mod] == tag){
				write_hit_l1++;
			}
			else{
				write_miss_l1++;
				if(dmcache[mod] == 0){
				dmcache[mod] = tag;
			}
				else{
					int flag = 0;
					for(long i=0;i<no_blocks_vic;i++){
						if(viccache[i]==tag){
							write_hit_vic++;
							flag = 1;
							long temp = viccache[i];
							viccache[i] = dmcache[mod];
							dmcache[mod] = temp;
							countw++;
							lruc[i]=countw;
						}
					}
					if(flag == 0){
						write_miss_vic++;
						long lru_index = min(lruc, no_blocks_vic);
						viccache[lru_index] = dmcache[mod];
						dmcache[mod] = tag;	
					}
				}
			}
		}
	}

	float hit_ratio = float(write_hit_l1+read_hit_l1+read_hit_vic+write_hit_vic)/float(queries);
	float miss_ratio = 1-hit_ratio;

	cout<<"Main memory size = "<<MM<<" KB"<<endl;
	cout<<"L1 cache size = "<<cache<<" KB"<<endl;
	cout<<"Victim cache size = "<<victim<<" Bytes"<<endl;
	cout<<"Block size = "<<blocksize<<" Bytes"<<endl<<endl;
	cout<<"Number of write miss in L1 cache = "<<write_miss_l1<<endl;
	cout<<"Number of write miss in Victim cache = "<<write_miss_vic<<endl;
	cout<<"Number of write hit in L1 cache = "<<write_hit_l1<<endl;
	cout<<"Number of write hit in Victim cache = "<<write_hit_vic<<endl;
	cout<<"Number of read miss in L1 cache = "<<read_miss_l1<<endl;
	cout<<"Number of read miss in Victim cache = "<<read_miss_vic<<endl;
	cout<<"Number of read hit in L1 cache = "<<read_hit_l1<<endl;
	cout<<"Number of read hit in Victim cache = "<<read_hit_vic<<endl<<endl;
	cout<<"Miss Ratio = "<<miss_ratio<<endl;
	cout<<"Hit Ratio = "<<hit_ratio<<endl;

	return 0;
}