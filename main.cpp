#include "lab_1.h"


int main(int argc, char *argv[]) {
    request request = getRequest(argc, argv);
    if (argc != 1){
        if (request.query[0] == "print"){
            ///сделать вывод
        } else {
            try {
                structures strucType = strucRequest(request.query[0][0]); //определяем по букве структуру
                request.query[0] = delFirstLetter(request.query[0]); //убираем букву для упрощения чтения команды
                switch (strucType) {
                    case Array:
                        switch (commandRequest(request.query[0])) {
                            case push:
                                arrPush(request);
                                break;
                            case del:
                                arrDel(request);
                                break;
                            case Get:
                                arrGet(request);
                                break;
                            default:
                                cout << "There is no such Array processing command" << endl;
                        }
                        break;
                    case stack:
                        switch (commandRequest(request.query[0])) {
                            case push:
                                stackPush(request);
                                break;
                            case pop:
                                stackPop(request);
                                break;
                            case Get:
                                stackGet(request);
                                break;
                            default:
                                cout << "There is no such Stack processing command" << endl;
                        }
                        break;
                    case List:
                        switch (commandRequest(request.query[0])) {
                            case push:
                                listPush(request);
                                break;
                            case del:
                                listDel(request);
                                break;
                            case Get:
                                listGet(request);
                                break;
                            default:
                                cout << "There is no such List processing command" << endl;
                        }
                        break;
                    case Queue:
                        switch (commandRequest(request.query[0])) {
                            case push:
                                queuePush(request);
                                break;
                            case pop:
                                queuePop(request);
                                break;
                            case Get:
                                queueGet(request);
                                break;
                            default:
                                cout << "There is no such Queue processing command" << endl;
                        }
                        break;
                    case HashSet:
                        break;
                }
            } catch (exception &ex) {
                cout << ex.what() << endl;
                return -1;
            }
        }
    }
    return 0;
}


string delFirstLetter(const string &input){
    string output;
    bool z = false;
    for(auto ch : input){
        if(!z){
            z = true;
        }
        else {
            output += ch;
        }
    }
    return output;
}


commands commandRequest(const string& input){
    if (input == "print") return commands::print;
    if (input == "push") return commands::push;
    if (input == "pop") return commands::pop;
    if (input == "del") return commands::del;
    if (input == "Get") return commands::Get;
    if (input == "set") return commands::set;
    if (input == "insert") return commands::insert;
    else{
        stringstream serr;
        serr << "This command isn't exist";
        throw runtime_error(serr.str());
    }
}


structures strucRequest(const char& input){
    if (input == 'A') return structures::Array;
    if (input == 'L') return structures::List;
    if (input == 'Q') return structures::Queue;
    if (input == 'S') return structures::stack;
    if (input == 'H') return structures::HashSet;
    else{
        stringstream serr;
        serr << "This structure is not exist";
        throw runtime_error(serr.str());
    }
}


request getRequest(int argc, char *argv[] ){
    request output;
    for (int i = 0; i < argc; ++i){
        if (static_cast<string>(argv[i]) == "--file") {
            if (argv[i + 1][0] != '-'){
                output.file = argv[i + 1];
            }
        }
        if (static_cast<string>(argv[i]) == "--query") {
            if (i + 1 == argc && argv[i][0] != '-'){
                output.query = split(static_cast<string>(argv[i]));
                continue;
            }
            else if (argv[i + 1][0] != '-'){
                output.query = split(static_cast<string>(argv[i + 1]));
            }
        }
    }
    return output;
}


