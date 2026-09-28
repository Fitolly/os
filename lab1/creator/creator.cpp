#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "employee.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        cerr << "Usage: Creator <file_name> <record_count>" << endl;
        return 1;
    }

    const char* fileName = argv[1];
    int         count = atoi(argv[2]);

    if (count <= 0) {
        cerr << "Record count must be positive." << endl;
        return 1;
    }

    ofstream out(fileName, ios::binary);
    if (!out) {
        cerr << "Cannot create file: " << fileName << endl;
        return 1;
    }

    employee emp;
    for (int i = 0; i < count; ++i) {
        cout << "\nRecord #" << (i + 1) << endl;
        cout << "  Employee number: ";
        cin >> emp.num;
        cout << "  Employee name:   ";
        cin >> emp.name;
        cout << "  Hours worked:    ";
        cin >> emp.hours;

        out.write(reinterpret_cast<char*>(&emp), sizeof(employee));
    }

    out.close();
    cout << "\nFile \"" << fileName << "\" created (" << count << " records)." << endl;
    return 0;
}