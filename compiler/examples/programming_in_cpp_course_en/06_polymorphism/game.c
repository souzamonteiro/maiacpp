/* Generated from C++98 source */
/* Target: C89 */

/* Minimal bridge prelude for MaiaC */
/* Runtime interface */
extern void   __exc_push(void);
extern void   __exc_pop(void);
extern int    __exc_active(void);
extern int    __exc_type(void);
extern void*  __exc_data(void);
extern void   __exc_throw(int type, void* data);
extern void   __exc_clear(void);
extern int    __exc_matches(int thrown_type, int catch_type);
extern void*  __malloc(unsigned long size);
extern void   __free(void* ptr);

/* String and memory function declarations (MaiaC-compatible, no #include needed) */
extern unsigned int strlen(const char* s);
extern int strcmp(const char* s1, const char* s2);
extern int strncmp(const char* s1, const char* s2, unsigned int n);
extern char* strcpy(char* dest, const char* src);
extern char* strncpy(char* dest, const char* src, unsigned int n);
extern char* strcat(char* dest, const char* src);
extern char* strncat(char* dest, const char* src, unsigned int n);
extern char* strstr(const char* haystack, const char* needle);
extern char* strchr(const char* s, int c);
extern char* strrchr(const char* s, int c);
extern unsigned int strspn(const char* s, const char* accept);
extern unsigned int strcspn(const char* s, const char* reject);
extern char* strtok(char* str, const char* delim);
extern int memcmp(const void* s1, const void* s2, unsigned int n);
extern void* memcpy(void* dest, const void* src, unsigned int n);
extern void* memmove(void* dest, const void* src, unsigned int n);
extern void* memset(void* s, int c, unsigned int n);
extern void* memchr(const void* s, int c, unsigned int n);

#define EXC_LifeForm 1
#define EXC_Human 2
#define EXC_Wizard 3
#define EXC_Witch 4
#define EXC_Knight 5
#define EXC_Princess 6
#define EXC_Villager 7
#define EXC_Monster 8
#define EXC_Dragon 9

typedef struct LifeForm {
  char* name;
  int life;
  int strength;
} LifeForm;

void LifeForm_init(LifeForm* self);
void LifeForm_init__pv(LifeForm* self, char* n);
void LifeForm_destroy(LifeForm* self);
void LifeForm_setName__pv(LifeForm* self, char* n);
char* LifeForm_getName(LifeForm* self);
void LifeForm_setLife__i(LifeForm* self, int v);
int LifeForm_getLife(LifeForm* self);
void LifeForm_setStrength__i(LifeForm* self, int f);
int LifeForm_getStrength(LifeForm* self);

void LifeForm_init(LifeForm* self) {
  (void)self;
}

void LifeForm_init__pv(LifeForm* self, char* n) {
  (void)self;
  (void)n;
}

void LifeForm_destroy(LifeForm* self) {
  (void)self;
}

void LifeForm_setName__pv(LifeForm* self, char* n) {
  (void)self;
  self->name = n;
  (void)n;
}

char* LifeForm_getName(LifeForm* self) {
  (void)self;
  return self->name;
}

void LifeForm_setLife__i(LifeForm* self, int v) {
  (void)self;
  if (v >= 0 && v <= 100) {
    self->life = v;
  }
  (void)v;
}

int LifeForm_getLife(LifeForm* self) {
  (void)self;
  return self->life;
}

void LifeForm_setStrength__i(LifeForm* self, int f) {
  (void)self;
  if (f >= 0 && f <= 100) {
    self->strength = f;
  }
  (void)f;
}

int LifeForm_getStrength(LifeForm* self) {
  (void)self;
  return self->strength;
}

typedef struct Human {
  LifeForm __base;
  char* personResponse;
  char* myResponse;
  void* __vptr;
} Human;

void Human_init(Human* self);
void Human_init__pv(Human* self, char* n);
void Human_destroy(Human* self);
void Human_say__pv(Human* self, char* s);
char* Human_respond__pv(Human* self, char* s);

