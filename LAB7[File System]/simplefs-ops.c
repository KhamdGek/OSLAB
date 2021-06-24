#include "simplefs-ops.h"
extern struct filehandle_t file_handle_array[MAX_OPEN_FILES]; 

int simplefs_create(char *filename){
    /*
	    Create file with name `filename` from disk
	*/
	struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
	for(int i = 0; i < NUM_INODES; ++i)
	{
		simplefs_readInode(i, inode);
		if(strcmp(inode->name, filename) == 0)
			return -1;
	}
	int inode_num = simplefs_allocInode();
	if(inode_num == -1)
		return -1;
    
    memcpy(inode->name, filename, sizeof(filename));


    inode->status = INODE_IN_USE;
    inode->file_size = 0;
    for(int i=0; i<MAX_FILE_SIZE; i++)	// MAX_FILE_SIZE = 4
    	inode->direct_blocks[i] = -1;
    simplefs_writeInode(inode_num, inode);
    free(inode);

    return inode_num;

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
		return;			
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


int simplefs_seek(int file_handle, int nseek){
    /*
	   increase `file_handle` offset by `nseek`
	*/
		if(file_handle_array[file_handle].offset + nseek < 0 || file_handle_array[file_handle].offset + nseek >= MAX_FILE_SIZE*BLOCKSIZE)
		return -1;

	file_handle_array[file_handle].offset += nseek;
	  
	return 0;

}
