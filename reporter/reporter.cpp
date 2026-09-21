#include <windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "employee.h"

using namespace std;

bool compareByNum(const employee& a, const employee& b) {
    return a.num < b.num;
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        cerr << "Usage: Reporter <binary_file> <report_file> <pay_per_hour>" << endl;
        return 1;
    }

    const char* binFileName = argv[1];
    const char* reportFileName = argv[2];
    double      payPerHour = atof(argv[3]);

    ifstream in(binFileName, ios::binary);
    if (!in) {
        cerr << "Cannot open binary file: " << binFileName << endl;
        return 1;
    }

    vector<employee> employees;
    employee emp;
    while (in.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        employees.push_back(emp);
    }
    in.close();

    if (employees.empty()) {
        cerr << "Binary file is empty or corrupted." << endl;
        return 1;
    }

    sort(employees.begin(), employees.end(), compareByNum);

    ofstream out(reportFileName);
    if (!out) {
        cerr << "Cannot create report file: " << reportFileName << endl;
        return 1;
    }

    out << "Report for file \"" << binFileName << "\"\n";
    out << string(60, '-') << "\n";
    out << left
        << setw(10) << "Number"
        << setw(15) << "Name"
        << setw(10) << "Hours"
        << setw(12) << "Salary" << "\n";
    out << string(60, '-') << "\n";

    out << fixed << setprecision(2);
    for (const auto& e : employees) {
        double salary = e.hours * payPerHour;
        out << left
            << setw(10) << e.num
            << setw(15) << e.name
            << setw(10) << e.hours
            << setw(12) << salary << "\n";
    }

    out.close();
    cout << "Report \"" << reportFileName << "\" created." << endl;
    return 0;
}