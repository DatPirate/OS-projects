// 3ways.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <thread>
#include <ctime>
#include <mutex>
#include <atomic>
#include <vector>


int count = 0;
std::mutex mtx;
std::atomic<int> acount{ 0 };
std::vector <std::thread> threads;

void inc() {
    for (int i = 0; i < 20000; i++) {
        count++;
    }
}

void mtx_inc() {
    mtx.lock();
    for (int i = 0; i < 20000; i++) {
        count++;
    }
    mtx.unlock();
}

void a_inc() {
    for (int i = 0; i < 20000; i++) {
        acount.fetch_add(1);
    }
}

int main()
{
    int start_time = clock();
    for (int i = 0; i < 1000; i++) {
        std::thread th(inc);
        threads.push_back(move(th));
    }
    for (auto& it : threads) {
        it.join();
    }
    int end_time = clock();
    std::cout << "stupid count = " << count << std::endl;
    std::cout << "exec time = " << end_time - start_time << std::endl;
    threads.clear();
    count = 0;
    start_time = clock();
    for (int i = 0; i < 1000; i++) {
        std::thread th(mtx_inc);
        threads.push_back(move(th));
    }
    for (auto& it : threads) {
        it.join();
    }
    end_time = clock();
    std::cout << "mutex count = " << count << std::endl;
    std::cout << "exec time = " << end_time - start_time << std::endl;
    threads.clear();
    start_time = clock();
    for (int i = 0; i < 1000; i++) {
        std::thread th(a_inc);
        threads.push_back(move(th));
    }
    for (auto& it : threads) {
        it.join();
    }
    end_time = clock();
    std::cout << "atomic count = " << acount << std::endl;
    std::cout << "exec time = " << end_time - start_time << std::endl;
    return 0;
}