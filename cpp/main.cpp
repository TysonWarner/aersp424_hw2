#include <iostream>
#include <string>
#include <map>
#include <random>
#include <memory>
#include <vector>
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
        : pos(0.0), vel(0.0), loiter_time(0.0), wait_time(0.0), at_SCE(0){   // Initializing values at zero
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
    // Virtual Destructor
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
                if (wait_time < 0.0)
                {
                    wait_time = 0.0;
                }
            }
            else
            {
                if (pos < distance)
                {
                    pos += vel * dt;
                    if (pos > distance)
                    {
                        pos = distance;
                    }
                    at_SCE = 0;
                }
                else
                {
                    if (destination == "SCE")
                    {
                        at_SCE = 1;
                    }
                    time_on_ground();
                    swap(origin,destination);
                    pos = 0.0;
                }
            }
        }
    }
    // Adding required "get" functions
    double getPos()
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
        this->vel = vel;
    }
    void setLoiter_time(double loiter_time)
    {
        this->loiter_time = loiter_time;
    }
    // Required functions "distance_to_SCE", "time_on_ground", "plane_type", "draw_from_normal_dist"
    double distance_to_SCE()
    {
        if (destination=="SCE")
        {
            // Returning difference between distance and position only if destination is SCE
            double a = distance - pos;
            return a; 
        }
        return -1.0;
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
        this->Airline = Airline;
    }
    // Destructor for Airliner
    ~Airliner()
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
    // Destructor for GeneralAviation
    ~GeneralAviation()
    {
    }
    // Overridden function "time_on_ground"
    virtual double time_on_ground()
    {
        wait_time = draw_from_normal_dist(600.0, 60.0);
        return wait_time;
    }
};

class ATC
{
private:
    vector<shared_ptr<Plane>> registered_planes;
    int MAX_LANDED_PLANE_NUM = 2;
    int AIRSPACE_DISTANCE = 50;
public:
    // Constructor
    ATC()
    {
    }
    // Destructor
    ~ATC()
    {
    }
    // Function to register a plane
    void register_plane(shared_ptr<Plane> plane)
    {
        registered_planes.push_back(plane);
    }
    // Function to access the registered_planes container
    const vector<shared_ptr<Plane>>& get_registered_planes()
    {
        return registered_planes;
    }
    // Function to control traffic using the registered planes
    void control_traffic()
    {
        for (auto& plane : registered_planes)
        {
            bool at_SCE = plane->getAt_SCE();
            double loiter_time = plane->getLoiter_time();
            double distance_to_SCE = plane->distance_to_SCE();

            int landed_planes = 0;
            
            int i = 0;
            while (i < registered_planes.size())
            {
                landed_planes += at_SCE;
                i++;
            }
            if (i >= registered_planes.size())
            {
                if (landed_planes >= MAX_LANDED_PLANE_NUM)
                {
                    i = 0;
                    while (i < registered_planes.size())
                    {
                        if (at_SCE==0 && distance_to_SCE <= AIRSPACE_DISTANCE && loiter_time == 0)
                        {
                            loiter_time = 100.0;
                        }
                    }
                }
            }
        }
    }
    


};

int main()
{
    ATC atc; // Instantiating an ATC object

    // Registering planes
    atc.register_plane(make_shared<Airliner>("AA", "SCE", "PHL"));
    atc.register_plane(make_shared<Airliner>("UA","SCE","ORD"));
    atc.register_plane(make_shared<Airliner>("UA","SCE","EWR"));
    atc.register_plane(make_shared<Airliner>("AA","SCE","ORD"));
    atc.register_plane(make_shared<GeneralAviation>("SCE","PHL"));
    atc.register_plane(make_shared<GeneralAviation>("SCE","EWR"));
    atc.register_plane(make_shared<GeneralAviation>("SCE","ORD"));
    atc.get_registered_planes()[0]->setVel(470.0 / 3600.0);
    atc.get_registered_planes()[1]->setVel(515.0 / 3600.0);
    atc.get_registered_planes()[2]->setVel(480.0 / 3600.0);
    atc.get_registered_planes()[3]->setVel(500.0 / 3600.0);
    atc.get_registered_planes()[4]->setVel(140.0 / 3600.0);
    atc.get_registered_planes()[5]->setVel(160.0 / 3600.0);
    atc.get_registered_planes()[6]->setVel(180.0 / 3600.0);

    // Timestep
    double timestep = 10.0;
    // Calling position at each timestep 'dt'    
    int i = 0;
    while (true)
    {
        cout << "\nAt timestep " << i << ":\n\n";
        int planeNumber = 1;
        for (auto& plane : atc.get_registered_planes())
        {
            cout << "Position of plane " << planeNumber << ": " << plane->getPos() << endl;
            plane->operate(timestep);
            planeNumber++;
        }
        atc.control_traffic();
        i++;
    }




    // // Instantiating and settin velocities the seven flights from the table on number 5
    // Airliner flight1("AA","SCE","PHL"); flight1.setVel(470.0/3600.0);
    // Airliner flight2("UA","SCE","ORD"); flight2.setVel(515.0/3600.0);
    // Airliner flight3("UA","SCE","EWR"); flight3.setVel(480.0/3600.0);
    // Airliner flight4("AA","SCE","ORD"); flight4.setVel(500.0/3600.0);
    // GeneralAviation flight5("SCE","PHL"); flight5.setVel(140.0/3600.0);
    // GeneralAviation flight6("SCE","EWR"); flight6.setVel(160.0/3600.0);
    // GeneralAviation flight7("SCE","ORD"); flight7.setVel(180.0/3600.0);

    // // Timestep
    // double timestep = 10.0;
    // // Calling position at each timestep 'dt'    
    // int i = 0;
    // while (true)
    // {
    //     cout << "\nAt timestep " << i << ":\n\n";
    //     cout << "Position of flight 1: " << flight1.getPos() << endl;
    //     flight1.operate(timestep);
    //     cout << "Position of flight 2: " << flight2.getPos() << endl;
    //     flight2.operate(timestep);
    //     cout << "Position of flight 3: " << flight3.getPos() << endl;
    //     flight3.operate(timestep);
    //     cout << "Position of flight 4: " << flight4.getPos() << endl;
    //     flight4.operate(timestep);
    //     cout << "Position of flight 5: " << flight5.getPos() << endl;
    //     flight5.operate(timestep);
    //     cout << "Position of flight 6: " << flight6.getPos() << endl;
    //     flight6.operate(timestep);
    //     cout << "Position of flight 7: " << flight7.getPos() << endl;
    //     flight7.operate(timestep);
    //     i++;
    // }
    return 0;
}