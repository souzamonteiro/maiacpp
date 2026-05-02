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
  float ac;
  float steering;
} Automobile;

void Automobile_init(Automobile* self);
void Automobile_init__pvipvffff(Automobile* self, char* color, int year, char* model, float engine_size, float price, float ac, float steering);
void Automobile_destroy(Automobile* self);

void Automobile_init(Automobile* self) {
  (void)self;
}

void Automobile_init__pvipvffff(Automobile* self, char* color, int year, char* model, float engine_size, float price, float ac, float steering) {
  (void)self;
  (void)color;
  (void)year;
  (void)model;
  (void)engine_size;
  (void)price;
  (void)ac;
  (void)steering;
}

void Automobile_destroy(Automobile* self) {
  (void)self;
}

typedef struct Car {
  Automobile __base;
} Car;

void Car_init(Car* self);
void Car_init__pvipvffff(Car* self, char* color, int year, char* model, float engine_size, float price, float ac, float steering);
void Car_destroy(Car* self);

void Car_init(Car* self) {
  (void)self;
  Automobile_init((Automobile*)self);
}

void Car_init__pvipvffff(Car* self, char* color, int year, char* model, float engine_size, float price, float ac, float steering) {
  (void)self;
  Automobile_init__pvipvffff((Automobile*)self, color, year, model, engine_size, price, ac, steering);
}

void Car_destroy(Car* self) {
  (void)self;
}

typedef struct Truck {
  Automobile __base;
} Truck;

void Truck_init(Truck* self);
void Truck_init__pvipvffff(Truck* self, char* color, int year, char* model, float engine_size, float price, float ac, float steering);
void Truck_destroy(Truck* self);

void Truck_init(Truck* self) {
  (void)self;
  Automobile_init((Automobile*)self);
}

void Truck_init__pvipvffff(Truck* self, char* color, int year, char* model, float engine_size, float price, float ac, float steering) {
  (void)self;
  Automobile_init__pvipvffff((Automobile*)self, color, year, model, engine_size, price, ac, steering);
}

void Truck_destroy(Truck* self) {
  (void)self;
}

typedef struct Tractor {
  Automobile __base;
} Tractor;

void Tractor_init(Tractor* self);
void Tractor_init__pvipvffff(Tractor* self, char* color, int year, char* model, float engine_size, float price, float ac, float steering);
void Tractor_destroy(Tractor* self);

void Tractor_init(Tractor* self) {
  (void)self;
  Automobile_init((Automobile*)self);
}

void Tractor_init__pvipvffff(Tractor* self, char* color, int year, char* model, float engine_size, float price, float ac, float steering) {
  (void)self;
  Automobile_init__pvipvffff((Automobile*)self, color, year, model, engine_size, price, ac, steering);
}

void Tractor_destroy(Tractor* self) {
  (void)self;
}

/* Global functions */
int main(void);

int main(void) {
  Car etios;
  Car_init__pvipvffff(&etios, "Silver", 2021, "XL", 1.4f, 50000.0f, 2000.0f, 3000.0f);
  Truck actros;
  Truck_init__pvipvffff(&actros, "Red", 2022, "X", 6.0f, 500000.0f, 20000.0f, 30000.0f);
  Tractor mf3400;
  Tractor_init__pvipvffff(&mf3400, "Blue", 2022, "MF 3400", 3.0f, 75000.0f, 3000.0f, 4000.0f);
  int vehicleType = 0;
  char wantAC = 0;
  char wantSteering = 0;
  double totalPrice = 0;
  char* model = 0;

  printf("Build your vehicle:\n");
  printf("Would you like to buy a car (1), truck (2) or tractor (3)? ");
  scanf("%d", &vehicleType);
  printf("\nWould you like a vehicle with air conditioning (y/n)? ");
  scanf(" %c", &wantAC);
  printf("\nWould you like a vehicle with power steering (y/n)? ");
  scanf(" %c", &wantSteering);
  switch (vehicleType) {
    case 1:
      model = "XL";
      totalPrice = 50000.0f;
      if (wantAC == 'y') {
        totalPrice += 2000.0f;
      }
      if (wantSteering == 'y') {
        totalPrice += 3000.0f;
      }
      break;
    case 2:
      model = "X";
      totalPrice = 500000.0f;
      if (wantAC == 'y') {
        totalPrice += 20000.0f;
      }
      if (wantSteering == 'y') {
        totalPrice += 30000.0f;
      }
      break;
    case 3:
      model = "MF 3400";
      totalPrice = 75000.0f;
      if (wantAC == 'y') {
        totalPrice += 3000.0f;
      }
      if (wantSteering == 'y') {
        totalPrice += 4000.0f;
      }
      break;
    default:
      printf("Invalid option!");
      return 0;
  }
  printf("The vehicle ");
  printf("%s", model);
  printf(" costs $");
  printf("%g", totalPrice);
  printf(".\n");
  return 0;
}
