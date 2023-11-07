#include <iostream>
#include <string>
#include <map>
#include <random>
using namespace std;

class Plane
{
protected:
    // Adding required variable
    double wait_time;
private:
    // Adding required variables
    double pos, vel, distance, loiter_time;
    bool at_SCE;
    string origin, destination;
    // Container from Question 1
    map<pair<string, string>, double> DistanceMap;
public:
    // Constructor taking in two strings “from” and “to” as input arguments
    Plane(string from, string to)
        : pos(0.0), vel(0.0), wait_time(0.0), loiter_time(0.0), at_SCE(0){   // Initializing values at zero
        DistanceMap[{"SCE", "PHL"}] = 160.0;
        DistanceMap[{"SCE", "ORD"}] = 640.0;
        DistanceMap[{"SCE", "EWR"}] = 220.0;
        DistanceMap[{"PHL", "SCE"}] = 160.0;
        DistanceMap[{"ORD", "SCE"}] = 640.0;
        DistanceMap[{"EWR", "SCE"}] = 220.0;
        // Storing input strings
        origin = from;
        destination = to;
        // Setting value of distance
        distance = DistanceMap[{origin, destination}];
    }
    // Virtual Deconstructor
    virtual ~Plane()
    {
    }
    // “operate” function with a double variable “dt” as an input, and return nothing (void)
    void operate(double dt)
    {
        if (loiter_time != 0.0)
        {
            loiter_time -= dt;
        }
        else
        {
            if (wait_time != 0.0)
            {
                wait_time -= dt;
            }
            else
            {
                if (pos < distance)
                {
                    pos += vel * dt;
                    at_SCE = 0;
                }
                else
                {
                    if (destination == "SCE")
                    {
                        at_SCE = 1;
                    }
                    else
                    {
                        time_on_ground();
                        swap(origin,destination);
                        pos = 0.0;
                    }
                }
            }
        }
    }
    // Adding required "get" functions
    double getPos() const
    {
        return pos;
    }
    double getVel() const
    {
        return vel;
    }
    double getLoiter_time() const
    {
        return loiter_time;
    }
    string getOrigin() const
    {
        return origin;
    }
    string getDestination() const
    {
        return destination;
    }
    bool getAt_SCE() const
    {
        return at_SCE;
    }
    // Adding required "set" functions
    void setVel(double vel)
    {
        vel = vel;
    }
    void setLoiter_time(double loiter_time)
    {
        loiter_time = loiter_time;
    }
    // Required functions "distance_to_SCE", "time_on_ground", "plane_type", "draw_from_normal_dist"
    double distance_to_SCE()
    {
        if (destination=="SCE")
        {
            // Returning difference between distance and position only if destination is at SCE
            return (distance-pos); 
        }
    }
    virtual double time_on_ground()
    {
        return wait_time;
    }
    virtual string plane_type()
    {
        return "GA";    // Returns "GA"
    }
    static double draw_from_normal_dist(double m, double sd)
    {
        std::random_device rd{};
        std::mt19937 gen{ rd() };
        std::normal_distribution<> d{ m, sd };
        return d(gen);
    }
};

class Airliner : public Plane
{
private:
    string Airline;
public:
    // Constructor for Airliner
    Airliner(string Airline, string from, string to) : Plane(from, to)
    {
        Airline = Airline;
    }
    // Deconstructor for Airliner
    virtual ~Airliner()
    {
    }
    // Overridden function "plane_type"
    virtual string plane_type()
    {
        return Airline;
    }
    // Overridden function "time_on_ground"
    virtual double time_on_ground()
    {
        wait_time = draw_from_normal_dist(1800.0, 600.0);
        return wait_time;
    }
};

class GeneralAviation : public Plane
{
public:
    // Constructor for GeneralAviation
    GeneralAviation(string from, string to) : Plane(from, to)
    {
    }
    // Deconstructor for GeneralAviation
    virtual ~GeneralAviation()
    {
    }
    // Overridden function "time_on_ground"
    virtual double time_on_ground()
    {
        wait_time = draw_from_normal_dist(600.0, 60.0);
        return wait_time;
    }
};

int main()
{
    // Instantiating and settin velocities the seven flights from the table on number 5
    Airliner flight1("AA","SCE","PHL"); flight1.setVel(470.0);
    Airliner flight2("UA","SCE","ORD"); flight1.setVel(515.0);
    Airliner flight3("UA","SCE","EWR"); flight1.setVel(480.0);
    Airliner flight4("AA","SCE","ORD"); flight1.setVel(500.0);
    GeneralAviation flight5("SCE","PHL"); flight1.setVel(140.0);
    GeneralAviation flight6("SCE","EWR"); flight1.setVel(160.0);
    GeneralAviation flight7("SCE","ORD"); flight1.setVel(180.0);
    // Timestep
    double timestep = 100;
    // while (true)
    // {
    //     flight1.operate(timestep);
    //     cout << flight1.getPos() << endl;
    // }
    cout << timestep << endl;
    return 0;
}