#include <bits/stdc++.h>
using namespace std;

int main(){

    freopen("inputfile.txt", "r", stdin); 	
    freopen("outputfile.txt", "w", stdout);

	long long int main_memory_size;
	long long int l1_cache_size;
	long long int victim_cache_size;
	long long int block_size;
	long long int queries;
	long long int number_of_hits_l1 = 0;
	long long int number_of_hits_victim = 0;
	long long int number_of_misses_l1 = 0;
	long long int number_of_misses_victim = 0;
	long long int lines_in_main_memory, lines_in_l1_cache, cache_index, lines_in_victim_cache, block_offset, tag;

	//Memory Size
	cin>>main_memory_size;
	cout<<main_memory_size<<endl;

	//Address Size	
	long long int physical_address=(long long int)log2(main_memory_size); 

	//Size Of L1 Cache
	cin>>l1_cache_size;
	cout<<l1_cache_size<<endl;

	//Size Of Victim Cache
	cin>>victim_cache_size;
	cout<<victim_cache_size<<endl;

	//Block Size
	cin>>block_size;
	cout<<block_size<<endl;

	lines_in_main_memory=(main_memory_size/block_size);
	long long int main_memory[lines_in_main_memory][block_size];
	lines_in_l1_cache=(l1_cache_size/block_size);
	cache_index=(long long int)log2(lines_in_l1_cache);
	lines_in_victim_cache=(victim_cache_size/block_size);
	block_offset=(long long int)log2(block_size);
	tag=physical_address-cache_index-block_offset;


	for(long long int i=0;i<lines_in_main_memory;i++){
		for(long long int j=0;j<block_size;j++){
			main_memory[i][j]=i*block_size+j;
		}
	} //Sequential Assiging Main Memory

	
	long long int l1_cache[lines_in_l1_cache][block_size];				
	long long int tag_array_of_l1_cache[lines_in_l1_cache];			
	long long int valid_array_l1[lines_in_l1_cache];
	memset(valid_array_l1,0,sizeof(valid_array_l1));		

	
	long long int victim_cache[lines_in_victim_cache][block_size];		
	long long int tag_array_of_victim_cache[lines_in_victim_cache];		
	long long int valid_array_victim[lines_in_victim_cache];
	memset(valid_array_victim,0,sizeof(valid_array_victim));		

	// LRU Counter
	long long int LRU_in_victim_cache[lines_in_victim_cache];
	for(long long int i=0;i<lines_in_victim_cache;i++){
		LRU_in_victim_cache[i]=i;
	}

	// addresses to search
	cin>>queries;
	cout<<queries<<endl;

	while(queries--){
		string query;
		cin>>query;

		long long int query_offset=0;
		long long int query_index=0;
		long long int query_tag_l1=0;
		long long int query_tag_victim=0;
		long long int counter=1;
		// calculating offset/word value
		for(long long int i=physical_address-1;i>=tag+cache_index;i--){
			query_offset+=((long long int)query[i]-(long long int)'0')*counter;
			counter*=2;
		}
		counter=1;

		// calculating index/line value
		for(long long int i=tag+cache_index-1;i>=tag;i--){
			query_index+=((long long int)query[i]-(long long int)'0')*counter;
			counter*=2;
		}
		counter=1;
		
		// calculating tag value of L1 cache
		for(long long int i=tag-1;i>=0;i--){
			query_tag_l1+=((long long int)query[i]-(long long int)'0')*counter;
			counter*=2;
		}
		counter=1;

		//Operations
		if(valid_array_l1[query_index]==1){ // true if  line of l1 cache contains some data 

			if(tag_array_of_l1_cache[query_index]==query_tag_l1){ // true if line of l1 cache contains required address

				number_of_hits_l1++;
				cout<<query<<" : ";cout<<l1_cache[query_index][query_offset]<<endl;
			}
			else{  //if line of l1 cache don't contain required data
			
				long long int counter2=0;
				number_of_misses_l1++;

				// going inside victim cache
				for(long long int i=0;i<lines_in_victim_cache&&counter2==0;i++){ //searching for every line in victim cache
				
					if(valid_array_victim[i]==1){
						// if line of victim contains some data and that line contains required address
							counter2++;
						number_of_hits_victim++;
						
						// transferring found block to l1 cache and replacing that with of L1 cache 

							//swap valid bit
							swap(valid_array_l1[query_index],valid_array_victim[i]);
							
							// udating tag array of l1 and victim
							long long int temp=tag_array_of_l1_cache[query_index]*pow(2,query_index)+query_index;
							tag_array_of_l1_cache[query_index]=query_tag_l1;
							tag_array_of_victim_cache[i]=temp;
							
							// swap address value
							for(long long int i1=0;i1<block_size;i1++){
								swap(l1_cache[query_index][i1],victim_cache[i][i1]);
							}

							LRU_in_victim_cache[i]=0;
								
							cout<<query<<" : ";cout<<l1_cache[query_index][query_offset]<<endl;
						}
				}
				}

			}
		}
		else{
			// if there is no data present in line of L1 cache
			long long int counter2=0;
				number_of_misses_l1++;

				//going inside victim cache
				for(long long int i=0;i<lines_in_victim_cache&&counter2==0;i++){
			
					if(valid_array_victim[i]==1 && tag_array_of_victim_cache[i]==query_tag_victim){	
						// if require address is found in victim cache
						counter2++;
						number_of_hits_victim++;
						
						
						// swaping block of L1 cache with victim cache
							swap(valid_array_l1[query_index],valid_array_victim[i]);
							
							tag_array_of_l1_cache[query_index]=query_tag_l1;
							tag_array_of_victim_cache[i]=0;
							for(long long int i1=0;i1<block_size;i1++){
								swap(l1_cache[query_index][i1],victim_cache[i][i1]);
							}

							LRU_in_victim_cache[i]=0;
								
							cout<<query<<" : ";cout<<l1_cache[query_index][query_offset]<<endl;
						}
				}
							if(counter2==0){
								// if required address is not found in victim cache
					number_of_misses_victim++;

					// finding suitable line in victim cache to fetch block of ram to victim
					for(long long int i=0;i<lines_in_victim_cache&&counter2==0;i++){
						if(LRU_in_victim_cache[i]==lines_in_victim_cache-1){	// found suitable line in victim cache
							counter2++;

							// copying data from RAM to Victim Cache
							tag_array_of_victim_cache[i]=query_tag_victim;
								for(long long int i1=0;i1<block_size;i1++){
								victim_cache[i][i1]=main_memory[query_tag_victim][i1];
							}
							valid_array_victim[i]=1;

							LRU_in_victim_cache[i]=0;

						}
					}

					// swaping block of L1 and victim cache
					for(long long int i=0;i<lines_in_victim_cache;i++){
					
					if(valid_array_victim[i]==1 && tag_array_of_victim_cache[i]==query_tag_victim){
							
							swap(valid_array_l1[query_index],valid_array_victim[i]);
							
							tag_array_of_l1_cache[query_index]=query_tag_l1;
							tag_array_of_victim_cache[i]=0;
							for(long long int i1=0;i1<block_size;i1++){
								swap(l1_cache[query_index][i1],victim_cache[i][i1]);
							}
								
							cout<<query<<" : ";cout<<l1_cache[query_index][query_offset]<<endl;
						}
					}


				}

		}
	}
 
	cout<<number_of_hits_l1<<endl;
	cout<<number_of_misses_l1<<endl;
	cout<<number_of_hits_victim<<endl;
	cout<<number_of_misses_victim<<endl;
	return 0;
}