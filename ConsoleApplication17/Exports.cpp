#include "Exports.h"
#include "DatabaseRepository.h"

static DatabaseRepository repo;
static vector<TaskDTO> cache;

void __cdecl InitManager()
{
    repo.Connect();
}

void __cdecl RefreshTasks()
{
    cache = repo.LoadAllTasks();
}

TaskDTO* __cdecl GetTasks(int* count)
{
    if (count)
        *count = (int)cache.size();

    return cache.empty() ? nullptr : cache.data();
}

void __cdecl DisposeManager()
{
    repo.Disconnect();
}

void __cdecl AddTask(TaskDTO task)
{
    repo.AddTask(task);
}

void __cdecl DeleteTask(int id)
{
    repo.DeleteTask(id);
}

void __cdecl ChangeStatus(int id, int status)
{
    repo.UpdateStatus(id, status);
}
