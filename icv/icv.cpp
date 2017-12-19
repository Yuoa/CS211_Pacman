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

	string outputFile = "<Standard Output>";
	string suffix = "_";
	int incomingType = PROCTYPE_UNKNOWN;
	bool outFileConfigFlag = false;
	bool suffixFlag = false;
	vector<char*> *targets = new vector<char*>();

	for (int t = 1; t < c; t++) {
	
		if (strcmp(v[t], "-i") == 0 && incomingType == PROCTYPE_UNKNOWN) {
		
			incomingType = PROCTYPE_IMGPATH;
		
		} else if (strcmp(v[t], "-o") == 0 && suffixFlag == false) {
		
			outFileConfigFlag = true;
		
		} else if (strcmp(v[t], "-d") == 0 && incomingType == PROCTYPE_UNKNOWN) {

			incomingType = PROCTYPE_IMGDIR;

		} else if (strcmp(v[t], "-s") == 0 && outFileConfigFlag == false) {
			
			suffixFlag = true;

		} else if (outFileConfigFlag) {
		
			outputFile = string(v[t]);

			outFileConfigFlag = false;

		} else if (suffixFlag) {
		
			suffix = string(v[t]);

			suffixFlag = false;
		
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

	if(outputFile.compare("<Standard Output>") != 0) {
	
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

	vector<string> *usedVar = new vector<string>();

	for (int t = 0; t < s; t++) {
	
		cout << "Starts to process image " << t + 1 << " of " << s << "...\n";

		Mat *origin = new Mat(imread(targets->at(t)));

		if(origin->empty()) {
		
			cout << "Failed to load image: " << targets->at(t) << "\n";

			if(string(targets->at(t)).find(".gif") > 0)
				cout << "(Hint: This program does not support GIF.)\n";
			
			if(t + 1 != s)
				cout << "Continue to next image.\n";

		} else {

			//origin->convertTo(origin, CV_)

			/* Determine it. */

			string target = string(targets->at(t));
			int lastDirIndicator = target.rfind("/");

			string pureTarget = target.substr((lastDirIndicator == string::npos) ? 0 : lastDirIndicator + 1, target.length());
			int extensionIndicator = pureTarget.find(".");

			string var = pureTarget.substr(0, (extensionIndicator == string::npos) ? pureTarget.length() : extensionIndicator);
			string originalVar = var; //Basically assign operator of C++ uses hard copy. 
			int increment = 1;

			for(int e = 0; e < usedVar->size(); e++)
				if(usedVar->at(e).compare(var) == 0)
					var = originalVar + suffix + to_string(increment++);
		
			int rows = origin->size().height;
			int columns = origin->size().width;

			cout << "File: " << pureTarget << "(" << target << ")"
				<< "\nVariable Name: " << var
				<< "\nWidth: " << columns << ", Height: " << rows << "\n";

			out << "unsigned char " << var << "[" << origin->total() * 4 << "] = { ";

			for(int e = 0; e < rows; e++)
				for(int m = 0; m < columns; m++) {

					if(e + m != 0) {
					
						out << ", ";
					
					}
				
					Vec4b px = origin->at<Vec4b>(Point(m, e));

					out << "0x" << hex << (int) px[0] << ", 0x" << (int) px[1] << ", 0x" << (int) px[2] << ", 0x" << (int) px[3];
				
				}

			out << " };\n";

			if(!standard)
				out << "\n";

			out.flush();

			usedVar->push_back(var);

			cout << "Finished.\n";
		
		}

		delete origin;

		cout << endl;
	
	}

	if (!standard)
		delete &out;

	delete targets;
	delete usedVar;

	cout << "Conversion Completed." << endl;

	return 0;	

}

int usage(char* vz) {

	cout << "Usage: " << vz << " [-o Output File Path] [-s Suffix]\n";

	int vzLength = strlen(vz);
	string spaces = "";

	for(int t = 0; t++ < vzLength; spaces += " ");

	cout << "       " << spaces << " -i Image Path ... | -d Image Directory Path\n"
		<< "\nOptions:\n"
		<< "\t-o      If you don\'t set output file path, defaultly output will\n"
		<< "\t        be printed to standard output.\n"
		<< "\t-s      Suffix to be used when the variable name is duplicated.\n"
		<< "\t        Default suffix is \"_\". If name \"red\" is duplicated,\n"
		<< "\t        secondary variable name will be \"red\"+(suffix)+\"2\".\n"
		<< "\t        (cf. Variable name of \"./test/red.jpg\" is \"red\".)\n"
		<< "\t-i      Write one (or more) image paths.\n"
		<< "\t-d      This program will be convert all files in directory.\n";
	
	return 0;

}

