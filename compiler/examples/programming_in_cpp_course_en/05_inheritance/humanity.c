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

#define EXC_Australopithecus 1
#define EXC_HomoHabilis 2
#define EXC_HomoErectus 3
#define EXC_HomoSapiens 4
#define EXC_HomoNeanderthalensis 5

typedef struct Australopithecus {
  char* name;
} Australopithecus;

void Australopithecus_init(Australopithecus* self);
void Australopithecus_init__pv(Australopithecus* self, char* n);
void Australopithecus_destroy(Australopithecus* self);
void Australopithecus_setName__pv(Australopithecus* self, char* n);
char* Australopithecus_getName(Australopithecus* self);

void Australopithecus_init(Australopithecus* self) {
  (void)self;
}

void Australopithecus_init__pv(Australopithecus* self, char* n) {
  (void)self;
  self->name = n;
}

void Australopithecus_destroy(Australopithecus* self) {
  (void)self;
}

void Australopithecus_setName__pv(Australopithecus* self, char* n) {
  (void)self;
  self->name = n;
  (void)n;
}

char* Australopithecus_getName(Australopithecus* self) {
  (void)self;
  return self->name;
}

typedef struct HomoHabilis {
  Australopithecus __base;
} HomoHabilis;

void HomoHabilis_init(HomoHabilis* self);
void HomoHabilis_init__pv(HomoHabilis* self, char* n);
void HomoHabilis_destroy(HomoHabilis* self);
int HomoHabilis_fight__N1T(HomoHabilis* self, void* who);

void HomoHabilis_init(HomoHabilis* self) {
  (void)self;
  Australopithecus_init((Australopithecus*)self);
}

void HomoHabilis_init__pv(HomoHabilis* self, char* n) {
  (void)self;
  Australopithecus_init__pv((Australopithecus*)self, n);
}

void HomoHabilis_destroy(HomoHabilis* self) {
  (void)self;
}

int HomoHabilis_fight__N1T(HomoHabilis* self, void* who) {
  (void)self;
  printf("%s fought with %s.\n", Australopithecus_getName((Australopithecus*)self), Australopithecus_getName((Australopithecus*)who));
  return 0;
  (void)who;
}

typedef struct HomoErectus {
  HomoHabilis __base;
} HomoErectus;

void HomoErectus_init(HomoErectus* self);
void HomoErectus_init__pv(HomoErectus* self, char* n);
void HomoErectus_destroy(HomoErectus* self);

void HomoErectus_init(HomoErectus* self) {
  (void)self;
  HomoHabilis_init((HomoHabilis*)self);
}

void HomoErectus_init__pv(HomoErectus* self, char* n) {
  (void)self;
  HomoHabilis_init__pv((HomoHabilis*)self, n);
}

void HomoErectus_destroy(HomoErectus* self) {
  (void)self;
}

typedef struct HomoSapiens {
  HomoErectus __base;
} HomoSapiens;

void HomoSapiens_init(HomoSapiens* self);
void HomoSapiens_init__pv(HomoSapiens* self, char* n);
void HomoSapiens_destroy(HomoSapiens* self);
void HomoSapiens_say__pv(HomoSapiens* self, char* s);
void HomoSapiens_say__pvN11HomoErectus(HomoSapiens* self, char* s, HomoErectus who);

void HomoSapiens_init(HomoSapiens* self) {
  (void)self;
  HomoErectus_init((HomoErectus*)self);
}

void HomoSapiens_init__pv(HomoSapiens* self, char* n) {
  (void)self;
  HomoErectus_init__pv((HomoErectus*)self, n);
}

void HomoSapiens_destroy(HomoSapiens* self) {
  (void)self;
}

void HomoSapiens_say__pv(HomoSapiens* self, char* s) {
  (void)self;
  printf("%s said %c%s%c.\n", Australopithecus_getName((Australopithecus*)self), 34, s, 34);
  (void)s;
}

void HomoSapiens_say__pvN11HomoErectus(HomoSapiens* self, char* s, HomoErectus who) {
  (void)self;
  printf("%s said %c%s%c to %s.\n", Australopithecus_getName((Australopithecus*)self), 34, s, 34, Australopithecus_getName((Australopithecus*)&who));
  (void)s;
  (void)who;
}

typedef struct HomoNeanderthalensis {
  HomoErectus __base;
} HomoNeanderthalensis;

void HomoNeanderthalensis_init(HomoNeanderthalensis* self);
void HomoNeanderthalensis_init__pv(HomoNeanderthalensis* self, char* n);
void HomoNeanderthalensis_destroy(HomoNeanderthalensis* self);

void HomoNeanderthalensis_init(HomoNeanderthalensis* self) {
  (void)self;
  HomoErectus_init((HomoErectus*)self);
}

void HomoNeanderthalensis_init__pv(HomoNeanderthalensis* self, char* n) {
  (void)self;
  HomoErectus_init__pv((HomoErectus*)self, n);
}

void HomoNeanderthalensis_destroy(HomoNeanderthalensis* self) {
  (void)self;
}

/* Global functions */
int main(void);

int main(void) {
  HomoNeanderthalensis fred;
  HomoNeanderthalensis_init__pv(&fred, "Fred");
  HomoSapiens adam;
  HomoSapiens_init__pv(&adam, "Adam");

  HomoSapiens_say__pv(&adam, "What a lovely day!");
  HomoSapiens_say__pvN11HomoErectus(&adam, "Who are you?", fred);
  HomoHabilis_fight__N1T((HomoHabilis*)&fred, &adam);
  return 0;
}
