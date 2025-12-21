#include "DatabaseRepository.h"

DatabaseRepository::DatabaseRepository() {
    hEnv = SQL_NULL_HENV; hDbc = SQL_NULL_HDBC; hStmt = SQL_NULL_HSTMT;

    connectionString = L"Driver={SQL Server};Server=DESKTOP-AS74V1H\\SQLEXPRESS;Database=TaskManager;Trusted_Connection=True;";
}

DatabaseRepository::~DatabaseRepository() {
    Disconnect();
}

bool DatabaseRepository::Connect() {
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) return false;
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) return false;

    SQLWCHAR outConn[1024];
    SQLSMALLINT outLen;
    SQLRETURN ret = SQLDriverConnectW(hDbc, NULL, (SQLWCHAR*)connectionString.c_str(), SQL_NTS, outConn, 1024, &outLen, SQL_DRIVER_NOPROMPT);

    return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
}

void DatabaseRepository::Disconnect() {
    if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    if (hDbc) { SQLDisconnect(hDbc); SQLFreeHandle(SQL_HANDLE_DBC, hDbc); }
    if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

vector<TaskDTO> DatabaseRepository::LoadAllTasks() {
    vector<TaskDTO> tasks;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) return tasks;

    const wchar_t* sql = L"SELECT t.Id, t.Title, c.Name, t.Priority, t.StatusId, DATEDIFF(s, '1970-01-01', t.Deadline) "
        L"FROM Tasks t JOIN Categories c ON t.CategoryId = c.Id";

    if (SQLExecDirectW(hStmt, (SQLWCHAR*)sql, SQL_NTS) == SQL_SUCCESS) {
        while (SQLFetch(hStmt) == SQL_SUCCESS) {
            TaskDTO t;
            SQLLEN ind;

            SQLGetData(hStmt, 1, SQL_C_LONG, &t.Id, 0, &ind);

            SQLGetData(hStmt, 2, SQL_C_WCHAR, t.Title, sizeof(t.Title), &ind);

            SQLGetData(hStmt, 3, SQL_C_WCHAR, t.CategoryName, sizeof(t.CategoryName), &ind);

            SQLGetData(hStmt, 4, SQL_C_LONG, &t.Priority, 0, &ind);

            SQLGetData(hStmt, 5, SQL_C_LONG, &t.StatusId, 0, &ind);

            long long ts;
            SQLGetData(hStmt, 6, SQL_C_SBIGINT, &ts, 0, &ind);
            t.Deadline = ts;

            tasks.push_back(t);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    hStmt = NULL;
    return tasks;
}