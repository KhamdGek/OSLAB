# Simple File System 
In this lab, we will implement some basic file operations in a simple filesystem that runs over an
emulated disk. You are provided files simplefs-disk.h and simplefs-disk.c that emulate the disk, and provide functions to access the disk. Using these emulation functions, you will implement some basic filesystem operations like opening, reading, and writing files, by modifying the files simplefs-ops.h and simplefs-ops.c.
## Expected funtions
- `int simplefs create(char *filename)` creates a file with a specified name in the filesystem. A file creation should succeed only if a file with the same name does not already exist in the filesystem, and if the system has space for an additional file to be created. During file creation, this function must allocate a free inode for the file and initialize it suitably on disk. This function returns the inode number of the newly created file on success, and -1 on a failure.
- `void simplefs delete(char *filename)` deletes a file with the specified name from the filesystem (if it exists), and frees up the resources of the file such as its data blocks and its inode. You may assume that the file is closed before deleting it.
- `int simplefs open(char *filename)` opens an existing file for reading and writing. This operation succeeds only if the file with the same name was previously created. This operation allocates an unused file handle from the global file handle array, initializes it suitably, and returns the index of the newly allocated file handle. This function returns -1 if the file open failed for any reason.
Please note that an array of file handles has already been defined for you; you must populate an entry in this array when opening a file. All subsequent read and write operations on the file will have to obtain the inode number of the file using this returned file handle array index.
- `void simplefs close(int file handle)` closes an open file and frees up its file handle. This operation will not delete the file from disk.
- `int simplefs read(int file handle, char *buf, int nbytes)` reads the specified number of bytes from the current offset in an open file into the given buffer. The requested number of bytes can span multiple blocks on disk. This function returns 0 on success and -1 on failure. Note that we do not wish to support partial reads, i.e., your implementation must read all the requested nbytes number of bytes into the buffer, or none at all. If reading the requested number of bytes causes you to go beyond the end of the file, then this operation should simply not read anything and must return a failure. Note that you must handle the cases where reads are not aligned at the block boundary, and can start or end in the middle of a block. Note that reading a file will NOT result in updating the offset value in the file handle.
- `int simplefs write(int file handle, char *buf, int nbytes)` writes the specified number of bytes from the current offset in an open file to disk, from the given buffer. The requested number of bytes can span multiple blocks on disk. This function returns 0 on success and -1 on failure. Note that we do not wish to support partial writes, i.e., your implementation must write all the requested nbytes number of bytes into the file, or none at all. If the requested number of bytes cannot be written (e.g., writing the requested number of bytes causes you to go beyond the maximum file size limit, or there are no empty disk blocks available to complete the write) then this operation should simply not write anything and must return a failure. Further, on such failed writes, any data blocks that have been allocated before encountering the failure must be freed up and returned back to the file system. That is, a failed write must leave the filesystem in the same state that it started with. Note that you must handle the cases where writes are not aligned at the block boundary, and can start or end in the middle of a block. Note that writing a file will NOT result in updating the offset value in the file handle.
- `int simplefs seek(int file handle, int nseek)` increments the file offset in the file handle by nseek bytes. As with Linux filesystems, the offset indicates the next byte that can be read/written from the file. Note that the offset should decrease if nseek is negative. The seek operation should not move the offset to beyond the current file size boundaries. This function returns 0 on a success and -1 on a failure. Note that this is the only function that can change the current offset value from which the next read or write operation can happen.
## Output for `testcase0.c`
![](images/0.jpg)

## Output for `testcase1.c`
![](images/1.jpg)

## Output for `testcase2.c`
![](images/2.jpg)

## Output for `testcase3.c`
![](images/3.jpg)

## Output for `testcase4.c`
![](images/4.jpg)

## Output for `testcase5.c`
![](images/5.jpg)

## Output for `testcase6.c`
![](images/6.jpg)

## Output for `testcase7.c`
![](images/71.jpg)
*Page-1*

![](images/72.jpg)
*Page-2*

![](images/72.jpg)
*Page-3*
