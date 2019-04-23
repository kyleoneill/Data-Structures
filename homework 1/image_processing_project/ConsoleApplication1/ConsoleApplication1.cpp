#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <vector>
#include <iterator>
#include <string>
using namespace std;

vector<string> readFile(ifstream& file) {
	string line;
	vector<string> out;
	while (getline(file, line)) {
		out.push_back(line);
	}
	return out;
}

void replace(char from, char to, vector<string>& input) { 
	for (vector<string>::size_type i = 0; i != input.size(); i++) {
		for (string::iterator it = input[i].begin(); it != input[i].end(); ++it) {
			if (*it == from) {
				*it = to;
			}
		}
	}
}

void erosion(char foreground, char background, vector<string>& input) {
	vector<string> original = input;
	for (vector<string>::size_type i = 0; i != original.size(); i++) {
		for (size_t j = 0; j < original[i].length(); j++) {
			if (original[i][j] == foreground) {
				if ((j != 0 && original[i][j - 1] == background) || (j != original[i].length() - 1 && original[i][j + 1] == background) || (i != 0 && original[i - 1][j] == background) || (i != original.size() - 1 && original[i + 1][j] == background)) {
					input[i][j] = background;
				}
			}
		}
	}
}

void dilation(char foreground, vector<string>& input) {
	vector<string> original = input;
	for (vector<string>::size_type i = 0; i != original.size(); i++) {
		for (size_t j = 0; j < original[i].length(); j++) {
			if (original[i][j] != foreground) {
				if ((j != 0 && original[i][j - 1] == foreground) || (j != original[i].length()-1 && original[i][j + 1] == foreground) || (i != 0 && original[i - 1][j] == foreground) || (i != original.size()-1 && original[i + 1][j] == foreground)) {
					input[i][j] = foreground;
				}
			}
		}
	}
}

int main(int argc, char* argv[]) {

	if (argc < 4) {
		cerr << "Not enough arguments" << endl;
		exit(1);
	}

	ifstream inFile;
	inFile.open(argv[1]);
	if (!inFile) {
		cerr << "Unable to open file " + string(argv[1]) << endl;
		exit(1);
	}

	vector<string> input = readFile(inFile);
	vector<string> output;

	if (string(argv[3]) == "replace") {
		replace(*argv[4], *argv[5], input);
	}
	else if (string(argv[3]) == "erosion") {
		erosion(*argv[4], *argv[5], input);
	}
	else if (string(argv[3]) == "dilation") {
		dilation(*argv[4], input);
	}
	else if (string(argv[3]) == "opening") {
		erosion(*argv[4], *argv[5], input);
		dilation(*argv[4], input);
	}
	else if (string(argv[3]) == "closing") {
		dilation(*argv[4], input);
		erosion(*argv[4], *argv[5], input);
	}

	string outFileName = "./" + string(argv[2]) + ".txt";
	std::ofstream outputFile(outFileName);
	std::ostream_iterator<string> outputIterator(outputFile, "\n");
	copy(input.begin(), input.end(), outputIterator);

	return 0;
}
