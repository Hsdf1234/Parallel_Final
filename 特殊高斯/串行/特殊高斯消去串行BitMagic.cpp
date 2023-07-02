#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>
#include "bm/bm.h"

using namespace std;

// ��Ӧ�����ݼ���������������������������Ԫ������������Ԫ������
#define num_columns 1011
#define num_elimination_rows 539
#define num_eliminated_rows 263

// ʹ��BitMagic���д洢��R����Ԫ�ӣ�E������Ԫ��
bm::bvector<> R[10000];  // R[i]��¼������Ϊi���±��0��ʼ��¼������Ԫ����
                               // ���Բ���ֱ����num_elimination_rows���������С
bm::bvector<> E[num_eliminated_rows];

// �����˹��ȥ������ʵ��
void solve() {
    // ѭ������ÿ������Ԫ��
    for (int i = 0; i < num_eliminated_rows; i++) {
        // �����ǰ����Ԫ��Ϊ�㣬��ֱ������
        if (E[i].none()) {
            continue;
        }

        // ѭ������ǰ����Ԫ�е�ÿһ��
        while (!E[i].none()) {
            // �ҵ���ǰ����Ԫ�е�����
            int k = num_columns - 1;
            while (E[i][k]==0 && k>=0) {
                k--;
            }
            // cout << "����" << k;

            // ��������Ӧ����Ԫ�Ӳ����ڣ�����С����񡱣�����ǰ�м������Ԫ�ӵļ�����
            if (!R[k].any()) {
                R[k] = E[i];
                // E[i].reset();
                // cout << "����";
                break;
            }
            // ��������Ӧ����Ԫ�Ӵ��ڣ��������ȥ����
            else {
                E[i] ^= R[k];
            }
        }
    }
}

void print() {
    for (int i=0; i<num_eliminated_rows; i++) {
        // cout << i << ':';
        for (int j=num_columns-1; j>=0; j--) {
            if (E[i][j] == 1) {
                cout << j << ' ';
            }
        }
        cout << endl;
    }
}

int main() {
    // ������Ԫ��
    ifstream file_R;
    char buffer[10000] = {0};
    // file_R.open("/home/data/Groebner/��������1 ��������130��������Ԫ��22������Ԫ��8/��Ԫ��.txt");
    file_R.open("D:/study/vscode/parallel/Parallel_Final_Research/�����˹/Groebner/��������4 ��������1011��������Ԫ��539������Ԫ��263/��Ԫ��.txt");
    if (file_R.fail()) {
        cout << "����ʧ��" << endl;
    }
    while (file_R.getline(buffer, sizeof(buffer))) {
        // ÿһ�ζ���һ�У���Ԫ��ÿ32λ��¼��һ��int��
        int bit;
        stringstream line(buffer);
        int first_in = 1;

        // ��Ԫ�ӵ�������������
        int index;
        while (line >> bit) {
            if (first_in) {
                first_in = 0;
                index = bit;
            }

            // ����index�е���Ԫ��bitset��Ӧλ ��1
            R[index][bit] = 1;
        }
    }
    file_R.close();
//--------------------------------
    // ���뱻��Ԫ��
    ifstream file_E;
    // file_E.open("/home/data/Groebner/��������1 ��������130��������Ԫ��22������Ԫ��8/����Ԫ��.txt");
    file_E.open("D:/study/vscode/parallel/Parallel_Final_Research/�����˹/Groebner/��������4 ��������1011��������Ԫ��539������Ԫ��263/����Ԫ��.txt");

    // ����Ԫ�е��������Ƕ��������
    int index = 0;
    while (file_E.getline(buffer, sizeof(buffer))) {
        // ÿһ�ζ���һ�У���Ԫ��ÿ32λ��¼��һ��int��
        int bit;
        stringstream line(buffer);
        while (line >> bit) {
            // ����index�е���Ԫ��bitset��Ӧλ ��1
            E[index][bit] = 1;
        }
        index++;
    }
//--------------------------------
    // ʹ��C++11��chrono������ʱ
    auto start = chrono::high_resolution_clock::now();
    solve();
    auto end = chrono::high_resolution_clock::now();
    auto diff = chrono::duration_cast<chrono::duration<double, milli>>(end - start);
    cout << diff.count() << "ms" << endl;
//--------------------------------
    // ��֤�����ȷ��
    // print();
    return 0;
}