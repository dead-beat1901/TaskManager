#pragma once
#include "Classes.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) void __cdecl InitManager();
	__declspec(dllexport) void __cdecl RefreshTasks();
	__declspec(dllexport) TaskDTO* __cdecl GetTasks(int* count);
	__declspec(dllexport) void __cdecl DisposeManager();

	__declspec(dllexport) void __cdecl AddTask(TaskDTO task);
	__declspec(dllexport) void __cdecl UpdateTask(TaskDTO task);
	__declspec(dllexport) void __cdecl DeleteTask(int id);
	__declspec(dllexport) void __cdecl ChangeStatus(int id, int status);

	__declspec(dllexport) CategoryDTO* __cdecl GetCategories(int* count);

#ifdef __cplusplus
}
#endif
