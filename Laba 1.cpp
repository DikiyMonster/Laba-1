#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>

using namespace std;
using namespace chrono;

// ��������� ����� � ������������ ������� �� 1 �� N
void generateFile(const string& filename, int N)
{
    ofstream file(filename);
    for (int i = 1; i <= N; ++i)
    {
        file << i << "\n";
        if (i % 10000 == 0) {
            file.flush();
        }
    }
}



// ���������������� ��������� ��������� ����� (��������� �� �����) � ������ � ������ ����
void sequentialProcessing(const string& filename, int multiplier)
{
    ifstream file(filename);
    int num;

    while (file >> num)
    {
        int result = num * multiplier;
    }
}

// ������������� ��������� ��������� �����(���������� � �������)
void parallelProcessing(const string& filename,int exponent, int numThreads)
{
    ifstream file(filename);
    vector<thread> threads;
    vector<int> nums;
    mutex myMutex;

    // ������ ��������� �� �����
    int num;
    while (file >> num)
    {
        nums.push_back(num);
    }

    // �������, ������� ��������� ������ �����
    auto threadFunction = [&](int start, int end)
        {
            for (int i = start; i < end; ++i)
            {
                {
                    lock_guard<mutex> lock(myMutex);
                    int result = pow(nums[i], exponent);
                }
            }
        };


    // ���������� ��������� ������� ����� ��������
    int chunkSize = nums.size() / numThreads;
    int remaining = nums.size() % numThreads;
    int start = 0;
    int end = 0;

    for (int i = 0; i < numThreads; ++i)
    {
        start = end;
        end = start + chunkSize;

        if (remaining > 0)
        {
            end++;
            remaining--;
        }

        threads.emplace_back(threadFunction, start, end);
    }

    // �������� ���������� ������ ���� �������
    for (auto& thread : threads)
    {
        thread.join();
    }

    file.close();
}

int main()
{
    setlocale(LC_ALL, "Russian");

    // ����� �������� N
    int N;
    cout << "������� �������� N: ";
    cin >> N;

    // ��������� ����� � ������������ ������� �� 1 �� N
    string filename = "numbers.txt";
    generateFile(filename, N);

    // ������� �������� ��� ���������
    int multiplier;
    cout << "������� ���������: ";
    cin >> multiplier;

    int exponent;
    cout << "������� �������: ";
    cin >> exponent;

    // ���������������� ��������� ��������� ����� � ������ � ������ ����
    sequentialProcessing(filename, multiplier);

    // ������� ���������� ������� ��� ������������ ���������
    int numThreads;
    cout << "������� ���������� ������� ��� ������������ ���������: ";
    cin >> numThreads;

    // ����� ������� ������������� ���������
    auto start = high_resolution_clock::now();

    // ������������� ��������� ��������� ����� � ������ � ������ ����
    parallelProcessing(filename,exponent, numThreads);
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);
    cout << "����� ������������� ���������: " << duration.count() << " ��" << endl;

    return 0;
}
