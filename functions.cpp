#include "functions.h"
#include<iostream>

using namespace std;
#include <limits.h>
const int max_int = INT_MAX;
#include <math.h>
#include <time.h>



Random::Random(bool pseudo)
/*

Post: The values of seed, add_on, and multiplier
are initialized.  The seed is initialized randomly only if
pseudo == false.

*/
{
    if (pseudo) seed = 1;
    else seed = time(NULL) % max_int;
    multiplier = 2743;
    add_on = 5923;
}

double Random::random_real()
/*

Post: A random real number between 0 and 1 is returned.

*/
{
    double max = max_int + 1.0;
    double temp = reseed();
    if (temp < 0) temp = temp + max;
    return temp / max;
}

int Random::random_integer(int low, int high)
/*

Post: A random integer between low and high (inclusive)
is returned.

*/
{
    if (low > high) return random_integer(high, low);
    else return ((int)((high - low + 1) * random_real())) + low;
}

int Random::poisson(double mean)
/*

Post:
A random integer, reflecting a Poisson distribution
with parameter mean, is returned.

*/
{
    double limit = exp(-mean);
    double product = random_real();
    int count = 0;
    while (product > limit) {
        count++;
        product *= random_real();
    }
    return count;
}

int Random::reseed()
/*

Post:
The seed is replaced by a psuedorandom successor.

*/
{
    seed = seed * multiplier + add_on;
    return seed;
}





void initialize(int& end_time, int& queue_limit,
    double& arrival_rate, double& departure_rate)
    /*
    Pre:  The user specifies the number of time units in the simulation,
          the maximal queue sizes permitted,
          and the expected arrival and departure rates for the airport.
    Post: The program prints instructions and initializes the parameters
          end_time, queue_limit, arrival_rate, and departure_rate to
          the specified values.
    Uses: utility function user_says_yes
    */

{
    cout << "This program simulates an airport with only one runway." << endl
        << "One plane can land or depart in each unit of time." << endl;
    cout << "Up to what number of planes can be waiting to land "
        << "or take off at any time? " << flush;
    cin >> queue_limit;

    cout << "How many units of time will the simulation run?" << flush;
    cin >> end_time;

    bool acceptable;
    do {
        cout << "Expected number of arrivals per unit time?" << flush;
        cin >> arrival_rate;
        cout << "Expected number of departures per unit time?" << flush;
        cin >> departure_rate;
        if (arrival_rate < 0.0 || departure_rate < 0.0)
            cerr << "These rates must be nonnegative." << endl;
        else
            acceptable = true;

        if (acceptable && arrival_rate + departure_rate > 1.0)
            cerr << "Safety Warning: This airport will become saturated. " << endl;

    } while (!acceptable);
}


void run_idle(int time)
/*
Post: The specified time is printed with a message that the runway is idle.
*/
{
    cout << time << ": Runway is idle." << endl;
}