void arrPush(const request& request){
//структура команды1 (по индексу): push имяМассива куда что
//структура команды2 (в конец): push имяМассива что
    fstream file(request.file, ios::in);
    if(!file.is_open()){
        stringstream serr;
        serr << "This file doesn't exist";
        throw runtime_error(serr.str());
    }
    fstream tmpFile("tmp.data", ios::out);
    if(!tmpFile.is_open()){
        stringstream serr;
        serr << "Tmp file doesn't exist";
        throw runtime_error(serr.str());
    }
    string name; //куда записать
    string value; //что записать
    int index;
    string variableLine; //считываемая строка с файла
    fileData var;
    if (request.query.size == 3) {
        name = request.query[1];
        index = -1;
        value = request.query[2];
    }
    else if (request.query.size == 4){
        name = request.query[1];
        index = stoi(request.query[2]);
        value = request.query[3];
    }
    else {
        stringstream serr;
        serr << "Wrong command syntax";
        throw runtime_error(serr.str());
    }
    bool varIsExist = false;
    while (getline(file, variableLine)){ //проверяем все существующие переменные
        if (variableLine == " " || variableLine.empty()) continue;
        var.name = split(variableLine, ';')[0]; //определяем их имена
        var.data = split(variableLine, ';')[1]; //и то, что они хранят
        if (var.name == name){ //если такая переменная существует
            varIsExist = true; //закрываем защёлку
            arr<string> currVar = split(var.data); //определяем реальную переменную этого Типа данных
            currVar.push_back(value); //закидываем то, что просят в конец
            if (currVar.size - 1 != index && index != -1){
                string tmp;//третий стакан
                for(size_t i = currVar.size - 1; i > index; --i){ //и меняем местами до нужного индекса
                    tmp = currVar.data[i];
                    currVar.data[i] = currVar.data[i - 1];
                    currVar.data[i - 1] = tmp;
                }
            }
            variableLine = var.name + ';' + unsplit(currVar);//превращаем переменную в текст
            tmpFile << variableLine << endl;
            //currVar.clear();
        }
        else {
            tmpFile << variableLine << endl;
        }
    }
    if (!varIsExist){
        cout << "making new array" << endl;
        if (index != 0 && index != -1){//попытка вставить на какой-либо индекс кроме нуля в несуществующую переменную. рука-лицо.
            stringstream serr;
            serr << "Wrong index";
            throw runtime_error(serr.str());
        }
        arr<string> newVar;//да, можно и не делать этого. но мы сделаем. потому, что можем.
        newVar.push_back(value);
        variableLine = name + ';' + unsplit(newVar);//превращаем переменную в текст
        tmpFile << variableLine;
        //newVar.clear();
    }
    file.close();
    tmpFile.close();
    file.open(request.file, ios::out);
    tmpFile.open("tmp.data", ios::in);
    while (getline(tmpFile, variableLine)){
        file << variableLine << endl;
    }
    file.close();
    tmpFile.close();
}


void arrDel(const request& request){
//структура команды: del имяМассива индекс
    fstream file(request.file, ios::in);
    if(!file.is_open()){
        stringstream serr;
        serr << "This file doesn't exist";
        throw runtime_error(serr.str());
    }
    fstream tmpFile("tmp.data", ios::out);
    if(!tmpFile.is_open()){
        stringstream serr;
        serr << "This file doesn't exist";
        throw runtime_error(serr.str());
    }
    string name = request.query[1]; //из какого массива удалять
    size_t index = stoi(request.query[2]); //с какой позиции
    string variableLine; //считываемая строка с файла
    fileData var;
    bool varIsExist = false;
    while (getline(file, variableLine)){ //проверяем все существующие переменные
        if (variableLine == " " || variableLine.empty()) continue;
        var.name = split(variableLine, ';')[0]; //определяем их имена
        var.data = split(variableLine, ';')[1]; //и то, что они хранят
        if (var.name == name){ //если такая переменная существует
            varIsExist = true; //закрываем защёлку
            arr<string> currVar = split(var.data); //определяем реальную переменную этого Типа данных
            currVar.del(index);//удаление
            variableLine = var.name + ';' + unsplit(currVar);//превращаем переменную в текст
            if (currVar.size == 0) variableLine = "";
            //currVar.clear();
            tmpFile << variableLine << endl;
        }
        else {
            tmpFile << variableLine << endl;
        }
    }
    file.close();
    tmpFile.close();
    if (!varIsExist){
        cout << "This array isn't exist" << endl;
    }
    else{
        file.open(request.file, ios::out);
        tmpFile.open("tmp.data", ios::in);
        while (getline(tmpFile, variableLine)){
            file << variableLine << endl;
        }
        file.close();
        tmpFile.close();
    }
}


