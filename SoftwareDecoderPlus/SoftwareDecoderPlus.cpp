#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <array>
#include <algorithm>
#include <chrono>

struct Satellite {
	int satelliteId;
	int firstRegister;
	int secondRegister;
	std::array<int, 1023> chipSequence;
};

/*TODO: 1.Step*/
void stepOne(std::string fileLocation, std::vector<int> &sumSignal) {
	std::ifstream gpsFile;
	std::string fileContent;
	gpsFile.open(fileLocation);
	if (gpsFile.is_open())
	{
		while(gpsFile >> fileContent){
			sumSignal.push_back(std::stoi(fileContent));
		}
		gpsFile.close();
	}
	else {
		std::cout << "Couldn't open file with name: " << fileLocation << std::endl;
	}
}

/*TODO: 2.Step*/
void processMotherSequenceOne(std::array < int, 10> &sequence) {
	int newBit = sequence[9] ^ sequence[2];
	std::rotate(sequence.rbegin(), sequence.rbegin() + 1, sequence.rend());
	sequence.at(0) = newBit;
}

void processMotherSequenceTwo(std::array < int, 10> &sequence) {
	int newBit = sequence[1] ^ sequence[2] ^ sequence[5] ^ sequence[7] ^ sequence[8] ^ sequence[9];
	std::rotate(sequence.rbegin(), sequence.rbegin() + 1, sequence.rend());
	sequence.at(0) = newBit;
}

void generateChipSequence(Satellite &sat) {
	std::array<int, 10> motherSequenceOne = { 1,1,1,1,1,1,1,1,1,1 };
	std::array<int, 10> motherSequenceTwo = { 1,1,1,1,1,1,1,1,1,1 };
	for (int i = 0; i < sat.chipSequence.size(); i++)
	{
		sat.chipSequence.at(i) = (motherSequenceOne.at(9) ^ (motherSequenceTwo.at(sat.firstRegister) ^ motherSequenceTwo.at(sat.secondRegister))); //Dauert 19ns
		processMotherSequenceOne(motherSequenceOne); //Dauer: 331ns
		processMotherSequenceTwo(motherSequenceTwo);
	}
}

void printResult(int result,int satID,int delta) {
	if (result > 200)
	{
		std::cout << "satellite " << satID << " has sent bit " << "1" << ".(delta = " << delta << ")" << std::endl;
	}
	else if (result < -200)
	{
		std::cout << "satellite " << satID << " has sent bit " << "0" << ".(delta = " << delta << ")" << std::endl;
	}
}

