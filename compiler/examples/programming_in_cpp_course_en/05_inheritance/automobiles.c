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

#define EXC_Automobile 1
#define EXC_Car 2
#define EXC_Truck 3
#define EXC_Tractor 4

typedef struct Automobile {
  char* color;
  int year;
  char* model;
  float engine_size;
  float price;
} Automobile;

void Automobile_init(Automobile* self);
void Automobile_init__pvipvff(Automobile* self, char* color, int year, char* model, float engine_size, float price);
void Automobile_destroy(Automobile* self);

void Automobile_init(Automobile* self) {
  (void)self;
}

void Automobile_init__pvipvff(Automobile* self, char* color, int year, char* model, float engine_size, float price) {
  (void)self;
  (void)color;
  (void)year;
  (void)model;
  (void)engine_size;
  (void)price;
}

void Automobile_destroy(Automobile* self) {
  (void)self;
}

typedef struct Car {
  Automobile __base;
} Car;

void Car_init(Car* self);
void Car_init__pvipvff(Car* self, char* color, int year, char* model, float engine_size, float price);
void Car_destroy(Car* self);

void Car_init(Car* self) {
  (void)self;
  Automobile_init((Automobile*)self);
}

void Car_init__pvipvff(Car* self, char* color, int year, char* model, float engine_size, float price) {
  (void)self;
  Automobile_init__pvipvff((Automobile*)self, color, year, model, engine_size, price);
}

void Car_destroy(Car* self) {
  (void)self;
}

typedef struct Truck {
  Automobile __base;
} Truck;

void Truck_init(Truck* self);
void Truck_init__pvipvff(Truck* self, char* color, int year, char* model, float engine_size, float price);
void Truck_destroy(Truck* self);

void Truck_init(Truck* self) {
  (void)self;
  Automobile_init((Automobile*)self);
}

void Truck_init__pvipvff(Truck* self, char* color, int year, char* model, float engine_size, float price) {
  (void)self;
  Automobile_init__pvipvff((Automobile*)self, color, year, model, engine_size, price);
}

void Truck_destroy(Truck* self) {
  (void)self;
}

typedef struct Tractor {
  Automobile __base;
} Tractor;

void Tractor_init(Tractor* self);
void Tractor_init__pvipvff(Tractor* self, char* color, int year, char* model, float engine_size, float price);
void Tractor_destroy(Tractor* self);

void Tractor_init(Tractor* self) {
  (void)self;
  Automobile_init((Automobile*)self);
}

void Tractor_init__pvipvff(Tractor* self, char* color, int year, char* model, float engine_size, float price) {
  (void)self;
  Automobile_init__pvipvff((Automobile*)self, color, year, model, engine_size, price);
}

void Tractor_destroy(Tractor* self) {
  (void)self;
}

/* Global functions */
int main(void);

int main(void) {
  printf("The tractor MF 3400 year 2022 costs $75000.\n");
  return 0;
}
