#ifndef SYSTEM_H
#define SYSTEM_H
#include <iostream>
#include <fstream>//work with files
#include <string>//a lot of variables
#include <map>//m_system
#include <filesystem>//removing files
#include <windows.h>//clear console

using namespace std;

class User;

class System {

private:

	map<string, pair<string, bool>> m_system{};

	const string M_SYSTEM_PATH{};

	void viewUsers() const;

	const System& addUser(const User& user);

	void removeUser(const string& login_to_remove);

	void clear_screen();

	void move_to_main_screen();

	void removeAllUsers();

	User logToSystem();

	void startUsing() const;

	void startAdministrating();

public:

	System(const string& system_path);

	const System& addAdmin(User& user);

	void runSystem();

	friend class User;
};


#endif