void Human_init(Human* self) {
  (void)self;
  LifeForm_init((LifeForm*)self);
}

void Human_init__pv(Human* self, char* n) {
  (void)self;
  LifeForm_init__pv((LifeForm*)self, n);
}

void Human_destroy(Human* self) {
  (void)self;
}

void Human_say__pv(Human* self, char* s) {
  (void)self;
  printf("%s said %c%s%c.\n", Australopithecus_getName((Australopithecus*)self), 34, s, 34);
  (void)s;
}

char* Human_respond__pv(Human* self, char* s) {
  (void)self;
  if ((strcmp(s, "Hello!") == 0))
  {
  return "How are you?";
  }
  else if ((strcmp(s, "How are you?") == 0))
  {
  return "Fine, and you?";
  }
  else if ((strcmp(s, "Fine, and you?") == 0))
  {
  return "I'm doing great!";
  }
  else if ((strcmp(s, "Goodbye!") == 0))
  {
  return "See you!";
  }
  else if ((strcmp(s, "Bye!") == 0))
  {
  return "See you!";
  }
  else
  {
  return "I don't know.";
  }
  (void)s;
}

typedef struct Wizard {
  Human __base;
  int magic;
} Wizard;

void Wizard_init(Wizard* self);
void Wizard_init__pv(Wizard* self, char* n);
void Wizard_destroy(Wizard* self);
void Wizard_setMagic__i(Wizard* self, int m);
int Wizard_getMagic(Wizard* self);

void Wizard_init(Wizard* self) {
  (void)self;
  Human_init((Human*)self);
}

void Wizard_init__pv(Wizard* self, char* n) {
  (void)self;
  Human_init__pv((Human*)self, n);
}

void Wizard_destroy(Wizard* self) {
  (void)self;
}

void Wizard_setMagic__i(Wizard* self, int m) {
  (void)self;
  if (m >= 0 && m <= 100) {
    self->magic = m;
  }
  (void)m;
}

int Wizard_getMagic(Wizard* self) {
  (void)self;
  return self->magic;
}

typedef struct Witch {
  Human __base;
  int magic;
} Witch;

void Witch_init(Witch* self);
void Witch_init__pv(Witch* self, char* n);
void Witch_destroy(Witch* self);
void Witch_setMagic__i(Witch* self, int m);
int Witch_getMagic(Witch* self);

void Witch_init(Witch* self) {
  (void)self;
  Human_init((Human*)self);
}

void Witch_init__pv(Witch* self, char* n) {
  (void)self;
  Human_init__pv((Human*)self, n);
}

void Witch_destroy(Witch* self) {
  (void)self;
}

void Witch_setMagic__i(Witch* self, int m) {
  (void)self;
  if (m >= 0 && m <= 100) {
    self->magic = m;
  }
  (void)m;
}

int Witch_getMagic(Witch* self) {
  (void)self;
  return self->magic;
}

typedef struct Knight {
  Human __base;
  int bravery;
  int armor;
  void* __vptr;
} Knight;

void Knight_init(Knight* self);
void Knight_init__pv(Knight* self, char* n);
void Knight_destroy(Knight* self);
void Knight_setBravery__i(Knight* self, int c);
int Knight_getBravery(Knight* self);
void Knight_setArmor__i(Knight* self, int a);
int Knight_getArmor(Knight* self);
char* Knight_respond__pv(Knight* self, char* s);

void Knight_init(Knight* self) {
  (void)self;
  Human_init((Human*)self);
}

void Knight_init__pv(Knight* self, char* n) {
  (void)self;
  Human_init__pv((Human*)self, n);
}

void Knight_destroy(Knight* self) {
  (void)self;
}

void Knight_setBravery__i(Knight* self, int c) {
  (void)self;
  if (c >= 0 && c <= 100) {
    self->bravery = c;
  }
  (void)c;
}

int Knight_getBravery(Knight* self) {
  (void)self;
  return self->bravery;
}

void Knight_setArmor__i(Knight* self, int a) {
  (void)self;
  if (a >= 0 && a <= 100) {
    self->armor = a;
  }
  (void)a;
}

