#include "TaskManager.h"
#include <algorithm>
#include <ctime>

using namespace std;

TaskManager::TaskManager() {
    repo.Connect();
    Reload();
}

void TaskManager::Reload() {
    cache = repo.LoadAllTasks();
    SortSmart(); // —ортируем сразу при загрузке
}

void TaskManager::AddTask(const TaskDTO& task) {
    repo.AddTask(task);
    Reload(); // ѕерезагружаем полностью, чтобы получить верный ID и сортировку
}

void TaskManager::UpdateTask(const TaskDTO& task) {
    repo.UpdateTask(task);
    Reload();
}

void TaskManager::DeleteTask(int taskId) {
    repo.DeleteTask(taskId);
    Reload();
}

void TaskManager::ChangeTaskStatus(int taskId, int newStatus) {
    repo.UpdateStatus(taskId, newStatus);
    Reload();
}

void TaskManager::SortSmart() {
    sort(cache.begin(), cache.end(), [](const TaskDTO& a, const TaskDTO& b) {
        if (a.Priority != b.Priority)
            return a.Priority < b.Priority;
        return a.Deadline < b.Deadline;
        });
}

TaskDTO* TaskManager::GetData(int* count) {
    if (count) *count = static_cast<int>(cache.size());
    return cache.empty() ? nullptr : cache.data();
}