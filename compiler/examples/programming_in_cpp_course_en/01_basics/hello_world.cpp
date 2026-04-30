// Include the input/output library.
#include <iostream>
// Import the std namespace.
using namespace std;

int main (void) {
        // Without importing the std namespace,
        // it would be necessary to tell the compiler
        // that cout belongs to this namespace.
        //std::cout << "Hello World!";
        cout << "Hello World!";
        return 0;  // We return 0 to indicate the program finished without problems.
}
