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
	std::string m_login{"login"};
	std::string m_password{"password"};
	bool m_is_admin{ false };

public:
	//----------------------------USER-CONSTRUCTORS-AND-DISTRUCTORS---------------------------------------
	User() = default;
	User(const string& login, const string& password) : m_login{ login }, m_password{ password }{};
	~User() {};//Пока оставил, хотя самостоятельно динамическую память не выделял

	//--------------------------------USER-FUNCTIONS------------------------------------------------------
	void getInfo() const { cout << "Login : " << m_login << '\n' << "Password : " << m_password << endl; }

	friend class System;
};

class System {

private:
	//-----------------------------SYSTEM-ATTRIBUTES------------------------------------------------------
	map<string, pair<string, bool>> m_system{};

	const string M_SYSTEM_PATH{};
	//----------------------------------------------------------------------------------------------------

	//----------------------------SYSTEM-FUNCTIONS--------------------------------------------------------

	void viewUsers() const {

		if (m_system.size() == 0) {
			cout << "There`s no users in the system";
		}
		else{
			for (auto it = m_system.cbegin(); it != m_system.cend(); ++it)
			{
				std::cout << it->first << "\n";
			}
		}
		cout << '\n';
	}

	const System& addUser(const User& user) {

		if (m_system.find(user.m_login) == m_system.end()) {
			pair<string, bool> pair = { user.m_password, user.m_is_admin };
			m_system[user.m_login] = pair;
			ofstream tempFile(M_SYSTEM_PATH + user.m_login + ".txt");
			tempFile << user.m_login << '\n';
			tempFile << user.m_password << '\n';
			tempFile << user.m_is_admin;
			cout << "User successfully added.\n";
		}
		else {
			cout << "You can`t register by " << user.m_login << " login." << endl;
		}
		return *this;
	}

	void removeUser(const string& login_to_remove) {

		m_system.erase(login_to_remove);
		filesystem::path path_to_remove(M_SYSTEM_PATH + login_to_remove + ".txt");
		remove(path_to_remove);
	}

	void clear_screen() {
		char fill = ' ';
		COORD tl = { 0,0 };
		CONSOLE_SCREEN_BUFFER_INFO s;
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(console, &s);
		DWORD written, cells = s.dwSize.X * s.dwSize.Y;
		FillConsoleOutputCharacter(console, fill, cells, tl, &written);
		FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
		SetConsoleCursorPosition(console, tl);
	}

	void move_to_main_screen() {
		cout << "Enter 1 to return to main menu : ";
		int move = 0;
		cin >> move;
		if (move == 1) {
			clear_screen();
		}
	}

	void removeAllUsers() {

		vector<string> logins_to_delete{};

		for (auto it = m_system.cbegin(); it != m_system.cend(); ++it)
		{
			logins_to_delete.push_back(it->first);
		}
		for (int i = 0; i < logins_to_delete.size(); i++) {
			removeUser(logins_to_delete[i]);
		}
		cout << "All users successfully deleted.\n";
	}

	User logToSystem() {
		string user_login = "";
		cout << "Login : ";
		cin >> user_login;
		string user_password = "";
		cout << "\nPassword : ";
		cin >> user_password;
		return User{ user_login, user_password};
	}

	void startUsing() const {
		cout << "You are user!";
	}

	void startAdministrating() {

		int console_act = -1;

		while (true) {

			cout << "1. View all users in the system\n" << "2. Register new user\n" << "3. Delete user\n" << "4. Delete all users\n" << "0. Exit Administrating console.\n";
			cin >> console_act;
			clear_screen();
			switch (console_act)
			{
			case 0:
			{
				console_act = 0;
				break;
			}
			case 1:
			{
				this->viewUsers();
				move_to_main_screen();
				break;
			}
			case 2:
			{
				string login_to_register;
				cout << "Enter new user`s login : ";
				cin >> login_to_register;
				cout << '\n';
				string password_to_register;
				cout << "Enter new user`s password: ";
				cin >> password_to_register;
				cout << '\n';
				this->addUser(User{ login_to_register, password_to_register });
				move_to_main_screen();
				break;
			}
			case 3:
			{
				string login_to_remove;
				cout << "Enter user`s login : ";
				cin >> login_to_remove;
				this->removeUser(login_to_remove);
				move_to_main_screen();
				break;
			}
			case 4:
			{
				this->removeAllUsers();
				move_to_main_screen();
				break;
			}
			default:
				break;
			}
			if (console_act == 0)
			{
				break;
			}
		}
	}
	//------------------------------------------------------------------------------------------------------

public:

	//----------------------------SYSTEM-CONSTRUCTORS-AND-DISTRUCTORS---------------------------------------
	System(const string& system_path) : M_SYSTEM_PATH{ system_path } {


		for (const auto& entry : filesystem::directory_iterator(M_SYSTEM_PATH)) {

			ifstream fill_system(entry.path());

			string tmp_login;
			string tmp_password;
			string tmp_is_admin;

			for (int i = 0; i < 3; i++) {

				if (i == 0) {
					getline(fill_system, tmp_login);
				}
				else if (i == 1) {
					getline(fill_system, tmp_password);
				}
				else {
					getline(fill_system, tmp_is_admin);
				}
			}
			pair<string, bool> m_pair = { tmp_password, stoi(tmp_is_admin)};
			m_system[tmp_login] = m_pair;
		}
	}
	~System() {};//Пока оставил, хотя самостоятельно динамическую память не выделял
	//------------------------------------------------------------------------------------------------------

	//----------------------------SYSTEM-FUNCTIONS----------------------------------------------------------
	const System& addAdmin(User& user) {

		if (m_system.find(user.m_login) == m_system.end()) {
			user.m_is_admin = true;
			pair<string, bool> pair = { user.m_password, user.m_is_admin };
			m_system[user.m_login] = pair;
			ofstream tempFile(M_SYSTEM_PATH + user.m_login + ".txt");
			tempFile << user.m_login << '\n';
			tempFile << user.m_password << '\n';
			tempFile << user.m_is_admin;
			cout << "admin successfully added.\n";
		}
		else {
			cout << "You can`t register by " << user.m_login << " login." << endl;
		}
		return *this;
	}
	void runSystem() {

		User logging_user = logToSystem();

		if (m_system.count(logging_user.m_login)) {
			if (m_system[logging_user.m_login].second == true) {
				startAdministrating();
			}
			else {
				cout << "You are user!";
			}
		}
	}
	//------------------------------------------------------------------------------------------------------

	friend class User;
};

int main() {
	//Путь в папку, в которой будут храниться текстовые файлы с данными о всех пользователях
	//Создать папку для хранения данных нужно заранее
	//Администратора возможно создать только в функции main()
	//Создание нового администратора через консоль невозможно
	//При входе в систему в роли обычного пользователя консоль выводит сообщение о том, что входящий обычный пользователь
	//При входе в систему в роли администратора появляется панель со всеми возможными действиями администратора

	const string FOLDER_PATH = "E:\\Coding\\VSProjects\\Registration_System\\users\\";
	System mainSystem(FOLDER_PATH);
	
	mainSystem.runSystem();

	return 1;
}