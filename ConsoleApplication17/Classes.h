#pragma once
#include <ctime>
#include <string>

enum TaskStatus {
    Status_New = 1,
    Status_InProgress = 2,
    Status_Done = 3
};

enum TaskPriority {
    Priority_High = 1,
    Priority_Medium = 2,
    Priority_Low = 3
};

struct TaskDTO {
    int Id;
    wchar_t Title[256];
    int CategoryId;
    wchar_t CategoryName[128];
    int Priority;
    int StatusId;
    long long Deadline;
};