#ifndef SINGLECONNLIST_H_INCLUDED
#define SINGLECONNLIST_H_INCLUDED

#include <iostream>
#include "Node.h"

using namespace std;

template<typename T>
struct sList {
    Node<T>* head;

    sList() : head(nullptr) {} //конструктор


    void headInsert(T value);
    void backInsert(T value);
    void delFirst();
    void delLast();
    void delByVal(T val);
    bool find(T value);

    friend ostream& operator<<(ostream& os, const sList<T>& ls) {
        Node<T> *curr = ls.head;
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

};

template struct sList<int>;
template struct sList<string>;

#endif // SINGLECONNLIST_H_INCLUDED
