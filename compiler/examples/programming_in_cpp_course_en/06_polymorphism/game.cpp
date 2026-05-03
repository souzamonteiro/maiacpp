#include<iostream>
using namespace std;

/*
 * Characters: wizard, witch, knight, princess, villager, monster, dragon.
 */
 
class LifeForm {
        protected:
                string name;
                int life;     // Range: 0 to 100.
                int strength; // Range: 0 to 100.

        public:
                LifeForm() {
                        life = 100;
                        strength = 100;
                }
                LifeForm(string n) {
                        name = n;
                        life = 100;
                        strength = 100;
                }
                void setName(string n) {
                        name = n;
                }
                string getName() {
                        return name;
                }
                void setLife(int v) {
                        if (v >= 0 && v <= 100) {
                                life = v;
                        }
                }
                int getLife() {
                        return life;
                }
                void setStrength(int f) {
                        if (f >= 0 && f <= 100) {
                                strength = f;
                        }
                }
                int getStrength() {
                        return strength;
                }
};

class Human : public LifeForm {
        public:
                Human() : LifeForm() {
                }
                Human(string n) : LifeForm(n) {
                }

                void say(string s) {
                        cout << getName() << " said: " << s << "\n";
                }

                virtual string respond(string s) {
                        if (s == "Hello!") {
                                return "How are you?";
                        } else if (s == "How are you?") {
                                return "Fine, and you?";
                        } else if (s == "Fine, and you?") {
                                return "I'm doing great!";
                        } else if (s == "Goodbye!") {
                                return "See you!";
                        } else if (s == "Bye!") {
                                return "See you!";
                        } else {
                                return "I don't know.";
                        }
                }

                template <class ClassTemplate>
                void converse(ClassTemplate person, string s) {
                        string personResponse;
                        string myResponse;
                        myResponse = s;
                        while (true) {
                                this->say(myResponse);
                                personResponse = person.respond(myResponse);
                                person.say(personResponse);
                                myResponse = this->respond(personResponse);
                                if (personResponse == "I don't know." || myResponse == "I don't know.") {
                                        break;
                                }
                        }
                }
};

class Wizard : public Human {
        protected:
                int magic;

        public:
                Wizard() : Human() {
                        magic = 100;
                }
                Wizard(string n) : Human(n) {
                        magic = 100;
                }

                void setMagic(int m) {
                        if (m >= 0 && m <= 100) {
                                magic = m;
                        }
                }
                int getMagic() {
                        return magic;
                }
};

class Witch : public Human {
        protected:
                int magic;

        public:
                Witch() : Human() {
                        magic = 100;
                }
                Witch(string n) : Human(n) {
                        magic = 100;
                }

                void setMagic(int m) {
                        if (m >= 0 && m <= 100) {
                                magic = m;
                        }
                }
                int getMagic() {
                        return magic;
                }
};

class Knight : public Human {
        protected:
                int bravery;
                int armor;

        public:
                Knight() : Human() {
                        bravery = 100;
                        armor = 100;
                }
                Knight(string n) : Human(n) {
                        bravery = 100;
                        armor = 100;
                }

                void setBravery(int c) {
                        if (c >= 0 && c <= 100) {
                                bravery = c;
                        }
                }
                int getBravery() {
                        return bravery;
                }
                void setArmor(int a) {
                        if (a >= 0 && a <= 100) {
                                armor = a;
                        }
                }
                int getArmor() {
                        return armor;
                }

                virtual string respond(string s) {
                        if (s == "Hello!") {
                                return "How do you do, noble person?";
                        } else if (s == "How are you?") {
                                return "Fine, and how is Your Lordship?";
                        } else if (s == "Fine, and you?") {
                                return "I am doing splendidly well!";
                        } else if (s == "Goodbye!") {
                                return "Until next time!";
                        } else if (s == "Bye!") {
                                return "Until next time!";
                        } else {
                                return "I don't know.";
                        }
                }
};

class Princess : public Human {
        protected:
                int intelligence;
                int beauty;
                int wealth;

        public:
            void init() {
                    intelligence = 50;
                    beauty = 100;
                    wealth = 20;
            }
                Princess() : Human() {
                    this->init();
                }
                Princess(string n) : Human(n) {
            this->init();
                }

