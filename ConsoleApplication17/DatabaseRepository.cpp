#include "DatabaseRepository.h"
#include <cwchar>
#include <cstdio>

using namespace std;

DatabaseRepository::DatabaseRepository() {}

DatabaseRepository::~DatabaseRepository()
{
    Disconnect();
}

bool DatabaseRepository::Connect()
{
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) return false;
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) return false;

    const wchar_t* conn =
        L"Driver={SQL Server};"
        L"Server=DESKTOP-AS74V1H\\SQLEXPRESS;"
        L"Database=TaskManager2;"
        L"Trusted_Connection=True;"
        L"TrustServerCertificate=True;";

    SQLRETURN ret = SQLDriverConnectW(hDbc, NULL, (SQLWCHAR*)conn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

void DatabaseRepository::Disconnect()
{
    if (hDbc) { SQLDisconnect(hDbc); SQLFreeHandle(SQL_HANDLE_DBC, hDbc); hDbc = SQL_NULL_HDBC; }
    if (hEnv) { SQLFreeHandle(SQL_HANDLE_ENV, hEnv); hEnv = SQL_NULL_HENV; }
}

vector<TaskDTO> DatabaseRepository::LoadAllTasks()
{
    vector<TaskDTO> tasks;
    SQLHSTMT stmt;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt) != SQL_SUCCESS) return tasks;

    const wchar_t* sql =
        L"SELECT t.Id, t.Title, t.CategoryId, c.Name, t.Priority, t.StatusId, "
        L"DATEDIFF(second, '1970-01-01', t.Deadline) "
        L"FROM Tasks t "
        L"JOIN Categories c ON t.CategoryId = c.Id";

    if (SQLExecDirectW(stmt, (SQLWCHAR*)sql, SQL_NTS) == SQL_SUCCESS)
    {
        while (SQLFetch(stmt) == SQL_SUCCESS)
        {
            TaskDTO t{};
            SQLLEN ind;
            SQLGetData(stmt, 1, SQL_C_LONG, &t.Id, 0, &ind);
            SQLGetData(stmt, 2, SQL_C_WCHAR, t.Title, sizeof(t.Title), &ind);
            SQLGetData(stmt, 3, SQL_C_LONG, &t.CategoryId, 0, &ind);
            SQLGetData(stmt, 4, SQL_C_WCHAR, t.CategoryName, sizeof(t.CategoryName), &ind);
            SQLGetData(stmt, 5, SQL_C_LONG, &t.Priority, 0, &ind);
            SQLGetData(stmt, 6, SQL_C_LONG, &t.StatusId, 0, &ind);
            SQLGetData(stmt, 7, SQL_C_SBIGINT, &t.Deadline, 0, &ind);
            tasks.push_back(t);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return tasks;
}

void DatabaseRepository::AddTask(const TaskDTO& task)
{
    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);
    wchar_t sql[512];
    swprintf_s(sql,
        L"INSERT INTO Tasks (Title, CategoryId, Priority, StatusId, Deadline) "
        L"VALUES (N'%ls', %d, %d, %d, DATEADD(second, %lld, '1970-01-01'))",
        task.Title, task.CategoryId, task.Priority, task.StatusId, task.Deadline);
    SQLExecDirectW(stmt, (SQLWCHAR*)sql, SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

void DatabaseRepository::UpdateTask(const TaskDTO& task)
{
    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);
    wchar_t sql[512];
    swprintf_s(sql,
        L"UPDATE Tasks SET Title=N'%ls', CategoryId=%d, Priority=%d, StatusId=%d, "
        L"Deadline=DATEADD(second, %lld, '1970-01-01') WHERE Id=%d",
        task.Title, task.CategoryId, task.Priority, task.StatusId, task.Deadline, task.Id);
    SQLExecDirectW(stmt, (SQLWCHAR*)sql, SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

void DatabaseRepository::DeleteTask(int taskId)
{
    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);
    wchar_t sql[128];
    swprintf_s(sql, L"DELETE FROM Tasks WHERE Id=%d", taskId);
    SQLExecDirectW(stmt, (SQLWCHAR*)sql, SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

void DatabaseRepository::UpdateStatus(int taskId, int newStatus)
{
    SQLHSTMT stmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt);
    wchar_t sql[128];
    swprintf_s(sql, L"UPDATE Tasks SET StatusId=%d WHERE Id=%d", newStatus, taskId);
    SQLExecDirectW(stmt, (SQLWCHAR*)sql, SQL_NTS);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

vector<CategoryDTO> DatabaseRepository::LoadCategories()
{
    vector<CategoryDTO> list;
    SQLHSTMT stmt;

    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &stmt) != SQL_SUCCESS)
        return list;

    const wchar_t* sql = L"SELECT Id, Name FROM Categories";

    if (SQLExecDirectW(stmt, (SQLWCHAR*)sql, SQL_NTS) == SQL_SUCCESS)
    {
        while (SQLFetch(stmt) == SQL_SUCCESS)
        {
            CategoryDTO c{};
            SQLLEN ind;

            SQLGetData(stmt, 1, SQL_C_LONG, &c.Id, 0, &ind);
            SQLGetData(stmt, 2, SQL_C_WCHAR, c.Name, sizeof(c.Name), &ind);

            list.push_back(c);
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return list;
}
