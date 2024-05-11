#include <iostream>
#include <thread>
#include <string>
#include <Windows.h>
#include <fstream>

using namespace std;


HANDLE sem = CreateSemaphore(NULL, 0, 1, L"xx");
bool justWritten = 0;

void readF() {
    ifstream file;
    file.open("E:/test.txt");
    DWORD wait = NULL;
    while (true) {
        wait = WaitForSingleObject(sem, INFINITY);
        if (!justWritten){
            if (wait == WAIT_OBJECT_0) {
                file.seekg(-1, ios_base::end);
                bool keepLooping = true;
                while (keepLooping) {
                    char ch;
                    file.get(ch);
                    if ((int)file.tellg() <= 1) {
                        file.seekg(0);
                        keepLooping = false;
                    }
                    else if (ch == '\n') {
                        keepLooping = false;
                    }
                    else {
                        file.seekg(-2, ios_base::cur);
                    }
                }
                string lastLine;
                getline(file, lastLine);
                cout << lastLine;
            }
        }
    }
}

void writeF() {
    ofstream file;
    while (true) {
        file.open("E:/test.txt", ios::app);
        string text;
        cin >> text;
        if (text == "exit") {
            return;
        }
        file << text << "\n";
        justWritten = 1;
        ReleaseSemaphore(sem, 1, NULL);
        file.close();
    }
}

int main()
{
    fstream file;
    file.open("E:/test.txt");
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
    thread writerThread(writeF);
    thread readerThread(readF);
    writerThread.join();
    readerThread.join();
}