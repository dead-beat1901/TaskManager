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
    int id;
    char title[100];
    char categoryName[50];
    int priority;
    int statusId;
    long long deadline;
};