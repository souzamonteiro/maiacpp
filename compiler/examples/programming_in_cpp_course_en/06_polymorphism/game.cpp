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
    char cont;
    
    system("clear");
    
    cout << "The Evil Princess\n";
    
printf(R"EOF(

 [][][] /""\ [][][]
  |::| /____\ |::|
  |[]|_|::::|_|[]|
  |::::::__::::::|
  |:::::/||\:::::|
  |:#:::||||::#::|
 #@*###&*##&*&#*&##
##@@*####*@@@###*@*#

)EOF");

    cout << "Do you wish to continue (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
    cout << " All was well in the peaceful kingdom of Ana Lucia\n";
    cout << " Until Princess Leia turned 15 years old\n";
    cout << "She then joined forces with the Witch\n";
    cout << "  And demanded of her parents the king and queen\n";
    cout << "that they order the villagers to hand over\n";
    cout << "Their youngest children for her sinister\n";
    cout << "              Experiments...\n\n";
    
    cout << "Do you wish to continue (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
    return 1;
}

int chapter1() {
    char cont;
    
    system("clear");
    
    cout << "    Chapter 1\n";
    cout << "James Seeks Help\n";
    
printf(R"EOF(

     """""""       
     ^-O-O-^       
---ooO--U--Ooo-----

)EOF");

    cout << "Do you wish to continue (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
    cout << "James, deeply worried about the terrible fate\n";
    cout << "  that has befallen the kingdom,\n";
    cout << "    seeks out Knight Arthur\n";
    
    cout << "Do you wish to continue (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
printf(R"EOF(

 _/ _ _ \_  
(o / | \ o)
 || o|o ||
 | \_|_/ |
 |  ___  |
 | (___) |
 |\_____/|
 | \___/ |
 \       /
  \__ __/
     U

)EOF");

    cout << "        Knight Arthur\n";
    cout << "   The kingdom needs your help \n";
    cout << " Princess Leia has been making the lives\n";
    cout << "      of the villagers a nightmare\n";
    cout << "     Can you help us?\n";
    
    cout << "Do you think Arthur should help (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");

    cout << "            Noble James\n";
    cout << "  For this journey we will need the help \n";
    cout << "     of the noble Wizard Merlin\n";
    cout << "    but he lives beyond the mountains\n";
    cout << "     and the journey is very dangerous\n";
    
    cout << "Do you wish to travel beyond the mountains\n";
    cout << "   and seek Wizard Merlin (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
    return 1;
}

