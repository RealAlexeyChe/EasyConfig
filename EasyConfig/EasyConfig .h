#pragma once
#include "IConfig.h"
#include "RegistryConfig.h"
#include "JsonConfig.h"
#include "IniConfig.h"
#include "XmlConfig.h"
#include <vector>
enum ConfigType {REGISTRY, JSON, INI, XML, ERR}; // Разные типы конфигураций


class EasyConfig {
public:
    // Загружает указанный тип конфигурации, при необходимости используя указанный путь, и изменяет
    // указатель на конкретную имплементацию конфигурации
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

        SetConcreteConfig(conf); // Изменить указатель на конкретную конфигурацию. 
                                 // Если указатель - nullptr, (bool)conf возвращает false
                                 // Cм. explicit operator bool()
        if (conf) {
            
            SetType(type); //Если загрузка прошла успешно, установить правильный тип конфигурации
            return true;
        }
        else {
            SetType(ERR);
            throw(std::exception("Config did not load!"));
            return false;
        }
	};


    // Получить значение из конфигурации
    template <typename T>
    void GetOption(const std::string& path, T& output) {
        ConfigValue value = output;
        concrete_config_->Access(path, value, GET);
        output = std::get<T>(value);
    }

    // Записать значение в конфигурацию
    template <typename T>
    bool SetOption(const std::string& path, T input) {

        ConfigValue value = input;
        concrete_config_->Access(path, value, SET);
        return true;
    }


    //Сохраняет конфигурацию
    bool Save(std::string filepath = 0) {

        //TODO Добавить проверку расширения
        return concrete_config_->Save(filepath);
    }
 
    // Если произошла ошибка при загрузке, 
    explicit operator bool() const
    {
        return concrete_config_;
    }



   
    ConfigType GetType() {
        return type_;
    }
    



private:
    ConfigType type_ = ERR; // Тип конфигурафии
	IConfig* concrete_config_; // Конкретная имплементация конфигурации

    IConfig* GetConcreteConfig() {
        return concrete_config_;
    }
    // Установить тип конфигурации
    void SetType(const ConfigType t) {
        type_ = t;
    }

    // Установить конкретную конфигурацию
    void SetConcreteConfig(IConfig* conf) {
        concrete_config_ = conf;
    }

    
};