
// Google Hash Code online qualification round 2021



#include <iostream>
#include <queue>
#include <string>
#include <map>
#include <vector>
#include <set>

using namespace std;

// For class-forwarding
class Intersection;
class Street;
class Car;


// Class for each Street
class Street{
public:
    string streetName;      
    int intersectionOut;
    int duration;      //duration to cross the road
    int light_duration = 0;     
    queue <Car*> queueCars;
    
    Street(string name,int finish,int duration): streetName(name) ,
                                                            duration(duration),
                                                            intersectionOut(finish){ }

    void addCar(Car* car){
        this->queueCars.push(car);
    }
    
    int getIntersectionOut(){
        return this->intersectionOut;
    }
};


// Class for each Intersection
class Intersection {
    // Public
    public:
    vector<string> streets;
    bool modified = false;
    int id;
    
    // Constructor
    Intersection(int id): id(id), modified(false) {}

    void add_street(string street) {
        streets.push_back(street);
    }
};


// Class for each Car
class Car{
    int id;
    queue<string> rues;
    public:
        // Constructor
        Car(int id): id(id) {}

        void add_rue(string street) {
            rues.push(street);
        }
        
        queue<string> get_rues() {
            return rues;
        }
};


// Function to define how long the traffic lights stay green
void green_light(vector<Car*> cars, vector<Street*> streets, Intersection** intersections) {
    // Defining points vector
    vector<int> points;
    for (int i=0 ; i<streets.size() ; ++i) {
        points.push_back(0);
    }

    // Getting streets based with point system
    // Based on each car how many times get passed each street
    for (int i=0 ; i<cars.size() ; ++i) {
        queue<string> names = cars[i]->get_rues();

        int size_queue = names.size();
        for (int j=0 ; j<size_queue ; ++j) {
            string temp = names.front(); 
            names.pop();

            int pos;
            for (int k=0 ; k<streets.size() ; ++k) {
                if (temp == streets[k]->streetName) {
                    pos = k;
                    break;
                }
            }
            ++points[pos];
        }
    }
    
    // Classifier for streets (Bubblesort)
    for (int i=0 ; i<streets.size()-1 ; ++i) {
        for (int j=i ; j>=0 ; --j) {
            if (points[j] < points[j+1]) {
                int temp = points[j];
                points[j] = points[j+1];
                points[j+1] = temp;

                Street* t = streets[j];
                streets[j] = streets[j+1];
                streets[j+1] = t;
            }
        }
    }

    // Diving streets into 3 categories
    // and giving corresponding light_seconds
    int max = points[0];
    int min = points[points.size()-1];

    int dif = (max-min) / 3;
    int mid1 = dif + min;
    int mid2 = dif + 2 * min;

    for (int i=0 ; i<streets.size() ; ++i) {
        if (points[i] < mid1) {
            streets[i]->light_duration = 1;
            intersections[streets[i]->getIntersectionOut()]->modified = true;
        }
        else if (points[i] >= mid1 && points[i] < mid2) {
            streets[i]->light_duration = 2;
            intersections[streets[i]->getIntersectionOut()]->modified = true;
        }
        else if (points[i] >= mid2) {
            streets[i]->light_duration = 3;
            intersections[streets[i]->getIntersectionOut()]->modified = true;
        }
    }
}


// Global map for names -> streets
map <string, Street*> names_to_streets;

int main(){
    int D;  // Simualtion Duration
    int I;  // Number of intersections
    int S;  // number of streets
    int V;  // the number of cars
    int F;  // bonus points

    cin >> D;
    cin >> I;
    cin >> S;
    cin >> V;
    cin >> F;

    vector<Street*> streets;
    vector<Car*> cars;
    Intersection* intersections[I];

    // Create Intersections
    for(int i = 0; i < I; i++){
        intersections[i] = new Intersection(i);
    }
    
    // Create Streets
    for(int i = 0; i < S; i++){ 
        int B;  // Start
        int E;  // End
        cin >> B;
        cin >> E;

        string street_name;
        cin >> street_name;

        int L;  // Duration
        cin >> L;

        Street* street = new Street(street_name, E, L);
        intersections[E]->add_street(street_name);
        streets.push_back(street);
        names_to_streets.insert(pair<string, Street*>(street_name, street));
    }

    // Create cars
    for(int i = 0; i < V; i++){
        int P;      // Number of roads a car goes to
        cin >> P;

        Car* car = new Car(i);
        for(int k = 0; k < P; k++){
            string rue;
            cin >> rue;
            car->add_rue(rue);
            if(k==0){   // Starting road of the car
                auto itr = names_to_streets.find(rue);      // Find the value of the key
                Street* street= itr->second;  
                street->addCar(car);    // Add car to street
            }
        }
        
        cars.push_back(car);
    }

    // Calculate 
    green_light(cars, streets, intersections);

    int A = 0;
    // Find out the number of intersections for which we specify the schedule
    for(int i = 0; i < I; i++){
        if(intersections[i]->modified)
            A++;
    }
    cout << A << endl;
    
    for(int id = 0; id < I; id++){
        if(intersections[id]->modified){
            // Print intersection id
            cout << id << endl;
            
            vector<string> streets = intersections[id]->streets;

            // Print the number of incoming streets (of the intersection id ) covered by this schedule
            cout << streets.size() << endl;
            for (auto i = streets.begin(); i != streets.end(); ++i){
                // The street name
                cout << *i << " ";
                auto itr = names_to_streets.find(*i);
                // How long each street will have a greenlight
                cout << itr->second->light_duration << endl;
            }
        }
    }
}
