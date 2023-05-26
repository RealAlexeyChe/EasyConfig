#pragma once
#include <fstream>
#include <rapidxml_ext.hpp>
#include <deque>
#include <sstream>
#include "IConfig.h"
#include "Utils.h"
typedef std::deque<std::string> Nodes; // ������� ���� �����

using namespace rapidxml;


//������������ .xml
class XmlConfig : public IConfig {

	
public:
	//�������� ����� .xml
	virtual IConfig* Load(std::string filepath) {
	    std::ifstream ifs(filepath); // �������� ������
	    if (!ifs.is_open()){
	        return nullptr;
	    }


		
		std::stringstream buffer;
		buffer << ifs.rdbuf();
		ifs.close();

		buffer_content_ = buffer.str(); // �.�. ���������� ���������� ������ ���������, ����� ������ ���� ������� � ������
		
		doc_.parse<parse_no_data_nodes>(&buffer_content_[0]); //���� parse_no_data_nodes ��������� ��������� ����������,
															  //� �� ����������� �������� https://www.setnode.com/blog/a-rapidxml-gotcha/
		
		return this;
	}
	
	//��������� ������������ � ���� .xml
	virtual bool Save(std::string filepath = "") {

		std::ofstream ofs; // ��������� �����

		if (filepath.empty()) {
			ofs.open(loader_filepath_); // ���� �������� ���� ������, ������������ ���� ��������� �����
		}
		else {
			ofs.open(filepath);
		}
		if (!ofs.is_open()) { return false; }

		ofs << doc_; // ������ ������� � ����
		ofs.close();
	    return true;
	}


	//��������� / ������ ��������
	virtual bool Access(const std::string path, ConfigValue& value, AccessType access) {
		Nodes nodes = PathToNodes(path); // ��������� ������� �����
		if (nodes.size() == 0) throw std::exception("Path is empty, no nodes to go thru");
		auto current_ptr = doc_.first_node(); // ��������� �������� ����, � ������� �������� �� ����� ���� ������ ������

		const std::string docRoot = current_ptr->name(); // �������� ��� ����� ���������
		const std::string pathRoot = nodes.front(); // �������� ������ ����������������� ���� �� �������
		nodes.pop_front(); // ������� ������ �� �������

		if (docRoot.compare(pathRoot) != 0) { // ��������� �� ������������ �����
			throw std::exception("Invalid root name");
		}

		// �������� ������ ������ 
		for (auto n : nodes) { 
			current_ptr = current_ptr->first_node(n.c_str());
		}


		std::string text{ 0 }; // �������� � ��������� ����
		switch (access) {
		case GET:
			text = current_ptr->value(); // ��������� �������� �� ����
			std::visit(StringToValue{ text }, value); // ������� �� ������ � �������� ������� ����
			return true;
		case SET:
			std::visit(ValueToString{ text }, value); // ������� �������� � ��������� ���
			char* saved_string = doc_.allocate_string(text.c_str()); // ��������� ������ ��� ���������� ���������� ��������
			current_ptr->value(saved_string); // ���������� ��������
			return true;
		}
	}
	// ��������� ������� � ������� �����
	// TODO ���������������� ���������
	Nodes PathToNodes(const std::string path, char separator = '.') {
		Nodes result;
		std::string source = path;
		
		size_t pos = 0; // ������� ����������

		// �������� ���� �� ���������� � �������� � �������
		while (pos != std::string::npos) {
			pos = source.find(separator);
			result.push_back(source.substr(0, pos));
			source = source.substr(pos + 1);
		}
		return result;
	}

private:
	
	xml_document<> doc_; // �������� XML � ����������� �� �����
	std::string buffer_content_; // ����� �����
	std::string loader_filepath_; // ���� ��������� �����
};