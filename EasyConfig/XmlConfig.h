#pragma once
#include <fstream>
#include <rapidxml_ext.hpp>
#include <deque>
#include <sstream>
#include "IConfig.h"
#include "Utils.h"
typedef std::deque<std::string> Nodes; // Очередь имен узлов

using namespace rapidxml;


//Конфигурация .xml
class XmlConfig : public IConfig {

	
public:
	//Загрузка файла .xml
	virtual IConfig* Load(std::string filepath) {
	    std::ifstream ifs(filepath); // Открытие потока
	    if (!ifs.is_open()){
	        return nullptr;
	    }


		
		std::stringstream buffer;
		buffer << ifs.rdbuf();
		ifs.close();

		buffer_content_ = buffer.str(); // Т.к. библиотека использует только указатели, буфер должен быть записан в память
		
		doc_.parse<parse_no_data_nodes>(&buffer_content_[0]); //Флаг parse_no_data_nodes позволяет сохранять измененные,
															  //а не изначальные значения https://www.setnode.com/blog/a-rapidxml-gotcha/
		
		return this;
	}
	
	//Сохраняет конфигурацию в файл .xml
	virtual bool Save(std::string filepath = "") {

		std::ofstream ofs; // Исходящий поток

		if (filepath.empty()) {
			ofs.open(loader_filepath_); // Если параметр пути пустой, использовать путь исходного файла
		}
		else {
			ofs.open(filepath);
		}
		if (!ofs.is_open()) { return false; }

		ofs << doc_; // Запись объекта в файл
		ofs.close();
	    return true;
	}


	//Получение / запись значения
	virtual bool Access(const std::string path, ConfigValue& value, AccessType access) {
		Nodes nodes = PathToNodes(path); // Получение очереди узлов
		if (nodes.size() == 0) throw std::exception("Path is empty, no nodes to go thru");
		auto current_ptr = doc_.first_node(); // Получение текущего узла, с помошью которого мы будем идти вглубь дерева

		const std::string docRoot = current_ptr->name(); // Получаем имя корня документа
		const std::string pathRoot = nodes.front(); // Получаем корень пользовательского пути из очереди
		nodes.pop_front(); // Удаляем корень из очереди

		if (docRoot.compare(pathRoot) != 0) { // Сравнение на правильность корня
			throw std::exception("Invalid root name");
		}

		// Проходим вглубь дерева 
		for (auto n : nodes) { 
			current_ptr = current_ptr->first_node(n.c_str());
		}


		std::string text{ 0 }; // Значение в текстовом виде
		switch (access) {
		case GET:
			text = current_ptr->value(); // Получение значения из узла
			std::visit(StringToValue{ text }, value); // Перевод из текста в величину нужного типа
			return true;
		case SET:
			std::visit(ValueToString{ text }, value); // Перевод значения в тесктовый вид
			char* saved_string = doc_.allocate_string(text.c_str()); // Аллокация памяти для сохранения текстового значения
			current_ptr->value(saved_string); // Сохранение значения
			return true;
		}
	}
	// Получение очереди с именами узлов
	// TODO Пользовательский сепаратор
	Nodes PathToNodes(const std::string path, char separator = '.') {
		Nodes result;
		std::string source = path;
		
		size_t pos = 0; // Позиция сепаратора

		// Отрезать путь до сепаратора и добавить к очереди
		while (pos != std::string::npos) {
			pos = source.find(separator);
			result.push_back(source.substr(0, pos));
			source = source.substr(pos + 1);
		}
		return result;
	}

private:
	
	xml_document<> doc_; // Документ XML с указателями на буфер
	std::string buffer_content_; // Буфер файла
	std::string loader_filepath_; // Путь исходного файла
};