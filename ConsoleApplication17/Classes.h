#pragma once
#include <cstdint>

enum TaskStatus {
    Status_New = 1,
    Status_InProgress = 2,
    Status_Done = 3
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