#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>
#include "employee.h"

using namespace std;

const wchar_t* CREATOR_EXE = L"Creator.exe";
const wchar_t* REPORTER_EXE = L"Reporter.exe";

bool runProcess(const wstring& commandLine, const wstring& processName)
{
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    wstring cmd = commandLine;

    if (!CreateProcessW(NULL, &cmd[0], NULL, NULL, FALSE, 0,
        NULL, NULL, &si, &pi))
    {
        cerr << "Cannot start process " << processName.c_str()
            << ". Error code: " << GetLastError() << endl;
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (exitCode != 0) {
        cerr << "Process " << processName.c_str()
            << " exited with code " << exitCode << endl;
        return false;
    }
    return true;
}

void printBinaryFile(const string& fileName)
{
    ifstream in(fileName, ios::binary);
    if (!in) {
        cerr << "Cannot open binary file: " << fileName << endl;
        return;
    }

    cout << "\n--- Contents of binary file \"" << fileName << "\" ---\n";
    cout << left
        << setw(10) << "Number"
        << setw(15) << "Name"
        << setw(10) << "Hours" << "\n";
    cout << string(35, '-') << "\n";

    employee emp;
    while (in.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        cout << left
            << setw(10) << emp.num
            << setw(15) << emp.name
            << setw(10) << emp.hours << "\n";
    }
    cout << string(35, '-') << "\n";
    in.close();
}

void printTextFile(const string& fileName)
{
    ifstream in(fileName);
    if (!in) {
        cerr << "Cannot open report file: " << fileName << endl;
        return;
    }

    cout << "\n--- Contents of report \"" << fileName << "\" ---\n";
    string line;
    while (getline(in, line)) {
        cout << line << "\n";
    }
    cout << string(60, '-') << "\n";
    in.close();
}

int main()
{
    string binFileName;
    int    recordCount;

    cout << "Enter binary file name: ";
    cin >> binFileName;

    cout << "Enter record count: ";
    cin >> recordCount;

    wstring creatorCmd = wstring(CREATOR_EXE) + L" " +
        wstring(binFileName.begin(), binFileName.end()) + L" " +
        to_wstring(recordCount);

    cout << "\nStarting Creator...\n";
    if (!runProcess(creatorCmd, L"Creator"))
        return 1;

    printBinaryFile(binFileName);

    string reportFileName;
    double payPerHour;

    cout << "\nEnter report file name: ";
    cin >> reportFileName;

    cout << "Enter pay per hour: ";
    cin >> payPerHour;

    wstring reporterCmd = wstring(REPORTER_EXE) + L" " +
        wstring(binFileName.begin(), binFileName.end()) + L" " +
        wstring(reportFileName.begin(), reportFileName.end()) + L" " +
        to_wstring(payPerHour);

    cout << "\nStarting Reporter...\n";
    if (!runProcess(reporterCmd, L"Reporter"))
        return 1;

    printTextFile(reportFileName);

    cout << "\nMain program finished.\n";
    return 0;
}