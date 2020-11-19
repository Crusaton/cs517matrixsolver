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
/**
 * The int main(int argc, char *argv[]) function is provided by Professor Kennedy from Old Dominion University's Computer Science Department.
 * 
 * All other code:
 * Written By: Paul Rodriguez
 * Due Date: 11/20/2020
 * Course: CS 517
 * Name: Semester Project
 */

void outputToFile(std::string, std::vector<double>&);
std::string outputFileNameGen(string, int);
vector<double> coreTempContainer(int, std::vector<CoreTempReading>);
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
    
    int coreCount = readings[0].second.size();
    vector<string> outputFileNames;
    vector<double> coreTemps;

    for(int i = 0; i < coreCount; i++)
    {
        std::string outputFileName = outputFileNameGen(inputFileName ,i);
        coreTemps = coreTempContainer(i, readings);
        outputToFile(outputFileName, coreTemps);
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

/**
 * Generate the output file name
 * 
 * @param string inputFileName representing the original input file name.
 * @param int i representing the current core counter.
 * 
 * @return output file name stored in a string.
 *  
 */
std::string outputFileNameGen(string inputFileName, int coreNumber)
{

    std::string core = "-core-";
    std::string ext = ".txt";
    std::string coreCounttoString = to_string(coreNumber);
    std::string outputFileName = inputFileName + core + coreCounttoString + ext;

    return outputFileName;

}

/**
 * Parse all the data from original file and seperate it.
 * 
 * @param int coreNumber representing the current core number being processed.
 * @param std::vector<CoreTempReading> readings representing the vector full of all unprocessed data.
 * 
 * @return specified cores temperature readings stored in a vector<double>.
 *
 */
vector<double> coreTempContainer(int coreNumber, std::vector<CoreTempReading> readings)
{
    vector<double> coreTemps;
    for(int j = 0; j < readings.size(); j++)
    {
        double coreTemp = readings[j].second[coreNumber];

        coreTemps.push_back(coreTemp);
    }

    return coreTemps;
}

/**
 * Output all of the processed data to respective text file
 * 
 * @param std::string outputFileName representing the output file name
 * @param std::vector<double>& coreTemps representing the processed data
 * 
 */
void outputToFile(std::string outputFileName, std::vector<double>& coreTemps)
{
    ofstream myfile(outputFileName);

    for(int j = 0; j < coreTemps.size(); j++)
    {
        myfile << coreTemps[j] << std::endl;
    }

    myfile.close();
}