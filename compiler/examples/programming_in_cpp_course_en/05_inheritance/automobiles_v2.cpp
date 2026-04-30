#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

class Automobile {
    public:
        string color;
        int year;
        string model;
        float engine_size;
        float price;
        float ac;
        float steering;
        
        Automobile() {
        }
        Automobile(string color, int year, string model, float engine_size, float price, float ac, float steering) {
            this->color = color;
            this->year = year;
            this->model = model;
            this->engine_size = engine_size;
            this->price = price;
            this->ac = ac;
            this->steering = steering;
        }
};

class Car : public Automobile {
    public:
        Car() : Automobile() {
        }
        Car(string color, int year, string model, float engine_size, float price, float ac, float steering) : Automobile(color, year, model, engine_size, price, ac, steering) {
        }
};

class Truck : public Automobile {
    public:
        Truck() : Automobile() {
        }
        Truck(string color, int year, string model, float engine_size, float price, float ac, float steering) : Automobile(color, year, model, engine_size, price, ac, steering) {
        }
};

class Tractor : public Automobile {
    public:
        Tractor() : Automobile() {
        }
        Tractor(string color, int year, string model, float engine_size, float price, float ac, float steering) : Automobile(color, year, model, engine_size, price, ac, steering) {
        }
};

int main(void) {
    Car etios("Silver", 2021, "XL", 1.4, 50000, 2000, 3000);
    Truck actros("Red", 2022, "X", 6.0, 500000, 20000, 30000);
    Tractor mf3400("Blue", 2022, "MF 3400", 3.0, 75000, 3000, 4000);
    int vehicleType;
    char wantAC;
    char wantSteering;
    float totalPrice = 0;
    string model = "";
    
    cout << "Build your vehicle:\n";
    cout << "Would you like to buy a car (1), truck (2) or tractor (3)? ";
    cin >> vehicleType;
    cout << "\nWould you like a vehicle with air conditioning (y/n)? ";
    cin >> wantAC;
    cout << "\nWould you like a vehicle with power steering (y/n)? ";
    cin >> wantSteering;
    
    switch (vehicleType) {
        case 1:
            model = etios.model;
            totalPrice = etios.price;
            
            if (wantAC == 'y') {
                totalPrice += etios.ac;
            }
            if (wantSteering == 'y') {
                totalPrice += etios.steering;
            }
            break;
        case 2:
            model = actros.model;
            totalPrice = actros.price;
            
            if (wantAC == 'y') {
                totalPrice += actros.ac;
            }
            if (wantSteering == 'y') {
                totalPrice += actros.steering;
            }
            break;
        case 3:
            model = mf3400.model;
            totalPrice = mf3400.price;
            
            if (wantAC == 'y') {
                totalPrice += mf3400.ac;
            }
            if (wantSteering == 'y') {
                totalPrice += mf3400.steering;
            }
            break;
        default:
            cout << "Invalid option!";
            
            return 0;
    }
    
    cout << "The vehicle " << model << " costs $" << totalPrice << ".\n";
    
    
        return 0;
}
