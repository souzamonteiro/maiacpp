template <typename T>
class Array {
public:
    T& operator[](int index) {
        return data[index];
    }

private:
    T data[10];
};
