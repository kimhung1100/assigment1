#include "SymbolTable.h"

void tokenize (string cuts[], string s, string del = " "){
    //Used for testcase tokenizer, students don't need to take care of this function.
    //vector<string> res;
    int start = 0;
    int end = (int)s.find(del);
    int i = 0;
    while (end != -1) {
        cuts[i] = s.substr(start, end - start);
        i++;
        start = (int)end + (int)del.size();
        end = (int)s.find(del, start);
    }
    cuts[i] = s.substr(start, end - start);
    return;
}
bool checkCharInName(string checkName) {
    if (checkName[0] < 'a' || checkName[0] > 'z') return false;
    for (int i = 1; i < checkName.size(); i++) {
        if( (checkName[i] >= 'a' && checkName[i] <= 'z') ||
        (checkName[i] >= 'A' && checkName[i] <= 'Z') ||
        checkName[i] == '_' || (checkName[i]>= '0' && checkName[i] <= '9'))
            continue;
        else return false;
    }
    return true;
}
bool checkValueType(string str) {
    // nhan so dau tien khac 0 fixbug
    if (str[0] >= '0' && str[0] <= '9') {
        for (size_t i = 1; i < str.size(); i++) {
            if(str[i] < '0' || str[i] > '9') return false;
        }
        return true;
    }
    else if (int(str[0]) == 39 && int(str[str.size() - 1]) == 39) {
        for (size_t i = 1; i < str.size() - 1; i++) {
            if( !(str[i] >= '0' &&  str[i] <= '9') ||
            !( (str[i] >= 'a') && (str[i] <= 'z')) ||
            !( str[i] >= 'A' && str[i] <= 'Z') ||
            str[i] != ' ' ) 
                return false;
        }
        return true;
    }
    else {
        if(!checkCharInName(str)) return false;
            return true;
    }
    return false;
}
bool checkInstruction(string cutS[]) {
    if(cutS[0] == "BEGIN"|| cutS[0] == "END" || cutS[0] == "PRINT" || cutS[0] == "RPRINT") {
        if(cutS[1] == "" && cutS[2] == "" ) return true; return false;
    }
    else if (cutS[0] == "INSERT") {
        
        if (!(cutS[2] == "string" || cutS[2] == "number"))  return false;

        if (!checkCharInName(cutS[1])) return false;
        
        return true;
    }
    else if (cutS[0] == "ASSIGN") {
        if (!checkCharInName(cutS[1]) || !checkValueType(cutS[2]))
            return false;
        return true;
    }
    else if (cutS[0] == "LOOKUP") {
        if (!checkCharInName(cutS[1]) || cutS[2] != "") return false;
        return true;
    }
    return false;
}
/*string del0(string typeOval) {
    
    int cnt0 = 0;
    for (size_t i = 0; i < typeOVal.size(); i++) {
        if (typeOVal[i] == '0') cnt0 ++;
        else break;
    }
    typeOVal.erase(0,cnt0);
    if(typeOVal.size() == 0) typeOVal = "0";
    
}*/
void SymbolTable::run(string filename) {
    ifstream fileIn;
    fileIn.open(filename, ios:: in);

    while(!fileIn.eof()) {
        char c[500];
        string s, name, typeOVal, ide;
        
        fileIn.getline(c,500);
        s = c;
        string cutS[4];
        tokenize(cutS, s, " ");
        if (!checkInstruction(cutS)) 
            throw InvalidInstruction(s);
        
        ide = cutS[0];

        if(ide == "BEGIN") {
            //scope +1
            scopeCur ++;
            continue;
        }
        else if(ide == "END") {
            if (scopeCur == 0) throw UnknownBlock();
            delNodeinScope(record, scopeCur);
            scopeCur --;
            continue;
            //delete all var in scope
        }
        else {
            name = cutS[1];
            typeOVal = cutS[2];
            //delete cutS;
            if (ide == "INSERT") {
                if (containsInScope(record, name, scopeCur))
                    throw Redeclared(s);
                else {
                    //Node* e = new Node(name, typeOVal,scopeCur);
                    if (contains(record, name) == -1)
                        insert(record, name, typeOVal, scopeCur);
                    else {
                        insertSameName(record, name, typeOVal, scopeCur);
                    }
                }
                //loi redeclared
            }
            else if (ide == "ASSIGN") {
                if(typeOVal[0] >= '0' && typeOVal[0] <= '9') {
                    //del0(typeOVal);
                    int state = searchForAssign(record, name, "number");
                    if(state == 0) {
                        throw Undeclared(s);
                    }else if(state == 1) {
                        throw TypeMismatch(s);
                    }
                    
                } else if ((int)typeOVal[0] == 39) {
                    int state = searchForAssign(record, name, "string");
                    if(state == 0) {
                        throw Undeclared(s);
                    }else if(state == 1) {
                        throw TypeMismatch(s);
                    }
                    

                } else if (typeOVal[0] >= 'a' && typeOVal[0] <= 'z') {
                    
                    if ((searchForAssign(record, name, "number") == 0 &&
                    searchForAssign(record, name, "string")) || (
                    searchForAssign(record, typeOVal, "number") &&
                    searchForAssign(record, typeOVal, "string") ) ) {
                        throw Undeclared(s);
                    }
                    else if ((searchForAssign(record, name, "number") == 2 &&
                    searchForAssign(record, typeOVal, "number") == 2 )||
                    (searchForAssign(record, name, "string") == 2 &&
                     searchForAssign(record, typeOVal, "string") == 2) ) {
                        1;
                    }
                    else {
                        throw TypeMismatch(s);
                    }

                } 
                // loi undeclared
                // type mismatch
                
            }
            
            else if (ide == "LOOKUP") {
                int scopeLook = contains(record, name);
                if (scopeLook == -1) throw Undeclared(name);
                else {cout << scopeLook << endl; continue;}

                // undeclared
            
            }else if (ide == "PRINT") {
                print(record, scopeCur);
                continue;
            } else if (ide == "RPRINT") {
                reprint(record, scopeCur);
                continue;
            }
        }
        
        cout << "success\n" ;
    }
    if(scopeCur != 0) throw UnclosedBlock(scopeCur);
    fileIn.close();
}

