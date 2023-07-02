#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

#define num_columns 1011
#define num_elimination_rows 539
#define num_eliminated_rows 263

#define NUM_THREADS 4

const int length = 203;

char R[10000][length]; // R����
char E[num_eliminated_rows][length]; // E����

vector<pair<int, int>> records; // �����¼������
bool lifted[num_eliminated_rows]; // ��ʾ���Ƿ��ѱ���Ԫ

mutex mtx_leader; // ������ - ���߳�
mutex mtx_division[NUM_THREADS - 1]; // ������ - �ָ��ͬ��
mutex mtx_elimination[NUM_THREADS - 1]; // ������ - ��Ԫͬ��

void print() {
    // ��ӡ���
    for (int i = 0; i < num_eliminated_rows; i++) {
        for (int j = num_columns - 1; j >= 0; j--) {
            if (((E[i][j / 5] >> (j - 5 * (j / 5)))) & 1 == 1) {
                cout << j << ' ';
            }
        }
        cout << endl;
    }
}

void thread_func(int id) {
    // �̺߳���
    for (int n = length - 1; n >= 0; n--) {
        if (id == 0) {
            records.clear();
            // ��Ԫ����
            for (int i = 0; i < num_eliminated_rows; i++) {
                if (lifted[i]) {
                    continue;
                }
                for (int j = 5 * (n + 1) - 1; j >= 5 * n; j--) {
                    if (E[i][n] >> (j - 5 * n) == 1) {
                        if (R[j][n] != 0) {
                            E[i][n] ^= R[j][n];
                            records.emplace_back(i, j);
                        } else {
                            for (auto pair : records) {
                                int row = pair.first;
                                int leader = pair.second;
                                if (row == i) {
                                    for (int k = n - 1; k >= 0; k--) {
                                        E[i][k] ^= R[leader][k];
                                    }
                                }
                            }
                            for (int k = 0; k < length; k++) {
                                R[j][k] = E[i][k];
                            }
                            lifted[i] = true;
                            break;
                        }
                    }
                }
            }
            // �����ָ��ͬ��������
            for (int i = 0; i < NUM_THREADS - 1; i++) {
                mtx_division[i].unlock();
            }
        } else {
            mtx_division[id - 1].lock(); // �������ȴ��ָ��ͬ��
        }
        // ִ����Ԫ����
        for (int i = id; i < n; i += NUM_THREADS) {
            for (auto pair : records) {
                int row = pair.first;
                int leader = pair.second;
                if (lifted[row]) {
                    continue;
                }
                E[row][i] ^= R[leader][i];
            }
        }
        if (id == 0) {
            // �������̻߳�����
            for (int i = 0; i < NUM_THREADS - 1; i++) {
                mtx_leader.lock();
            }
            // ������Ԫͬ��������
            for (int i = 0; i < NUM_THREADS - 1; i++) {
                mtx_elimination[i].unlock();
            }
        } else {
            mtx_leader.unlock(); // �������̻߳�����
            mtx_elimination[id - 1].lock(); // �������ȴ���Ԫͬ��
        }
    }
}

void solve() {
    vector<thread> threads;
    // �����߳�
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(thread_func, i);
    }
    // �ȴ��߳̽���
    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    ifstream file_R;
    char buffer[10000] = {0};
    // ��ȡR�����ļ�
    file_R.open("D:/study/vscode/parallel/Parallel_Final_Research/�����˹/Groebner/��������4 ��������1011��������Ԫ��539������Ԫ��263/��Ԫ��.txt");
    if (file_R.fail()) {
        cout << "����ʧ��" << endl;
        return 1;
    }
    while (file_R.getline(buffer, sizeof(buffer))) {
        int bit;
        stringstream line(buffer);
        int first_in = 1;
        int index;
        while (line >> bit) {
            if (first_in) {
                first_in = 0;
                index = bit;
            }
            R[index][bit / 5] |= 1 << (bit - (bit / 5) * 5);
        }
    }
    file_R.close();

    ifstream file_E;
    // ��ȡE�����ļ�
    file_E.open("D:/study/vscode/parallel/Parallel_Final_Research/�����˹/Groebner/��������4 ��������1011��������Ԫ��539������Ԫ��263/����Ԫ��.txt");
    int index = 0;
    while (file_E.getline(buffer, sizeof(buffer))) {
        int bit;
        stringstream line(buffer);
        while (line >> bit) {
            E[index][bit / 5] |= (1 << (bit - (bit / 5) * 5));
        }
        index++;
    }
    file_E.close();

    solve();

    return 0;
}
