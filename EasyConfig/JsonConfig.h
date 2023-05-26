#pragma once
#include <fstream>
#include "IConfig.h"
#include <vector>
//#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/stringbuffer.h>

#include <rapidjson/pointer.h>

#include "JsonIO.h"


// TODO ��������� / ������ �������� � ���������

//������������ .json
class JsonConfig : public IConfig {
	
public:
	//�������� ����� .json
	virtual IConfig* Load(std::string filepath) {

		std::ifstream ifs(filepath); // ����� �����
		if (!ifs.is_open()) { 
			std::cout << "Can't open file " << filepath ;
			return nullptr;
		}

		rapidjson::IStreamWrapper isw(ifs);
		doc.ParseStream(isw); // ������� ������

		ifs.close();
		loader_filepath_ = filepath; // ���������� ���� ������������ �����

		return this;
	
	}
	//��������� ������������ � ���� .json
	virtual bool Save(std::string filepath = "") {
		std::ofstream ofs; // ��������� �����
		if (filepath.empty()) {
			ofs.open(loader_filepath_); // ���� �������� ���� ������, ������������ ���� ��������� �����
		}
		else {
			ofs.open(filepath); 
		}
		if (!ofs.is_open()) { return false; }; // ���� ���� �� ������� �������

		rapidjson::OStreamWrapper osw(ofs);
		rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);

		doc.Accept(writer); // �������� � ����

		ofs.close();

		return true;



	}

	//��������� / ������ ��������
	virtual bool Access(const std::string path, ConfigValue& value, AccessType access) {
		auto value_ptr = GetValueFromNode(path); // ��������� ��������� �� ������ �������� ����
		switch (access) {
		case GET:
			std::visit(JsonGetter{ value_ptr }, value); // ��������� ��������
			return true;
		case SET:
			std::pair<rapidjson::Value*, rapidjson::Document*>  pair; // ��� ���������� ������ ���������� �������� setter'� �������� �� 
			pair.first = value_ptr;									// ������ json ��� ��������� ������ � ���������� ��������
			pair.second = &doc;										// ��������� ������ 


			std::visit(JsonSetter{ pair }, value); // ������ ��������
			return true;
		}
	}



private:




	rapidjson::Document doc; // ������ ��������� json 
	

	std::string loader_filepath_; // ���� � ������������ �����
	


	rapidjson::Value* GetValueFromNode(std::string path) {
		    ConvertPath(path); // �������������� ����
			return rapidjson::Pointer(path.c_str()).Get(doc); // ��������� �������� �� ���������� ����
		        
		}

	//������������ ���� � ������ JSON Pointer  https ://rapidjson.org/md_doc_pointer.html
	// "root.first.second.value" -> "/root/first/second/value"
	void ConvertPath(std::string& path) {
		if (path[0] != '/') path = "/" + path;
		int pos;
		while ((pos = path.find('.')) != std::string::npos) {
		    path[pos] = '/';
		}
	}
};