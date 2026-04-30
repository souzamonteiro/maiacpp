#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int main(void) {
    float position;
    float initial_position;
    float velocity;
    float time;
    
    cout << "Enter the initial position of the object in m: ";
    cin >> initial_position;
    cout << "Enter the velocity of the object in m/s: ";
    cin >> velocity;
    cout << "Enter the elapsed time in s: ";
    cin >> time;
    
    position = initial_position + velocity * time;
    
    printf("Current position of the object: %.2f m.\n", position);
    
        return 0;
}
