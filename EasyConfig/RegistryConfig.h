#pragma once

#include <map>
#include <tuple>
#include <Registry.hpp>
#include "IConfig.h"
#include "RegistryIO.h"
using namespace m4x1m1l14n;




enum RegistryAccessType { READ, WRITE }; 

typedef std::tuple<std::wstring, std::wstring, std::wstring> RegistryPath;

// Конфигурация из реестра
class RegistryConfig: public IConfig {
public:

    // Реестр не нужно загружать
	virtual IConfig* Load(std::string path) {
		return this;
	}
    // Реестр не нужно сохранять
	virtual bool Save(std::string filepath = "") {	
		return true;
	}

    //Получение / запись значения
    virtual bool Access(const std::string path, ConfigValue& value, AccessType access) {
        auto info = GetAccessInfo(path, READ); // Получение указателя на ключ и названия значения
        switch (access) {
        case GET:
            std::visit(RegistryGetter{ info }, value); // Чтение значения
            return true;
        case SET:
            std::visit(RegistrySetter{ info }, value); // Запись значения
            return true;
        }
    }

    
 
private: 

    // Меняет значение текущего указателя на указатель ключа по указанному пути
    RegIoInfo GetAccessInfo(std::string rawpath, RegistryAccessType access) {
        RegistryPath path = ConvertPath(rawpath);

        auto root_name = std::get<0>(path);
        auto key_path = std::get<1>(path);
        auto value_name = std::get<2>(path);

        auto root = rootMap[root_name];

        Registry::RegistryKey_ptr key_ptr; // Указатель на ключ
        
        if (access == READ)
            key_ptr = root->Open(key_path, Registry::DesiredAccess::Read); // Открытие ключа для чтения
        if (access == WRITE)
            key_ptr = root->Open(key_path, Registry::DesiredAccess::Write); // Открытие ключа для записи
        
        
        return RegIoInfo{ key_ptr, value_name };

    }
    
    // Конвертирует путь в удобный формат {корень, путь до ключа, название значения}
    RegistryPath ConvertPath(const std::string path) {
        std::wstring source = toWstring(path); // Библиотека использует тип wstring для всех операций, 
        

        size_t pos{0};

        // Получем корень и отрезаем его от остальной 
        pos = source.find('\\');
        std::wstring root = (source.substr(0, pos));
        source = source.substr(pos + 1);

        // Получем путь к ключу и отрезаем от названия значения 
        pos = source.rfind('\\');
        std::wstring key_path = source.substr(0, pos);
        source = source.substr(pos + 1);

        // Получаем оставшееся название значения 
        std::wstring value_name = source;

        return RegistryPath{ root, key_path, value_name };
    }
    


    std::map<std::wstring, Registry::RegistryKey_ptr> rootMap{  // Маппинг текстового названия корня в указатель на него
            {L"HKEY_CLASSES_ROOT", Registry::ClassesRoot},
            {L"HKEY_CURRENT_USER", Registry::CurrentUser},
            {L"HKEY_LOCAL_MACHINE", Registry::LocalMachine},
            {L"HKEY_USERS", Registry::Users},
            {L"HKEY_CURRENT_CONFIG", Registry::CurrentConfig}
    };



  

};