int chapter2() {
    char cont;
    
    system("clear");
    
    cout << "  Chapter 2\n";
    cout << "  The Journey\n";
    
printf(R"EOF(

        _    .  ,   .           .
    *  / \_ *  / \_      _  *        *   /\'__        *
      /    \  /    \,   ((        .    _/  /  \  *'.
 .   /\/\  /\/ :' __ \_  `          _^/  ^/    `--.
    /    \/  \  _/  \-'\      *    /.' ^_   \_   .'\  *
  /\  .-   `. \/     \ /==~=-=~=-=-;.  _/ \ -. `_/   \
 /  `-.__ ^   / .-'.--\ =-=~_=-=~=^/  _ `--./ .-'  `-
/jgs     `.  / /       `.~-^=-=~=^=.-'      '-._ `._

)EOF");

    cout << "Do you wish to continue (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
    cout << "James and Arthur reach the kingdom mountains\n";
    cout << "  Arthur warns James that a terrible\n";
    cout << "     monster guards the entrance\n";
    cout << "       James trembles with fear\n";
    
    cout << "Do you think James should face his fear\n";
    cout << "and continue (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
printf(R"EOF(

        .-"""".
       /       \
   __ /   .-.  .\
  /  `\  /   \/  \
  |  _ \/   .==.==.
  | (   \  /____\__\
   \ \      (_()(_()
    \ \            '---._
     \                   \_
  /\ |`       (__)________/
 /  \|     /\___/
|    \     \||VV
|     \     \|"""",
|      \     ______)
\       \  /`
jgs      \(

)EOF");

    cout << "The monster appears at the foot of the mountain\n";
    cout << "Do you think James should face\n";
    cout << "the monster (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
    james.setLife(james.getLife() - 10);
    
    cout << "              James is wounded\n";
    cout << "     James now has " << james.getLife() << " life points\n";
    
    cout << "Do you think James should continue with\n";
    cout << "Arthur (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
    printf(R"EOF(

    _     _
   /.\   ( )
  /   \   \\
  \,-./    \
  ( oo)  /\{}
   \_=/ /  \ \
  //  \/ ,'   \
 ||    |'      \
 ||    |        \
 m|    |
  |____|
    `-`-

)EOF");

    cout << "James and Arthur find Wizard Merlin\n";
    arthur.converse<Wizard>(merlin, "Hello!");
    arthur.say("We need your help");
    arthur.say("Princess Leia has been terrorizing the kingdom");
    arthur.say("Can you help us?");
    
    cout << "Do you think Merlin should help (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");

    return 1;
}

int chapter3() {
        char cont;
    
    system("clear");
    
    cout << "   Chapter 3\n";
    cout << "  The Confrontation\n";
    
printf(R"EOF(

        w*W*W*W*w
         \"."."/
          //`\\
         (/a a\)
         (\_-_/) 
        .-~'='~-.
       /`~`"Y"`~`\
      / /(_ * _)\ \
     / /  )   (  \ \
     \ \_/\\_//\_/ / 
      \/_) '*' (_\/
        |       |
        |       |
        |       |
        |       |
        |       |
        w*W*W*W*w

)EOF");

    cout << "Do you wish to continue (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
printf(R"EOF(

       \****__              ____                                              
         |    *****\_      --/ *\-__                                          
         /_          (_    ./ ,/----'                                         
Art by     \__         (_./  /                                                
 Ironwing     \__           \___----^__                                       
               _/   _                  \                                      
        |    _/  __/ )\"\ _____         *\                                    
        |\__/   /    ^ ^       \____      )                                   
         \___--"                    \_____ )                                  
                                          "
)EOF");

    cout << "James, Arthur and Merlin return to the castle\n";
    cout << "       Merlin summons a dragon\n";
    
    cout << "Do you think the dragon should attack the princess (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
printf(R"EOF(

                _ ___                /^^\ /^\  /^^\_
    _          _X X  \            ,,/ '` ~ `'~~ ', `\.
  _/o\_ _ _ _/~`.`...'~\        ./~~..,'`','',.,' '  ~:
 / `,'.~,~.~  .   , . , ~|,   ,/ .,' , ,. .. ,,.   `,  ~\_
( ' _' _ '_` _  '  .    , `\_/ .' ..' '  `  `   `..  `,   \_
 ~V~ V~ V~ V~ ~\ `   ' .  '    , ' .,.,''`.,.''`.,.``. ',   \_
  _/\ /\ /\ /\_/, . ' ,   `_/~\_ .' .,. ,, , _/~\_ `. `. '.,  \_
 < ~ ~ '~`'~'`, .,  .   `_: ::: \_ '      `_/ ::: \_ `.,' . ',  \_
  \ ' `_  '`_    _    ',/ _::_::_ \ _    _/ _::_::_ \   `.,'.,`., \-,-,-,_,_,
   `'~~ `'~~ `'~~ `'~~  \(_)(_)(_)/  `~~' \(_)(_)(_)/ ~'`\_.._,._,'_;_;_;_;_;

)EOF");

    tiamat.setLife(tiamat.getLife() - 100);
    
    cout << "              The dragon is gravely wounded\n";
    cout << "     The dragon now has " << tiamat.getLife() << " life points\n";
    
    cout << "Do you wish to continue (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
    james.setLife(james.getLife() - 50);
    arthur.setLife(arthur.getLife() - 20);
    
    cout << "              James and Arthur are wounded\n";
    cout << "     James now has " << james.getLife() << " life points\n";
    cout << "     Arthur now has " << arthur.getLife() << " life points\n";
    
    cout << "Do you think James and Arthur should continue (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
    printf(R"EOF(

                        ______
             ______,---'__,---'
         _,-'---_---__,---'
  /_    (,  ---____',
 /  ',,   `, ,-'
;/)   ,',,_/,'
| /\   ,.'//\
`-` \ ,,'    `.
     `',   ,-- `.
     '/ / |      `,         _
     //'',.\_    .\\      ,{==>-
  __//   __;_`-  \ `;.__,;'
((,--,) (((,------;  `--' jv

)EOF");

    cout << "Wizard Merlin summons the terrible Griffin\n";
    
    system("clear");
    
    leia.setLife(leia.getLife() - 100);
    
    cout << "              Leia is gravely wounded\n";
    cout << "     Leia now has " << leia.getLife() << " life points\n";
    
    cout << "Do you think James, Arthur and Merlin should continue (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
    printf(R"EOF(

                                   .''.       
       .''.      .        *''*    :_\/_:     . 
      :_\/_:   _\(/_  .:.*_\/_*   : /\ :  .'.:.'.
  .''.: /\ :   ./)\   ':'* /\ * :  '..'.  -=:o:=-
 :_\/_:'.:::.    ' *''*    * '.\'/.' _\(/_'.':'.'
 : /\ : :::::     *_\/_*     -= o =-  /)\    '  *
  '..'  ':::'     * /\ *     .'/.\'.   '
      *            *..*         :
jgs     *

)EOF");

    cout << "Our heroes win!\n";
    
    cout << "Continue (y/n)? ";
    cin >> cont;
    
    if (cont == 'n') {
        return 0;
    }
    
    system("clear");
    
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
