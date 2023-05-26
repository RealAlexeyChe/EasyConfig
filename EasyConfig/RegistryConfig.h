#pragma once

#include <map>
#include <tuple>
#include <Registry.hpp>
#include "IConfig.h"
#include "RegistryIO.h"
using namespace m4x1m1l14n;




enum RegistryAccessType { READ, WRITE }; 

typedef std::tuple<std::wstring, std::wstring, std::wstring> RegistryPath;

// ������������ �� �������
class RegistryConfig: public IConfig {
public:

    // ������ �� ����� ���������
	virtual IConfig* Load(std::string path) {
		return this;
	}
    // ������ �� ����� ���������
	virtual bool Save(std::string filepath = "") {	
		return true;
	}

    //��������� / ������ ��������
    virtual bool Access(const std::string path, ConfigValue& value, AccessType access) {
        auto info = GetAccessInfo(path, READ); // ��������� ��������� �� ���� � �������� ��������
        switch (access) {
        case GET:
            std::visit(RegistryGetter{ info }, value); // ������ ��������
            return true;
        case SET:
            std::visit(RegistrySetter{ info }, value); // ������ ��������
            return true;
        }
    }

    
 
private: 

    // ������ �������� �������� ��������� �� ��������� ����� �� ���������� ����
    RegIoInfo GetAccessInfo(std::string rawpath, RegistryAccessType access) {
        RegistryPath path = ConvertPath(rawpath);

        auto root_name = std::get<0>(path);
        auto key_path = std::get<1>(path);
        auto value_name = std::get<2>(path);

        auto root = rootMap[root_name];

        Registry::RegistryKey_ptr key_ptr; // ��������� �� ����
        
        if (access == READ)
            key_ptr = root->Open(key_path, Registry::DesiredAccess::Read); // �������� ����� ��� ������
        if (access == WRITE)
            key_ptr = root->Open(key_path, Registry::DesiredAccess::Write); // �������� ����� ��� ������
        
        
        return RegIoInfo{ key_ptr, value_name };

    }
    
    // ������������ ���� � ������� ������ {������, ���� �� �����, �������� ��������}
    RegistryPath ConvertPath(const std::string path) {
        std::wstring source = toWstring(path); // ���������� ���������� ��� wstring ��� ���� ��������, 
        

        size_t pos{0};

        // ������� ������ � �������� ��� �� ��������� 
        pos = source.find('\\');
        std::wstring root = (source.substr(0, pos));
        source = source.substr(pos + 1);

        // ������� ���� � ����� � �������� �� �������� �������� 
        pos = source.rfind('\\');
        std::wstring key_path = source.substr(0, pos);
        source = source.substr(pos + 1);

        // �������� ���������� �������� �������� 
        std::wstring value_name = source;

        return RegistryPath{ root, key_path, value_name };
    }
    


    std::map<std::wstring, Registry::RegistryKey_ptr> rootMap{  // ������� ���������� �������� ����� � ��������� �� ����
            {L"HKEY_CLASSES_ROOT", Registry::ClassesRoot},
            {L"HKEY_CURRENT_USER", Registry::CurrentUser},
            {L"HKEY_LOCAL_MACHINE", Registry::LocalMachine},
            {L"HKEY_USERS", Registry::Users},
            {L"HKEY_CURRENT_CONFIG", Registry::CurrentConfig}
    };



  

};

