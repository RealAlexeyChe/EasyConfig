#pragma once
#include "IConfig.h"

struct StringToValue {
    const char* input;
    StringToValue(std::string& str) : input(str.c_str()) {}
  
    void operator()(int& i) { i = std::atoi(input);}
    void operator()(bool& b) { b = (input == "true") ? true : false;};
    void operator()(double& d) { d = std::stod(input);};
    void operator()(std::string& str) {str = input;}

};

struct ValueToString {
    std::string& output;
    ValueToString(std::string& str) : output(str) {}
   
    void operator()(const int& i) { output = std::to_string(i); }
    void operator()(const bool& b) { output = (b) ? "true" : "false"; };
    void operator()(const double& d) { output = std::to_string(d); };
    void operator()(const std::string& str) { output = str; };
};




