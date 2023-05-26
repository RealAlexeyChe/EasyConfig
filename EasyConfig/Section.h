#pragma once
#include "EasyConfig .h"
class Section {
public:
    virtual ~Section() { } // Нам не нужно удалять config_, так как конфигурация ещё открыта



    //Создает секцию из указанного пути и родителя-конфигурации
    Section(const std::string& path, EasyConfig* config) {

        section_path_ = path; //Путь до используемой секции
        config_ = config; 
    }

    // Создает полный пуить и вызывает GetOption родителя
    template <typename T>
    void GetOption(const std::string& path, T& output) {
        config_->GetOption(CreateFullPath(path), output);
    }

    // Создает полный пуить и вызывает SetOption родителя
    template <typename T>
    void SetOption(const std::string& path, const T& input) {
        config_->SetOption(CreateFullPath(path), input);
    }
    

private:
    // Получение полного пути в зависимости от типа конфигурации
    // TODO пользовательский сепаратор
    std::string CreateFullPath(const std::string& str) {
        switch (config_->GetType()) {
        case REGISTRY:
            return section_path_ + "\\" + str;
        case JSON:
            return section_path_ + "." + str;
        case INI:
            return section_path_ + "." + str;
        case XML:
            return section_path_ + "." + str;

        default:
            throw (std::exception("Unknows config type"));
        }
    }


    std::string section_path_; // Путь до секции
    EasyConfig* config_; // Указатель на родителя

};