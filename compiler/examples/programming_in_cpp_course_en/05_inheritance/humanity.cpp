#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

class Australopithecus {
    protected:
        string name;
        
    public:
        Australopithecus() {
        }
        Australopithecus(string n) {
            name = n;
        }
        void setName(string n) {
            name = n;
        }
        string getName(void) {
            return name;
        }
};

class HomoHabilis : public Australopithecus {
    public:
        HomoHabilis() : Australopithecus() {
        }
        HomoHabilis(string n) : Australopithecus(n) {
        }
        template <class T>
        void fight(T who) {
            cout << getName() << " fought with " << who.getName() <<".\n";
        }
};

class HomoErectus : public HomoHabilis {
    public:
        HomoErectus() : HomoHabilis() {
        }
        HomoErectus(string n) : HomoHabilis(n) {
        }
};

class HomoSapiens : public HomoErectus {
    public:
        HomoSapiens() : HomoErectus() {
        }
        HomoSapiens(string n) : HomoErectus(n) {
        }
        void say(string s) {
            cout << getName() << " said \"" << s << "\".\n";
        }
        void say(string s, HomoErectus who) {
            cout << getName() << " said \"" << s << "\" to " << who.getName() <<".\n";
        }
};

class HomoNeanderthalensis : public HomoErectus {
    public:
        HomoNeanderthalensis() : HomoErectus() {
        }
        HomoNeanderthalensis(string n) : HomoErectus(n) {
        }
};

int main(void) {
    HomoNeanderthalensis fred("Fred");
    HomoSapiens adam("Adam");
    
    adam.say("What a lovely day!");
    adam.say("Who are you?", fred);
    fred.fight(adam);
    
        return 0;
}
