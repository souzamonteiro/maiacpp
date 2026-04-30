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
        
        Automobile() {
        }
        Automobile(string color, int year, string model, float engine_size, float price) {
            this->color = color;
            this->year = year;
            this->model = model;
            this->engine_size = engine_size;
            this->price = price;
        }
};

class Car : public Automobile {
    public:
        Car() : Automobile() {
        }
        Car(string color, int year, string model, float engine_size, float price) : Automobile(color, year, model, engine_size, price) {
        }
};

class Truck : public Automobile {
    public:
        Truck() : Automobile() {
        }
        Truck(string color, int year, string model, float engine_size, float price) : Automobile(color, year, model, engine_size, price) {
        }
};

class Tractor : public Automobile {
    public:
        Tractor() : Automobile() {
        }
        Tractor(string color, int year, string model, float engine_size, float price) : Automobile(color, year, model, engine_size, price) {
        }
};

int main(void) {
    Car etios("Silver", 2021, "XL", 1.4, 50000);
    Truck actros("Red", 2022, "X", 6.0, 500000);
    Tractor mf3400("Blue", 2022, "MF 3400", 3.0, 75000);
    
    cout << "The tractor " << mf3400.model << " year " << mf3400.year << " costs $" << mf3400.price << ".\n";
    
        return 0;
}
