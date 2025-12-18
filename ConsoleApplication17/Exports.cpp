#include <windows.h>
#include "TaskManager.h"

TaskManager* g_manager = nullptr;

extern "C" {

    __declspec(dllexport) void __stdcall InitManager() {
        if (!g_manager)
            g_manager = new TaskManager();
    }

    __declspec(dllexport) void __stdcall RefreshTasks() {
        if (g_manager) {
            g_manager->Reload();
            g_manager->Sort();
        }
    }

    __declspec(dllexport) TaskDTO* __stdcall GetTasks(int* count) {
        if (!g_manager || !count) {
            if (count) *count = 0;
            return nullptr;
        }

        TaskDTO* data = g_manager->GetData(count);
        return data;
    }

    __declspec(dllexport) void __stdcall DisposeManager() {
        if (g_manager) {
            delete g_manager;
            g_manager = nullptr;
        }
    }
}
