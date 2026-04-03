/**
 * C++98/03 Test Suite - Testa todas as funcionalidades do parser
 * 
 * Compilar com: g++ -std=c++98 -Wall -Wextra -pedantic -o cpp_test cpp_test.cpp
 * Executar: ./cpp_test
 * 
 * Saída esperada: Verificação de todas as funcionalidades
 */

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <stdexcept>
#include <cassert>
#include <cstring>
#include <cstdlib>

// ============================================================================
// 1. NAMESPACES - Testa namespace, using-declaration, using-directive
// ============================================================================

namespace Math {
    const double PI = 3.14159265358979323846;
    
    double square(double x) {
        return x * x;
    }
    
    namespace Advanced {
        double cube(double x) {
            return x * x * x;
        }
    }
}

namespace Geometry {
    using Math::PI;
    
    class Circle {
    public:
        explicit Circle(double r) : radius(r) {}
        double area() const { return PI * radius * radius; }
    private:
        double radius;
    };
}

namespace Outer {
    namespace Inner {
        int value = 42;
    }
    
    using namespace Inner;
}

using namespace Geometry;
using namespace Outer;

// ============================================================================
// 2. CLASSES - Testa classes, herança, virtual functions, const, mutable
// ============================================================================

class Base {
public:
    Base() : m_data(0) {}
    explicit Base(int x) : m_data(x) {}
    virtual ~Base() {}
    
    virtual int getValue() const { return m_data; }
    virtual void setValue(int x) { m_data = x; }
    
    // const member function
    int getConstValue() const { return m_data; }
    
    // static member
    static int getCount() { return s_count; }
    
protected:
    int m_data;
    static int s_count;
    
public:
    // Helper for pointer to member test
    static int Base::* getDataPtr() { return &Base::m_data; }
};

int Base::s_count = 0;

class Derived : public Base {
public:
    Derived() : Base(), m_extra(0) { s_count++; }
    explicit Derived(int x, int y = 0) : Base(x), m_extra(y) { s_count++; }
    virtual ~Derived() { s_count--; }
    
    virtual int getValue() const { return m_data + m_extra; }
    
    // override with different return type (covariant)
    virtual Derived* clone() const { return new Derived(*this); }
    
    // static member function
    static void showCount() {
        std::cout << "Derived count: " << s_count << std::endl;
    }
    
private:
    int m_extra;
};

// ============================================================================
// 3. TEMPLATES - Testa templates de classe e função
// ============================================================================

// Class template
template <typename T, size_t N = 10>
class Array {
public:
    Array() : m_size(0) {}
    
    void push_back(const T& value) {
        if (m_size < N) {
            m_data[m_size++] = value;
        }
    }
    
    T& operator[](size_t index) {
        return m_data[index];
    }
    
    const T& operator[](size_t index) const {
        return m_data[index];
    }
    
    size_t size() const { return m_size; }
    static size_t capacity() { return N; }
    
private:
    T m_data[N];
    size_t m_size;
};

// Function template
template <typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// Function template specialization
template <>
const char* max<const char*>(const char* a, const char* b) {
    return (std::strcmp(a, b) > 0) ? a : b;
}

// Template with multiple parameters
template <typename T, typename U>
struct Pair {
    T first;
    U second;
    
    Pair() : first(T()), second(U()) {}
    Pair(const T& f, const U& s) : first(f), second(s) {}
};

// ============================================================================
// 4. EXCEPTION HANDLING - Testa try, catch, throw
// (dynamic exception specifications são deprecated, usamos throw() apenas)
// ============================================================================

class MyException : public std::exception {
public:
    MyException(const char* msg) : m_msg(msg) {}
    virtual const char* what() const throw() { return m_msg; }
private:
    const char* m_msg;
};

class TestExceptions {
public:
    // Sem exception-specification para compatibilidade com C++17
    void mayThrow(bool shouldThrow) {
        if (shouldThrow) {
            throw MyException("Error occurred");
        }
    }
    
    void rethrowExample() {
        try {
            throw std::runtime_error("Test");
        } catch (const std::exception& e) {
            std::cout << "Caught: " << e.what() << std::endl;
            throw;  // rethrow
        }
    }
    
    // Função com throw() - não lança exceções
    void noThrow() throw() {
        // não lança exceções
    }
};

// ============================================================================
// 5. OPERATOR OVERLOADING - Testa sobrecarga de operadores
// ============================================================================

