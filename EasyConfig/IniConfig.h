#pragma once

#include <SimpleIni.h>
#include "IConfig.h"
#include "Utils.h"

//Конфигурация .ini
class IniConfig : public IConfig {
	typedef std::pair<std::string, std::string> SectionAndValueName; 

public:

	//Загрузка файла .ini
	virtual IConfig* Load(std::string filepath) {
		
		SI_Error rc = ini.LoadFile(filepath.c_str()); //rc - информация о результате чтения файла, при ошибке возвращает значение < 0
		if (rc < 0) { 
			
			return nullptr;
		};


		loader_filepath_ = filepath; // Сохранение пути изначального файла
		return this;

	}

	//Сохраняет конфигурацию в файл .ini
	virtual bool Save(std::string filepath = "") {

		SI_Error rc;

		if (filepath.empty()) { // Если параметр пути пустой, сохраняем в тот файл, из которого загружали во время Load()
			rc = ini.SaveFile(loader_filepath_.c_str());
		}
		else {
			rc = ini.SaveFile(filepath.c_str());
		}

		if (rc < 0) { // Если сохранено не удалось, значение rc < 0 
			return false;
		}
		else {
			return true;
		}
		
	}

	//Получение / запись значения
	virtual bool Access(const std::string path, ConfigValue& value, AccessType access) {
		SectionAndValueName pair = SeparatePath(path); 

		std::string section = pair.first; 
		std::string value_name = pair.second;
		std::string text{ 0 }; // Значение value в тестовом виде

		switch (access) {
		case GET:
			text = ini.GetValue(section.c_str(), value_name.c_str()); // Получение текстового значения из ячейки
			std::visit(StringToValue{ text }, value); // Перевод из текста в значение необходимого типа, и запись этого значения в value 
			return true;
		case SET:
			std::visit(ValueToString{ text }, value); // Перевод value в текст
			ini.SetValue(section.c_str(), value_name.c_str(), text.c_str()); // // Перевод запись тестового значения value в нужную ячейку
			return true;
		}
	}

	

	//Разбивает полный путь на секцию и название значения 
	SectionAndValueName SeparatePath(std::string path) {
		
		//TODO заменить на сепаратор пользователя
		char separator = '.';


		SectionAndValueName pair;
		size_t pos; 
		pos = path.find(separator); // Позиция сепаратора

		// Если проверка не проходит, существует второй сепаратор,
		// а .ini не может содержать субсекции
		if (path.rfind(separator) != pos) { throw std::exception("Path should only counain section and value"); } 

		pair.first = path.substr(0, pos);
		pair.second = path.substr(pos+1);
		return pair;
	}


private:
	CSimpleIniA ini; // Объект документа ini
	std::string loader_filepath_; // Путь к загруженному файлу
};