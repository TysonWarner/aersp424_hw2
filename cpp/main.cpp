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
    map<pair<string, string>, int> DistanceMap;



public:
    // Constructor taking in two strings “from” and “to” as input arguments
    Plane(string& from, string& to)
        : pos(0.0), vel(0.0), distance(0.0), loiter_time(0.0), at_SCE(0){   // Initializing values at zero
        DistanceMap[{"SCE", "PHL"}] = 160;
        DistanceMap[{"SCE", "ORD"}] = 640;
        DistanceMap[{"SCE", "EWR"}] = 220;
        DistanceMap[{"PHL", "SCE"}] = 160;
        DistanceMap[{"ORD", "SCE"}] = 640;
        DistanceMap[{"EWR", "SCE"}] = 220;

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

        if (loiter_time!=0.0)
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
                if (pos<distance)
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
    virtual void time_on_ground()       // Might need to change "void"
    {
        
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

int main()
{
    


    return 0;
}