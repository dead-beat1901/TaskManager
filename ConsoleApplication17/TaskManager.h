#pragma once
#include "DatabaseRepository.h"
#include <vector>
#include <algorithm>

class TaskManager {
private:
    DatabaseRepository repo;
    std::vector<TaskDTO> cache;

public:
    void Reload() {
        if (repo.Connect()) {
            cache = repo.LoadAllTasks();
            repo.Disconnect();
        }
    }

    void Sort() {
        std::sort(cache.begin(), cache.end(), [](const TaskDTO& a, const TaskDTO& b) {
            if (a.priority != b.priority) return a.priority < b.priority;
            return a.deadline < b.deadline;
            });
    }

    TaskDTO* GetData(int* count) {
        if (count) *count = (int)cache.size();
        return cache.data();
    }
};