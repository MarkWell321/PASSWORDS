#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <fstream>
#include <list>  
#include <Windows.h>
#include <cstdio>  
#include <iterator>

using namespace std;

class rec
{
public:
	bool operator < (const rec&b);
	rec(string pass, string user, string res);
	string password;
	string username;
	string resource;
	void encrypt(string key);
	void decipher(string key);
	void out();
};

bool rec::operator < (const rec&b) {
	if (resource == b.resource) {
		return username < b.username;
	}
	return resource < b.resource;
}

string random_password(int n) {
	string res = "";
	srand(time(0));//при каждом новом запуске программы значени€ генерировались разными
	string help;//ASII: 48-57(÷ифры) 93-96(спецсимволы) 33-47(спецсимволы) 58-64(спецсимволы)  123-126(спецсимволы) 91-92(спецсимволы) 65-90(заглавные буквы) 97-122(строчные буквы)
	int a = 48;
	for (n; n > 0; n--) {
		switch (rand() % 7 + 1) {
		case 1:case 2://cпецсимволы
			while (true) {
				int a = 48;
				while ((48 <= a && a <= 57) || (65 <= a && a <= 90) || (97 <= a && a <= 122)) {
					a = rand() % 94 + 33;
				}
				help = a;
				if (res.find(help) == -1) {
					res += help;
					break;
				}
			}
			break;
		case 3://цифры
			while (true) {
				help = rand() % 10 + 48;
				if (res.find(help) == -1) {
					res += help;
					break;
				}
			}
			break;
		case 4:case 5://строчные буквы
			while (true) {
				help = rand() % 26 + 97;
				if (res.find(help) == -1) {
					res += help;
					break;
				}
			}
			break;
		case 6:case 7://заглавные буквы
			while (true) {
				help = rand() % 26 + 65;
				if (res.find(help) == -1) {
					res += help;
					break;
				}
			}
			break;
		}
	}
	return res;
}

rec::rec(string pass, string user, string res) {//при считывании из файла
	password = pass;
	username = user;
	resource = res;
}

void rec::encrypt(string key) {//шифровка парол€
	/*for (int i = 0; i < password.size() / 2 ; i++) {
		swap(password[i], password[password.size() - i - 1]);
	}*/
	char help;
	int j = 0;
	for (int i = 0; i < password.size(); i++)
	{
		int change = key[j] - 48;
		help = password[i];
		if (help + change > 126) {
			help += (change - 94);
		}
		else {
			help += change;
		}
		password[i] = help;
		j++;
		if (j == key.size()) {
			j = 0;
		}
	}
}

void rec::decipher(string key) {//дешифровка парол€
	char help;
	int j = 0;
	for (int i = 0; i < password.size(); i++)
	{
		int change = key[j] - 48;
		help = password[i];
		if (help - change < 33) {
			help -= change; 
			help += 94;
		}
		else {
			help -= change;
		}
		password[i] = help;
		j++;
		if (j == key.size()) {
			j = 0;
		}
	}
	/*for (int i = 0; i < password.size() / 2; i++) {
		swap(password[i], password[password.size() - i - 1]);
	}*/
}

list <rec> input(string filename, string key){//считывает из файла и создает список всех записей, дешифру€ их
	list <rec> res;
	ifstream in(filename);
	while (!in.eof()) {
		string help;
		getline(in, help);
		int j1 = help.find(':');
		string resource = help.substr(0, j1 );
		int j2 = help.find('-');
		string username = help.substr(j1 + 2, j2 - j1 - 3);
		string password = help.substr(j2 + 2, help.size() - 1);
		rec NEW(password, username, resource);
		NEW.decipher(key);
		res.push_back(NEW);
	}
	res.sort();
	return res;
	in.close();
}

int check(list <rec>input) {
	int sum = 0;
	while (!input.empty()) {
		rec help = input.front();
		input.pop_front();
		for (int i = 0; i < help.password.size(); i++) {
			sum += help.password[i];
		}
	}
	return sum;
}

void output(list <rec>&input,string filename) {
	int sum = check(input);
	string key = to_string(sum);
	cout << "Your new key: " << key;
	_getch();
	ofstream out(filename);
	rec help = input.front();
	input.pop_front();
	out << help.resource << ": ";
	out << help.username << " - ";
	help.encrypt(key);
	out << help.password;
	while (!input.empty()) {
		rec help = input.front();
		input.pop_front();
		out << '\n' << help.resource << ": ";
		out << help.username << " - ";
		help.encrypt(key);
		out << help.password;
	}
	out.close();
}