class Complex {
public:
    Complex(double r = 0.0, double i = 0.0) : re(r), im(i) {}
    
    // Unary operators
    Complex operator-() const { return Complex(-re, -im); }
    Complex operator+() const { return *this; }
    
    // Binary operators
    Complex operator+(const Complex& other) const {
        return Complex(re + other.re, im + other.im);
    }
    
    Complex operator-(const Complex& other) const {
        return Complex(re - other.re, im - other.im);
    }
    
    Complex operator*(const Complex& other) const {
        return Complex(re * other.re - im * other.im,
                       re * other.im + im * other.re);
    }
    
    // Assignment operators
    Complex& operator+=(const Complex& other) {
        re += other.re;
        im += other.im;
        return *this;
    }
    
    // Comparison operators
    bool operator==(const Complex& other) const {
        return re == other.re && im == other.im;
    }
    
    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }
    
    // Subscript operator
    double& operator[](size_t index) {
        return (index == 0) ? re : im;
    }
    
    const double& operator[](size_t index) const {
        return (index == 0) ? re : im;
    }
    
    // Function call operator
    double operator()(double x) const {
        return re * x + im;
    }
    
    // Stream operators (friend)
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
    
private:
    double re, im;
};

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.re << " + " << c.im << "i";
    return os;
}

// ============================================================================
// 6. TEMPLATE SPECIALIZATION - Testa especialização e partial specialization
// ============================================================================

// Primary template
template <typename T>
class Wrapper {
public:
    static std::string name() { return "generic"; }
    T value;
};

// Full specialization
template <>
class Wrapper<int> {
public:
    static std::string name() { return "int specialization"; }
    int value;
};

// Partial specialization for pointers
template <typename T>
class Wrapper<T*> {
public:
    static std::string name() { return "pointer partial specialization"; }
    T* value;
};

// ============================================================================
// 7. CONSTRUCTORS, DESTRUCTORS, INITIALIZERS
// ============================================================================

class Resource {
public:
    Resource() : m_id(++s_lastId) {
        std::cout << "Resource " << m_id << " constructed" << std::endl;
    }
    
    Resource(const Resource& other) : m_id(++s_lastId) {
        std::cout << "Resource " << m_id << " copy constructed" << std::endl;
    }
    
    ~Resource() {
        std::cout << "Resource " << m_id << " destroyed" << std::endl;
    }
    
    Resource& operator=(const Resource& other) {
        std::cout << "Resource " << m_id << " assigned" << std::endl;
        return *this;
    }
    
    int id() const { return m_id; }
    
private:
    int m_id;
    static int s_lastId;
};

int Resource::s_lastId = 0;

class Container {
public:
    // Default constructor
    Container() : m_data(0), m_res() {}
    
    // Constructor with initializer list
    Container(int x) : m_data(x), m_res() {}
    
    // Copy constructor
    Container(const Container& other) : m_data(other.m_data), m_res(other.m_res) {}
    
    // Assignment operator
    Container& operator=(const Container& other) {
        if (this != &other) {
            m_data = other.m_data;
            m_res = other.m_res;
        }
        return *this;
    }
    
    // Destructor
    ~Container() {}
    
private:
    int m_data;
    Resource m_res;
};

// ============================================================================
// 8. STATIC MEMBERS AND CONSTANTS
// ============================================================================

class Constants {
public:
    static const int MAX_SIZE = 100;
    static const std::string DEFAULT_NAME;
    static int instanceCount;
    
    Constants() { instanceCount++; }
    ~Constants() { instanceCount--; }
    
    static void printCount() {
        std::cout << "Instances: " << instanceCount << std::endl;
    }
};

const std::string Constants::DEFAULT_NAME = "default";
int Constants::instanceCount = 0;

// ============================================================================
// 9. ENUMERATIONS - Testa enum
// ============================================================================

enum Color {
    RED = 1,
    GREEN = 2,
    BLUE = 4
};

enum Flags {
    FLAG_READ = 0x01,
    FLAG_WRITE = 0x02,
    FLAG_EXEC = 0x04
};

// Enum with values
enum Status {
    STATUS_OK = 0,
    STATUS_ERROR = -1,
    STATUS_PENDING = 1
};

// ============================================================================
// 10. TYPEDEFS AND TYPE ALIASES
// ============================================================================

typedef std::vector<int> IntVector;
typedef std::list<std::string> StringList;
typedef int (*FunctionPtr)(int, int);

// Function for function pointer
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

