#pragma once
#include <vector>
#include <string>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include "Classes.h"

using namespace std;

class DatabaseRepository {
private:
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    wstring connectionString;

public:
    DatabaseRepository();
    ~DatabaseRepository();

    bool Connect();
    void Disconnect();

    std::vector<TaskDTO> LoadAllTasks();
    void UpdateStatus(int taskId, int newStatus);
};