int Knight_getArmor(Knight* self) {
  (void)self;
  return self->armor;
}

char* Knight_respond__pv(Knight* self, char* s) {
  (void)self;
  if ((strcmp(s, "Hello!") == 0))
  {
  return "How do you do, noble person?";
  }
  else if ((strcmp(s, "How are you?") == 0))
  {
  return "Fine, and how is Your Lordship?";
  }
  else if ((strcmp(s, "Fine, and you?") == 0))
  {
  return "I am doing splendidly well!";
  }
  else if ((strcmp(s, "Goodbye!") == 0))
  {
  return "Until next time!";
  }
  else if ((strcmp(s, "Bye!") == 0))
  {
  return "Until next time!";
  }
  else
  {
  return "I don't know.";
  }
  (void)s;
}

typedef struct Princess {
  Human __base;
  int intelligence;
  int beauty;
  int wealth;
} Princess;

void Princess_init(Princess* self);
void Princess_init__pv(Princess* self, char* n);
void Princess_destroy(Princess* self);
void Princess_setIntelligence__i(Princess* self, int i);
int Princess_getIntelligence(Princess* self);
void Princess_setBeauty__i(Princess* self, int b);
int Princess_getBeauty(Princess* self);
void Princess_setWealth__i(Princess* self, int d);
int Princess_getWealth(Princess* self);
char* Princess_respond__pv(Princess* self, char* s);

void Princess_init(Princess* self) {
  (void)self;
  Human_init((Human*)self);
}

void Princess_init__pv(Princess* self, char* n) {
  (void)self;
  Human_init__pv((Human*)self, n);
}

void Princess_destroy(Princess* self) {
  (void)self;
}

void Princess_setIntelligence__i(Princess* self, int i) {
  (void)self;
  if (i >= 0 && i <= 100) {
    self->intelligence = i;
  }
  (void)i;
}

int Princess_getIntelligence(Princess* self) {
  (void)self;
  return self->intelligence;
}

void Princess_setBeauty__i(Princess* self, int b) {
  (void)self;
  if (b >= 0 && b <= 100) {
    self->beauty = b;
  }
  (void)b;
}

int Princess_getBeauty(Princess* self) {
  (void)self;
  return self->beauty;
}

void Princess_setWealth__i(Princess* self, int d) {
  (void)self;
  if (d >= 0 && d <= 100) {
    self->wealth = d;
  }
  (void)d;
}

int Princess_getWealth(Princess* self) {
  (void)self;
  return self->wealth;
}

char* Princess_respond__pv(Princess* self, char* s) {
  (void)self;
  if ((strcmp(s, "Hello!") == 0))
  {
  return "I'm good, and you?";
  }
  else if ((strcmp(s, "I'm good, and you?") == 0))
  {
  return "I'm great!";
  }
  else if ((strcmp(s, "None of your business!") == 0))
  {
  return "How rude!";
  }
  else if ((strcmp(s, "Get out of here!") == 0))
  {
  return "I'll call my father!";
  }
  else if ((strcmp(s, "Get lost!") == 0))
  {
  return "You're done for!";
  }
  else if ((strcmp(s, "You're done for!") == 0))
  {
  return "I don't care!";
  }
  else
  {
  return "I don't know.";
  }
  (void)s;
}

typedef struct Villager {
  Human __base;
  int loyalty;
  int honesty;
} Villager;

void Villager_init(Villager* self);
void Villager_init__pv(Villager* self, char* n);
void Villager_destroy(Villager* self);
void Villager_setLoyalty__i(Villager* self, int l);
int Villager_getLoyalty(Villager* self);
void Villager_setHonesty__i(Villager* self, int h);
int Villager_getHonesty(Villager* self);
char* Villager_respond__pv(Villager* self, char* s);

void Villager_init(Villager* self) {
  (void)self;
  Human_init((Human*)self);
}

void Villager_init__pv(Villager* self, char* n) {
  (void)self;
  Human_init__pv((Human*)self, n);
}

void Villager_destroy(Villager* self) {
  (void)self;
}

