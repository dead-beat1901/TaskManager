#include <windows.h>
#include <cwchar>
#include "TaskManager.h"

TaskManager* g_manager = nullptr;

extern "C" {


    __declspec(dllexport) void __stdcall InitManager() {
        if (g_manager == nullptr) {
            g_manager = new TaskManager();
        }
    }


    __declspec(dllexport) void __stdcall RefreshTasks() {
        if (g_manager != nullptr) {
            g_manager->Reload();
            g_manager->Sort();


            int count = 0;
            TaskDTO* data = g_manager->GetData(&count);


            if (count > 0 && data != nullptr) {

                data[0].Id = 777;
                wcscpy_s(data[0].Title, 100, L"Связь с C++ работает!");
                wcscpy_s(data[0].CategoryName, 50, L"Отладка");
                data[0].Priority = 1;
            }
        }
    }

    __declspec(dllexport) TaskDTO* __stdcall GetTasks(int* count) {
        if (g_manager != nullptr) {
            return g_manager->GetData(count);
        }
        if (count) *count = 0;
        return nullptr;
    }

    __declspec(dllexport) void __stdcall DisposeManager() {
        if (g_manager != nullptr) {
            delete g_manager;
            g_manager = nullptr;
        }
    }
}

__declspec(dllexport) void __stdcall AddTask(const wchar_t* title, int categoryId, int priority) {
    if (g_manager) {

    }
}

__declspec(dllexport) void __stdcall DeleteTask(int taskId) {
    if (g_manager) {

    }
}