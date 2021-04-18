#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ---Helper Functions---
// Zeitmessung(Eine Durchführung): 0ms 
void printResult(int result,int satID,int delta) {

	if (result > 200)
	{
		printf("satellite %d has sent bit 1.(delta=%d)\n",satID+1,delta-1);
	}
	else if (result < -200)
	{
		printf("satellite %d has sent bit 0.(delta=%d)\n",satID+1,delta-1);
	}

}

// Zeitmessung(Eine Durchführung): 0ms
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
// Zeitmessung(Eine Durchführung): 0ms
void stepOne(char* fileLocation, int sumSignal[]) {
	// FILE* fp = fopen(fileLocation, "r");
	// char buf[10];
	// int i = 0;
	// while(fscanf(fp, "%s", buf) != EOF)
	// {
	// 	sumSignal[i] = atoi(buf);
	// 	i++;
	// }
	// fclose(fp);

	char buf[10];
	FILE* fp;
	if(fopen_s(&fp,fileLocation, "r")!= 0 ){
		perror("File doesn't exists!");
	}
	int i = 0;
	while(fscanf_s(fp, "%s", buf) != EOF)
	{
		sumSignal[i] = atoi(buf);
		i++;
	}
	fclose(fp);
}

// ---Second Step---
// Zeitmessung(Eine Durchführung): 0ms
void processMotherSequenceOne(int* sequence) {
	int newBit = sequence[9] ^ sequence[2];
	rotateArray(sequence, 10);
	sequence[0] = newBit;
}

// Zeitmessung(Eine Durchführung): 0ms
void processMotherSequenceTwo(int* sequence) {
	int newBit = sequence[1] ^ sequence[2] ^ sequence[5] ^ sequence[7] ^ sequence[8] ^ sequence[9];
	rotateArray(sequence, 10);
	sequence[0] = newBit;
}

// Zeitmessung(Eine Durchführung): 0ms
void generateChipSequence(int xorValues[24][2], int chipSequences[24][1023], int id) {

	int motherSequenceOne[10] = { 1,1,1,1,1,1,1,1,1,1 };
	int motherSequenceTwo[10] = { 1,1,1,1,1,1,1,1,1,1 };

	for(int j = 0; j < 1023; j++)
	{
		chipSequences[id][j] = (motherSequenceOne[9] ^ (motherSequenceTwo[xorValues[id][0]] ^ motherSequenceTwo[xorValues[id][1]]));
		processMotherSequenceOne(motherSequenceOne);
		processMotherSequenceTwo(motherSequenceTwo);
	}
}

//Die komplette Ausführung dauert 
//156 Milliseconds
void stepTwo(int xorValues[24][2], int chipSequences[24][1023], int* sumSignal) {
	for (int i = 0; i < 24; i++) {
		generateChipSequence(xorValues, chipSequences, i);	// 24x Ausführung => 0ms	
		for (int delta = 0; delta < 1023; delta++) { 		// Benötigt 78ms (ohne Rotation und print Result), mit Print bleibt die Zeit gleich, Rotieren führt zu einer Erhöhung auf 156ms
		 	int result = 0;
		 	for (int calc = 0; calc < 1023; calc++) {
		 		result += chipSequences[i][calc] * sumSignal[calc]; //Dreifache For-Schleife eventuell noch weiter optimierbar 
			}
		 	printResult(result, i, delta);  
		  	rotateArray(chipSequences[i],1023); //Rotate muss optimiert werden, bei hoher SequenzLänge sehr aufwendig
		}
	}
}

int main(int argc, char** argv)
{
	clock_t start, stop;
	start = clock();
	if (argc > 1)
	{	
		char* fileLocation = argv[1];
		int sumSignal[1023];
		int xorValues[24][2] = {{1,5},{2,6},{3,7},{4,8},{0,8},{1,9},{0,7},{1,8},{2,9},{1,2},{2,3},{4,5},{5,6},{6,7},{7,8},{8,9},{0,3},{1,4},{2,5},{3,6},{4,7},{5,8},{0,2},{3,5}};
		int chipSequences[24][1023];

		stepOne(fileLocation, sumSignal); //Hier: 0ms
		stepTwo(xorValues, chipSequences,sumSignal); //Hier: Behebung eines Fehler führt zu einer Verbesserung auf 156ms
		stop = clock();
		printf("%.0f Milliseconds\n", ((double)(stop - start) / CLOCKS_PER_SEC) * 1000);
		return 0;
	}	else {
		printf("Fehlender Parameter erkannt. \nBitte uebergeben Sie eine Pfad zu einer Datei die eine Summensignal enthaelt.\n");
		return -1;
	}
	
}
