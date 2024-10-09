#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <string>
#include <iostream>
using namespace std;

template <typename T>
struct listNode {
public:
    T value;
    listNode* next;
    listNode* previous;

    //конструкторы
    listNode(T val, listNode* following, listNode* prev) : value(val), next(following), previous(prev) {}
    listNode(T val) : value(val), next(nullptr), previous(nullptr) {}
    listNode() : value(T()), next(nullptr), previous(nullptr) {} //не уверен, но тут могут возникать ошибки, если нет конструктора по умолчанию для АТД
};



template <typename T>
struct list{
    listNode<T>* first;
    listNode<T>* last;

    //конструктор
    list<T>(): first(nullptr), last(nullptr) {}

    void headInsert(T value);
    void backInsert(T value);
    void delFirst();
    void delLast();
    void delByVal(T val);
    bool find(T value);


    friend ostream& operator<<(ostream& os, const list<T>& ls) {
        listNode<T> *curr = ls.first;
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

template struct list<int>;
template struct list<string>;

list<string> splitToList(const string &input, char delimiter = ' ');
string unSplitList(const list<string>& input, char delimiter = ' ');


#endif // LIST_H_INCLUDED
