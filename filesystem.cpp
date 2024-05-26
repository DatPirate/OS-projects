#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <windows.h>
#include <mutex>

#include <vector>
#include <string>
#include <sstream>
#include <iterator>

#define MAX_FILES 1000
#define MAX_FILE_NAME 32
#define MAX_DATA_SIZE 1024

using namespace std;

typedef struct File {
	string name;
	string data;
	int size;
} File;

vector<File> files;
vector<string> cmds = { "create", "read", "write", "delete", "find", "info", "exit"};
mutex mt;

void createFile(string name) {
	if (name.length() > MAX_FILE_NAME) {
		cout << "Ошибка: имя файла слишком длинное\n";
	}
	else {
		if (files.size() < MAX_FILES) {
			const auto& it = find_if(files.begin(), files.end(), [name](const File ff) { return ff.name == name; });
			if (it == files.end()) {
				File newfile;
				newfile.name = name;
				newfile.data = "";
				newfile.size = 0;
				files.push_back(newfile);
			}else {
				cout << "Ошибка: Файл с таким именем уже существует\n";
			}
		}else {
				cout << "Ошибка: Достигнут лимит количества файлов\n";
		}
	}
}

void readFile(string name) {
	mt.lock();
	const auto& it = find_if(files.begin(), files.end(), [name](const File ff) { return ff.name == name; });
	if (it != files.end()) {
		cout << it->data << "\n";
	}
	else {
		cout << "Ошибка: Файл с таким именем не найден\n";
	}
	mt.unlock();
}

void writeFile(string name, string text) {
	mt.lock();
	if (text.length() > MAX_DATA_SIZE) {
		cout << "Ошибка: Превышен допустимый размер данных\n";
	}
	else {
		const auto& it = find_if(files.begin(), files.end(), [name](const File ff) { return ff.name == name; });
		if (it != files.end()) {
			it->data = text;
			it->size = text.length()-1;
		}
		else {
			cout << "Ошибка: Файл с таким именем не найден\n";
		}
	}
	mt.unlock();
}

void deleteFile(string name) {
	mt.lock();
	const auto& it = find_if(files.begin(), files.end(), [name](const File ff) { return ff.name == name; });
	if (it != files.end()) {
		files.erase(it);
	}
	else {
		cout << "Ошибка: Файл с таким именем не найден\n";
	}
	mt.unlock();
}

void fsInfo() {
	//print all the data about fs
	cout << "Количество файлов: " << to_string(static_cast<int>(files.size())) << "\n";
	cout << "Файлы:\n";
	for (auto& file : files) {
		cout << file.name << " Размер: " << to_string(file.size) << "\n";
	}
}

void findByData(string data) {
	mt.lock();
	const auto& it = find_if(files.begin(), files.end(), [data](const File ff) { return ff.data.find(data) != string::npos; });
	if (it != files.end()) {
		cout << "Найденный файл: " << it->name << "\n";
	}
	else {
		cout << "Файл с таким содержимым не найден\n";
	}
	mt.unlock();
}

void waitForInput() {
	while (true) {
		cout << "Введите команду:\n";
		string input;
		getline(cin, input);
		istringstream iss(input);
		vector<string> words;
		string word;
		while (iss) {
			iss >> word;
			words.push_back(word);
		}
		if (words.size() >= 1) {
			if (find(cmds.begin(), cmds.end(), words[0]) != cmds.end()) {
				if (words[0] == "create") {
					if (words.size() == 3) {
						createFile(words[1]);
						continue;
					}
					else {
						cout << "Ошибка: Неверное количество параметров\n";
						continue;
					}
				}
				if (words[0] == "read") {
					if (words.size() == 3) {
						readFile(words[1]);
						continue;
					}
				}
				if (words[0] == "write") {
					if (words.size() >= 4) {
						string data = "";
						for (size_t i = 2; i < words.size() - 1; i++) {
							data = data + words[i];
							if (i + 1 != words.size()) {
								data = data + " ";
							}
						}
						writeFile(words[1], data);
						continue;
					}
					else {
						cout << "Ошибка: Неверное количество параметров\n";
						continue;
					}
				}
				if (words[0] == "delete") {
					if (words.size() == 3) {
						deleteFile(words[1]);
						continue;
					}
					else {
						cout << "Ошибка: Неверное количество параметров\n";
						continue;
					}
				}
				if (words[0] == "find") {
					if (words.size() == 3) {
						findByData(words[1]);
						continue;
					}
					else {
						cout << "Ошибка: Неверное количество параметров\n";
						continue;
					}
				}
				if (words[0] == "info") {
					if (words.size() == 2) {
						fsInfo();
						continue;
					}
					else {
						cout << "Ошибка: Неверное количество параметров\n";
						continue;
					}
				}
				if (words[0] == "exit") {
					return;
				}
			}
			else {
				cout << "Ошибка: команда не найдена\n";
				continue;
			}
		}
		else {
			cout << "Ошибка: команда не введена\n";
			continue;
		}
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	cout << "Список команд:\n";
	cout << "create filename - создать файл с названием filename\n";
	cout << "read filename - вывести на экран данные файла с именем filename\n";
	cout << "write filename data - записать в файл с названием filename данные data\n";
	cout << "delete filename - удалить файл с именем filename\n";
	cout << "find data - найти файл с заданными данными data\n";
	cout << "info - получить информацию о файловой системе\n";
	cout << "exit - выход из программы\n";
	waitForInput();
	return 0;
}

