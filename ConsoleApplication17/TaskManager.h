#pragma once
#include <vector>
#include "Classes.h"
#include "DatabaseRepository.h"

using namespace std;

class TaskManager {
private:
    vector<TaskDTO> cache;
    DatabaseRepository repo;

public:
    TaskManager();

    void Reload();

    void AddTask(const TaskDTO& task);
    void UpdateTask(const TaskDTO& task);
    void DeleteTask(int taskId);
    void ChangeTaskStatus(int taskId, int newStatus);

    void SortSmart();
    int CheckDeadlines();

    TaskDTO* GetData(int* count);
    void Clear();
};
