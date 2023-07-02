#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>
#include <bitset>
#include <vector>
#include <nmmintrin.h>

using namespace std;

// MAXM:���������MAXN������
const int MAXM = 10000;
const int MAXN = 3799;

// ʹ��bitset���д洢��R����Ԫ�ӣ�E������Ԫ��
bitset<MAXN> R[MAXM];
bitset<MAXN> E[MAXM];

// ����Ԫ������
int m = 1953;

// �ҵ���ǰ����Ԫ�е�����
int lp(bitset<5> temp) {
    int k = 4;
    while (temp[k]==0 && k>=0) {
        k--;
    }
    return k;
}

// �����˹��ȥ������Neonʵ�֣�����ÿһ��ȡ5����Ԫ��/����Ԫ�г���
void solve() {
    int n = MAXN - 1;
    while (n >= 4) {
        // cout << "��ǰnΪ" << n << endl;
        // if (n==8) {
        //     cout << "start dbg";
        // }
        // ÿһ�ּ���n~n-4��5�е���Ԫ�ӣ�Ҳ����min_R = n-4, max_R = n
        // ʹ��λ���㣬������MAXN-n-1λ������Ҫ��5λ��Ϊ���λ��Ȼ��������MAXN-5λ���������λ
        bitset<5> R_temp[MAXM];
        bitset<5> E_temp[MAXM];
        for (int i=0; i<MAXM; i++) {
            R_temp[i] = bitset<5>(((R[i]<<(MAXN-n-1)) >> (MAXN-5)).to_ulong());
            E_temp[i] = bitset<5>(((E[i]<<(MAXN-n-1)) >> (MAXN-5)).to_ulong());
        }
        vector<pair<int, int>> records;
        // ��¼��5�е���Ԫ����
        // ��������Ԫ��
        for (int i=0; i<5; i++) {
            bool is_eliminated = 0;
            // ��Ԫ������¼�����������¼ʹ��vector<pair<int, int>>�洢����һ��int��¼����Ԫ�в������кţ��ڶ���int��¼�������ڵ��к�
            // ������Ԫ�ӵ���
            for (int j=4; j>=0; j--) {
                if (lp(E_temp[i]) == j) {
                    E_temp[i] ^= R_temp[j];
                    is_eliminated = 1;
                    records.emplace_back(i, j+n-4);  // ��¼
                }
            }
            if (!is_eliminated) {
                // ��Ϊ����������Ϊ��������ȥ
                if (!E_temp[i].none()) {
                    R[lp(E_temp[i])] = E[i];
                }
                break;
            }
        }
        // ������������n�н��в��м��㣬����records�еļ�¼���ж��̲߳��������ڸո�û�д��ȥ����������ʣ����n�У�
        // ÿ4��һ����в��л������ϴ�records��ȡ��
        for (auto pair : records) {
            int row = pair.first;
            int leader = pair.second;
            int m;
            for (m=n; m>=19; m-=20) {
                // ����Ԫ��
                bitset<5> a1_bit = bitset<5>(((E[row]<<(MAXN-m-1)) >> (MAXN-5)).to_ulong());
                bitset<5> a2_bit = bitset<5>(((E[row]<<(MAXN-(m-5)-1)) >> (MAXN-5)).to_ulong());
                bitset<5> a3_bit = bitset<5>(((E[row]<<(MAXN-(m-10)-1)) >> (MAXN-5)).to_ulong());
                bitset<5> a4_bit = bitset<5>(((E[row]<<(MAXN-(m-15)-1)) >> (MAXN-5)).to_ulong());
                // ��Ԫ��
                bitset<5> b1_bit = bitset<5>(((R[leader]<<(MAXN-m-1)) >> (MAXN-5)).to_ulong());
                bitset<5> b2_bit = bitset<5>(((R[leader]<<(MAXN-(m-5)-1)) >> (MAXN-5)).to_ulong());
                bitset<5> b3_bit = bitset<5>(((R[leader]<<(MAXN-(m-10)-1)) >> (MAXN-5)).to_ulong());
                bitset<5> b4_bit = bitset<5>(((R[leader]<<(MAXN-(m-15)-1)) >> (MAXN-5)).to_ulong());
                // �γ�����������neon����
                int a1 = a1_bit.to_ulong();
                int a2 = a2_bit.to_ulong();
                int a3 = a3_bit.to_ulong();
                int a4 = a4_bit.to_ulong();
                int b1 = b1_bit.to_ulong();
                int b2 = b2_bit.to_ulong();
                int b3 = b3_bit.to_ulong();
                int b4 = b4_bit.to_ulong();
                int arr_a[4] = {a1, a2, a3, a4};
                int arr_b[4] = {b1, b2, b3, b4};
                __m128i va = _mm_set_epi32(a4, a3, a2, a1);
                __m128i vb = _mm_set_epi32(b4, b3, b2, b1);
                va = _mm_xor_si128(va, vb);  // ���
                _mm_storeu_si128((__m128i*)arr_a, va);  // ���ȥ
                // �洢�ر���Ԫ�е�λͼ����arr_a[i]��5��λ������λ����
                // ���ѭ������arr_a���飬�ڴ�ѭ������arr_a[i]��λ��
                for (int i=0; i<4; i++) {
                    for (int j=0; j<5; j++) {
                        E[row].set(m-j, arr_a[i] & 0x1);
                        arr_a[i] >>= 1;
                    }
                }
            }
            // ʣ�µ�ֱ��һ��һ�����Ϳ����ˣ�ʹ������
            for (; m>=0; m--) {
                std::bitset<MAXN> mask;
                mask.reset();
                mask.set(m, 1);
                E[row] ^= (R[leader]&mask);
            }
        }
        n -= 5;
    }
    // ���n����5�ˣ�Ҳ���ü�¼��
    bitset<5> R_temp[MAXM];
    bitset<5> E_temp[MAXM];
    for (int i=0; i<MAXM; i++) {
        R_temp[i] = bitset<5>(((R[i]<<(MAXN-n-1)) >> (MAXN-n-1)).to_ulong());
        E_temp[i] = bitset<5>(((E[i]<<(MAXN-n-1)) >> (MAXN-n-1)).to_ulong());
    }
    // ��������Ԫ��
    for (int i=0; i<5; i++) {
        bool is_eliminated = 0;
        // ��Ԫ������¼�����������¼ʹ��vector<pair<int, int>>�洢����һ��int��¼����Ԫ�в������кţ��ڶ���int��¼�������ڵ��к�
        // ������Ԫ�ӵ���
        for (int j=4; j>=0; j--) {
            if (lp(E_temp[i]) == j) {
                E_temp[i] ^= R_temp[j];
                is_eliminated = 1;
            }
        }
        if (!is_eliminated) {
            // ��Ϊ����������Ϊ��������ȥ
            if (!E_temp[i].none()) {
                R[lp(E_temp[i])] = E[i];
            }
            break;
        }
    }
    // �洢��ȥ
    for (int i=0; i<MAXM; i++) {
        for (int j=0; j<n+1; j++) {
            E[i].set(j, E_temp[i][j]);
        }
    }
}

int main() {
    // ������Ԫ��
    ifstream file_R;
    char buffer[10000] = {0};
    // file_R.open("/home/data/Groebner/��������1 ��������130��������Ԫ��22������Ԫ��8/��Ԫ��.txt");
    file_R.open("D:/study/vscode/parallel/Parallel_Final_Research/�����˹/Groebner/��������4 ��������1011��������Ԫ��539������Ԫ��263/��Ԫ��.txt");
    if (file_R.fail()) {
        cout << "wow" << endl;
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
    return 0;
}