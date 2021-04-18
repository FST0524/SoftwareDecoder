#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ---Helper Functions---
/*
Info: Index beginnt bei 0 ist aber eig. um eins höher!!!
*/
void printResult(int result,int satID,int delta) {

	if (result > 200)
	{
		printf("satellite %d has sent bit 1.(delta=%d)\n",satID+1,delta+1);
	}
	else if (result < -200)
	{
		printf("satellite %d has sent bit 0.(delta=%d)\n",satID+1,delta+1);
	}

}

void rotateArray(int* sequence, int seqLength)
{
	int lastElement = sequence[seqLength-1];
	for(int i = seqLength-1; i > 0;i--)
	{
		sequence[i] = sequence[i-1];
	}
	sequence[0] = lastElement;
}

// ---First Step---
void stepOne(char* fileLocation, int sumSignal[1023]) {
	FILE* fp = fopen(fileLocation, "r");
	char buf[10];
	int i = 0;
	while(fscanf(fp, "%s", buf) != EOF)
	{
		sumSignal[i] = atoi(buf);
		i++;
	}
	fclose(fp);
}

// ---Second Step---
void processMotherSequenceOne(int sequence[10]) {
	int newBit = sequence[9] ^ sequence[2];
	rotateArray(sequence, 10);
	sequence[0] = newBit;
}

void processMotherSequenceTwo(int sequence[10]) {
	int newBit = sequence[1] ^ sequence[2] ^ sequence[5] ^ sequence[7] ^ sequence[8] ^ sequence[9];
	rotateArray(sequence, 10);
	sequence[0] = newBit;
}

void generateChipSequence(int id, int chipSequences[24][2046], const int xorValues[24][2]) {
	int motherSequenceOne[10] = { 1,1,1,1,1,1,1,1,1,1 };
	int motherSequenceTwo[10] = { 1,1,1,1,1,1,1,1,1,1 };
	register unsigned j = 0;
	
	for(j = 0; j != 1022; ++j)
	{
		int value = (motherSequenceOne[9] ^ (motherSequenceTwo[xorValues[id][0]] ^ motherSequenceTwo[xorValues[id][1]]));
		chipSequences[id][j] = value;
		chipSequences[id][1023+j] = value;
		processMotherSequenceOne(motherSequenceOne);
		processMotherSequenceTwo(motherSequenceTwo);
	}
}

void stepTwo(const int xorValues[24][2],int sumSignal[1023],int chipSequences[24][2046]) {
	register unsigned int i = 0;
	register unsigned delta = 0;
	register unsigned calc = 0;
	register int result = 0;
	for (i = 0; i != 23; i++) {
		generateChipSequence(i,chipSequences,xorValues);
		for (delta = 0; delta != 1022; ++delta) {
		 	result = 0;
		 	for (calc = 0; calc != 1022; ++calc) {
		 		result += chipSequences[i][calc+delta] * sumSignal[calc];
			}
		 	printResult(result, i, delta);  
		}
	}
}

int main(int argc, char** argv)
{
	if (argc > 1)
	{	
		clock_t start, stop;
		start = clock();
		char* fileLocation = argv[1];
		int sumSignal[1023];
		const int xorValues[24][2] = {{1,5},{2,6},{3,7},{4,8},{0,8},{1,9},{0,7},{1,8},{2,9},{1,2},{2,3},{4,5},{5,6},{6,7},{7,8},{8,9},{0,3},{1,4},{2,5},{3,6},{4,7},{5,8},{0,2},{3,5}};
		int chipSequences[24][2046];

		stepOne(fileLocation, sumSignal);
		stepTwo(xorValues,sumSignal, chipSequences);

		stop = clock();
		printf("%.0f Milliseconds\n", ((double)(stop - start) / CLOCKS_PER_SEC) * 1000);
		return 0;
	}	else {
		printf("Fehlender Parameter erkannt. \nBitte uebergeben Sie eine Pfad zu einer Datei die eine Summensignal enthaelt.\n");
		return -1;
	}
	
}