void Villager_setLoyalty__i(Villager* self, int l) {
  (void)self;
  if (l >= 0 && l <= 100) {
    self->loyalty = l;
  }
  (void)l;
}

int Villager_getLoyalty(Villager* self) {
  (void)self;
  return self->loyalty;
}

void Villager_setHonesty__i(Villager* self, int h) {
  (void)self;
  if (h >= 0 && h <= 100) {
    self->honesty = h;
  }
  (void)h;
}

int Villager_getHonesty(Villager* self) {
  (void)self;
  return self->honesty;
}

char* Villager_respond__pv(Villager* self, char* s) {
  (void)self;
  if ((strcmp(s, "Hello!") == 0))
  {
  return "I'm good, and you?";
  }
  else if ((strcmp(s, "How are you?") == 0))
  {
  return "None of your business!";
  }
  else if ((strcmp(s, "Fine, and you?") == 0))
  {
  return "Get out of here!";
  }
  else if ((strcmp(s, "I'm great!") == 0))
  {
  return "Get out of here!";
  }
  else if ((strcmp(s, "I'll call my father!") == 0))
  {
  return "I don't care!";
  }
  else if ((strcmp(s, "Goodbye!") == 0))
  {
  return "Get lost!";
  }
  else if ((strcmp(s, "Bye!") == 0))
  {
  return "I don't care!";
  }
  else
  {
  return "I don't know.";
  }
  (void)s;
}

typedef struct Monster {
  LifeForm __base;
  int sympathy;
} Monster;

void Monster_init(Monster* self);
void Monster_init__pv(Monster* self, char* n);
void Monster_destroy(Monster* self);
void Monster_setSympathy__i(Monster* self, int s);
int Monster_getSympathy(Monster* self);

void Monster_init(Monster* self) {
  (void)self;
  LifeForm_init((LifeForm*)self);
}

void Monster_init__pv(Monster* self, char* n) {
  (void)self;
  LifeForm_init__pv((LifeForm*)self, n);
}

void Monster_destroy(Monster* self) {
  (void)self;
}

void Monster_setSympathy__i(Monster* self, int s) {
  (void)self;
  if (s >= 0 && s <= 100) {
    self->sympathy = s;
  }
  (void)s;
}

int Monster_getSympathy(Monster* self) {
  (void)self;
  return self->sympathy;
}

typedef struct Dragon {
  Monster __base;
  int fire;
} Dragon;

void Dragon_init(Dragon* self);
void Dragon_init__pv(Dragon* self, char* n);
void Dragon_destroy(Dragon* self);
void Dragon_setFire__i(Dragon* self, int f);
int Dragon_getFire(Dragon* self);

void Dragon_init(Dragon* self) {
  (void)self;
  Monster_init((Monster*)self);
}

void Dragon_init__pv(Dragon* self, char* n) {
  (void)self;
  Monster_init__pv((Monster*)self, n);
}

void Dragon_destroy(Dragon* self) {
  (void)self;
}

void Dragon_setFire__i(Dragon* self, int f) {
  (void)self;
  if (f >= 0 && f <= 100) {
    self->fire = f;
  }
  (void)f;
}

int Dragon_getFire(Dragon* self) {
  (void)self;
  return self->fire;
}

/* Global functions */
int introduction(void);
int chapter1(void);
int chapter2(void);
int chapter3(void);
int main(void);

int introduction(void) {
  return 1;
}

int chapter1(void) {
  return 1;
}

int chapter2(void) {
  return 1;
}

int chapter3(void) {
  return 1;
}

int main(void) {
  if (!introduction()) {
    system("clear");
    printf("Game over!");
    return 0;
  } else {
    if (!chapter1()) {
      system("clear");
      printf("Game over!");
      return 0;
    } else {
      if (!chapter2()) {
        system("clear");
        printf("Game over!");
        return 0;
      } else {
        if (!chapter3()) {
          system("clear");
          printf("Game over!");
          return 0;
        } else {
          system("clear");
          printf("Congratulations! You win!");
          return 0;
        }
      }
    }
  }
  return 0;
}
