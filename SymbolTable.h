#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"


struct Node {
    
public:
    string name;
    string type;
    string value;
    int scope;
    class Node* next;
    
    //Node* nextType;
    Node() {
        name = "";type = ""; value = ""; next = NULL;
    }
    Node(string name, string type, string value,int scope){
        this->name = name;
        this->type = type;
        this->value = value;
        this->scope = scope;
        this->next = nullptr;
    }
};
struct RecordVar {
    Node* head;
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
    bool insert(RecordVar& record, Node* element);
    void assign(string name, string value);
    void delNode();
    
    bool isEmpty(RecordVar& record);
    
    bool searchAndAssign(RecordVar& record, string name);
    bool containsInScope(RecordVar& record, string name, int scope);
    string lookUp();
    void print();
    void rPrint();
    friend class Node;
};
#endif