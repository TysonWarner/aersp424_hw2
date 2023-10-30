#include <iostream>
#include <string>
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
    // Might need to add stuff from main
public:
    // Constructor taking in two strings “from” and “to” as input arguments
    Plane();
    string from, to;
    // Virtual Deconstructor
    virtual ~Plane();
    // “operate” function with a double variable “dt” as an input, and return nothing (void)
    void operate(double dt)
    {

    }
    // Adding required "get" functions
    
};

int main()
{
    // Question 1 (Might need to cut and paste into private part of class Plane)
    double distance[3] = { 160,640,220 };
    string location1[3] = { "SCE","SCE","SCE" };
    string location2[3] = { "PHL","ORD","EWR" };

    return 0;
}