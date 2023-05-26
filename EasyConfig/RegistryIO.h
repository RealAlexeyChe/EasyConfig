#pragma once
#include <Registry.hpp>
#include <utility>

using namespace m4x1m1l14n;
typedef std::pair<Registry::RegistryKey_ptr, std::wstring> RegIoInfo;

std::wstring toWstring(std::string value) {
    return std::wstring(value.begin(), value.end());
}
std::string toString(std::wstring value) {
    return std::string(value.begin(), value.end());
}


struct RegistryGetter {
    Registry::RegistryKey_ptr keyPtr;
    std::wstring valueName;

    RegistryGetter(const RegIoInfo info) {
        this->keyPtr = info.first;
        this->valueName = info.second;

    }
    void operator()(int& i) { i = keyPtr->GetInt32(valueName); };
    void operator()(bool& b) { b = keyPtr->GetBoolean(valueName); };
    void operator()(double& d) {
        std::wstring result = keyPtr->GetString(valueName);
        double temp;
        temp = stod(result);
        d = temp;
    };
    void operator()(std::string& str) {
        str = toString(keyPtr->GetString(valueName));
    }
};


struct RegistrySetter {

    Registry::RegistryKey_ptr keyPtr;
    std::wstring valueName;

    RegistrySetter(const RegIoInfo info) {
        this->keyPtr = info.first;
        this->valueName = info.second;

    }
    void operator()(const int& i) { keyPtr->SetInt32(valueName, i); };
    void operator()(const bool& b) { keyPtr->SetBoolean(valueName, b); };
    void operator()(const double& d) { keyPtr->SetString(valueName, std::to_wstring(d)); };
    void operator()(const std::string& str) {
        keyPtr->SetString(toWstring(str), valueName);
    }


};