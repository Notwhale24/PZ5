#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>

using namespace std;

string getCurrentTimeString() {
    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);

    tm localTime{};
    localtime_s(&localTime, &t);  // Безопасно для Windows

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H-%M", &localTime);
    return string(buffer);
}

class MyArray {
protected:
    int* data;
    int size;

public:
    MyArray(int n) : size(n), data(new int[n]{}) {
        for (int i = 0; i < size; i++)
            data[i] = 0;
    }

    MyArray(const MyArray& other) : size(other.size), data(new int[size]) {
        copy(other.data, other.data + size, data);
    }

    virtual ~MyArray() {
        delete[] data;
    }

    void setValue(int index, int value) {
        if (index < 0 || index >= size) {
            cout << "Error in the index" << endl;
            return;
        }
        if (value < -100 || value > 100) {
            cout << "Error in the value" << endl;
            return;
        }
        data[index] = value;
    }

    int getValue(int index) const {
        if (index < 0 || index >= size) {
            cout << "Index error" << endl;
            return 0;
        }
        return data[index];
    }

    virtual void print() const {
        for (int i = 0; i < size; i++)
            cout << data[i] << (i + 1 < size ? " " : "");
        cout << endl;
    }

    void addValue(int value) {
        if (value < -100 || value > 100) {
            cout << "Value error" << endl;
            return;
        }
        int* newData = new int[size + 1];
        copy(data, data + size, newData);
        newData[size] = value;
        delete[] data;
        data = newData;
        ++size;
    }

    void add(const MyArray& other) {
        int minSize = min(size, other.size);
        for (int i = 0; i < minSize; i++) {
            int sum = data[i] + other.data[i];
            if (sum < -100 || sum > 100) {
                cout << "Overflow range error at index " << i << endl;
                continue;
            }
            data[i] = sum;
        }
    }

    void sub(const MyArray& other) {
        int minSize = min(size, other.size);
        for (int i = 0; i < minSize; i++) {
            int diff = data[i] - other.data[i];
            if (diff < -100 || diff > 100) {
                cout << "Overflow range error at index " << i << endl;
                continue;
            }
            data[i] = diff;
        }
    }
};

class ArrTxt : public MyArray {
public:
    using MyArray::MyArray;

    void print() const override {
        string filename = getCurrentTimeString() + ".txt";
        ofstream fout(filename);
        if (!fout.is_open()) {
            cout << "File recording error" << endl;
            return;
        }

        fout << size << "\n";
        fout << data[0];
        for (int i = 1; i < size; i++)
            fout << " " << data[i];
        fout << "\n";

        fout.close();
        cout << "The data is saved to a file: " << filename << endl;
    }
};

class ArrCSV : public MyArray {
public:
    using MyArray::MyArray;

    void print() const override {
        string filename = getCurrentTimeString() + ".csv";
        ofstream fout(filename);
        if (!fout.is_open()) {
            cout << "File recording error" << endl;
            return;
        }

        fout << size << "\n";
        fout << data[0];
        for (int i = 1; i < size; i++)
            fout << ";" << data[i];
        fout << "\n";

        fout.close();
        cout << "The data is saved to a file: " << filename << endl;
    }
};

int main() {
    system("chcp 65001");

    MyArray* arr[2];
    arr[0] = new ArrTxt(20);
    arr[1] = new ArrCSV(20);

    for (int i = 0; i < 20; i++) {
        arr[0]->setValue(i, i + 1);
        arr[1]->setValue(i, i + 1);
    }

    for (int i = 0; i < 2; i++) {
        arr[i]->print();
        delete arr[i];
    }

    return 0;
}

