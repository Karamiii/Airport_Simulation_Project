#pragma once
#include"Queue.h"
#include"Plane.h"


enum Runway_activity { idle, land, takeoff_activity };


class Runway {
public:
	Runway(int limit);
	virtual Error_code can_land(const Plane& current);
	virtual Error_code can_depart(const Plane& current);
	virtual Runway_activity activity(int time, Plane& moving);
	void shut_down(int time) const;

protected:
	Extended_queue landing;
	Extended_queue takeoff;
	int queue_limit;
	int num_land_requests;        //  number of planes asking to land
	int num_takeoff_requests;     //  number of planes asking to take off
	int num_landings;             //  number of planes that have landed
	int num_takeoffs;             //  number of planes that have taken off
	int num_land_accepted;        //  number of planes queued to land
	int num_takeoff_accepted;     //  number of planes queued to take off
	int num_land_refused;         //  number of landing planes refused
	int num_takeoff_refused;      //  number of departing planes refused
	int land_wait;                //  total time of planes waiting to land
	int takeoff_wait;             //  total time of planes waiting to take off
	int idle_time;                //  total time runway is idle
};



class EnhancedRunway : public Runway {
public:
	EnhancedRunway(int limit);
	Error_code can_land(const Plane& current) override;
	Error_code can_depart(const Plane& current) override;
	Runway_activity activity(int time, Plane& moving) override;
};


class EnhancedRunwayP3 : public Runway {
public:
	EnhancedRunwayP3(int limit);
	Error_code can_land(const Plane& current) override;
	Error_code can_depart(const Plane& current) override;
	Runway_activity activity(int time, Plane& moving) override;
};


class ThreeRunwayEnhancedRunway : public Runway {
public:
	ThreeRunwayEnhancedRunway(int limit);
	Error_code can_land(const Plane& current) override;
	Error_code can_depart(const Plane& current) override;
	Runway_activity activity(int time, Plane& moving) override;

private:
	Extended_queue third_runway;
};