// ============================================================================
// 11. NESTED CLASSES
// ============================================================================

class OuterClass {
public:
    OuterClass(int v) : m_value(v) {}
    
    class Inner {
    public:
        Inner(int x) : m_x(x) {}
        int getValue() const { return m_x; }
    private:
        int m_x;
    };
    
    // Nested class can access outer members
    class Nested {
    public:
        Nested(const OuterClass& outer) : m_outer(outer) {}
        int getOuterValue() const { return m_outer.m_value; }
    private:
        const OuterClass& m_outer;
    };
    
private:
    int m_value;
};

// ============================================================================
// 12. CAST OPERATORS - Testa static_cast, dynamic_cast, const_cast, reinterpret_cast
// ============================================================================

void testCasts() {
    // static_cast
    double pi = 3.14159;
    int intPi = static_cast<int>(pi);
    assert(intPi == 3);
    
    // dynamic_cast
    Base* basePtr = new Derived(10, 5);
    Derived* derivedPtr = dynamic_cast<Derived*>(basePtr);
    assert(derivedPtr != 0);
    assert(derivedPtr->getValue() == 15);
    delete basePtr;
    
    // const_cast
    const int constValue = 42;
    int* mutablePtr = const_cast<int*>(&constValue);
    // Not modifying, just demonstrating cast
    (void)mutablePtr;
    
    // reinterpret_cast
    int value = 0x12345678;
    char* bytePtr = reinterpret_cast<char*>(&value);
    (void)bytePtr;
}

// ============================================================================
// 13. TYPEID AND RTTI
// ============================================================================

#include <typeinfo>

void testTypeInfo() {
    Base* base = new Derived();
    const std::type_info& ti = typeid(*base);
    std::cout << "Type: " << ti.name() << std::endl;
    delete base;
}

// ============================================================================
// 14. NEW AND DELETE - Testa new, delete, placement new
// ============================================================================

class PlacementTest {
public:
    PlacementTest(int x) : m_x(x) {
        std::cout << "PlacementTest constructed with " << x << std::endl;
    }
    ~PlacementTest() {
        std::cout << "PlacementTest destroyed" << std::endl;
    }
private:
    int m_x;
};

void testNewDelete() {
    // Regular new/delete
    int* p = new int(42);
    delete p;
    
    // Array new/delete
    int* arr = new int[10];
    delete[] arr;
    
    // Placement new
    char buffer[sizeof(PlacementTest)];
    PlacementTest* pt = new(buffer) PlacementTest(123);
    pt->~PlacementTest();  // Explicit destructor call
}

// ============================================================================
// 15. FUNCTION OVERLOADING
// ============================================================================

void print(int x) {
    std::cout << "int: " << x << std::endl;
}

void print(double x) {
    std::cout << "double: " << x << std::endl;
}

void print(const char* s) {
    std::cout << "string: " << s << std::endl;
}

void print(const std::string& s) {
    std::cout << "std::string: " << s << std::endl;
}

// ============================================================================
// 16. TEMPLATE FUNCTION OVERLOADING
// ============================================================================

template <typename T>
void show(const T& t) {
    std::cout << "Generic: " << t << std::endl;
}

template <>
void show<int>(const int& t) {
    std::cout << "Specialized int: " << t << std::endl;
}

void show(const char* t) {
    std::cout << "Non-template: " << t << std::endl;
}

// ============================================================================
// 17. CONST AND VOLATILE
// ============================================================================

class ConstTest {
public:
    ConstTest() : m_value(0), m_mutableValue(0) {}
    
    void modify() { m_value++; }
    int get() const { return m_value; }
    
    mutable int m_mutableValue;  // mutable member
    
private:
    int m_value;
};

// ============================================================================
// 18. FRIEND FUNCTIONS AND CLASSES
// ============================================================================

class FriendDemo {
public:
    FriendDemo(int x) : m_secret(x) {}
    
    friend void showSecret(const FriendDemo& demo);
    friend class FriendAccessor;
    
private:
    int m_secret;
};

void showSecret(const FriendDemo& demo) {
    std::cout << "Secret: " << demo.m_secret << std::endl;
}

class FriendAccessor {
public:
    int getSecret(const FriendDemo& demo) const {
        return demo.m_secret;
    }
};

// ============================================================================
// 19. POINTERS TO MEMBERS
// ============================================================================

