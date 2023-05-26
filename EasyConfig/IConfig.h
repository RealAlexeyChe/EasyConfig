#pragma once
#include <iostream>
#include <string>
#include <variant>

typedef std::variant<int, bool, double, std::string> ConfigValue;
enum AccessType{GET, SET};

//Интерфейс конфигурации
class IConfig{
	
public:
		


		//Загружает файл(по пути path), реестр или базу данных определённгого типа и инициирует все необходимое   
		//Если это удалось, возвращает указатель для себя, или nullptr, если произошла ошибка
		virtual IConfig* Load(std::string path) = 0;


		//Сохраняет (если это возможно), изменения в тот же файл, если нет параметра пути, или в файл по указанному пути
		//Возвращает false, если сохранение не удалось

		//TODO: Добавить возможность во всех импленентациях не сразу изменять значение с помощью
		//SetOption(), а вместо этого накапливать измененняемые данниые и записывать их только после
		//вызова Save()
		virtual bool Save(std::string filepath = "") = 0;


		//Получает или записывает (в зависимости от access) значение по указанному пути в value.
		
		//Если присутствует возмжность проверки пути / правильности типа значения (и присутствует импленентация этой
		//проверки, возвращает bool, если нет, всегда возвращает true;

		virtual bool Access(const std::string path, ConfigValue& value, AccessType access) = 0;

		//TODO Проверка пути / правильности типа
		//enum ValueType{INT, BOOL, STRING, ... , INVALID_PATH, EMPTY_VALUE};
		//virtual ValueType Check(const std::string path, ConfigValue& value, AccessType access) = 0
};
