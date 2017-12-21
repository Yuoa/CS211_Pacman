//By Labus YUOA, 2017.
#include <cstdio>
#include <ostream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <cctype>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

int gpl();
int usage(char*);

int main(int c, char** v) {

	/* Check Argument Count */

	if (c < 2) {

		cout << "ICV does NOT support alpha(transparency) channel for now.\n\n";

		return usage(v[0]);

	}

	/* Basic Variables */

	string outputFile = "<Standard Output>";
	string prefix = "img_";
	string suffix = "_";
	bool outFileConfigFlag = false;
	bool suffixFlag = false;
	bool prefixFlag = false;
	vector<char*> *targets = new vector<char*>();

	/* Analyze Arguments */

	for (int t = 1; t < c; t++) {
	
		if(strcmp(v[t], "-v") == 0) {
		
			cout << "Version: ICV_REV201712211\n";

			return gpl();
		
		} else if (strcmp(v[t], "-o") == 0 && suffixFlag == false && prefixFlag == false) {
		
			outFileConfigFlag = true;
		
		} else if (strcmp(v[t], "-s") == 0 && outFileConfigFlag == false && prefixFlag == false) {
			
			suffixFlag = true;

		} else if (strcmp(v[t], "-p") == 0 && outFileConfigFlag == false && suffixFlag == false) {
		
			prefixFlag = true;
		
		} else if (outFileConfigFlag) {
		
			outputFile = string(v[t]);

			outFileConfigFlag = false;

		} else if (suffixFlag) {
		
			suffix = string(v[t]);

			suffixFlag = false;
		
		} else if (prefixFlag) {
		
			prefix = string(v[t]);

			prefixFlag = false;
		
		} else if (v[t][0] == '-') {
		
			delete targets;

			return usage(v[0]);
		
		} else {
		
			targets->push_back(v[t]);	
		
		}
	
	}

	/* Print Configurations */

	cout << "[Configuration]"
		<< "\nInput Count : " << targets->size()
		<< "\nOutput File : " << outputFile
		<< "\n\n";

	/* Expand if target is folder */

	cout << "Expanding directories... ";

	int s = targets->size();

	vector<char*> *temp = new vector<char*>();

	for(int t = 0; t < s; t++) {
	
		struct stat buf;

		if(stat(targets->at(t), &buf) == 0 && S_ISDIR(buf.st_mode)) {

			/* Directory Exists */

			DIR *target = opendir(targets->at(t));

			if(target == NULL)
				cout << "\nDir \"" << targets->at(t) << "\" does not exist. Remove this from list.\n";
			else {

				struct dirent *file = NULL;
			
				while((file = readdir(target)) != NULL)
					if(file->d_name[0] != '.') {

						/* Manually deep-copying c_str() */

						string concaten = string(targets->at(t)).append(file->d_name);

						char *dbuf = new char[concaten.size() + 1];

						memcpy(dbuf, concaten.c_str(), concaten.size() + 1);

						temp->push_back(dbuf);

					}

				closedir(target);

			}

			targets->erase(targets->begin() + t);

			t--;
			s--;
		
		}
	
	}

	targets->insert(targets->end(), temp->begin(), temp->end());

	delete temp;

	cout << "OK.\n\n";

	/* Create or Copy Output Stream */

	cout << "Setting output stream... ";

	ostream *tout = &cout;
	bool standard = true;

	if(outputFile.compare("<Standard Output>") != 0) {
	
		/* Check If File Already Exists for User Notification */

		struct stat buf;

		if(stat(outputFile.c_str(), &buf) == 0)
			cout << "(overwrite mode) ";

		/* Open File for Write */

		ofstream *fout = new ofstream();
			
		fout->open(outputFile, ios::out | ios::trunc);

		tout = fout;

		if(!fout->is_open()) {

			/* Error Occured in Opening File */
		
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

	/* Analyze Image and Save Pixel Data */

	vector<string> *usedVar = new vector<string>();

	s = targets->size();

	for (int t = 0; t < s; t++) {
	
		cout << "Starts to process image " << t + 1 << " of " << s << "...\n";

		Mat *origin = new Mat(imread(targets->at(t)));

		/* Check If Mat is Empty */

		if(origin->empty()) {
		
			cout << "Failed to load image: " << targets->at(t) << "\n";
			
			if(string(targets->at(t)).find(".gif") > 0)
				cout << "(Hint: This program does not support GIF.)\n";
			
			if(t + 1 != s)
				cout << "Continue to next image.\n";

		} else {

			/* Do Some Pretreatment Here */

			//We determined to not support alpha. So do nothing in here.

			/* Determine Variable Name */

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
		
			if(isdigit(var.c_str()[0]))
				var = prefix + var;

			/* Print Conversion Information */

			int rows = origin->size().height;
			int columns = origin->size().width;

			cout << "File: " << pureTarget << "(" << target << ")"
				<< "\nVariable Name: " << var
				<< "\nWidth: " << columns << ", Height: " << rows << "\n";

			/* Start Conversion */

			out << "unsigned char " << var << "[" << origin->total() * 3 << "]={";

			for(int e = 0; e < rows; e++)
				for(int m = 0; m < columns; m++) {

					if(e + m != 0)
						out << ",";
				
					Vec3b px = origin->at<Vec3b>(Point(m, e));
					
					/* Print as decimal form(len=<3) is always better than hexadecimal form(len=<4) */

					
					out << (int) px[0] //Blue
						<< "," << (int) px[1] //Green
						<< "," << (int) px[2]; //Red
				
				}

			out << "};\n";

			out.flush();

			/* Conversion Finished */

			usedVar->push_back(var);

			cout << "Finished.\n";
		
		}

		delete origin;

		cout << endl;
	
	}

	/* Program Termination */

	if (!standard)
		delete &out;

	delete targets;
	delete usedVar;

	cout << "Conversion Completed." << endl;

	return 0;

}

int usage(char* vz) {

	/* Print Usage Message */

	cout << "Usage: " << vz << " [-v] [-o Output File Path] [-p Prefix]\n";

	int vzLength = strlen(vz);
	string spaces = "";

	for(int t = 0; t++ < vzLength; spaces += " ");

	cout << "       " << spaces << " [-s Suffix] Image|Directory ...\n"
		<< "\nOptions:\n"
		<< "\t-v      Print ICV Version.\n"
		<< "\t-o      If you don\'t set output file path, defaultly output will\n"
		<< "\t        be printed to standard output.\n"
		<< "\t-p      Prefix to be used when the variable name starts with \n"
		<< "\t        number. Default is \"img_\". If name is \"19th_birthday\",\n"
		<< "\t        prefixed name will be (prefix)+\"19th_birthday\".\n"
		<< "\t-s      Suffix to be used when the variable name is duplicated.\n"
		<< "\t        Default suffix is \"_\". If name \"red\" is duplicated,\n"
		<< "\t        secondary name will be \"red\"+(suffix)+\"1\".\n";

	cout << "\nNote:\n"
		<< "\t        Variable name of \"./test/red.blue.white.jpg\" is \"red\".\n";
	
	return 0;

}

int gpl() {

	cout << "License: \n\n"
		<< "    ICV converts jpg/png images to pixel data for mangoboard.\n"
		<< "    Copyright (C) 2017  Yuoa\n\n"
    		<< "    This program is free software: you can redistribute it and/or modify\n"
    		<< "    it under the terms of the GNU General Public License as published by\n"
    		<< "    the Free Software Foundation, either version 3 of the License, or\n"
    		<< "    (at your option) any later version.\n\n"
		<< "    This program is distributed in the hope that it will be useful,\n"
	    	<< "    but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    		<< "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    		<< "    GNU General Public License for more details.\n"
    		<< "    You should have received a copy of the GNU General Public License\n"
    		<< "    along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n";

}

