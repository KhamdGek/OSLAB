#include "simplefs-ops.h"
extern struct filehandle_t file_handle_array[MAX_OPEN_FILES]; 

int simplefs_create(char *filename){
    /*
	    Create file with name `filename` from disk
	*/
	struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));/* Create a pointer of inode*/
	for(int i = 0; i < NUM_INODES; ++i)                    /*check that the file with the ivn name is already exist or not if exist then it return -1 
	                                                        which tells that the file with the given name is already exist*/
	{
		simplefs_readInode(i, inode);
		if(strcmp(inode->name, filename) == 0)
			return -1;
	}
	int inode_number = simplefs_allocInode();                /*It allocate the inode to the new file. If all the existing inodes are filled then it return -1 
	                                                       that tells there is not free inode at that time*/ 
	if(inode_number == -1)
		return -1;
    
    memcpy(inode->name, filename, sizeof(filename));
                                                               /*Now we if we not find the file with the same name and also we find free inode then we first
							       change the status of inode that it is in use by assigning it 1 and then we give the name to that new file
							       assign blocks to that new file and alsi give the file size and return that node at the end*/

    inode->status = INODE_IN_USE;
    inode->file_size = 0;
    for(int j=0; j<MAX_FILE_SIZE; j++)	// MAX_FILE_SIZE = 4
    	inode->direct_blocks[j] = -1;
    simplefs_writeInode(inode_number, inode);
    free(inode);

    return inode_number;

}


int simplefs_open(char *filename){
    /*
	    open file with name `filename`
	*/
	int inode_number = -1;
    struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));/* Create a pointer of inode*/
                                                                 /*Now we want to open the file so we create inode pointer which then check in this loop that 
								 either that file exist in file system or not if we find the given name file then we break loop and take the
								 inode number*/
	for(int k = 0; k < NUM_INODES; ++k)
	{
		simplefs_readInode(k, inode);
		if(strcmp(inode->name, filename) == 0)
		{
			inode_number = k;
			break;
		}
	}

	if(inode_number == -1)                                  /*if we not find the given name file then inode num is -1 mean that file is not exist*/
	{	
		return -1;		
	}

	int fd = -1;
	for(int l=0; l<MAX_OPEN_FILES; l++)                 /*we have 20 files so we begin loop from 1 to 20 then if we find inode number then we assign offset of this file
	                                                      equal to the file size and inode number to them and break the loop so that to open the file*/
	{
        if(file_handle_array[l].inode_number == -1)
        {	
        	file_handle_array[l].offset = inode->file_size;
        	file_handle_array[l].inode_number = inode_number;
        	fd = l;
        	break;
        }
    }
}

void simplefs_close(int file_handle){
    /*
	    close file pointed by `file_handle`
	*/
	/*to close the file we take that file and then put its offset 0 and inode number equal to -1*/
	file_handle_array[file_handle].offset = 0;
	file_handle_array[file_handle].inode_number = -1;


}

void simplefs_delete(char *filename){
    /*
	    delete file with name `filename` from disk
	*/
	int inode_number = -1;
	struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));   /* Create a pointer of inode*/
	for(int a = 0; a < NUM_INODES; ++a)     /*Now we want to open the file so we create inode pointer which then check in this loop that 
	                                         either that file exist in file system or not if we find the given name file then we break loop and take the inode number*/
	{
		simplefs_readInode(a, inode);
		if(strcmp(inode->name, filename) == 0)
		{
			inode_number = a;
			break;
		}
	}

	if(inode_number == -1)/*if file is not exist then the inode num is -1 so it return -1 that the file is not exist*/
	{	
		return -1;			
	}

	for(int s=0; s<MAX_FILE_SIZE; s++)        /*we run lopp over all files and find whose block size is occupied and not equal to -1 then by using
	                                            the simplefs_freeDataBlock() unction we free the block of that file and finally we delete the file*/
	{
		if(inode->direct_blocks[s] != -1)
		{
			simplefs_freeDataBlock(inode->direct_blocks[s]);
		}
	}

	simplefs_freeInode(inode_number);

}

