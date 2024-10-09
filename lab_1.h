#ifndef LAB_1_H_INCLUDED
#define LAB_1_H_INCLUDED

#include "Stack.h"
#include "arr.h"
#include "list.h"
#include "singleConnList.h"
#include "queue.h"

#include <fstream>

enum commands{
    print,
    Get,
    push,
    pop,
    del,
    insert,
    set
};


enum structures{
    Array,
    List,
    Queue,
    stack,
    HashSet
};


struct request{
    arr<string> query;
    string file;
};


struct fileData{
    string name;
    string data;
};


string delFirstLetter(const string &input);

structures strucRequest(const char& input);
commands commandRequest(const string& input);
request getRequest(int argc, char *argv[]);


void arrPush(const request& request);
void arrDel(const request& request);
void arrGet(const request& request);

void stackPush(const request& request);
void stackPop(const request& request);
void stackGet(const request& request);

void listPush(const request& request);
void listDel(const request& request);
void listGet(const request& request);

void queuePush(const request& request);
void queuePop(const request& request);
void queueGet(const request& request);

#endif // LAB_1_H_INCLUDED
