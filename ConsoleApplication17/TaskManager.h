#pragma once
#include <vector>
#include <algorithm>
#include "Classes.h"
#include "DatabaseRepository.h"

class TaskManager {
private:
    std::vector<TaskDTO> cache;
    DatabaseRepository repo;

public:
    TaskManager() {}

    void Reload() {
        if (repo.Connect()) {
            cache = repo.LoadAllTasks();
            repo.Disconnect();
        }
    }

    void Sort() {
        if (cache.empty()) return;

        std::sort(cache.begin(), cache.end(), [](const TaskDTO& a, const TaskDTO& b) {
            return a.Priority < b.Priority;
            });
    }

    TaskDTO* GetData(int* count) {
        if (cache.empty()) {
            if (count) *count = 0;
            return nullptr;
        }

        if (count) {
            *count = static_cast<int>(cache.size());
        }

        return cache.data();
    }

    void Clear() {
        cache.clear();
    }
};