void SymbolTable :: insert(RecordVar& record, string name, string type, int scope){
    Node* element = new Node(name, type, scope);
    if(record.head == NULL) {
        record.head = element;
        record.tail = element;
        record.tail ->next = NULL;
        record.head ->prev = NULL;
        record.size ++;
    } else { //add last
        record.tail->next = element;
        element->prev = record.tail;
        record.tail = element;
        record.tail->next = NULL;
        record.size ++;
    }
    return;
}
void SymbolTable :: insertSameName(RecordVar& record, string name, string type, int scope) {
    //add last
    Node* element = new Node(name, type, scope);
    record.tail->next = element;
    element->prev = record.tail;
    record.tail = element;
    record.tail->next = NULL;
    record.size ++;
    //add last name
    Node* p = record.head;
    while(p) {
        if (p->name == element->name) {
            if(p->sameName == nullptr) {
                p->sameName = element;
            }
            else {
                Node* q = p;
                while(!q->sameName) q = q->sameName;
                q->sameName = element;
                element->sameName = NULL;
            }
            return;
        }
        p = p ->next;
    }
}

bool SymbolTable :: isEmpty(RecordVar& record) {
    if (record.size == 0 ) return true;
    else return false;
}

bool SymbolTable :: containsInScope(RecordVar& record, string name, int scope) {
    if(record.head == nullptr && record.size == 0) return false;
    Node* p = record.tail;
    while(p) {
        if(p->name == name && p->scope == scope) {return true;}
        p = p ->prev;
    }
    //delete p;
    return false;
}
int SymbolTable :: contains(RecordVar& record, string name) {
    if(record.head == nullptr && record.size == 0) return -1;
    Node* p = record.tail;
    while(p) {
        if(p->name == name) {/*delete p;*/ return p->scope;}
        p = p ->prev;
    }
    /*delete p;*/ return -1;
}

int SymbolTable :: searchForAssign(RecordVar& record, string name, string type) {
    if(record.head == nullptr && record.size == 0) return 0;
    Node* p = record.tail;
    while(p) {
        if (p->name == name) {
            if(p->type == type) return 2;
            else return 1;
        }
        p = p->prev;
    }
    /*delete p;*/ return 0;
}
void SymbolTable :: delNodeinScope(RecordVar& record, int scopeCur){
    Node* p = record.tail;
    while(p) {
        if(p->scope == scopeCur -1) break;
        p = p->prev;
    }
    record.tail = p;
    record.tail->next = NULL;
}
void SymbolTable :: print(RecordVar& record, int scope) {
    if(record.head == nullptr && record.size == 0) return;
    for (size_t i = 0; i <= scope; i++) {
        Node* p = record.head;
        while(p) {
            if(i == 0) {
                if(p->sameName == nullptr && p->scope == 0)
                    cout << p->name << "//" << p->scope << " ";
            }
            else if(p->sameName == nullptr) {
                if(p->scope == i)
                    cout << p->name << "//" << p->scope << " ";
            }/*
            else {
                Node* q = p;
                while(q->sameName) q= q->sameName;
                if(q->scope == i) {
                    cout << q->name << "//" << q->scope << " ";}
                
            }*/
            p = p->next;
        }
    }
    cout << endl;
}
void SymbolTable :: reprint(RecordVar& record, int scope) {
    if(record.head == nullptr && record.size == 0) return;
    for (int i = scope; i >= 0 ; i--) {
        Node* p = record.tail;
        while(p) {
            if(p->sameName == nullptr && p->scope == i)
                cout << p->name << "//" << p->scope << " ";
            p = p->prev;
        }
    }
    cout << endl;
}


