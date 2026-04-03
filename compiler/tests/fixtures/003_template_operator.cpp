template <typename T>
class Box {
public:
    T& operator[](int i) { return data[i]; }
private:
    T data[4];
};
