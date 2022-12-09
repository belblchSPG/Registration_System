#include "System.h"
#include "User.h"

using namespace std;

System::System(const string& system_path) : M_SYSTEM_PATH{ system_path } {


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
		pair<string, bool> m_pair = { tmp_password, stoi(tmp_is_admin) };
		m_system[tmp_login] = m_pair;
	}
}

void System::viewUsers() const {

	if (m_system.size() == 0) {
		cout << "There`s no users in the system";
	}
	else {
		for (auto it = m_system.cbegin(); it != m_system.cend(); ++it)
		{
			std::cout << it->first << "\n";
		}
	}
	cout << '\n';
}

const System& System::addUser(const User& user) {

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

void System::removeUser(const string& login_to_remove) {

	m_system.erase(login_to_remove);
	filesystem::path path_to_remove(M_SYSTEM_PATH + login_to_remove + ".txt");
	remove(path_to_remove);
}

void System::clear_screen() {
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

void System::move_to_main_screen() {
	cout << "Enter 1 to return to main menu : ";
	int move = 0;
	cin >> move;
	if (move == 1) {
		clear_screen();
	}
}

void System::removeAllUsers() {

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

User System::logToSystem() {
	string user_login = "";
	cout << "Login : ";
	cin >> user_login;
	string user_password = "";
	cout << "\nPassword : ";
	cin >> user_password;
	return User{ user_login, user_password };
}

void System::startUsing() const {
	cout << "You are user!";
}

void System::startAdministrating() {

	clear_screen();
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

const System& System::addAdmin(User& user) {

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

void System::runSystem() {

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