void testPointersToMembers() {
    // Usando o método público para obter o ponteiro
    int Base::*dataPtr = Base::getDataPtr();
    
    Base b(100);
    assert(b.*dataPtr == 100);
    
    // Teste com ponteiro para função membro
    int (Base::*getPtr)() const = &Base::getValue;
    assert((b.*getPtr)() == 100);
}

// ============================================================================
// 20. FUNCTION POINTERS
// ============================================================================

int execute(int a, int b, int (*func)(int, int)) {
    return func(a, b);
}

// ============================================================================
// MAIN - Testa todas as funcionalidades
// ============================================================================

int main() {
    std::cout << "=== C++ Parser Test Suite ===" << std::endl;
    std::cout << "Testing all language features..." << std::endl;
    std::cout << std::endl;
    
    // 1. Namespace tests
    std::cout << "1. Testing namespaces..." << std::endl;
    std::cout << "PI = " << Math::PI << std::endl;
    std::cout << "Square of 5 = " << Math::square(5) << std::endl;
    std::cout << "Cube of 3 = " << Math::Advanced::cube(3) << std::endl;
    Circle circle(2.5);
    std::cout << "Circle area = " << circle.area() << std::endl;
    std::cout << "Inner namespace value = " << Inner::value << std::endl;
    std::cout << std::endl;
    
    // 2. Class tests
    std::cout << "2. Testing classes and inheritance..." << std::endl;
    Base baseObj(10);
    Derived derivedObj(20, 5);
    std::cout << "Base value = " << baseObj.getValue() << std::endl;
    std::cout << "Derived value = " << derivedObj.getValue() << std::endl;
    std::cout << "Base::getConstValue = " << baseObj.getConstValue() << std::endl;
    Base::getCount();
    Derived::showCount();
    std::cout << std::endl;
    
    // 3. Template tests
    std::cout << "3. Testing templates..." << std::endl;
    Array<int, 5> intArray;
    intArray.push_back(10);
    intArray.push_back(20);
    intArray.push_back(30);
    std::cout << "Array size = " << intArray.size() << std::endl;
    std::cout << "Array[0] = " << intArray[0] << std::endl;
    std::cout << "Max(10, 20) = " << max(10, 20) << std::endl;
    std::cout << "Max(3.14, 2.71) = " << max(3.14, 2.71) << std::endl;
    Pair<int, std::string> p(1, "one");
    std::cout << "Pair: " << p.first << ", " << p.second << std::endl;
    std::cout << std::endl;
    
    // 4. Exception tests
    std::cout << "4. Testing exceptions..." << std::endl;
    TestExceptions te;
    try {
        te.mayThrow(true);
    } catch (const MyException& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    try {
        te.rethrowExample();
    } catch (const std::exception& e) {
        std::cout << "Rethrown: " << e.what() << std::endl;
    }
    std::cout << std::endl;
    
    // 5. Operator overloading tests
    std::cout << "5. Testing operator overloading..." << std::endl;
    Complex comp1(3, 4);
    Complex comp2(1, 2);
    Complex comp3 = comp1 + comp2;
    Complex comp4 = -comp1;
    std::cout << "comp1 = " << comp1 << std::endl;
    std::cout << "comp2 = " << comp2 << std::endl;
    std::cout << "comp1 + comp2 = " << comp3 << std::endl;
    std::cout << "-comp1 = " << comp4 << std::endl;
    std::cout << "comp1(5) = " << comp1(5) << std::endl;
    std::cout << "comp1[0] = " << comp1[0] << std::endl;
    std::cout << std::endl;
    
    // 6. Template specialization tests
    std::cout << "6. Testing template specialization..." << std::endl;
    Wrapper<double> wd;
    Wrapper<int> wi;
    Wrapper<double*> wp;
    std::cout << "Wrapper<double> name = " << wd.name() << std::endl;
    std::cout << "Wrapper<int> name = " << wi.name() << std::endl;
    std::cout << "Wrapper<double*> name = " << wp.name() << std::endl;
    std::cout << std::endl;
    
    // 7. Constructors/Destructors tests
    std::cout << "7. Testing constructors/destructors..." << std::endl;
    {
        Container cont1;
        Container cont2(42);
        Container cont3(cont2);
        cont1 = cont2;
    }
    std::cout << "(Resources destroyed after scope)" << std::endl;
    std::cout << std::endl;
    
    // 8. Static members tests (renomeado para ConstantsObj para evitar conflito)
    std::cout << "8. Testing static members..." << std::endl;
    Constants::printCount();
    Constants constObj1, constObj2;
    Constants::printCount();
    std::cout << "MAX_SIZE = " << Constants::MAX_SIZE << std::endl;
    std::cout << "DEFAULT_NAME = " << Constants::DEFAULT_NAME << std::endl;
    std::cout << std::endl;
    
    // 9. Enumeration tests
    std::cout << "9. Testing enumerations..." << std::endl;
    Color color = GREEN;
    Flags flags = static_cast<Flags>(FLAG_READ | FLAG_WRITE);
    std::cout << "Color value = " << color << std::endl;
    std::cout << "Flags value = " << flags << std::endl;
    std::cout << std::endl;
    
    // 10. Typedef tests
    std::cout << "10. Testing typedefs..." << std::endl;
    IntVector vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    StringList lst;
    lst.push_back("hello");
    lst.push_back("world");
    FunctionPtr fp = &add;
    std::cout << "add(5,3) = " << fp(5, 3) << std::endl;
    std::cout << "execute(6,2,multiply) = " << execute(6, 2, multiply) << std::endl;
    std::cout << std::endl;
    
    // 11. Nested classes tests
    std::cout << "11. Testing nested classes..." << std::endl;
    OuterClass outer(99);
    OuterClass::Inner inner(50);
    OuterClass::Nested nested(outer);
    std::cout << "Inner value = " << inner.getValue() << std::endl;
    std::cout << "Nested outer value = " << nested.getOuterValue() << std::endl;
    std::cout << std::endl;
    
    // 12. Casts tests
    std::cout << "12. Testing casts..." << std::endl;
    testCasts();
    std::cout << "Casts completed successfully" << std::endl;
    std::cout << std::endl;
    
    // 13. TypeInfo tests
    std::cout << "13. Testing typeid..." << std::endl;
    testTypeInfo();
    std::cout << std::endl;
    
    // 14. New/Delete tests
    std::cout << "14. Testing new/delete..." << std::endl;
    testNewDelete();
    std::cout << std::endl;
    
    // 15. Function overloading tests
    std::cout << "15. Testing function overloading..." << std::endl;
    print(10);
    print(3.14);
    print("hello");
    print(std::string("world"));
    std::cout << std::endl;
    
    // 16. Template function overloading tests
    std::cout << "16. Testing template function overloading..." << std::endl;
    show(42);
    show(3.14);
    show("test");
    std::cout << std::endl;
    
    // 17. Const/Volatile tests
    std::cout << "17. Testing const/volatile..." << std::endl;
    ConstTest ct;
    ct.modify();
    std::cout << "get() = " << ct.get() << std::endl;
    ct.m_mutableValue = 100;
    std::cout << "mutable member = " << ct.m_mutableValue << std::endl;
    std::cout << std::endl;
    
    // 18. Friend tests
    std::cout << "18. Testing friend functions/classes..." << std::endl;
    FriendDemo fd(12345);
    showSecret(fd);
    FriendAccessor fa;
    std::cout << "Secret via accessor: " << fa.getSecret(fd) << std::endl;
    std::cout << std::endl;
    
    // 19. Pointers to members tests
    std::cout << "19. Testing pointers to members..." << std::endl;
    testPointersToMembers();
    std::cout << "Pointers to members test passed" << std::endl;
    std::cout << std::endl;
    
    // 20. Function pointers tests
    std::cout << "20. Testing function pointers..." << std::endl;
    std::cout << "execute(7,3,add) = " << execute(7, 3, add) << std::endl;
    std::cout << "execute(7,3,multiply) = " << execute(7, 3, multiply) << std::endl;
    std::cout << std::endl;
    
    // 21. STL container tests (verifica includes)
    std::cout << "21. Testing STL containers..." << std::endl;
    std::map<std::string, int> ages;
    ages["Alice"] = 30;
    ages["Bob"] = 25;
    for (std::map<std::string, int>::const_iterator it = ages.begin();
         it != ages.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    std::cout << std::endl;
    
    // 22. Static cast with pointer to void
    std::cout << "22. Testing static_cast with void*..." << std::endl;
    int number = 999;
    void* vptr = static_cast<void*>(&number);
    int* iptr = static_cast<int*>(vptr);
    std::cout << "Value after void* roundtrip: " << *iptr << std::endl;
    std::cout << std::endl;
    
    std::cout << "=== ALL TESTS COMPLETED SUCCESSFULLY ===" << std::endl;
    std::cout << "Parser validation: All features tested" << std::endl;
    
    return 0;
}