#pragma once
#include "TaskManager.h"

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) void __cdecl InitManager();
	__declspec(dllexport) void __cdecl RefreshTasks();
	__declspec(dllexport) TaskDTO* __cdecl GetTasks(int* count);
	__declspec(dllexport) void __cdecl DisposeManager();
	__declspec(dllexport) void __cdecl AddTask(TaskDTO task);
	__declspec(dllexport) void __cdecl DeleteTask(int id);
	__declspec(dllexport) void __cdecl ChangeStatus(int id, int status);

#ifdef __cplusplus
}
#endif
