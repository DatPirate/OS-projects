#include <iostream>
#include <thread>
#include <ctime>
using namespace std;
int mx1[3][3] = { {1, 2, 3},{4, 5, 6},{7, 8, 9} };
int mx2[3][3] = { {2, 6, 8},{3, 7, 9},{1, 4, 5} };
int mxres[3][3];

void pr3() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
                mxres[i][j] += mx1[i][0] * mx2[0][j] + mx1[i][1] * mx2[1][j] + mx1[i][2] * mx2[2][j];
        }
    }
}

void prmx() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << mxres[i][j] << " ";
        }
    }
}

int main()
{
    int start_time = clock();
    std::thread th1(pr3);
    th1.join();
    std::thread th2(prmx);
    th2.join();
    int end_time = clock();
    cout << "\n" << end_time - start_time;
}
