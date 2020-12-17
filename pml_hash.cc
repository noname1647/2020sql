/**
 * PMLHash::PMLHash 
 * 
 * @param  {char*} file_path : the file path of data file
 * if the data file exist, open it and recover the hash
 * if the data file does not exist, create it and initial the hash
 */
PMLHash::PMLHash(const char* file_path) {
//    FILE *fr, *fw;  
//    int len;  
//    char *pbuf, *p;  
//    char dockey[TABLE_SIZE];  
  
//    if(file_path == NULL || *file_path == '\0')  
//        return;  
//
//    fr = fopen(file_path, "rb");  //读取文件file_path  
//    if(fr == NULL)  
//    {  
//        printf("open read or write file error!\n");  
//		
//        return;  
//    }  
    int is_pmem;
    size_t mapped_len;

    if ((start_addr = pmem_map_file(file_path, FILE_SIZE, PMEM_FILE_CREATE, 0666, &mapped_len, &is_pmem)) == NULL)
    {
        perror("pmem_map_file");
        exit(1);
    }
    else
    {
        overflow_addr = (void *)((uint64_t)start_addr + FILE_SIZE / 2);
        table_arr = (pm_table *)((uint64_t)start_addr + sizeof(metadata));
        meta = (metadata *)start_addr;
    }
}  																																																																																																																
							 
/**
 * PMLHash::~PMLHash 
 * 
 * unmap and close the data file
 */
PMLHash::~PMLHash() {
    pmem_unmap(start_addr, FILE_SIZE);
}
/**
 * PMLHash 
 * 
 * split the hash table indexed by the meta->next
 * update the metadata
 */
void PMLHash::split() {
    // fill the split table

    // fill the new table

    // update the next of metadata

}
/**
 * PMLHash 
 * 
 * @param  {uint64_t} key     : key
 * @param  {size_t} hash_size : the N in hash func: idx = hash % N
 * @return {uint64_t}         : index of hash table array
 * 
 * need to hash the key with proper hash function first
 * then calculate the index by N module
 */
uint64_t PMLHash::hashFunc(const uint64_t &key, const size_t &hash_size) {
	return (unsigned long)key%hash_size;
}

/**
 * PMLHash 
 * 
 * @param  {uint64_t} offset : the file address offset of the overflow hash table
 *                             to the start of the whole file
 * @return {pm_table*}       : the virtual address of new overflow hash table
 */
pm_table* PMLHash::newOverflowTable(uint64_t &offset) {

}

/**
 * PMLHash 
 * 
 * @param  {uint64_t} key   : inserted key
 * @param  {uint64_t} value : inserted value
 * @return {int}            : success: 0. fail: -1
 * 
 * insert the new kv pair in the hash
 * 
 * always insert the entry in the first empty slot
 * 
 * if the hash table is full then split is triggered
 */
int PMLHash::insert(const uint64_t &key, const uint64_t &value) {
	uint64_t hashVal = hashFunc(key, size);
	while(kv_arr[hashVal]!=NULL && kv_arr[hashVal].key != -1){
		hashVal++;
		hashVal=hashVal % size;
	}
	kv_arr[hashVal] = value;

}

/**
 * PMLHash 
 * 
 * @param  {uint64_t} key   : the searched key
 * @param  {uint64_t} value : return value if found
 * @return {int}            : 0 found, -1 not found
 * 
 * search the target entry and return the value
 */

int PMLHash::search(const uint64_t &key, uint64_t &value) {

	uint64_t hashVal = hashFunc(key, size);
	while(kv_arr[hashVal]!=NULL){
		if(kv_arr[hashVal].key==key)
			{return 0;}
		hashVal++;
		hashVal = hashVal % size;
	}
 return -1;
}

/**
 * PMLHash 
 * 
 * @param  {uint64_t} key : target key
 * @return {int}          : success: 0. fail: -1
 * 
 * remove the target entry, move entries after forward
 * if the overflow table is empty, remove it from hash
 */
int PMLHash::remove(const uint64_t &key) {
	uint64_t hashVal = hashFunc(key, size);
	while(kv_arr[hashVal]!=NULL){
		if(kv_arr[hashVal].key==key)
			kv_arr[hashVal] = 0;
			return 0;
			}
			hashVal++;
			hashVal = hashVal % size;
	}
	return -1;
}

/**
 * PMLHash 
 * 
 * @param  {uint64_t} key   : target key
 * @param  {uint64_t} value : new value
 * @return {int}            : success: 0. fail: -1
 * 
 * update an existing entry
 */
int PMLHash::update(const uint64_t &key, const uint64_t &value) {
	uint64_t hashVal = hashFunc(key, size);
	while(kv_arr[hashVal]!=NULL){
		if(kv_arr[hashVal].key==key)
			kv_arr[hashVal] = value;
			return 0;
			}
			hashVal++;
			hashVal = hashVal % size;
	}
	return -1;
}