void add_rec(list <rec> & input) {
	string res;
	cout << "Enter name of resource : ";
	cin >> res;
	string user;
	cout << "Enter username : ";
	cin >> user;
	cout << "Do you want to create a random password?" << '\n';
	string help;
	string pass;
	cin >> help;
	while (help != "yes" && help != "no") {
		system("cls");
		cout << "Write the correct answer(yes or no)" << '\n';
		Sleep(500);
		system("cls");
		cout << "Do you want to create a random password?" << '\n';
		string menu;
		cin >> menu;
		help = menu;
	}
	if (help == "yes") {
		system("cls");
		int a;
		cout << "Enter the length of the password: ";
		cin >> a;
		pass = random_password(a);
		cout << "Your password: " << pass;
		_getch();
	}
	else {
		system("cls");
		cout << "Enter the password for this account: ";
		cin >> pass;
	}
	rec NEW(pass, user, res);
	input.push_back(NEW);
	input.sort();
	system("cls");
}

void delete_rec(list <rec> & input) {

}

ostream& operator<<(ostream& os, const rec & rc)
{
	os <<">>  " << rc.resource << ": ";
	os << rc.username << " - ";
	os << rc.password <<'\n' ;
	return os;
}

void menu() {
	list <rec> all;
	string key;
	string filename;
	cout << "Enter the absolute path to the file: ";
	cin >> filename;
	system("cls");
	ifstream in;
	in.open(filename);
	if (!in) {
		cout << "Cant find this file. Do you want to create a new file?" << '\n';
		string help;
		cin >> help;
		while (help != "yes" && help != "no") {
			system("cls");
			cout << "Write the correct answer(yes or no)" << '\n';
			Sleep(500);
			system("cls");
			cout << "Cant find this file. Do you want to create a new file?" << '\n';
			string menu;
			cin >> menu;
			help = menu;
		}
		if (help == "no") {
			return;
		}
	}
	else {
		string a;
		in >> a;
		if (!in.eof()) {
			cout << "Enter the decryption key: ";
			cin >> key;
			for (int i = 0; i < key.size(); i++) {
				if (key[i] > 58 || key[i] < 48) {
					ofstream out("Copy.txt");
					out << in.rdbuf();
					out.close();
					in.close();
					remove(filename.data());
					cout << "The key was wrong. The original file was deleted.";
					Sleep(200);
					return;
				}
			}
			all = input(filename, key);
			if (atoi(key.c_str()) != check(all)) {
				ofstream out("Copy.txt");
				out << in.rdbuf();
				out.close();
				in.close();
				remove(filename.data());
				cout << "The key was wrong. The original file was deleted.";
				Sleep(200);
				return;
			}
		}
		else {
			cout << "Warning, your file is empty. Work and record continue in this file";
			Sleep(250);
		}
	}
	in.close();
	int i = 1;
	system("cls");
	cout << "------------------" << '\n' << "|1)Add new record|" << '\n' << "------------------" << '\n' << " 2)Display all records" << '\n' << " 3)Exit";
	while (true) {
		char q = _getch();
		system("cls");
		if (q == 13) {
			switch (i) {
			case 1: add_rec(all); break;
			case 2:
				system("mode con cols=250 lines=250");
				copy(all.begin(), all.end(), ostream_iterator<rec>(cout, ""));
				_getch();
				system("mode con cols=120 lines=30");
				break;
			case 3: 
				if (!all.empty()) {
					output(all, filename);
				}
				return;
				break;
			}
		}
		else if (q == 72) {
			if (i == 1) {
				i = 3;
			}
			else {
				i--;
			}
		}
		else if (q == 80) {
			if (i == 3) {
				i = 1;
			}
			else {
				i++;
			}
		}
		system("cls");
		switch (i) {
		case 1:cout << "------------------" << '\n' << "|1)Add new record|" << '\n' << "------------------" << '\n' << " 2)Display all records" << '\n' << " 3)Exit"; break;
		case 2:cout << " 1)Add new record " << '\n' << "-----------------------" << '\n' << "|2)Display all records|" << '\n' << "-----------------------" << '\n' << " 3)Exit"; break;
		case 3:cout << " 1)Add new record" << '\n' << " 2)Display all records" << '\n' << "--------" << '\n' << "|3)Exit|" << '\n' << "--------"; break;
		}
	}
}

int main() {
	menu();
}