                void setIntelligence(int i) {
                        if (i >= 0 && i <= 100) {
                                intelligence = i;
                        }
                }
                int getIntelligence() {
                        return intelligence;
                }
                void setBeauty(int b) {
                        if (b >= 0 && b <= 100) {
                                beauty = b;
                        }
                }
                int getBeauty() {
                        return beauty;
                }
                void setWealth(int d) {
                        if (d >= 0 && d <= 100) {
                                wealth = d;
                        }
                }
                int getWealth() {
                        return wealth;
                }

                string respond(string s) {
                        if (s == "Hello!") {
                                return "I'm good, and you?";
                        } else if (s == "I'm good, and you?") {
                                return "I'm great!";
                        } else if (s == "None of your business!") {
                                return "How rude!";
                        } else if (s == "Get out of here!") {
                                return "I'll call my father!";
                        } else if (s == "Get lost!") {
                                return "You're done for!";
                        } else if (s == "You're done for!") {
                                return "I don't care!";
                        } else {
                                return "I don't know.";
                        }
                }
};

class Villager : public Human {
        protected:
                int loyalty;
                int honesty;

        public:
            void init() {
                    loyalty = 0;
                    honesty = 0;
            }
                Villager() : Human() {
                    this->init();
                }
                Villager(string n) : Human(n) {
                    this->init();
                }

                void setLoyalty(int l) {
                        if (l >= 0 && l <= 100) {
                                loyalty = l;
                        }
                }
                int getLoyalty() {
                        return loyalty;
                }
                void setHonesty(int h) {
                        if (h >= 0 && h <= 100) {
                                honesty = h;
                        }
                }
                int getHonesty() {
                        return honesty;
                }

                string respond(string s) {
                        if (s == "Hello!") {
                                return "I'm good, and you?";
                        } else if (s == "How are you?") {
                                return "None of your business!";
                        } else if (s == "Fine, and you?") {
                                return "Get out of here!";
                        } else if (s == "I'm great!") {
                                return "Get out of here!";
                        } else if (s == "I'll call my father!") {
                                return "I don't care!";
                        } else if (s == "Goodbye!") {
                                return "Get lost!";
                        } else if (s == "Bye!") {
                                return "I don't care!";
                        } else {
                                return "I don't know.";
                        }
                }
};

class Monster : public LifeForm {
        protected:
                int sympathy;

        public:
            void init() {
                    sympathy = 0;
            }
                Monster() : LifeForm() {
                    this->init();
                }
                Monster(string n) : LifeForm(n) {
                    this->init();
                }

                void setSympathy(int s) {
                        if (s >= 0 && s <= 100) {
                                sympathy = s;
                        }
                }
                int getSympathy() {
                        return sympathy;
                }
};

class Dragon : public Monster {
        protected:
                int fire;

        public:
            void init() {
                    fire = 0;
            }
                Dragon() : Monster() {
                    this->init();
                }
                Dragon(string n) : Monster(n) {
                    this->init();
                }

                void setFire(int f) {
                        if (f >= 0 && f <= 100) {
                                fire = f;
                        }
                }
                int getFire() {
                        return fire;
                }
};

Wizard merlin("Merlin");
Witch pathologicalWitch("Pathological Witch");
Knight arthur("Arthur");
Villager james("James");
Princess leia("Leia");
Monster lucas("Lucas");
Dragon tiamat("Tiamat");

int introduction() {
    return 1;
}

int chapter1() {
    return 1;
}

int chapter2() {
    return 1;
}

int chapter3() {
    return 1;
}

int main(void) {
	if (!introduction()) {
	    system("clear");
	    
	    cout << "Game over!";
	    
	    return 0;
	} else {
	    if (!chapter1()) {
    	    system("clear");
	    
	        cout << "Game over!";
	        
            return 0;
	    } else {
	        if (!chapter2()) {
        	    system("clear");
    	    
    	        cout << "Game over!";
    	        
                return 0;
    	    } else {
    	        if (!chapter3()) {
            	    system("clear");
        	    
        	        cout << "Game over!";
        	        
                    return 0;
        	    } else {
        	        system("clear");
        	    
        	        cout << "Congratulations! You win!";
        	        
                    return 0;
        	    }
    	    }
	    }
	}
	
	return 0;
}
