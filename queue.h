#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <iostream>
#include "Node.h"
using namespace std;

template<typename T>
struct queue{
    Node<T>* head;
    Node<T>* tail;

    queue() : head(nullptr), tail(nullptr) {} //конструктор

    //методы
    void push(T val);
    void pop();
    T getFirst();
    void clear();


    friend ostream& operator<<(ostream& os, const queue<T>& que) {
        Node<T>* curr = que.head;
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


template struct queue<int>;
template struct queue<string>;

queue<string> splitToQueue(const string &input, char delimiter = ' ');
string unSplitQueue(const queue<string>& input, char delimiter = ' ');

#endif // QUEUE_H_INCLUDED
