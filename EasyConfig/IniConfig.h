#pragma once

#include <SimpleIni.h>
#include "IConfig.h"
#include "Utils.h"

//������������ .ini
class IniConfig : public IConfig {
	typedef std::pair<std::string, std::string> SectionAndValueName; 

public:

	//�������� ����� .ini
	virtual IConfig* Load(std::string filepath) {
		
		SI_Error rc = ini.LoadFile(filepath.c_str()); //rc - ���������� � ���������� ������ �����, ��� ������ ���������� �������� < 0
		if (rc < 0) { 
			
			return nullptr;
		};


		loader_filepath_ = filepath; // ���������� ���� ������������ �����
		return this;

	}

	//��������� ������������ � ���� .ini
	virtual bool Save(std::string filepath = "") {

		SI_Error rc;

		if (filepath.empty()) { // ���� �������� ���� ������, ��������� � ��� ����, �� �������� ��������� �� ����� Load()
			rc = ini.SaveFile(loader_filepath_.c_str());
		}
		else {
			rc = ini.SaveFile(filepath.c_str());
		}

		if (rc < 0) { // ���� ��������� �� �������, �������� rc < 0 
			return false;
		}
		else {
			return true;
		}
		
	}

	//��������� / ������ ��������
	virtual bool Access(const std::string path, ConfigValue& value, AccessType access) {
		SectionAndValueName pair = SeparatePath(path); 

		std::string section = pair.first; 
		std::string value_name = pair.second;
		std::string text{ 0 }; // �������� value � �������� ����

		switch (access) {
		case GET:
			text = ini.GetValue(section.c_str(), value_name.c_str()); // ��������� ���������� �������� �� ������
			std::visit(StringToValue{ text }, value); // ������� �� ������ � �������� ������������ ����, � ������ ����� �������� � value 
			return true;
		case SET:
			std::visit(ValueToString{ text }, value); // ������� value � �����
			ini.SetValue(section.c_str(), value_name.c_str(), text.c_str()); // // ������� ������ ��������� �������� value � ������ ������
			return true;
		}
	}

	

	//��������� ������ ���� �� ������ � �������� �������� 
	SectionAndValueName SeparatePath(std::string path) {
		
		//TODO �������� �� ��������� ������������
		char separator = '.';


		SectionAndValueName pair;
		size_t pos; 
		pos = path.find(separator); // ������� ����������

		// ���� �������� �� ��������, ���������� ������ ���������,
		// � .ini �� ����� ��������� ���������
		if (path.rfind(separator) != pos) { throw std::exception("Path should only counain section and value"); } 

		pair.first = path.substr(0, pos);
		pair.second = path.substr(pos+1);
		return pair;
	}


private:
	CSimpleIniA ini; // ������ ��������� ini
	std::string loader_filepath_; // ���� � ������������ �����
};