void arrGet(const request& request){
//структура команды: get имяМассива индекс
    fstream file(request.file, ios::in);
    if(!file.is_open()){
        stringstream serr;
        serr << "This file doesn't exist";
        throw runtime_error(serr.str());
    }
    string name = request.query[1]; //из какого массива считать
    size_t index = stoi(request.query[2]); //с какой позиции
    string variableLine; //считываемая строка с файла
    fileData var;
    bool varIsExist = false;
    while (getline(file, variableLine)){ //проверяем все существующие переменные
        if (variableLine == " " || variableLine.empty()) continue;
        var.name = split(variableLine, ';')[0]; //определяем их имена
        var.data = split(variableLine, ';')[1]; //и то, что они хранят
        if (var.name == name){ //если такая переменная существует
            varIsExist = true; //закрываем защёлку
            arr<string> currVar = split(var.data); //определяем реальную переменную этого Типа данных
            if (index < currVar.size) cout << currVar[index] << endl;
            else cout << "Index out of range" << endl;
            //currVar.clear();
        }
    }
    if (!varIsExist){
        cout << "This array isn't exist" << endl;
    }
}


void stackPush(const request& request){
//структура команды: push имяСтека чтоЗаписать
    fstream file(request.file, ios::in);
    if(!file.is_open()){
        stringstream serr;
        serr << "This file doesn't exist";
        throw runtime_error(serr.str());
    }
    fstream tmpFile("tmp.data", ios::out);
    if(!tmpFile.is_open()){
        stringstream serr;
        serr << "Tmp file doesn't exist";
        throw runtime_error(serr.str());
    }
    if (request.query.size != 3) {
        stringstream serr;
        serr << "Wrong command syntax";
        throw runtime_error(serr.str());
    }
    string name = request.query[1];
    string value = request.query[2]; //что записать
    string variableLine; //считываемая строка с файла
    fileData var;
    bool varIsExist = false;
    while (getline(file, variableLine)){ //проверяем все существующие переменные
        if (variableLine == " " || variableLine.empty()) continue;
        var.name = split(variableLine, ';')[0]; //определяем их имена
        var.data = split(variableLine, ';')[1]; //и то, что они хранят
        if (var.name == name){ //если такая переменная существует
            varIsExist = true; //закрываем защёлку
            Stack<string> currVar = splitStack(var.data); //определяем реальную переменную этого Типа данных
            currVar.push(value); //закидываем то, что просят в конец
            variableLine = var.name + ';' + unSplitStack(currVar);//превращаем переменную в текст
            currVar.clear();//никаких утечек!
            tmpFile << variableLine << endl;
        }
        else {
            tmpFile << variableLine << endl;
        }
    }
    if (!varIsExist){
        cout << "making new Stack" << endl;
        Stack<string> newVar;//да, делаем это всегда.
        newVar.push(value);
        variableLine = name + ';' + unSplitStack(newVar);//превращаем переменную в текст
        tmpFile << variableLine;
    }
    file.close();
    tmpFile.close();
    file.open(request.file, ios::out);
    tmpFile.open("tmp.data", ios::in);
    while (getline(tmpFile, variableLine)){
        file << variableLine << endl;
    }
    file.close();
    tmpFile.close();
}


void stackPop(const request& request){
//команда: pop имяСтека
    fstream file(request.file, ios::in);
    if(!file.is_open()){
        stringstream serr;
        serr << "This file doesn't exist";
        throw runtime_error(serr.str());
    }
    fstream tmpFile("tmp.data", ios::out);
    if(!tmpFile.is_open()){
        stringstream serr;
        serr << "Tmp file doesn't exist";
        throw runtime_error(serr.str());
    }
    if (request.query.size != 2) {
        stringstream serr;
        serr << "Wrong command syntax";
        throw runtime_error(serr.str());
    }
    string name = request.query[1];
    string variableLine; //считываемая строка с файла
    fileData var;
    bool varIsExist = false;
    while (getline(file, variableLine)){ //проверяем все существующие переменные
        if (variableLine == " " || variableLine.empty()) continue;
        var.name = split(variableLine, ';')[0]; //определяем их имена
        var.data = split(variableLine, ';')[1]; //и то, что они хранят
        if (var.name == name){ //если такая переменная существует
            varIsExist = true; //закрываем защёлку
            Stack<string> currVar = splitStack(var.data); //определяем реальную переменную этого Типа данных
            currVar.pop(); //удаляем
            variableLine = var.name + ';' + unSplitStack(currVar);//превращаем переменную в текст
            currVar.clear();
            tmpFile << variableLine << endl;
        }
        else {
            tmpFile << variableLine << endl;
        }
    }
    file.close();
    tmpFile.close();
    if (!varIsExist){
        cout << "Tis stack doesn't exist" << endl;
    } else {
        file.open(request.file, ios::out);
        tmpFile.open("tmp.data", ios::in);
        while (getline(tmpFile, variableLine)){
            file << variableLine << endl;
        }
        file.close();
        tmpFile.close();
    }
}


