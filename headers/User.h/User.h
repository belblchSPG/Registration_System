#ifndef USER_H
#define USER_H
#include <iostream>
#include <fstream>//work with files
#include <string>//a lot of variables
#include <map>//m_system
#include <filesystem>//removing files
#include <windows.h>//clear console

using namespace std;

class User {

private:
	//----------------------------USER-ATTRIBUTES---------------------------------------------------------
	std::string m_login{ "login" };
	std::string m_password{ "password" };
	bool m_is_admin{ false };

public:
	//----------------------------USER-CONSTRUCTORS-AND-DISTRUCTORS---------------------------------------
	User();
	User(const string& login, const string& password);

	//--------------------------------USER-FUNCTIONS------------------------------------------------------
	void getInfo() const;

	friend class System;
};


#endif 