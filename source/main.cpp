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
#include <cmath>

#include "parseTemps.h"

using namespace std;
/**
 * The int main(int argc, char *argv[]) function's input validation and parsing is provided by Professor Kennedy from Old Dominion University's Computer Science Department.
 * 
 * All other code:
 * Written By: Paul Rodriguez
 * Due Date: 11/20/2020
 * Course: CS 517
 * Name: Semester Project
 * 
 */

void outputToFile(int, std::string, std::vector<double>&, double&, double&, std::vector<pair<double,float>>&);
std::string outputFileNameGen(string, int);
vector<double> coreTempContainer(int, std::vector<CoreTempReading>);
void globalLeastSquaresApproximation(int, double&, double&, std::vector<double>&);
void piecewiseLinearInterpolation(int, std::vector<double>&, std::vector<pair<double,float>>&);
double cOfZeroCalc(double, double, double);
float cOfOneCalc(double, double, double, double);


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
    vector<pair<double,float>> pieceLinInt;
    vector<double> coreTemps;
    double slope;
    double intercept;

    /**
     * 
     * Main loop that will drive program.
     * Will iterate up to core count from input file.
     * Note: This should be scalable to use on larger or smaller core CPU's (I think?)
     * 
     **/
    for(int i = 0; i < coreCount; i++)
    {
        coreTemps = coreTempContainer(i, readings);
        int sizeOfCoreTemps = (coreTemps.size() - 1);
        std::string outputFileName = outputFileNameGen(inputFileName ,i);
        globalLeastSquaresApproximation(sizeOfCoreTemps, slope, intercept, coreTemps);
        piecewiseLinearInterpolation(sizeOfCoreTemps, coreTemps, pieceLinInt);
        outputToFile(sizeOfCoreTemps, outputFileName, coreTemps, slope, intercept, pieceLinInt);
        pieceLinInt.clear();
    }

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
    int readingsSize = readings.size();
    // For loop that will iterate through unprocessed data vector and push specific core number's data to push into new vector.
    for(int j = 0; j < readingsSize; j++)
    {
        double coreTemp = readings[j].second[coreNumber];

        coreTemps.push_back(coreTemp);
    }

    return coreTemps;
}

/**
 * Output all of the processed data to respective text file
 * 
 * @param int sizeOfCoreTemps representing the size of core temperature vector. (I only did this to avoid warning flags being thrown by Ubuntu..comparing int to long).
 * @param std::string outputFileName representing the output file name.
 * @param std::vector<double>& coreTemps representing the processed data.
 * @param double slope representing the slope of the line.
 * @param double intercept representing the intercept of the line.
 * @param std::vector<pair<double,float>>& pieceLinInt representing the vector containing all of the piecewise polynomial values.
 * 
 */
void outputToFile(int sizeOfCoreTemps, std::string outputFileName, std::vector<double>& coreTemps, double& slope, double& intercept, std::vector<pair<double, float>>& pieceLinInt)
{
    ofstream myfile(outputFileName);
    std::string outputX = " <= x <";
    int y_i = 0;

    myfile << std::setw(5) << "xk" << std::setw(5) << " <= x <" << std::setw(8) <<  "xk+1;" << " y_i" << std::setw(7) << "=" << std::setw(11) << "c0 " << "     +" << "        c1x;" << " type" << std::endl;

    // For loop to iterate through all of the interpolation polynomials and output them to the file.
    // Note: Find way to make this not look ugly but keep proper formatting.
    for(int z = 0; z < sizeOfCoreTemps; z++)
    {
        ios init(NULL);
        int xk = (z*30);
        int xkplusone = ((z+1)*30);
        myfile << fixed << setprecision(4);
        myfile << std::setw(5) << xk << outputX << std::setw(7) << xkplusone << ";" << std::setw(2) << " y_";
        myfile << setw(7);
        myfile << std::left << y_i;
        myfile << "=";
        //myfile.copyfmt(init);
        myfile << setw(15);
        myfile << std::right << pieceLinInt[z].first << " +" << setw(10) << pieceLinInt[z].second << "x; interpolation" << std::endl;
        y_i++;
        myfile.copyfmt(init);
    }
    pieceLinInt.clear();
    myfile.close();
}

/**
 * 
 * Calculates the global least squares approximation for all temperatures pulled from core.
 * 
 * @param int sizeOfCoreTemps representing the size of core temperature vector. (I only did this to avoid warning flags being thrown by Ubuntu..comparing int to long).
 * @param double& slope representing the slope of the polynomial.
 * @param double& intercept representing the intercept of the polynomial.
 * @param std::vector<double>& coreTemps representing the vector containing the core temperatures.
 * 
 */
void globalLeastSquaresApproximation(int sizeOfCoreTemps, double& slope, double& intercept, std::vector<double>& coreTemps)
{
    double xsum = 0;
    double x2sum = 0;
    double ysum = 0;
    double xysum = 0;

    for(int i = 0; i < sizeOfCoreTemps; i++)
    {
        xsum = xsum+coreTemps[i];
        ysum = ysum+i;
        x2sum = x2sum+pow(coreTemps[i], 2);
        xysum = xysum+coreTemps[i]*i;
    }

    slope = (sizeOfCoreTemps * xysum - xsum * ysum)/(sizeOfCoreTemps * x2sum - xsum * xsum);
    intercept = (x2sum * ysum - xsum * xysum)/(x2sum * sizeOfCoreTemps - xsum * xsum);

}

/**
 * 
 * Calculates the piecewise linear interpolation for all temperatures pulled from core.
 * 
 * @param int sizeOfCoreTemps representing the size of core temperature vector. (I only did this to avoid warning flags being thrown by Ubuntu..comparing int to long).
 * @param std::vector<double>& coreTemps representing the vector containing all of the core temperatures.
 * @param std::vector<pair<double,float>>& pieceLinInt representing the vector containing all of the piecewise polynomial values.
 * 
 */
void piecewiseLinearInterpolation(int sizeOfCoreTemps, std::vector<double>& coreTemps, std::vector<pair<double,float>>& pieceLinInt)
{
    double x0, x1, f0, f1, c0;
    float c1;

    for(int i = 0; i < sizeOfCoreTemps; i++)
    {
        x0 = (i * 30);
        f0 = coreTemps[i];
        x1 = ((i + 1) * 30);
        f1 = coreTemps[i+1];
        c1 = cOfOneCalc(x0, f0, x1, f1);
        c0 = cOfZeroCalc(x0, f0, c1);

        pieceLinInt.push_back(make_pair(c0, c1));
    }
}

/**
 * 
 * Returns c0 represented by a floating point number.
 * 
 * @param double x0 representing the x coordinate of the first point.
 * @param double f0 representing the y coordinate of the first point.
 * @param double c1 representing the slope of the line (retrieved by cOfOneCalc).
 * 
 * @return float c0
 * 
 */
double cOfZeroCalc(double x0, double f0, double c1)
{
    double c0 = (f0 - (c1 * x0));

    return c0;
}

/**
 * 
 * Returns c1 represented by a floating point number.
 * 
 * @param double x0 representing the x coordinate of the first point.
 * @param double f0 representing the y coordinate of the first point.
 * @param double x1 representing the x coordinate of the second point.
 * @param double f1 representing the y coordinate of the second point.
 * 
 * @return float c1
 * 
 */
float cOfOneCalc(double x0, double f0, double x1, double f1)
{
    // Slope calculation
    double c1 = ((f1-f0)/(x1-x0));

    return c1;
}