void stackGet(const request& request){
//структура команды: get имяСтека
    fstream file(request.file, ios::in);
    if(!file.is_open()){
        stringstream serr;
        serr << "This file doesn't exist";
        throw runtime_error(serr.str());
    }
    if (request.query.size != 2) {
        stringstream serr;
        serr << "Wrong command syntax";
        throw runtime_error(serr.str());
    }
    string name = request.query[1]; //из какого стека считать
    string variableLine; //считываемая строка с файла
    fileData var;
    bool varIsExist = false;
    while (getline(file, variableLine)){ //проверяем все существующие переменные
        if (variableLine == " " || variableLine.empty()) continue;
        var.name = split(variableLine, ';')[0]; //определяем их имена
        var.data = split(variableLine, ';')[1]; //и то, что они хранят
        if (var.name == name){ //если такая переменная существует
            varIsExist = true; //закрываем защёлку
            Stack<string> currVar = splitStack(var.data); //определяем реальную переменную этого Типа данных
            cout << currVar.getLast() << endl;
            currVar.clear();
        }
    }
    if (!varIsExist){
        cout << "This stack isn't exist" << endl;
    }
}


void listPush(const request& request){
//структура команды: push имяСписка кудаЗаписать чтоЗаписать
    fstream file(request.file, ios::in);
    if(!file.is_open()){
        stringstream serr;
        serr << "This file doesn't exist";
        throw runtime_error(serr.str());
    }
    fstream tmpFile("tmp.data", ios::out);
    if(!tmpFile.is_open()){
        stringstream serr;
        serr << "Tmp file doesn't exist";
        throw runtime_error(serr.str());
    }
    if (request.query.size != 4) {
        stringstream serr;
        serr << "Wrong command syntax";
        throw runtime_error(serr.str());
    }
    string name = request.query[1]; //имя списка
    string place = request.query[2]; // начало/конец
    if (place != "begin" && place != "end"){
        stringstream serr;
        serr << "Wrong insert place";
        throw runtime_error(serr.str());
    }
    string value = request.query[3]; //что записать
    string variableLine; //считываемая строка с файла
    fileData var;
    bool varIsExist = false;
    while (getline(file, variableLine)){ //проверяем все существующие переменные
        if (variableLine == " " || variableLine.empty()) continue;
        var.name = split(variableLine, ';')[0]; //определяем их имена
        var.data = split(variableLine, ';')[1]; //и то, что они хранят
        if (var.name == name){ //если такая переменная существует
            varIsExist = true; //закрываем защёлку
            list<string> currVar = splitToList(var.data); //определяем реальную переменную этого Типа данных
            if (place == "begin"){
                currVar.headInsert(value); //закидываем то, что просят в начало
            }
            else {
                currVar.backInsert(value);
            }
            variableLine = var.name + ';' + unSplitList(currVar);//превращаем переменную в текст
            //currVar.clear();
            tmpFile << variableLine << endl;
        }
        else {
            tmpFile << variableLine << endl;
        }
    }
    if (!varIsExist){
        cout << "making new list" << endl;
        list<string> newVar;//да, делаем это всегда.
        newVar.headInsert(value);
        variableLine = name + ';' + unSplitList(newVar);//превращаем переменную в текст
        tmpFile << variableLine;
    }
    file.close();
    tmpFile.close();
    file.open(request.file, ios::out);
    tmpFile.open("tmp.data", ios::in);
    while (getline(tmpFile, variableLine)){
        file << variableLine << endl;
    }
    file.close();
    tmpFile.close();
}


