/*!
* \file rw-main.cpp
* \author Robert 'Bob' Caddy (rvc@pitt.edu)
* \brief The main function for the random walk program
* \date 2021-06-30
*
* \copyright Copyright (c) 2021
*
*/

#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <omp.h>

#include "utility.h"

using std::cout;
using std::cin;
using std::endl;

/*!
 * \brief Main function for the random walk program. Controls execution of the
 * various components and provides user output.
 *
 * \param argc Total number of command line arguments
 * \param argv Array of arguments.
 * \return int 0 if successful
 */
int main(int argc, char const *argv[])
{
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    // Setting for program
    int const numWalkers = 1E6;  /// The total number of walkers to make
    int const numSteps   = 1E4;  /// The number of steps that each walker should take
    std::string savePath;        /// The path to save the output to
    int ompNumThreads;           /// Number of OMP threads to use

    // Gather inputs
    if (argc == 2) savePath = argv[1];  /// The path to save the file to
    if (argc == 3)
    {
        ompNumThreads = std::stoi(argv[2]);
    }
    else
    {
        ompNumThreads = omp_get_max_threads();
    }
    cout << "Running with " << ompNumThreads << " OpenMP threads" << endl;

    // Initialize/Seed the random number generator
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<double> distr(0., 1);

    // Declare and allocate the arrays of final positions
    stdVector1D xPosFinal(numWalkers, 0.),
                yPosFinal(numWalkers, 0.);

    // Setup parallel region
    #pragma omp parallel num_threads(ompNumThreads)
    {
        #pragma omp for
        // Main loop. Loop through each walker
        for (size_t i = 0; i < numWalkers; i++)
        {
            double xTemp=0., yTemp=0.;

            // Loop through each step for each walker
            for (size_t j = 0; j < numSteps; j++)
            {
                // generate a random double
                double randNum = distr(eng);

                // Choose which value to update
                if (randNum < 0.25)
                {
                    xTemp += 1.;
                }
                else if (randNum < 0.5)
                {
                    xTemp -= 1.;
                }
                else if (randNum < 0.75)
                {
                    yTemp += 1.;
                }
                else
                {
                    yTemp -= 1.;
                }
            }
        // Save the last position of the walker
        xPosFinal[i] = xTemp;
        yPosFinal[i] = yTemp;
        }
    }

    // Save the vectors to a csv file
    utils::saveState(xPosFinal, yPosFinal, savePath);

    // Stop timer and print execution time. Time options are nanoseconds,
    // microseconds, milliseconds, seconds, minutes, hours. To pick one just
    // change `using FpTime` and the cout statement suitably.
    auto stop = std::chrono::high_resolution_clock::now();
    using FpTime = std::chrono::duration<float, std::chrono::seconds::period>;
    static_assert(std::chrono::treat_as_floating_point<FpTime::rep>::value,"Rep required to be floating point");
    auto duration = FpTime(stop - start);
    cout << "Time to execute: " << duration.count() << " seconds";
    return 0;
}