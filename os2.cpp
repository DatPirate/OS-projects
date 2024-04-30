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
    /*int start_time = clock();
    std::thread th1(inc);
    std::thread th2(inc);
    std::thread th3(inc);
    std::thread th4(inc);
    std::thread th5(inc);
    std::thread th6(inc);
    std::thread th7(inc);
    std::thread th8(inc);
    std::thread th9(inc);
    std::thread th10(inc);
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();
    th7.join();
    th8.join();
    th9.join();
    th10.join();
    int end_time = clock();
    std::cout << "count = " << count << std::endl;
    std::cout << "exec time = " << end_time - start_time << std::endl;
    count = 0;
    start_time = clock();
    std::thread th11(mtx_inc);
    std::thread th12(mtx_inc);
    std::thread th13(mtx_inc);
    std::thread th14(mtx_inc);
    std::thread th15(mtx_inc);
    std::thread th16(mtx_inc);
    std::thread th17(mtx_inc);
    std::thread th18(mtx_inc);
    std::thread th19(mtx_inc);
    std::thread th20(mtx_inc);
    th11.join();
    th12.join();
    th13.join();
    th14.join();
    th15.join();
    th16.join();
    th17.join();
    th18.join();
    th19.join();
    th20.join();
    end_time = clock();
    std::cout << "count = " << count << std::endl;
    std::cout << "exec time = " << end_time - start_time << std::endl;
    start_time = clock();
    std::thread th21(a_inc);
    std::thread th22(a_inc);
    std::thread th23(a_inc);
    std::thread th24(a_inc);
    std::thread th25(a_inc);
    std::thread th26(a_inc);
    std::thread th27(a_inc);
    std::thread th28(a_inc);
    std::thread th29(a_inc);
    std::thread th30(a_inc);
    th21.join();
    th22.join();
    th23.join();
    th24.join();
    th25.join();
    th26.join();
    th27.join();
    th28.join();
    th29.join();
    th30.join();
    end_time = clock();
    std::cout << "count = " << acount << std::endl;
    std::cout << "exec time = " << end_time - start_time << std::endl;*/
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