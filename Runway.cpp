#include "Runway.h"
#include<iostream>

using namespace std;




Runway::Runway(int limit)
/*
Post:  The Runway data members are initialized to record no
       prior Runway use and to record the limit on queue sizes.
*/

{
    queue_limit = limit;
    num_land_requests = num_takeoff_requests = 0;
    num_landings = num_takeoffs = 0;
    num_land_refused = num_takeoff_refused = 0;
    num_land_accepted = num_takeoff_accepted = 0;
    land_wait = takeoff_wait = idle_time = 0;
}


Error_code Runway::can_land(const Plane& current)
/*
Post:  If possible, the Plane current is added to the
       landing Queue; otherwise, an Error_code of overflow is
       returned. The Runway statistics are updated.
Uses:  class Extended_queue.
*/

{
    Error_code result;
    if (landing.size() < queue_limit)
        result = landing.append(current);
    else
        result = fail;
    num_land_requests++;

    if (result != success)
        num_land_refused++;
    else
        num_land_accepted++;

    return result;
}


Error_code Runway::can_depart(const Plane& current)
/*
Post:  If possible, the Plane current is added to the
       takeoff Queue; otherwise, an Error_code of overflow is
       returned. The Runway statistics are updated.
Uses:  class Extended_queue.
*/

{
    Error_code result;
    if (takeoff.size() < queue_limit)
        result = takeoff.append(current);
    else
        result = fail;
    num_takeoff_requests++;
    if (result != success)
        num_takeoff_refused++;
    else
        num_takeoff_accepted++;

    return result;
}


Runway_activity Runway::activity(int time, Plane& moving)
/*
Post:  If the landing Queue has entries, its front
       Plane is copied to the parameter moving
       and a result  land is returned. Otherwise,
       if the takeoff Queue has entries, its front
       Plane is copied to the parameter moving
       and a result  takeoff is returned. Otherwise,
       idle is returned. Runway statistics are updated.
Uses:  class Extended_queue.
*/

{
    Runway_activity in_progress;
    if (!landing.empty()) {
        landing.retrieve(moving);
        land_wait += time - moving.started();
        num_landings++;
        in_progress = land;
        landing.serve();
    }

    else if (!takeoff.empty()) {
        takeoff.retrieve(moving);
        takeoff_wait += time - moving.started();
        num_takeoffs++;
        in_progress = takeoff_activity;
        takeoff.serve();
    }

    else {
        idle_time++;
        in_progress = idle;
    }
    return in_progress;
}


void Runway::shut_down(int time) const
/*
Post: Runway usage statistics are summarized and printed.
*/

{
    cout << "Simulation has concluded after " << time << " time units." << endl
        << "Total number of planes processed "
        << (num_land_requests + num_takeoff_requests) << endl
        << "Total number of planes asking to land "
        << num_land_requests << endl
        << "Total number of planes asking to take off "
        << num_takeoff_requests << endl
        << "Total number of planes accepted for landing "
        << num_land_accepted << endl
        << "Total number of planes accepted for takeoff "
        << num_takeoff_accepted << endl
        << "Total number of planes refused for landing "
        << num_land_refused << endl
        << "Total number of planes refused for takeoff "
        << num_takeoff_refused << endl
        << "Total number of planes that landed "
        << num_landings << endl
        << "Total number of planes that took off "
        << num_takeoffs << endl
        << "Total number of planes left in landing queue "
        << landing.size() << endl
        << "Total number of planes left in takeoff queue "
        << takeoff.size() << endl;
    cout << "Percentage of time runway idle "
        << 100.0 * ((float)idle_time) / ((float)time) << "%" << endl;
    cout << "Average wait in landing queue "
        << ((float)land_wait) / ((float)num_landings) << " time units";
    cout << endl << "Average wait in takeoff queue "
        << ((float)takeoff_wait) / ((float)num_takeoffs)
        << " time units" << endl;
    cout << "Average observed rate of planes wanting to land "
        << ((float)num_land_requests) / ((float)time)
        << " per time unit" << endl;
    cout << "Average observed rate of planes wanting to take off "
        << ((float)num_takeoff_requests) / ((float)time)
        << " per time unit" << endl;
}


EnhancedRunway::EnhancedRunway(int limit) : Runway(limit) {}

Error_code EnhancedRunway::can_land(const Plane& current) {
    if (landing.size() < queue_limit) {
        landing.append(current);
        ++num_land_requests;
        return success;
    }
    else {
        ++num_land_refused;
        return fail;
    }
}

Error_code EnhancedRunway::can_depart(const Plane& current) {
    if (takeoff.size() < queue_limit) {
        takeoff.append(current);
        ++num_takeoff_requests;
        return success;
    }
    else {
        ++num_takeoff_refused;
        return fail;
    }

}

Runway_activity EnhancedRunway::activity(int time, Plane& moving)  {
    Runway_activity in_progress;
    if (!landing.empty()) {
        in_progress = land;
        landing.retrieve(moving);
        land_wait += time - moving.started();
        ++num_landings;
        landing.serve();
    }
    else if (!takeoff.empty()) {
        in_progress = takeoff_activity;
        takeoff.retrieve(moving);
        takeoff_wait += time - moving.started();
        ++num_takeoffs;
        takeoff.serve();
    }
    else {
        in_progress = idle;
        ++idle_time;
    }
    return in_progress;
}

EnhancedRunwayP3::EnhancedRunwayP3(int limit) : Runway(limit) {}

