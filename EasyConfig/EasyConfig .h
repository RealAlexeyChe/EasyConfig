#pragma once
#include "IConfig.h"
#include "RegistryConfig.h"
#include "JsonConfig.h"
#include "IniConfig.h"
#include "XmlConfig.h"
#include <vector>
enum ConfigType {REGISTRY, JSON, INI, XML, ERR}; // ������ ���� ������������


class EasyConfig {
public:
    // ��������� ��������� ��� ������������, ��� ������������� ��������� ��������� ����, � ��������
    // ��������� �� ���������� ������������� ������������
	bool Load(ConfigType type, std::string path = "") {
        
        IConfig* conf = nullptr;

		switch (type) {
		case REGISTRY:
            conf = new RegistryConfig();
            break;
        case JSON:
            conf = new JsonConfig();
            break;
        case INI:
            conf = new IniConfig();
            break;
        case XML: 
            conf = new XmlConfig();
            break;
        } 
        conf->Load(path);

        SetConcreteConfig(conf); // �������� ��������� �� ���������� ������������. 
                                 // ���� ��������� - nullptr, (bool)conf ���������� false
                                 // C�. explicit operator bool()
        if (conf) {
            
            SetType(type); //���� �������� ������ �������, ���������� ���������� ��� ������������
            return true;
        }
        else {
            SetType(ERR);
            throw(std::exception("Config did not load!"));
            return false;
        }
	};


    // �������� �������� �� ������������
    template <typename T>
    void GetOption(const std::string& path, T& output) {
        ConfigValue value = output;
        concrete_config_->Access(path, value, GET);
        output = std::get<T>(value);
    }

    // �������� �������� � ������������
    template <typename T>
    bool SetOption(const std::string& path, T input) {

        ConfigValue value = input;
        concrete_config_->Access(path, value, SET);
        return true;
    }


    //��������� ������������
    bool Save(std::string filepath = 0) {

        //TODO �������� �������� ����������
        return concrete_config_->Save(filepath);
    }
 
    // ���� ��������� ������ ��� ��������, 
    explicit operator bool() const
    {
        return concrete_config_;
    }



   
    ConfigType GetType() {
        return type_;
    }
    



private:
    ConfigType type_ = ERR; // ��� ������������
	IConfig* concrete_config_; // ���������� ������������� ������������

    IConfig* GetConcreteConfig() {
        return concrete_config_;
    }
    // ���������� ��� ������������
    void SetType(const ConfigType t) {
        type_ = t;
    }

    // ���������� ���������� ������������
    void SetConcreteConfig(IConfig* conf) {
        concrete_config_ = conf;
    }

    
};