void stepTwo(std::vector<int> sumSignal,std::vector<Satellite> &sats) {
	//Durchführung: 565ms
	//Ohne Rotate: 311ms
	//Ohne Rotate und print: 306-313 
	auto start = std::chrono::steady_clock::now();

	for (std::vector<Satellite>::iterator it = sats.begin(); it != sats.end(); ++it) {
		//Generierung der Chipsequence dauert für alle Satelliten 15ms -> Ca. 1ms - 0,75ms pro Durchlauf
		//640ns pro Ausführung
		generateChipSequence(*it); 
		//Ohne Rotate und print: 14ms
		//Ohne Rotate mit Print: 14ms

		for (auto delta = 0; delta < 1023; delta++) {
			auto result = 0;
			for (auto calc = 0; calc < 1023; calc++) {
				result += it->chipSequence.at(calc) * sumSignal.at(calc); //Berechnung dauert bei einer Ausführung 14ns-30ns
			}
			printResult(result, it->satelliteId, delta);
			std::rotate(it->chipSequence.rbegin(), it->chipSequence.rbegin() + 1, it->chipSequence.rend()); // Dauert 12ns-20ns
		}
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	std::cout << "elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds).count() << "ms\n";	
}

/*
Helper Functions
*/
void initSatellites(std::vector<Satellite> &sats) {

	Satellite typeOne,typeTwo,typeThree,typeFour,typeFive,typeSix,
		typeSeven,typeEight,typeNine,typeTen,typeEleven,typeTwelve,
		typeThirteen,typeFourteen,typeFifteen,typeSixteen,typeSeventeen,
		typeEighteen,typeNineteen,typeTwenty,typeTwentyOne,typeTwentyTwo,
		typeTwentyThree,typeTwentyFour;

	typeOne.satelliteId = 1;
	typeOne.firstRegister = 2-1;
	typeOne.secondRegister = 6-1;

	typeTwo.satelliteId = 2;
	typeTwo.firstRegister = 3 - 1;
	typeTwo.secondRegister = 7 - 1;

	typeThree.satelliteId = 3;
	typeThree.firstRegister = 4 - 1;
	typeThree.secondRegister = 8 - 1;

	typeFour.satelliteId = 4;
	typeFour.firstRegister = 5 - 1;
	typeFour.secondRegister = 9 - 1;

	typeFive.satelliteId = 5;
	typeFive.firstRegister = 1 - 1;
	typeFive.secondRegister = 9 - 1;

	typeSix.satelliteId = 6;
	typeSix.firstRegister = 2 - 1;
	typeSix.secondRegister = 10 - 1;

	typeSeven.satelliteId = 7;
	typeSeven.firstRegister = 1 - 1;
	typeSeven.secondRegister = 8 - 1;

	typeEight.satelliteId = 8;
	typeEight.firstRegister = 2 - 1;
	typeEight.secondRegister = 9 - 1;

	typeNine.satelliteId = 9;
	typeNine.firstRegister = 3 - 1;
	typeNine.secondRegister = 10 - 1;

	typeTen.satelliteId = 10;
	typeTen.firstRegister = 2 - 1;
	typeTen.secondRegister = 3 - 1;

	typeEleven.satelliteId = 11;
	typeEleven.firstRegister = 3 - 1;
	typeEleven.secondRegister = 4 - 1;

	typeTwelve.satelliteId = 12;
	typeTwelve.firstRegister = 5 - 1;
	typeTwelve.secondRegister = 6 - 1;

	typeThirteen.satelliteId = 13;
	typeThirteen.firstRegister = 6 - 1;
	typeThirteen.secondRegister = 7 - 1;

	typeFourteen.satelliteId = 14;
	typeFourteen.firstRegister = 7 - 1;
	typeFourteen.secondRegister = 8 - 1;

	typeFifteen.satelliteId = 15;
	typeFifteen.firstRegister = 8 - 1;
	typeFifteen.secondRegister = 9 - 1;

	typeSixteen.satelliteId = 16;
	typeSixteen.firstRegister = 9 - 1;
	typeSixteen.secondRegister = 10 - 1;

	typeSeventeen.satelliteId = 17;
	typeSeventeen.firstRegister = 1 - 1;
	typeSeventeen.secondRegister = 4 - 1;

	typeEighteen.satelliteId = 18;
	typeEighteen.firstRegister = 2 - 1;
	typeEighteen.secondRegister = 5 - 1;

	typeNineteen.satelliteId = 19;
	typeNineteen.firstRegister = 3 - 1;
	typeNineteen.secondRegister = 6 - 1;

	typeTwenty.satelliteId = 20;
	typeTwenty.firstRegister = 4 - 1;
	typeTwenty.secondRegister = 7 - 1;

	typeTwentyOne.satelliteId = 21;
	typeTwentyOne.firstRegister = 5 - 1;
	typeTwentyOne.secondRegister = 8 - 1;

	typeTwentyTwo.satelliteId = 22;
	typeTwentyTwo.firstRegister = 6 - 1;
	typeTwentyTwo.secondRegister = 9 - 1;

	typeTwentyThree.satelliteId = 23;
	typeTwentyThree.firstRegister = 1 - 1;
	typeTwentyThree.secondRegister = 3 - 1;

	typeTwentyFour.satelliteId = 24;
	typeTwentyFour.firstRegister = 4 - 1;
	typeTwentyFour.secondRegister = 6 - 1;

	sats.push_back(typeOne);
	sats.push_back(typeTwo);
	sats.push_back(typeThree);
	sats.push_back(typeFour);
	sats.push_back(typeFive);
	sats.push_back(typeSix);
	sats.push_back(typeSeven);
	sats.push_back(typeEight);
	sats.push_back(typeNine);
	sats.push_back(typeTen);

	sats.push_back(typeEleven);
	sats.push_back(typeTwelve);
	sats.push_back(typeThirteen);
	sats.push_back(typeFourteen);
	sats.push_back(typeFifteen);
	sats.push_back(typeSixteen);
	sats.push_back(typeSeventeen);
	sats.push_back(typeEighteen);
	sats.push_back(typeNineteen);
	sats.push_back(typeTwenty);

	sats.push_back(typeTwentyOne);
	sats.push_back(typeTwentyTwo);
	sats.push_back(typeTwentyThree);
	sats.push_back(typeTwentyFour);
}

int main(int argc, char *argv[])
{
	
	if (argc > 1)
	{
		std::string fileLocation = argv[1];
		std::vector<int> sumSignal;
		std::vector<Satellite> satellites;
		initSatellites(satellites);
		
		stepOne(fileLocation, sumSignal);
		
		stepTwo(sumSignal, satellites);

		return 0;
	}	else {
		std::cout << "Fehlender Parameter erkannt." << std::endl << "Bitte uebergeben Sie eine Pfad zu einer Datei die eine Summensignal enthaelt." << std::endl;
		return -1;
	}
}
// -- Zeitmessungen --
// auto start = std::chrono::steady_clock::now();
// auto end = std::chrono::steady_clock::now();
// std::chrono::duration<double> elapsed_seconds = end-start;
// std::cout << "elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(elapsed_seconds).count() << "ns\n";