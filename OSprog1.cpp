#include <iostream>
#include <thread>
#include <ctime>
using namespace std;
int mx1[3][3] = { {1, 2, 3},{4, 5, 6},{7, 8, 9} };
int mx2[3][3] = { {2, 6, 8},{3, 7, 9},{1, 4, 5} };
int mxres1[3][3];
int mxres2[3][3];

void mnmx2(int a) {
    for (int i = 0; i < 3; i++) {
        mxres2[a][i] = mx1[a][0] * mx2[0][i] + mx1[a][1] * mx2[1][i] + mx1[a][2] * mx2[2][i];
    }
}

void mnmx1(int a) {
    for (int i = 0; i < 3; i++) {
        mxres1[a][i] = mx1[a][0] * mx2[0][i] + mx1[a][1] * mx2[1][i] + mx1[a][2] * mx2[2][i];
    }
}

void prmx() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << mxres1[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << mxres2[i][j] << " ";
        }
        cout << "\n";
    }
}

int main()
{
    int start_time = clock();
    mnmx1(0);
    mnmx1(1);
    mnmx1(2);
    int end_time = clock();
    cout << "Without multithread:\n" << end_time - start_time << "\n";
    start_time = clock();
    std::thread th1(mnmx2, 0);
    std::thread th2(mnmx2, 1);
    std::thread th3(mnmx2, 2);
    th1.join();
    th2.join();
    th3.join();
    end_time = clock();
    cout << "With multithread\n" << end_time - start_time << "\n";
    prmx();
}
