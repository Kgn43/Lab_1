#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <iostream>
#include "Node.h"
using namespace std;

template <typename T>
struct Stack{
    Node<T>* head;

public:
    Stack() : head(nullptr) {} //конструктор


    friend ostream& operator<<(ostream& os, const Stack<T>& st) {
        auto curr = st.head;
        os << "{";
        while (curr != nullptr){
            os << "[" << curr->value << "]";
            if (curr->next != nullptr) {
                os << ", ";
            }
            curr = curr->next;
        }
        os << "}";
        return os;
    }

    //методы
    void push(T val);
    void pop();
    T getLast();
    void out();
    void clear();
};


template struct Stack<int>;
template struct Stack<string>;


Stack<string> splitStack(const string &input, char delimiter = ' ');
string unSplitStack(const Stack<string>& input, char delimiter = ' ');

#endif // STACK_H_INCLUDED