Error_code EnhancedRunwayP3::can_land(const Plane& current) {

    

    if (landing.size() < queue_limit) {
        landing.append(current);
        ++num_land_requests;
        return success;
    }
    else if (takeoff.empty()) {
        // Both runways can be used for landing if takeoff queue is empty
        landing.append(current);
        ++num_land_requests;
        return success;
    }
    else {
        ++num_land_refused;
        return fail;
    }
}

Error_code EnhancedRunwayP3::can_depart(const Plane& current) {

    if (takeoff.size() < queue_limit) {
        takeoff.append(current);
        ++num_takeoff_requests;
        return success;
    }
    else if (landing.empty()) {
        // Both runways can be used for takeoff if landing queue is empty
        takeoff.append(current);
        ++num_takeoff_requests;
        return success;
    }
    else {
        ++num_takeoff_refused;
        return fail;
    }
}

Runway_activity EnhancedRunwayP3::activity(int time, Plane& moving) {
    Runway_activity in_progress = idle;

    // Check if a plane is waiting to land
    if (!landing.empty()) {
        in_progress = land;
        landing.retrieve(moving);
        land_wait += time - moving.started();
        ++num_landings;
        landing.serve();
    }
    // If no planes are waiting to land, check if a plane is waiting to take off
    else if (!takeoff.empty()) {
        in_progress = takeoff_activity;
        takeoff.retrieve(moving);
        takeoff_wait += time - moving.started();
        ++num_takeoffs;
        takeoff.serve();
    }
    // If neither queue has any planes, the runway is idle
    else {
        idle_time++;
    }

    // Check if a plane is waiting to land or take off, and there is an empty runway available
    if (in_progress == idle) {
        Plane temp;
        // Check if there is a plane waiting to land and a runway is available
        if (!landing.empty() && landing.retrieve(temp) == success && temp.started() <= time) {
            in_progress = land;
            moving = temp;
            land_wait += time - moving.started();
            ++num_landings;
            landing.serve();
        }
        // Check if there is a plane waiting to take off and a runway is available
        else if (!takeoff.empty() && takeoff.retrieve(temp) == success && temp.started() <= time) {
            in_progress = takeoff_activity;
            moving = temp;
            takeoff_wait += time - moving.started();
            ++num_takeoffs;
            takeoff.serve();
        }
    }

    return in_progress;
}

ThreeRunwayEnhancedRunway::ThreeRunwayEnhancedRunway(int limit)
    : Runway(limit)
{
}

Error_code ThreeRunwayEnhancedRunway::can_land(const Plane& current)
{
    if (landing.size() < queue_limit) {
        landing.append(current);
        ++num_land_requests;
        return success;
    }
    else if (takeoff.empty() && third_runway.empty()) {
        // Both runways can be used for landing if takeoff and third runway queues are empty
        landing.append(current);
        ++num_land_requests;
        return success;
    }
    else if (!third_runway.empty()) {
        // Use the third runway for landing if available
        third_runway.append(current);
        ++num_land_requests;
        return success;
    }
    else {
        ++num_land_refused;
        return fail;
    }
}

Error_code ThreeRunwayEnhancedRunway::can_depart(const Plane& current)
{
    if (takeoff.size() < queue_limit) {
        takeoff.append(current);
        ++num_takeoff_requests;
        return success;
    }
    else if (landing.empty() && third_runway.empty()) {
        // Both runways can be used for takeoff if landing and third runway queues are empty
        takeoff.append(current);
        ++num_takeoff_requests;
        return success;
    }
    else if (!third_runway.empty()) {
        // Use the third runway for takeoff if available
        third_runway.append(current);
        ++num_takeoff_requests;
        return success;
    }
    else {
        ++num_takeoff_refused;
        return fail;
    }
}

Runway_activity ThreeRunwayEnhancedRunway::activity(int time, Plane& moving)
{
    Runway_activity in_progress = idle;

    // Check if a plane is waiting to land
    if (!landing.empty()) {
        in_progress = land;
        landing.retrieve(moving);
        land_wait += time - moving.started();
        ++num_landings;
        landing.serve();
    }
    // If no planes are waiting to land, check if a plane is waiting to take off
    else if (!takeoff.empty()) {
        in_progress = takeoff_activity;
        takeoff.retrieve(moving);
        takeoff_wait += time - moving.started();
        ++num_takeoffs;
        takeoff.serve();
    }
    // If neither queue has any planes, check if a plane is waiting to use the third runway
    else if (!third_runway.empty()) {
        in_progress = takeoff_activity;
        third_runway.retrieve(moving);
        takeoff_wait += time - moving.started();
        ++num_takeoffs;
        third_runway.serve();
    }
    // If none of the queues have any planes, the runway is idle
    else {
        idle_time++;
    }

    // Check if a plane is waiting to land or take off, and there is an empty runway available
    if (in_progress == idle && (!landing.empty() || !takeoff.empty() || !third_runway.empty())) {
        in_progress = takeoff_activity;
        if (!landing.empty()) {
            landing.retrieve(moving);
            land_wait += time - moving.started();
            ++num_landings;
            landing.serve();
        }
        else if (!takeoff.empty()) {
            in_progress = takeoff_activity;
            takeoff.retrieve(moving);
            takeoff_wait += time - moving.started();
            ++num_takeoffs;
            takeoff.serve();
        }
        else if (!third_runway.empty()) {
            in_progress = takeoff_activity;
            third_runway.retrieve(moving);
            takeoff_wait += time - moving.started();
            ++num_takeoffs;
            third_runway.serve();
        }
    }

    return in_progress;
}
