#ifndef ARR_H_INCLUDED
#define ARR_H_INCLUDED

#include <iostream>
#include <sstream>
using namespace std;

template <typename T>
struct arr {
    T* data;      // Указатель на динамический массив
    size_t size;  // Текущий размер вектора
    size_t capacity; // Вместимость вектора

    //конструктор
    arr<T>() : data(nullptr), size(0), capacity(0) {}

    void realoc();
    void push_back(const T& value);
    size_t getSize() const;
    void erase();
    void print();
    void del(size_t index);
    void clear();


    // Перегрузка оператора вывода
    friend ostream& operator<<(ostream& os, const arr& arr) {
        os << "[";
        for (size_t i = 0; i < arr.size; ++i) {
            os << arr.data[i];
            if (i < arr.size - 1) {
                os << " ";
            }
        }
        os << "]";
        return os;
    }

    // Метод для получения элемента по индексу
    T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

};

template struct arr<int>;
template struct arr<string>;


arr<string> split(const string &input, char delimiter = ' ');
string unsplit(const arr<string>& array, char delimiter = ' ');

#endif // ARR_H_INCLUDED
