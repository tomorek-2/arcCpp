#pragma once
#include "string"
struct Var {
    int type = 0; //0 Это num, 1 это str. bool это int. 2 ЭтоМаркер?
    std::string str = "";
    double num = 0;

};

extern "C" {
    Var getValue(const char* str);
    void setValue(const char* key, Var value);
    void logInArc(const char* text, int level);

}