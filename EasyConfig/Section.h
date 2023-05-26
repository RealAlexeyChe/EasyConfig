#pragma once
#include "EasyConfig .h"
class Section {
public:
    virtual ~Section() { } // ��� �� ����� ������� config_, ��� ��� ������������ ��� �������



    //������� ������ �� ���������� ���� � ��������-������������
    Section(const std::string& path, EasyConfig* config) {

        section_path_ = path; //���� �� ������������ ������
        config_ = config; 
    }

    // ������� ������ ����� � �������� GetOption ��������
    template <typename T>
    void GetOption(const std::string& path, T& output) {
        config_->GetOption(CreateFullPath(path), output);
    }

    // ������� ������ ����� � �������� SetOption ��������
    template <typename T>
    void SetOption(const std::string& path, const T& input) {
        config_->SetOption(CreateFullPath(path), input);
    }
    

private:
    // ��������� ������� ���� � ����������� �� ���� ������������
    // TODO ���������������� ���������
    std::string CreateFullPath(const std::string& str) {
        switch (config_->GetType()) {
        case REGISTRY:
            return section_path_ + "\\" + str;
        case JSON:
            return section_path_ + "." + str;
        case INI:
            return section_path_ + "." + str;
        case XML:
            return section_path_ + "." + str;

        default:
            throw (std::exception("Unknows config type"));
        }
    }


    std::string section_path_; // ���� �� ������
    EasyConfig* config_; // ��������� �� ��������

};