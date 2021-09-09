#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"


struct Node {
    
public:
    string name;
    string type;
    //string value;
    int scope;

    Node* next;
    Node* prev;
    Node* sameName;
    //Node* nextType;
    Node() {
        name = "";type = ""; next = nullptr; prev = nullptr; sameName = nullptr;
    }
    Node(string name, string type,int scope){
        this->name = name;
        this->type = type;
        //this->value = value;
        this->scope = scope;
        this->next = nullptr;
        this->prev = nullptr;
        this->sameName = nullptr;
    }
};
struct RecordVar {
    Node* head;
    Node* tail;
    int size;
    RecordVar(): head(NULL), size(0) {}
    RecordVar(Node* head, int size): head(head), size(size) {}
};
class SymbolTable {
    
public:
    int scopeCur;
    RecordVar record;
    SymbolTable() {
        scopeCur = 0;
    }
    //SymbolTable(Node * head, int size): head(head), size(size) {}

    void run(string filename);
    void insert(RecordVar& record, string name, string type, int scope);
    void insertSameName(RecordVar& record, string name, string type, int scope);
    void assign(string name, string value);
    void delNodeinScope(RecordVar& record,int scopeCur);
    
    bool isEmpty(RecordVar& record);
    
    int searchForAssign(RecordVar& record, string name, string type);
    bool containsInScope(RecordVar& record, string name, int scope);
    int contains(RecordVar& record, string name);
    
    void print(RecordVar& record, int scope);
    void reprint(RecordVar& record, int scope);
    friend class Node;
};
#endif