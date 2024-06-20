#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define SIZE 5
// TO DO: define a data type SegmentTable using struct and typedef
//		it should work with SegmentTable_Display()

typedef struct {
	int size;
	int limit[SIZE];
	int base[SIZE];
} SegmentTable;

int SegmentTable_Load(char *filename, SegmentTable *table);
void SegmentTable_Destroy(SegmentTable *table);
void SegmentTable_Display(SegmentTable *table);
int SegmentTable_l2p(SegmentTable *table, int seg, int offset);
void SegmentationFault(SegmentTable *table, int seg, int offset);

int main(int argc, char *argv[])
// DO NOT modify this function
{
	if(argc < 2){
		printf("Usage: %s <seg_table>\n", argv[0]);
		return 0;
	}
	char *table_file = argv[1];

	SegmentTable table = { 0 };

	SegmentTable_Load(table_file, &table);
	SegmentTable_Display(&table);

	printf("Valid cases: \n");
	for(int seg = 0; seg < table.size; seg++){
		int offset = 0, p = 0;

		offset = 0;
		p = SegmentTable_l2p(&table, seg, offset);
		printf("\t(%d, %d) --> %d\n", seg, offset, p);

		offset = table.limit[seg] / 2;
		p = SegmentTable_l2p(&table, seg, offset);
		printf("\t(%d, %d) --> %d\n", seg, offset, p);

		offset = table.limit[seg] - 1;
		p = SegmentTable_l2p(&table, seg, offset);
		printf("\t(%d, %d) --> %d\n", seg, offset, p);
	}

	printf("Error cases: \n");
	{
		int seg = table.size;
		int offset = 0;
		int p = SegmentTable_l2p(&table, seg, offset);
		printf("\t(%d, %d) --> %d\n", seg, offset, p);
	}
	for(int seg = 0; seg < table.size; seg++){
		int offset = table.limit[seg];
		int p = SegmentTable_l2p(&table, seg, offset);
		printf("\t(%d, %d) --> %d\n", seg, offset, p);
	}

	printf("Bye!\n");

	return 0;
}

int SegmentTable_Load(char *filename, SegmentTable *table)
{
	int result = TRUE;
	
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Failed to open file %s.\n", filename);
		result = FALSE;
		exit(-1);
	}

	fscanf(fp, "%d", &table->size);
	
	for(int i =0; i<table->size; i++){
		fscanf(fp, "%d", &table->limit[i]);
	}
	for(int i =0; i<table->size; i++){
		fscanf(fp, "%d", &table->base[i]);
	}
	fclose(fp);
	return result;
}

void SegmentTable_Destroy(SegmentTable *table)
{


}

void SegmentTable_Display(SegmentTable *table)
// DO NOT modify this function
{
	printf("size = %d\n", table->size);
	for(int i = 0; i < table->size; i++){
		printf("seg %3d: limit = %d, base = %d\n", i, table->limit[i], table->base[i]);
	}
}

int SegmentTable_l2p(SegmentTable *table, int seg, int offset)
{
	int p = 0;
	if(seg == table->size){
		SegmentationFault(table, seg, offset);
		return -1;
	}
	if(offset>table->limit[seg]){
		SegmentationFault(table,seg,offset);
	}
	p = table->base[seg] + offset;


	return p;
}

int SegmentTable_p2l(SegmentTable *table, int seg, int offset)
// NEVER MODIFY OR ERASE this function, or the solution can be regarded as cheating.
{
	int l = 0;

	return l;
}

void SegmentationFault(SegmentTable *table, int seg, int offset)
{
	printf("Segmentation Fault! seg = %d, offset = %d\n", seg, offset);
	if(seg >= 0 && seg < table->size)
		printf("segment %d: limit = %d, base = %d\n", seg, table->limit[seg], table->base[seg]);

	fflush(stdout);
}