void listDel(const request& request){
//команда: pop имяСписка откуда/поКакомуЗначению
    fstream file(request.file, ios::in);
    if(!file.is_open()){
        stringstream serr;
        serr << "This file doesn't exist";
        throw runtime_error(serr.str());
    }
    fstream tmpFile("tmp.data", ios::out);
    if(!tmpFile.is_open()){
        stringstream serr;
        serr << "Tmp file doesn't exist";
        throw runtime_error(serr.str());
    }
    if (request.query.size != 3) {
        stringstream serr;
        serr << "Wrong command syntax";
        throw runtime_error(serr.str());
    }
    string name = request.query[1];
    string wh = request.query[2]; //what/where
    string variableLine; //считываемая строка с файла
    fileData var;
    bool varIsExist = false;
    while (getline(file, variableLine)){ //проверяем все существующие переменные
        if (variableLine == " " || variableLine.empty()) continue;
        var.name = split(variableLine, ';')[0]; //определяем их имена
        var.data = split(variableLine, ';')[1]; //и то, что они хранят
        if (var.name == name){ //если такая переменная существует
            varIsExist = true; //закрываем защёлку
            list<string> currVar = splitToList(var.data); //определяем реальную переменную этого Типа данных
            if (wh == "begin") {
                currVar.delFirst();
            }
            else if (wh == "end"){
                currVar.delLast();
            }
            else {
                currVar.delByVal(wh);
            }
            variableLine = var.name + ';' + unSplitList(currVar);//превращаем переменную в текст
            //currVar.clear();
            tmpFile << variableLine << endl;
        }
        else {
            tmpFile << variableLine << endl;
        }
    }
    file.close();
    tmpFile.close();
    if (!varIsExist){
        cout << "Tis list doesn't exist" << endl;
    } else {
        file.open(request.file, ios::out);
        tmpFile.open("tmp.data", ios::in);
        while (getline(tmpFile, variableLine)){
            file << variableLine << endl;
        }
        file.close();
        tmpFile.close();
    }
}


void listGet(const request& request){
//структура команды: get имяСписка искомоеЗначение
    fstream file(request.file, ios::in);
    if(!file.is_open()){
        stringstream serr;
        serr << "This file doesn't exist";
        throw runtime_error(serr.str());
    }
    if (request.query.size != 3) {
        stringstream serr;
        serr << "Wrong command syntax";
        throw runtime_error(serr.str());
    }
    string name = request.query[1]; //в каком списке искать
    string value = request.query[2]; //что ищем
    string variableLine; //считываемая строка с файла
    fileData var;
    bool varIsExist = false;
    while (getline(file, variableLine)){ //проверяем все существующие переменные
        if (variableLine == " " || variableLine.empty()) continue;
        var.name = split(variableLine, ';')[0]; //определяем их имена
        var.data = split(variableLine, ';')[1]; //и то, что они хранят
        if (var.name == name){ //если такая переменная существует
            varIsExist = true; //закрываем защёлку
            list<string> currVar = splitToList(var.data); //определяем реальную переменную этого Типа данных
            if (currVar.find(value)){
                cout << "value " << value << " is in the list " << name << endl;
            }
            else {
                cout << "value " << value << " isn't in the list " << name << endl;
            }
            //currVar.clear();
        }
    }
    if (!varIsExist){
        cout << "This list isn't exist" << endl;
    }
}


