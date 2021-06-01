#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const int VM_SIZE=256;
const int PAGE_SIZE=256;
const int TLB_SIZE=16;
const int MM_SIZE=256;

int main(int argc, char* argv[])
{
	FILE *fptr;
	fptr=fopen(argv[1],"r");
	if(!fptr)
	{
		printf("Error in opening file");
		return 0;
	}
	char *value=NULL;
	size_t len=0;
	ssize_t read; 
	long long pg_number,offset,page_table,totalhits=0,fault=0,pages=0;
	int qpos=0;						
	int phy_address=0,frame,log_address;
	int tlb[TLB_SIZE][2];
	int pagetable[PAGE_SIZE];

    /*filling the block of memory allocted with -1 if there is no val*/
    int sizecal=TLB_SIZE*2*sizeof(tlb[0][0]);
	memset(tlb,-1,sizecal);
    // int *ptr=(pagetable*) calloc(sizeof(pagetable));
    // while(ptr)
    // {
    //     ptr==-1;
    // }

    sizecal=sizeof(pagetable);
	memset(pagetable,-1,sizecal);
	int mask=255,i,hit;
    /*the addresses are looked if page is present for that address then it will not have value=-1*/
	while((read=getline(&value,&len,fptr))!=-1)
	{
		pages++;     
		// printf(getline(&value,&len,fptr));
		//get page number and offset from logical address
		pg_number=atoi(value);
        /*bitshifting and masking to extract page number and offset from the file being read*/
		pg_number=pg_number>>8;
		pg_number=pg_number & mask;
		offset=atoi(value);
		offset=offset & mask;
		log_address=atoi(value);
		printf("%lld %lld\n",pg_number,offset);
        frame=0,phy_address=0;	
		hit=0;			//1 if found in TLB
		//CHECK IN TLB
		for(i=0;i<TLB_SIZE;i++)
		{
			if(tlb[i][0]==pg_number)
			{
				hit=1;
				totalhits++;
				frame=tlb[i][1];
				break;
			}
		}
		//if present in tlb
		if(hit)
			printf("TLB HIT\n");		
		//search in pagetable
		else
		{
			int f=0;
			for(i=0;i<PAGE_SIZE;i++)
			{
				if(pagetable[i]==pg_number)
				{
					frame=i;
					fault++;
					break;
				}
				if(pagetable[i]==-1)
				{
					f=1;
					break;
				}
			}
			if(f)
			{
				pagetable[i]=pg_number;
				frame=i;
			}
			//replace in tlb using fifo
			tlb[qpos][0]=pg_number;
			tlb[qpos][1]=i;
			qpos++;
			qpos=qpos%15;		
		}
		if(log_address<10000)
		printf("VIRTUAL ADDRESS = %d \t\t\t",log_address);
		else
		printf("VIRTUAL ADDRESS = %d \t\t",log_address);
		
		phy_address=frame*PAGE_SIZE + offset;
		printf("PHYSICAL ADDRESS = %d\n",phy_address);
	}
	double hitrate=(double)totalhits/pages*100;
	double faultrate=(double)fault/pages*100;
	printf("\nThe Hit rate in tlb table is= %.2f %c", hitrate,'%');
	printf("\nThe Miss rate in tlb table is= %.2f %c",(100-hitrate),'%');
	printf("\nThe Hit rate in page table is= %.2f %c", faultrate,'%');
	printf("\nThe Miss rate in page table is= %.2f %c\n", (100-faultrate),'%');
}