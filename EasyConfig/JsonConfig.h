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


// TODO Получение / запись значений в аттрибуты

//Конфигурация .json
class JsonConfig : public IConfig {
	
public:
	//Загрузка файла .json
	virtual IConfig* Load(std::string filepath) {

		std::ifstream ifs(filepath); // Поток файла
		if (!ifs.is_open()) { 
			std::cout << "Can't open file " << filepath ;
			return nullptr;
		}

		rapidjson::IStreamWrapper isw(ifs);
		doc.ParseStream(isw); // Парсинг потока

		ifs.close();
		loader_filepath_ = filepath; // Сохранение пути изначального файла

		return this;
	
	}
	//Сохраняет конфигурацию в файл .json
	virtual bool Save(std::string filepath = "") {
		std::ofstream ofs; // Исходящий поток
		if (filepath.empty()) {
			ofs.open(loader_filepath_); // Если параметр пути пустой, использовать путь исходного файла
		}
		else {
			ofs.open(filepath); 
		}
		if (!ofs.is_open()) { return false; }; // Если файл не удалось открыть

		rapidjson::OStreamWrapper osw(ofs);
		rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);

		doc.Accept(writer); // Записать в файл

		ofs.close();

		return true;



	}

	//Получение / запись значения
	virtual bool Access(const std::string path, ConfigValue& value, AccessType access) {
		auto value_ptr = GetValueFromNode(path); // Получение указателя на нужный значение узла
		switch (access) {
		case GET:
			std::visit(JsonGetter{ value_ptr }, value); // Получение значения
			return true;
		case SET:
			std::pair<rapidjson::Value*, rapidjson::Document*>  pair; // Для сохранения строки необходимо передать setter'у референс на 
			pair.first = value_ptr;									// объект json для аллокации памяти и сохранение значения
			pair.second = &doc;										// текстовой строки 


			std::visit(JsonSetter{ pair }, value); // Запись значения
			return true;
		}
	}



private:




	rapidjson::Document doc; // Объект документа json 
	

	std::string loader_filepath_; // Путь к загруженному файлу
	


	rapidjson::Value* GetValueFromNode(std::string path) {
		    ConvertPath(path); // Конвертировать путь
			return rapidjson::Pointer(path.c_str()).Get(doc); // Получение значение по указанному пути
		        
		}

	//Конвертирует путь в формат JSON Pointer  https ://rapidjson.org/md_doc_pointer.html
	// "root.first.second.value" -> "/root/first/second/value"
	void ConvertPath(std::string& path) {
		if (path[0] != '/') path = "/" + path;
		int pos;
		while ((pos = path.find('.')) != std::string::npos) {
		    path[pos] = '/';
		}
	}
};