#include <iostream>
#include <string>
#include <typeinfo>
using namespace std;

template <typename T, size_t N>
class Vector {
public:
    static const size_t WIDTH = N;

    Vector() {
        for (size_t i = 0; i < N; ++i) {
            _data[i] = T();
        }
    }
    
    Vector(const T& initial_value) {
        for (size_t i = 0; i < N; ++i) {
            _data[i] = initial_value;
        }
    }
    
    void operator+=(const Vector& rhs) {
        for (size_t i = 0; i < N; ++i) {
            _data[i] += rhs._data[i];
        }
    }
    
    void operator-=(const Vector& rhs) {
        for (size_t i = 0; i < N; ++i) {
            _data[i] -= rhs._data[i];
        }
    }

    template <typename S>
    void operator*=(const S& scalar) {
        for (size_t i = 0; i < N; ++i) {
            _data[i] *= scalar;
        }
    }

    template <typename S>
    void operator/=(const S& scalar) {
        for (size_t i = 0; i < N; ++i) {
            _data[i] /= scalar;
        }
    }

    bool operator==(const Vector& rhs) const {
        for (size_t i = 0; i < N; ++i) {
            if (!(_data[i] == rhs._data[i])) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Vector& rhs) const {
        return !operator==(rhs);
    }
    
    T& operator[](int index) { return _data[index]; }
    const T& operator[](int index) const { return _data[index]; }

private:
    T _data[N];
};


int main() {
    
    Vector <int, 100> v1;
    Vector <int, 10> v2;
    cout << typeid(std::string).name() << "  " << typeid(v1).name() << "  "<< typeid(v2).name() << endl; 
    // NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE  6VectorIiLm100EE  6VectorIiLm10EE
    return 0;
}