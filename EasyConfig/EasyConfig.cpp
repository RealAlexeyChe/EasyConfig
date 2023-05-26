// EasyConfig.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Section.h"
#include "EasyConfig .h"
#include "Section.h"
int main()
{
    EasyConfig conf;
    conf.Load(REGISTRY);

    std::string current_build;
    int ubr;

    conf.GetOption("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\CurrentBuild", current_build);
    conf.GetOption("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\UBR", ubr);
    std::cout << "Windows build: " << current_build << "." << ubr << std::endl;

    Section sect("HKEY_CURRENT_USER\\Volatile Environment", &conf);

    std::string username, appdata;

    sect.GetOption("APPDATA", appdata);
    sect.GetOption("USERNAME", username);
    
    std::cout << "Username: " << username << std::endl;
    std::cout << "Appdata: " << appdata << std::endl << std::endl;

    conf.Load(JSON, "test.json");

    std::string type;
    std::string hostname;
    int port;

    conf.GetOption("App.Globals.DB.Type", type);

    Section db("App.Globals.DB", &conf);

    db.GetOption("HostName", hostname);
    db.GetOption("PortNo", port);

    std::string localhost = "localhost";

    db.SetOption("HostName", localhost);
    db.SetOption("PortNo", 9909);
    
    std::cout << "Database: " << type << " " << hostname << " " << "PortNo: " << port << std::endl;
    conf.Save("out.json");

    conf.Load(INI, "test.ini");

    std::string owner;
    conf.GetOption("owner.name", owner);

    //conf.SetOption("owner.organization", "albert"); // FIX STRING LITERALS

    Section database_info("database", &conf);

    std::string server_ip;

    database_info.GetOption("server", server_ip);
    std::cout << "Owner: " << owner << " IP:    " << server_ip << std::endl;

    conf.Save("out.ini");



    conf.Load(XML, "test.xml");

    std::string author;
    
    conf.GetOption("catalog.book.author", author);

    std::cout << "Author: " << author << std::endl;

    author = "Stephen King";
    conf.SetOption("catalog.book.author", author);

    conf.GetOption("catalog.book.author", author);

    std::cout << "New author: " << author << std::endl;

    conf.Save("out.xml");

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
