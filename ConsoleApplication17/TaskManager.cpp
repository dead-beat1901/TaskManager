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
}

void TaskManager::AddTask(const TaskDTO& task) {
    repo.AddTask(task);
    cache.push_back(task);
}

void TaskManager::UpdateTask(const TaskDTO& task) {
    repo.UpdateTask(task);

    for (auto& t : cache) {
        if (t.Id == task.Id) {
            t = task;
            break;
        }
    }
}

void TaskManager::DeleteTask(int taskId) {
    repo.DeleteTask(taskId);

    cache.erase(
        remove_if(cache.begin(), cache.end(),
            [taskId](const TaskDTO& t) {
                return t.Id == taskId;
            }),
        cache.end()
    );
}

void TaskManager::ChangeTaskStatus(int taskId, int newStatus) {
    repo.UpdateStatus(taskId, newStatus);

    for (auto& t : cache) {
        if (t.Id == taskId) {
            t.StatusId = newStatus;
            break;
        }
    }
}

void TaskManager::SortSmart() {
    sort(cache.begin(), cache.end(), [](const TaskDTO& a, const TaskDTO& b) {
        if (a.Priority != b.Priority)
            return a.Priority > b.Priority;
        return a.Deadline < b.Deadline;
        });
}

int TaskManager::CheckDeadlines() {
    time_t now = time(nullptr);
    int overdue = 0;

    for (const auto& t : cache) {
        if (t.StatusId != Status_Done && t.Deadline < now)
            overdue++;
    }
    return overdue;
}

TaskDTO* TaskManager::GetData(int* count) {
    if (count)
        *count = static_cast<int>(cache.size());

    return cache.empty() ? nullptr : cache.data();
}

void TaskManager::Clear() {
    cache.clear();
}
