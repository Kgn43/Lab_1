#include "list.h"


template<typename T>
void list<T>::backInsert(T value) {
    listNode<T>* newLast = new listNode(value);
    if (last == nullptr){
        last = newLast;
        first = newLast;
    }
    else{
        newLast->previous = last;
        last->next = newLast;
        last = newLast;
    }
}

template<typename T>
void list<T>::headInsert(T value) {
    listNode<T>* newFirst = new listNode(value);
    if (last == nullptr){
        last = newFirst;
        first = newFirst;
    }
    else{
        first->previous = newFirst;
        newFirst->next = first;
        first = newFirst;
    }
}


template<typename T>
void list<T>::delLast() {
    last = last->previous;
    delete last->next;
    last->next = nullptr;
}


template<typename T>
void list<T>::delFirst() {
    first = first->next;
    delete first->previous;
    first->previous = nullptr;
}


template<typename T>
bool list<T>::find(T value) {
    listNode<T> *tmp = first;
    while (tmp != nullptr){
        if (tmp->value == value){
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}



template<typename T>
void list<T>::delByVal(T val) {
    listNode<T> *tmp = first;
    while (tmp != nullptr){
        if (tmp->value == val){
            tmp->next->previous = tmp->previous;
            tmp->previous->next = tmp->next;
            delete tmp;
            break;
        }
        tmp = tmp->next;
    }
}


list<string> splitToList(const string &input, char delimiter){
    string word;
    list<string> output;
    for (auto ch : input){
        if (ch == delimiter) {
            output.backInsert(word);
            word = "";
        }
        else{
            word += ch;
        }
    }
    if (word != ""){
        output.backInsert(word);
    }
    return output;
}


//template<typename T>
string unSplitList(const list<string>& input, char delimiter){
    listNode<string>* curr = input.first;
    string output;
    while (curr != nullptr){
        output += curr->value + delimiter;
        curr = curr->next;
    }
    return output;
}
