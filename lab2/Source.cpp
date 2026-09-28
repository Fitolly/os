#include <windows.h>
#include <iostream>
#include <vector>
#include <climits>

struct SharedData {
    std::vector<int> arr;
    int minVal = INT_MAX;
    int maxVal = INT_MIN;
    int minIdx = -1;
    int maxIdx = -1;
    double average = 0.0;
};

SharedData g_data;

DWORD WINAPI MinMaxThread(LPVOID lpParam) {
    SharedData* data = (SharedData*)lpParam;
    size_t n = data->arr.size();

    for (size_t i = 0; i < n; ++i) {
        if (data->arr[i] < data->minVal) {
            data->minVal = data->arr[i];
            data->minIdx = (int)i;
        }
        Sleep(7);

        if (data->arr[i] > data->maxVal) {
            data->maxVal = data->arr[i];
            data->maxIdx = (int)i;
        }
        Sleep(7);
    }

    std::cout << "[min_max] Minimum element: " << data->minVal
        << " (index " << data->minIdx << ")\n";
    std::cout << "[min_max] Maximum element: " << data->maxVal
        << " (index " << data->maxIdx << ")\n";

    return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam) {
    SharedData* data = (SharedData*)lpParam;
    long long sum = 0;

    for (size_t i = 0; i < data->arr.size(); ++i) {
        sum += data->arr[i];
        Sleep(12);
    }

    data->average = (double)sum / data->arr.size();
    std::cout << "[average] Arithmetic mean: " << data->average << "\n";

    return 0;
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    int n;
    std::cout << "Enter array size: ";
    std::cin >> n;

    g_data.arr.resize(n);
    std::cout << "Enter " << n << " integers:\n";
    for (int i = 0; i < n; ++i) {
        std::cin >> g_data.arr[i];
    }

    HANDLE hMinMax = CreateThread(NULL, 0, MinMaxThread, &g_data, 0, NULL);
    HANDLE hAverage = CreateThread(NULL, 0, AverageThread, &g_data, 0, NULL);

    if (hMinMax == NULL || hAverage == NULL) {
        std::cerr << "Failed to create threads!\n";
        return 1;
    }

    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    if (g_data.minIdx != -1)
        g_data.arr[g_data.minIdx] = (int)g_data.average;
    if (g_data.maxIdx != -1 && g_data.maxIdx != g_data.minIdx)
        g_data.arr[g_data.maxIdx] = (int)g_data.average;

    std::cout << "\n[main] Array after replacing min and max with the mean ("
        << g_data.average << "):\n";
    for (int x : g_data.arr) std::cout << x << " ";
    std::cout << "\n";

    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    return 0;
}