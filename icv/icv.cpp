//By labus YUOA, 2017.
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
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

	string outDirectory = "./";
	int incomingType = PROCTYPE_UNKNOWN;
	bool outDirectoryConfigFlag = false;
	vector<char*> *targets = new vector<char*>();

	for (int t = 1; t < c; t++) {
	
		if (strcmp(v[t], "-i") == 0 && incomingType == PROCTYPE_UNKNOWN) {
		
			incomingType = PROCTYPE_IMGPATH;
		
		} else if (strcmp(v[t], "-o") == 0) {
		
			outDirectoryConfigFlag = true;
		
		} else if (strcmp(v[t], "-d") == 0 && incomingType == PROCTYPE_UNKNOWN) {

			incomingType = PROCTYPE_IMGDIR;

		} else if (outDirectoryConfigFlag) {
		
			outDirectory = string(v[t]);

			outDirectoryConfigFlag = false;

		} else if (incomingType == PROCTYPE_UNKNOWN || v[t][0] == '-') {
		
			return usage(v[0]);
		
		} else {
		
			targets->push_back(v[t]);	
		
		}
	
	}

	cout << "[Configuration]"
		<< "\nInput Mode  : " << PROCTYPE_STR[incomingType]
		<< "\nInput Count : " << targets->size()
		<< "\nOutput Dir  : " << outDirectory
		<< "\n\n";

	if(incomingType == PROCTYPE_UNKNOWN) {
	
		cout << "Please set input mode via using \"-i\" or \"-d\" option.\n\n";

		return usage(v[0]);
	
	}

	int s = targets->size();

	for (int t = 0; t < s; t++) {
	
		cout << "Starts to process image " << t << " of " << s << "...\n";

		Mat *origin = new Mat(imread(targets->at(t)));

		if(origin->empty()) {
		
			cout << "Failed to load image: " << targets->at(t)
				<< "\nContinue to next image.\n";

		} else {
		
			int rows = origin->size().height;
			int columns = origin->size().width;

			cout << "Width: " << columns << ", Height: " << rows << endl;

			for(int e = 0; e < rows; e++)
				for(int m = 0; m < columns; m++) {
				
					Vec3b px = origin->at<Vec3b>(Point(m, e));

						
				
				}
		
		}

		cout << endl;
	
	}

	cout << "Program Completed." << endl;

	return 0;	

}


int usage(char* vz) {

	cout << "Usage: " << vz << " [-o Output Directory Path] [-i Image Path ... | -d Image Directory Path]\n";
	
	return 0;

}
