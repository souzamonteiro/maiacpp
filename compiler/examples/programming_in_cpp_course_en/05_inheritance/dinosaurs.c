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

#define EXC_LifeForm 1
#define EXC_Dinosaur 2
#define EXC_Brontosaurus 3
#define EXC_Pterodactyl 4
#define EXC_Tyrannosaurus 5

typedef struct LifeForm {
  int __dummy;
} LifeForm;

void LifeForm_init(LifeForm* self);
void LifeForm_destroy(LifeForm* self);

void LifeForm_init(LifeForm* self) {
  (void)self;
}

void LifeForm_destroy(LifeForm* self) {
  (void)self;
}

typedef struct Dinosaur {
  LifeForm __base;
  char* name;
} Dinosaur;

void Dinosaur_init(Dinosaur* self);
void Dinosaur_init__pv(Dinosaur* self, char* n);
void Dinosaur_destroy(Dinosaur* self);
void Dinosaur_setName__pv(Dinosaur* self, char* n);
char* Dinosaur_getName(Dinosaur* self);
int Dinosaur_eat__N1T(Dinosaur* self, void* other);

void Dinosaur_init(Dinosaur* self) {
  (void)self;
}

void Dinosaur_init__pv(Dinosaur* self, char* n) {
  (void)self;
  self->name = n;
}

void Dinosaur_destroy(Dinosaur* self) {
  (void)self;
}

void Dinosaur_setName__pv(Dinosaur* self, char* n) {
  (void)self;
  self->name = n;
  (void)n;
}

char* Dinosaur_getName(Dinosaur* self) {
  (void)self;
  return self->name;
}

int Dinosaur_eat__N1T(Dinosaur* self, void* other) {
  (void)self;
  LifeForm* __lf_other = (LifeForm*)other;
  printf("%s ate %s.\n", self->name, __lf_other->name);
  (void)other;
}

typedef struct Brontosaurus {
  Dinosaur __base;
} Brontosaurus;

void Brontosaurus_init__pv(Brontosaurus* self, char* n);
void Brontosaurus_destroy(Brontosaurus* self);

void Brontosaurus_init__pv(Brontosaurus* self, char* n) {
  (void)self;
  Dinosaur_init__pv((Dinosaur*)self, n);
}

void Brontosaurus_destroy(Brontosaurus* self) {
  (void)self;
}

typedef struct Pterodactyl {
  Dinosaur __base;
} Pterodactyl;

void Pterodactyl_init__pv(Pterodactyl* self, char* n);
void Pterodactyl_destroy(Pterodactyl* self);

void Pterodactyl_init__pv(Pterodactyl* self, char* n) {
  (void)self;
  Dinosaur_init__pv((Dinosaur*)self, n);
}

void Pterodactyl_destroy(Pterodactyl* self) {
  (void)self;
}

typedef struct Tyrannosaurus {
  Dinosaur __base;
} Tyrannosaurus;

void Tyrannosaurus_init__pv(Tyrannosaurus* self, char* n);
void Tyrannosaurus_destroy(Tyrannosaurus* self);

void Tyrannosaurus_init__pv(Tyrannosaurus* self, char* n) {
  (void)self;
  Dinosaur_init__pv((Dinosaur*)self, n);
}

void Tyrannosaurus_destroy(Tyrannosaurus* self) {
  (void)self;
}

/* Global functions */
int main(void);

int main(void) {
  Brontosaurus dino;
  Brontosaurus_init__pv(&dino, "Dino");
  Pterodactyl peter;
  Pterodactyl_init__pv(&peter, "Peter");
  Tyrannosaurus rex;
  Tyrannosaurus_init__pv(&rex, "Rex");

  printf("The name of dinosaur dino is %s.\n", dino.__base.name);
  printf("The name of dinosaur peter is %s.\n", peter.__base.name);
  printf("The name of dinosaur rex is %s.\n", rex.__base.name);
  printf("%s", rex.__base.name);
  printf(" ate ");
  printf("%s", Dinosaur_getName((Dinosaur*)&dino));
  printf(".\n");
  return 0;
}
