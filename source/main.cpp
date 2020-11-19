#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>
#include <sstream>
#include <memory>
#include <iterator>
#include <algorithm>
#include <utility>

#include "parseTemps.h"

using namespace std;

vector<string> outputFileNameGen(string, int);
vector<double> coreTempContainer(int, std::vector<CoreTempReading>);
void display();
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // Input validation
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " input_file_name" << "\n";
        return 1;
    }

    ifstream input_temps(argv[1]);
    if (!input_temps) {
        cout << "ERROR: " << argv[1] << " could not be opened" << "\n";
        return 2;
    }
    // End Input Validation

    string inputFileName(argv[1]);
    // vector
    auto readings = parse_raw_temps<std::vector<CoreTempReading>>(input_temps);
    
    // list
    // auto readings = parse_raw_temps<std::list<CoreTempReading>>(input_temps);

    int coreCount = readings[0].second.size();
    outputFileNameGen(inputFileName ,coreCount);

    for(int i = 0; i < coreCount; i++)
    {
        coreTempContainer(i, readings);
    }
    /*
    // Output everything to match the Python version
    for (const CoreTempReading& theReading : readings) 
    {
        cout << "(" << theReading.first << ", [";

        const vector<double>& coreTemps = theReading.second;
        for (int i = 0; i < coreTemps.size() - 1; i++) {
            cout << coreTemps[i] << ", ";
        }
        cout << *(coreTemps.end() - 1) << "])" << "\n";
        int coreCount = coreTemps.size();
    }
    */
    return 0;    
}
vector<string> outputFileNameGen(string inputFileName, int coreCount)
{
    vector<string> outputFileNames;
    string outputFileName = "";
    string core = "-core-";
    string ext = ".txt";
    cout << coreCount << " cores detected." << endl;

    for(int i = 0; i < coreCount; i++)
    {
        string coreCounttoString = to_string(coreCount);
        string outputFileName = inputFileName + core + coreCounttoString + ext;

        outputFileNames.push_back(outputFileName);
    }

    cout << "Output File names: " << std::endl;

    for(int i = 0; i < outputFileNames.size(); i++)
    {
        cout << outputFileNames[i] << std::endl;
    }

    return outputFileNames;
}
vector<double> coreTempContainer(int coreNumber, std::vector<CoreTempReading> readings)
{
    vector<double> coreTemps;
    cout << "Core Number:" << coreNumber << std::endl;
    for(int j = 0; j < readings.size(); j++)
    {
        double coreTemp = readings[j].second[coreNumber];

        coreTemps.push_back(coreTemp);
    }

    for(int i = 0; i < coreTemps.size(); i++)
    {
        cout << coreTemps[i] << std::endl;
    }

    return coreTemps;
}
void display()
{
    
}