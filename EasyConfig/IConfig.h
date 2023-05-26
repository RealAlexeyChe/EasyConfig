#pragma once
#include <iostream>
#include <string>
#include <variant>

typedef std::variant<int, bool, double, std::string> ConfigValue;
enum AccessType{GET, SET};

//��������� ������������
class IConfig{
	
public:
		


		//��������� ����(�� ���� path), ������ ��� ���� ������ ������������� ���� � ���������� ��� �����������   
		//���� ��� �������, ���������� ��������� ��� ����, ��� nullptr, ���� ��������� ������
		virtual IConfig* Load(std::string path) = 0;


		//��������� (���� ��� ��������), ��������� � ��� �� ����, ���� ��� ��������� ����, ��� � ���� �� ���������� ����
		//���������� false, ���� ���������� �� �������

		//TODO: �������� ����������� �� ���� �������������� �� ����� �������� �������� � �������
		//SetOption(), � ������ ����� ����������� ������������� ������� � ���������� �� ������ �����
		//������ Save()
		virtual bool Save(std::string filepath = "") = 0;


		//�������� ��� ���������� (� ����������� �� access) �������� �� ���������� ���� � value.
		
		//���� ������������ ���������� �������� ���� / ������������ ���� �������� (� ������������ ������������� ����
		//��������, ���������� bool, ���� ���, ������ ���������� true;

		virtual bool Access(const std::string path, ConfigValue& value, AccessType access) = 0;

		//TODO �������� ���� / ������������ ����
		//enum ValueType{INT, BOOL, STRING, ... , INVALID_PATH, EMPTY_VALUE};
		//virtual ValueType Check(const std::string path, ConfigValue& value, AccessType access) = 0
};
