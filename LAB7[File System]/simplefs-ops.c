#include "simplefs-ops.h"
extern struct filehandle_t file_handle_array[MAX_OPEN_FILES]; // Array for storing opened files

int simplefs_create(char *filename){
    /*
	    Create file with name `filename` from disk
	*/
	struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
	//check if same filename exists return -1
	for(int i = 0; i < NUM_INODES; ++i)
	{
		simplefs_readInode(i, inode);
		if(strcmp(inode->name, filename) == 0)
			return -1;
	}
	
	int inode_num = simplefs_allocInode();
	if(inode_num == -1)
		return -1;


	// create this inode
    
    memcpy(inode->name, filename, sizeof(filename));


    inode->status = INODE_IN_USE;
    inode->file_size = 0;
    for(int i=0; i<MAX_FILE_SIZE; i++)	// MAX_FILE_SIZE = 4
    	inode->direct_blocks[i] = -1;
    simplefs_writeInode(inode_num, inode);
    free(inode);

    return inode_num;

}


void simplefs_delete(char *filename){
    /*
	    delete file with name `filename` from disk
	*/
	int inode_num = -1;
	struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
	//check if same filename exists return -1
	for(int i = 0; i < NUM_INODES; ++i)
	{
		simplefs_readInode(i, inode);
		if(strcmp(inode->name, filename) == 0)
		{
			inode_num = i;
			break;
		}
	}

	if(inode_num == -1)
	{	
		return;			// not exist
	}

	for(int i=0; i<MAX_FILE_SIZE; i++)
	{
		if(inode->direct_blocks[i] != -1)
		{
			simplefs_freeDataBlock(inode->direct_blocks[i]);
		}
	}

	simplefs_freeInode(inode_num);
	free(inode);

}

int simplefs_open(char *filename){
    /*
	    open file with name `filename`
	*/
	int inode_num = -1;
    struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
	//check if same filename exists return -1
	for(int i = 0; i < NUM_INODES; ++i)
	{
		simplefs_readInode(i, inode);
		if(strcmp(inode->name, filename) == 0)
		{
			inode_num = i;
			break;
		}
	}

	if(inode_num == -1)
	{	
		return -1;			// not exist
	}

	int fd = -1;
	for(int i=0; i<MAX_OPEN_FILES; i++)
	{
        if(file_handle_array[i].inode_number == -1)
        {	
        	file_handle_array[i].offset = inode->file_size;
        	file_handle_array[i].inode_number = inode_num;
        	fd = i;
        	break;
        }
    }
    free(inode);

    if(fd == -1)
    	return -1;


}

void simplefs_close(int file_handle){
    /*
	    close file pointed by `file_handle`
	*/
	file_handle_array[file_handle].offset = 0;
	file_handle_array[file_handle].inode_number = -1;


}

int simplefs_read(int file_handle, char *buf, int nbytes){
    /*
	    read `nbytes` of data into `buf` from file pointed by `file_handle` starting at current offset
	*/
	int block_id = file_handle_array[file_handle].offset/BLOCKSIZE;
	int final_block_id = (file_handle_array[file_handle].offset + nbytes-1)/BLOCKSIZE;

	if(final_block_id > 3)
		return -1;


	// now read all blocks from block_id to final_block_id to take n_bytes
	int num_blocks = final_block_id - block_id + 1;

	char *buf_copy = (char *)malloc(sizeof(char)*num_blocks*BLOCKSIZE);
	char *buff[num_blocks];
	int take[num_blocks];

	//find inode here for direct_blocks
	struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
	simplefs_readInode(file_handle_array[file_handle].inode_number, inode);


	if(inode->direct_blocks[final_block_id] == -1)	//unallocated data block
		return -1;

	int offset = file_handle_array[file_handle].offset;


	for(int i = 0; i < num_blocks; ++i)
	{
		buff[i] = (char*)malloc(64 * sizeof(char));	// 64 bytes
	}

	int sum = 0;
	for(int i = block_id; i <= final_block_id; ++i)
	{	
		simplefs_readDataBlock(inode->direct_blocks[i], buff[i-block_id]);
		
		if(i == block_id)
		{
			take[i-block_id] = offset%64;
			sum += 64-take[i-block_id];
		}

		else if (i == final_block_id)
		{			
			take[i-block_id] = nbytes - sum;
			sum += take[i-block_id];
		}

		else
		{	
			take[i-block_id] = 64;
			sum += take[i-block_id];
		}
		
	}
	//concat into buf and send
	strcpy(buf_copy, buff[0] + take[0]);		// copy see later

	for(int i = 1; i < num_blocks; ++i)
	{	
		strcat(buf_copy, buff[i]);
	}

	
	if(num_blocks > 1)
	{	
		buf_copy[strlen(buf_copy)-64+take[num_blocks-1]] = '\0';
		//buf = buf - (64-take[num_blocks-1]); 
	}



	for(int i = 0; i < num_blocks; ++i)
	{
		free(buff[i]); // = (char*)malloc(64 * sizeof(char));	// 64 bytes
	}
	free(inode);
	memcpy(buf, buf_copy, nbytes);
	free(buf_copy);
	return 0;

}


