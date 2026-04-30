#include<iostream>
using namespace std;

class LifeForm {
        public:
                LifeForm(void) {
                }
};

class Dinosaur : public LifeForm {
        protected:
                string name;

        public:
                Dinosaur(void) {
                        name = "";
                }
                Dinosaur(string n) {
                        name = n;
                }
                void setName(string n){
                        name = n;
                }
                string getName(void) {
                        return name;
                }
                template <class T>
                void eat(T other) {
                        cout << name << " ate " << other.getName() << ".\n";
                }
};

class Brontosaurus : public Dinosaur {
        public:
                Brontosaurus(string n) : Dinosaur(n) {
                }
};

class Pterodactyl : public Dinosaur {
        public:
                Pterodactyl(string n) : Dinosaur(n) {
                }
};

class Tyrannosaurus : public Dinosaur {
        public:
                Tyrannosaurus(string n) : Dinosaur(n) {
                }
};

int main(void) {
        Brontosaurus dino("Dino");
        Pterodactyl peter("Peter");
        Tyrannosaurus rex("Rex");

        cout << "The name of dinosaur dino is " << dino.getName() << ".\n";
        cout << "The name of dinosaur peter is " << peter.getName() << ".\n";
        cout << "The name of dinosaur rex is " << rex.getName() << ".\n";

        rex.eat<Dinosaur>(dino);

        return 0;
}
