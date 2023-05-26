#pragma once

#include <utility>
#include <rapidjson/rapidjson.h>



struct JsonGetter {
    rapidjson::Value* value = new rapidjson::Value;

    JsonGetter(rapidjson::Value* val) {
        value = val;

    }
    void operator()(int& i) { i = value->GetInt(); };
    void operator()(bool& b) { b = value->GetBool(); };
    void operator()(double& d) { d = value->GetDouble(); };
    void operator()(std::string& str) { str = value->GetString(); };
    
};


struct JsonSetter {
    rapidjson::Value* value;
    rapidjson::Document* doc;


    JsonSetter(std::pair<rapidjson::Value*, rapidjson::Document*> pair) {
        value = pair.first;
        doc = pair.second;

    }

    void operator()(const int& i) { value->SetInt(i); };
    void operator()(const bool& b) { value->SetBool(b); };
    void operator()(const double& d) { value->SetDouble(d); };
    void operator()(const std::string& str) {
        value->SetString(str.c_str(), str.length(), doc->GetAllocator());

    }

};