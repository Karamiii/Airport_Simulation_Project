#include"Plane.h"
#include"Runway.h"
#include"Queue.h"
#include"functions.h"
#include<iostream>
#include<string>
#include <sstream>


using namespace std;


// Create a template function so it can use different objects without having to make seperate functions for
// each Object of different classes
template<typename T>
void run_simulation(T& small_airport, int end_time, int queue_limit, double arrival_rate, double departure_rate);



int get_valid_input() {
    int num;
    string input;
    bool valid_input = false;

    while (!valid_input) {

        getline(cin, input);

        stringstream ss(input);
        if (ss >> num) {
            // Conversion successful
            valid_input = true;
        }
        else {
            // Invalid input
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();

            cout << "Your choce: ";
        }
    }

    return num;
}



int main()     //  Airport simulation program
/*
Pre:  The user must supply the number of time intervals the simulation is to
      run, the expected number of planes arriving, the expected number
      of planes departing per time interval, and the
      maximum allowed size for runway queues.
Post: The program performs a random simulation of the airport, showing
      the status of the runway at each time interval, and prints out a
      summary of airport operation at the conclusion.
Uses: Classes Runway/EnhancedRunway/EnhancedRunwayP3, Plane, Random and functions run_idle, initialize.
*/

{
    int end_time;            //  time to run simulation
    int queue_limit;         //  size of Runway queues
    double arrival_rate, departure_rate;
    int choice;
    bool quit = false;



    while (!quit) {
        cout << "Airport Simulator Menu" << endl;
        cout << "1. Run P1 version of Airport simulator" << endl;
        cout << "2. Run P2 version of Airport simulator" << endl;
        cout << "3. Run P3 version of Airport simulator" << endl;
        cout << "4. Run P4 version of Airport simulator" << endl;
        cout << "5. Run P5 version of Airport simulator" << endl;
        cout << "6. Quit the program" << endl << endl;
        cout << "Your choice: ";
        choice = get_valid_input();

        cout << endl << endl;


        switch (choice)
        {
        case 1:
        {

            cout << "Running P1 version of simulator" << endl << endl;
            cout << "Version uses 2 runways both can be used for landing and takeoff" << endl << endl;
            initialize(end_time, queue_limit, arrival_rate, departure_rate);
            // Create the Class Objects to Heap in order to preserve stack memory
            Runway* small_airport = new Runway(queue_limit);
            run_simulation(*small_airport, end_time, queue_limit, arrival_rate, departure_rate);
            // Delete Obeject from heap after the simulation is done
            delete small_airport;
            break;
        }
        case 2:
        {
            cout << "Running P2 version of simulator" << endl << endl;
            cout << "Version uses 2 runways. One only for takeoffs and one only for landings" << endl << endl;
            initialize(end_time, queue_limit, arrival_rate, departure_rate);
            EnhancedRunway* small_airport = new EnhancedRunway(queue_limit);
            run_simulation(*small_airport, end_time, queue_limit, arrival_rate, departure_rate);
            delete small_airport;
            break;
        }

        case 3:
        {
            cout << "Running P3 version of simulator" << endl << endl;
            cout << "Version uses 2 runways. One mainly for takeoffs and one mainly for landings. If one of the\n";
            cout << "queues is empy, then both runways can be used for the other queue. Also if the landing queue is fulland another plane arrives to land, then takeoffs will be stopped\n";
            cout << "and both runways used to clear the backlog of landing planes.\n" << endl;
            initialize(end_time, queue_limit, arrival_rate, departure_rate);
            EnhancedRunwayP3* small_airport = new EnhancedRunwayP3(queue_limit);
            run_simulation(*small_airport, end_time, queue_limit, arrival_rate, departure_rate);
            delete small_airport;
            break;
        }


        case 4:
        {
            cout << "Running P4 version of simulator" << endl << endl;
            initialize(end_time, queue_limit, arrival_rate, departure_rate);
            ThreeRunwayEnhancedRunway* small_airport = new ThreeRunwayEnhancedRunway(queue_limit);
            run_simulation(*small_airport, end_time, queue_limit, arrival_rate, departure_rate);
            delete small_airport;

        }
        case 6:
        {

            cout << "Quitting program..." << endl << endl << endl;
            quit = true;
            break;

        }
        default:
        {
            cout << "Invalid choice. Please choose a number from 1 to 6.\n\n" << endl;
            break;
        }
        }


    }

    return 0;
}



template<typename T>
void run_simulation(T& small_airport, int end_time, int queue_limit, double arrival_rate, double departure_rate)
{
    int flight_number = 0;
    Random variable;
    for (int current_time = 0; current_time < end_time; current_time++) { //  loop over time intervals
        int number_arrivals = variable.poisson(arrival_rate);  //  current arrival requests
        for (int i = 0; i < number_arrivals; i++) {
            Plane current_plane(flight_number++, current_time, arriving);
            if (small_airport.can_land(current_plane) != success)
                current_plane.refuse();
        }

        int number_departures = variable.poisson(departure_rate); //  current departure requests
        for (int j = 0; j < number_departures; j++) {
            Plane current_plane(flight_number++, current_time, departing);
            if (small_airport.can_depart(current_plane) != success)
                current_plane.refuse();
        }

        Plane moving_plane;
        switch (small_airport.activity(current_time, moving_plane)) {
            //  Let at most one Plane onto the Runway at current_time.
        case land:
            moving_plane.land(current_time);
            break;
        case takeoff_activity:
            moving_plane.fly(current_time);
            break;
        case idle:
            run_idle(current_time);
        }
    }
    small_airport.shut_down(end_time);
    cout << endl << endl;
}
