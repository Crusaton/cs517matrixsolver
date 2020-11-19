#ifndef PARSE_TEMPS_H_INCLUDED
#define PARSE_TEMPS_H_INCLUDED

#include <iostream>
#include <iterator>
#include <algorithm>
#include <utility>

/**
 * A pair of values where the
 *   - _first_ attribute represents the time at which the readind was taken
 *   - _second is a vector with _n_ temperature readings, where _n_ is the
 *     number of CPU Cores
 */
using CoreTempReading = std::pair<int, std::vector<double>>;

/**
 * Take an input file and time-step size and parse all core temps.
 *
 * @tparam CoreTempReadingContainer type of container to use (it must implement
 *     push_back and emplace_back)
 *
 * @param original_temps an input file
 * @param step_size time-step in seconds
 *
 * @return a vector of 2-tuples (pairs) containing time step and core
 *         temperature readings
 */
template<typename CoreTempReadingContainer>
CoreTempReadingContainer parse_raw_temps(std::istream& original_temps,
                                         int step_size = 30)
{
    CoreTempReadingContainer allTheReadings;

    // Input Parsing Variables
    int step = 0;
    std::string line;

    while (getline(original_temps, line)) {
        std::istringstream input(line);

        std::vector<double> next_temperature_set;
        std::transform(std::istream_iterator<std::string>(input),
                       std::istream_iterator<std::string>(),
                       std::back_inserter(next_temperature_set),
                       [](const std::string& raw_reading) -> double {
                           return stod(raw_reading);
                       });

        allTheReadings.emplace_back(step, next_temperature_set);
        step += step_size;
    }

    return allTheReadings;
}

#endif
