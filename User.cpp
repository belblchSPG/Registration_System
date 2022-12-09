#include "User.h"
#include "System.h"

using namespace std;

User::User() = default;

User::User(const string & login, const string & password) : m_login{ login }, m_password{ password }{};

void User::getInfo() const {
	cout << "Login : " << m_login << '\n' << "Password : " << m_password << endl; }
