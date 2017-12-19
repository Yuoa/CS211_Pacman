//By labus YUOA, 2017.
#include <ostream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#define PROCTYPE_UNKNOWN 0
#define PROCTYPE_IMGPATH 1
#define PROCTYPE_IMGDIR 2

using namespace cv;
using namespace std;

string PROCTYPE_STR[3] = { "PROCTYPE_UNKNOWN", "PROCTYPE_IMGPATH", "PROCTYPE_IMGDIR" };

int usage(char*);

int main(int c, char** v) {

	cout << "Image converter for mangoboard.\n\n";

	if (c < 2)
		return usage(v[0]);

	string outputFile = "STANDARD OUTPUT";
	int incomingType = PROCTYPE_UNKNOWN;
	bool outFileConfigFlag = false;
	vector<char*> *targets = new vector<char*>();

	for (int t = 1; t < c; t++) {
	
		if (strcmp(v[t], "-i") == 0 && incomingType == PROCTYPE_UNKNOWN) {
		
			incomingType = PROCTYPE_IMGPATH;
		
		} else if (strcmp(v[t], "-o") == 0) {
		
			outFileConfigFlag = true;
		
		} else if (strcmp(v[t], "-d") == 0 && incomingType == PROCTYPE_UNKNOWN) {

			incomingType = PROCTYPE_IMGDIR;

		} else if (outFileConfigFlag) {
		
			outputFile = string(v[t]);

			outFileConfigFlag = false;

		} else if (incomingType == PROCTYPE_UNKNOWN || v[t][0] == '-') {
		
			delete targets;

			return usage(v[0]);
		
		} else {
		
			targets->push_back(v[t]);	
		
		}
	
	}

	cout << "[Configuration]"
		<< "\nInput Mode  : " << PROCTYPE_STR[incomingType]
		<< "\nInput Count : " << targets->size()
		<< "\nOutput File : " << outputFile
		<< "\n\n";

	if(incomingType == PROCTYPE_UNKNOWN) {
	
		cout << "Please set input mode via using \"-i\" or \"-d\" option.\n\n";

		delete targets;

		return usage(v[0]);
	
	}

	int s = targets->size();

	cout << "Setting output stream... ";

	ostream *tout = &cout;
	bool standard = true;

	if(outputFile.compare("STANDARD OUTPUT") == 0) {
	
		outputFile = "<Standard Output>";
	
	} else {
	
		struct stat buf;

		if(stat(outputFile.c_str(), &buf) == 0)
			cout << "(overwrite mode)";

		ofstream *fout = new ofstream();
			
		fout->open(outputFile, ios::out | ios::trunc);

		tout = fout;

		if(!fout->is_open()) {
		
			cout << "ERROR!\n";
			cerr << "Program is now going to be terminated due to output file open error.\n";

			delete fout;
			delete targets;

			exit(0);
		
		}

		standard = false;
	
	}

	ostream &out = *tout;

	cout << "OK.\n\n";

	for (int t = 0; t < s; t++) {
	
		cout << "Starts to process image " << t << " of " << s << "...\n";

		Mat *origin = new Mat(imread(targets->at(t)));

		if(origin->empty()) {
		
			cout << "Failed to load image: " << targets->at(t)
				<< "\nContinue to next image.\n";

		} else {

			//origin->convertTo(origin, CV_)

			string target = string(targets->at(t));
		
			int rows = origin->size().height;
			int columns = origin->size().width;

			cout << "Width: " << columns << ", Height: " << rows << "\n\n";

			out << "unsigned char " << target.substr(0, target.find(".")) << "[" << origin->total() * 4 << "] = { ";

			for(int e = 0; e < rows; e++)
				for(int m = 0; m < columns; m++) {

					if(e + m != 0) {
					
						out << ", ";
					
					}
				
					Vec4b px = origin->at<Vec4b>(Point(m, e));

					out << "0x" << hex << (int) px[0] << ", 0x" << (int) px[1] << ", 0x" << (int) px[2] << ", 0x" << (int) px[3];
				
				}

			out << " };\n\n";

			out.flush();
		
		}

		delete origin;

		cout << endl;
	
	}

	if (!standard)
		delete &out;

	delete targets;

	cout << "Program Completed." << endl;

	return 0;	

}

int usage(char* vz) {

	cout << "Usage: " << vz << " [-o Output File Path] [-i Image Path ... | -d Image Directory Path]\n"
		<< "\nOptions:\n\n"
		<< "-o      If you don\'t set output file path, defaultly output will be\n"
		<< "        printed to standard output.\n"
		<< "-d      This program will be convert all files in directory.\n"
		<< "\n";
	
	return 0;

}

