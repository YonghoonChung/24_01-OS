#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) ((x) >= (y) ? (x) : (y))

#define TRUE 1
#define FALSE 0

unsigned pagenum_bits = 0;
unsigned offset_bits = 0;
unsigned process_size = 0;
unsigned *frames = NULL;

unsigned page_size = 0;
unsigned no_entry = 0;
unsigned offset_mask = 0;

int LoadPageTable(const char *filename);
unsigned MakeMask(unsigned offset_bits);
unsigned logical2physical(unsigned logical);

int main(int argc, char *argv[])
{
	if(argc < 2){
		printf("Usage: %s <pagetable_file>\n", argv[0]);
		return 0;
	}

	char *pagefile = argv[1];
	printf("page file: %s\n", pagefile);

	LoadPageTable(pagefile);

	// display logical addresses and the corresponding physical addresses
	int step = MAX(1, process_size / 64);
	for(unsigned logical = 0; logical < process_size; logical += step){
		unsigned physical = logical2physical(logical);
		printf("logical %4d (0x%04x) ==> physical %4d (0x%04x)\n", logical, logical, physical, physical);
	}

	if(frames){
		free(frames);
		frames = NULL;
	}

	return 0;
}

int LoadPageTable(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Failed to open file %s\n", filename);	
		return FALSE;
	}

	int ret = fscanf(fp, "%u %u %u", &pagenum_bits, &offset_bits, &process_size);
	if(ret < 3){
		printf("Failed to read data in line %d\n", __LINE__);
		exit(-1);
	}
	
	// TO DO: compute page_size and no_entry (the number of pages in the process)
	page_size = 1 << offset_bits;
	no_entry = (process_size + page_size - 1)/page_size;



	// TO DO: create offset_mask using MakeMask()
	offset_mask = MakeMask(offset_bits);



	printf("pagenum_bits = %u\n", pagenum_bits);
	printf("offset_bits = %u\n", offset_bits);
	printf("process_size = %u\n", process_size);
	printf("page_size = %u\n", page_size);
	printf("offset_mask = %x\n", offset_mask);
	printf("no_entry = %u\n", no_entry);

	frames = (int*)malloc(no_entry * sizeof(int));
	if(frames == NULL){
		printf("Failed to allocate memory in line %d\n", __LINE__);
		exit(-1);
	}
	
	for(int i = 0; i < no_entry; i++)
		fscanf(fp, "%u", &frames[i]);

	fclose(fp);


	return TRUE;
}

unsigned logical2physical(unsigned logical)
{
	// TO DO: convert logical address to physical address using the page table
	unsigned page_no = logical >> offset_bits;
	unsigned offset = logical & offset_mask;

	unsigned physical_frame = frames[page_no];

	unsigned physical_addr = (physical_frame << offset_bits)| offset;

	return  physical_addr;


}

unsigned MakeMask(unsigned offset_bits)
// make offset mask
// e.g., 3 --> 0111, 5 --> 011111, ...
{
	unsigned mask = 0;
	for(int i = 0; i < offset_bits; i++)
		mask |= (1 << i);

	return mask;
}