int simplefs_write(int file_handle, char *buf, int nbytes){
    /*
	    write `nbytes` of data from `buf` to file pointed by `file_handle` starting at current offset
	*/
	int block_id = file_handle_array[file_handle].offset/BLOCKSIZE;
	int final_block_id = (file_handle_array[file_handle].offset + nbytes-1)/BLOCKSIZE;

	if(final_block_id > 3)
		return -1;

	// now write all blocks from block_id to final_block_id to take n_bytes
	int num_blocks = final_block_id - block_id + 1;
	int cur_byte = 0;

	//find inode here for direct_blocks
	struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
	simplefs_readInode(file_handle_array[file_handle].inode_number, inode);
	

	int offset = file_handle_array[file_handle].offset;


	if(offset%64 != 0)		// write into middle of some block
	{
		char* temp_buff = (char*)malloc(64 * sizeof(char));
		char* temp_buff2 = (char*)malloc(64 * sizeof(char));
		simplefs_readDataBlock(inode->direct_blocks[block_id], temp_buff);
		memcpy(temp_buff2, temp_buff, 64);

		//concat first offset%64 bytes with 64-offset%64 of buf
		for(int x = offset%64; x<64 && cur_byte < nbytes; ++x)
		{
			temp_buff[x] = buf[cur_byte++];
		}

		simplefs_writeDataBlock(inode->direct_blocks[block_id], temp_buff);
		free(temp_buff);

		//now write all else
		//write from start of block
		for(int i = block_id + 1; i <= final_block_id; ++i)
		{
			char* temp_buff = (char*)malloc(64 * sizeof(char));

			for(int x = 0; x<64 && cur_byte < nbytes; ++x)
			{
				temp_buff[x] = buf[cur_byte++];
			}	

			if(inode->direct_blocks[i] == -1)
				inode->direct_blocks[i] = simplefs_allocDataBlock();

			if(inode->direct_blocks[i] == -1)
				return -1;

			if(inode->direct_blocks[i] == -1)	// if no more blocks left
			{		
				//one case left: if no more left and started in middle: erase that
				//erase the previous ones
				simplefs_writeDataBlock(inode->direct_blocks[block_id], temp_buff2);
				for(int j = block_id + 1; j < i; ++j)
				{	

					simplefs_freeDataBlock(inode->direct_blocks[i]);
					free(temp_buff);
					free(inode);
				}
				return -1;
			}

			simplefs_writeDataBlock(inode->direct_blocks[i], temp_buff);
			free(temp_buff);

		}
		inode->file_size += nbytes;
		simplefs_writeInode(file_handle_array[file_handle].inode_number, inode);
	
		free(temp_buff2);
		free(inode);

		return 0;
	}

	//write from start of block
	for(int i = block_id; i <= final_block_id; ++i)
	{
		char* temp_buff = (char*)malloc(64 * sizeof(char));

		for(int x = 0; x<64 && cur_byte < nbytes; ++x)
		{
			temp_buff[x] = buf[cur_byte++];
		}

		if(inode->direct_blocks[i] == -1)
			inode->direct_blocks[i] = simplefs_allocDataBlock();

		if(inode->direct_blocks[i] == -1)	// if no more blocks left
		{	
			//erase the previous ones
			for(int j = block_id; j < i; ++j)
			{
				simplefs_freeDataBlock(inode->direct_blocks[i]);
				free(temp_buff);
				free(inode);
			}
			return -1;
		}

		simplefs_writeDataBlock(inode->direct_blocks[i], temp_buff);
		free(temp_buff);

	}
	inode->file_size += nbytes;

	simplefs_writeInode(file_handle_array[file_handle].inode_number, inode);

	free(inode);
	return 0;
}


int simplefs_seek(int file_handle, int nseek){
    /*
	   increase `file_handle` offset by `nseek`
	*/
		if(file_handle_array[file_handle].offset + nseek < 0 || file_handle_array[file_handle].offset + nseek >= MAX_FILE_SIZE*BLOCKSIZE)
		return -1;

	file_handle_array[file_handle].offset += nseek;
	  
	return 0;

}