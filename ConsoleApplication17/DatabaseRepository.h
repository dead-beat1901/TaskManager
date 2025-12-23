#pragma once
#include <vector>
#include <string>
#include <windows.h>
#include <sqlext.h>
#include "Classes.h"

using namespace std;

class DatabaseRepository
{
private:
    SQLHENV hEnv = SQL_NULL_HENV;
    SQLHDBC hDbc = SQL_NULL_HDBC;
    wstring connectionString;

public:
    DatabaseRepository();
    ~DatabaseRepository();

    bool Connect();
    void Disconnect();

    vector<TaskDTO> LoadAllTasks();

    void AddTask(const TaskDTO& task);
    void DeleteTask(int taskId);
    void UpdateStatus(int taskId, int newStatus);
    void UpdateTask(const TaskDTO& task);

};