void queuePush(const request& request){
//структура команды: push имяОчереди чтоЗаписать
    fstream file(request.file, ios::in);
    if(!file.is_open()){
        stringstream serr;
        serr << "This file doesn't exist";
        throw runtime_error(serr.str());
    }
    fstream tmpFile("tmp.data", ios::out);
    if(!tmpFile.is_open()){
        stringstream serr;
        serr << "Tmp file doesn't exist";
        throw runtime_error(serr.str());
    }
    if (request.query.size != 3) {
        stringstream serr;
        serr << "Wrong command syntax";
        throw runtime_error(serr.str());
    }
    string name = request.query[1]; //имя очереди
    string value = request.query[2]; //что записать
    string variableLine; //считываемая строка с файла
    fileData var;
    bool varIsExist = false;
    while (getline(file, variableLine)){ //проверяем все существующие переменные
        if (variableLine == " " || variableLine.empty()) continue;
        var.name = split(variableLine, ';')[0]; //определяем их имена
        var.data = split(variableLine, ';')[1]; //и то, что они хранят
        if (var.name == name){ //если такая переменная существует
            varIsExist = true; //закрываем защёлку
            queue<string> currVar = splitToQueue(var.data); //определяем реальную переменную этого Типа данных
            currVar.push(value);
            variableLine = var.name + ';' + unSplitQueue(currVar);//превращаем переменную в текст
            currVar.clear();
            tmpFile << variableLine << endl;
        }
        else {
            tmpFile << variableLine << endl;
        }
    }
    if (!varIsExist){
        cout << "making new queue" << endl;
        queue<string> newVar;//да, делаем это всегда.
        newVar.push(value);
        variableLine = name + ';' + unSplitQueue(newVar);//превращаем переменную в текст
        tmpFile << variableLine;
    }
    file.close();
    tmpFile.close();
    file.open(request.file, ios::out);
    tmpFile.open("tmp.data", ios::in);
    while (getline(tmpFile, variableLine)){
        file << variableLine << endl;
    }
    file.close();
    tmpFile.close();
}


void queuePop(const request& request){
//команда: pop имяОчереди
    fstream file(request.file, ios::in);
    if(!file.is_open()){
        stringstream serr;
        serr << "This file doesn't exist";
        throw runtime_error(serr.str());
    }
    fstream tmpFile("tmp.data", ios::out);
    if(!tmpFile.is_open()){
        stringstream serr;
        serr << "Tmp file doesn't exist";
        throw runtime_error(serr.str());
    }
    if (request.query.size != 2) {
        stringstream serr;
        serr << "Wrong command syntax";
        throw runtime_error(serr.str());
    }
    string name = request.query[1]; //имя очереди
    string variableLine; //считываемая строка с файла
    fileData var;
    bool varIsExist = false;
    while (getline(file, variableLine)){ //проверяем все существующие переменные
        if (variableLine == " " || variableLine.empty()) continue;
        var.name = split(variableLine, ';')[0]; //определяем их имена
        var.data = split(variableLine, ';')[1]; //и то, что они хранят
        if (var.name == name){ //если такая переменная существует
            varIsExist = true; //закрываем защёлку
            queue<string> currVar = splitToQueue(var.data); //определяем реальную переменную этого Типа данных
            currVar.pop();
            variableLine = var.name + ';' + unSplitQueue(currVar);//превращаем переменную в текст
            currVar.clear();
            tmpFile << variableLine << endl;
        }
        else {
            tmpFile << variableLine << endl;
        }
    }
    file.close();
    tmpFile.close();
    if (!varIsExist){
        cout << "Tis queue doesn't exist" << endl;
    } else {
        file.open(request.file, ios::out);
        tmpFile.open("tmp.data", ios::in);
        while (getline(tmpFile, variableLine)){
            file << variableLine << endl;
        }
        file.close();
        tmpFile.close();
    }
}


void queueGet(const request& request){
//структура команды: get имяОчереди
    fstream file(request.file, ios::in);
    if(!file.is_open()){
        stringstream serr;
        serr << "This file doesn't exist";
        throw runtime_error(serr.str());
    }
    if (request.query.size != 2) {
        stringstream serr;
        serr << "Wrong command syntax";
        throw runtime_error(serr.str());
    }
    string name = request.query[1]; //в какой очереди искать
    string variableLine; //считываемая строка с файла
    fileData var;
    bool varIsExist = false;
    while (getline(file, variableLine)){ //проверяем все существующие переменные
        if (variableLine == " " || variableLine.empty()) continue;
        var.name = split(variableLine, ';')[0]; //определяем их имена
        var.data = split(variableLine, ';')[1]; //и то, что они хранят
        if (var.name == name){ //если такая переменная существует
            varIsExist = true; //закрываем защёлку
            queue<string> currVar = splitToQueue(var.data); //определяем реальную переменную этого Типа данных
            cout << currVar.getFirst() << endl;
            currVar.clear();
        }
    }
    if (!varIsExist){
        cout << "This queue isn't exist" << endl;
    }
}

