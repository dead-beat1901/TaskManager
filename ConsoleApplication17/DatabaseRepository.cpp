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

std::vector<TaskDTO> DatabaseRepository::LoadAllTasks() {
    std::vector<TaskDTO> tasks;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) return tasks;

    const wchar_t* sql = L"SELECT t.Id, t.Title, c.Name, t.Priority, t.StatusId, DATEDIFF(s, '1970-01-01', t.Deadline) "
        L"FROM Tasks t JOIN Categories c ON t.CategoryId = c.Id";

    if (SQLExecDirectW(hStmt, (SQLWCHAR*)sql, SQL_NTS) == SQL_SUCCESS) {
        while (SQLFetch(hStmt) == SQL_SUCCESS) {
            TaskDTO t;
            SQLLEN ind;
            SQLGetData(hStmt, 1, SQL_C_LONG, &t.id, 0, &ind);
            SQLGetData(hStmt, 2, SQL_C_CHAR, t.title, sizeof(t.title), &ind);
            SQLGetData(hStmt, 3, SQL_C_CHAR, t.categoryName, sizeof(t.categoryName), &ind);
            SQLGetData(hStmt, 4, SQL_C_LONG, &t.priority, 0, &ind);
            SQLGetData(hStmt, 5, SQL_C_LONG, &t.statusId, 0, &ind);

            long long ts;
            SQLGetData(hStmt, 6, SQL_C_SBIGINT, &ts, 0, &ind);
            t.deadline = ts;
            tasks.push_back(t);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    hStmt = NULL;
    return tasks;
}

void DatabaseRepository::UpdateStatus(int taskId, int newStatus) {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    const wchar_t* sql = L"UPDATE Tasks SET StatusId = ? WHERE Id = ?";
    SQLPrepareW(hStmt, (SQLWCHAR*)sql, SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &newStatus, 0, NULL);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &taskId, 0, NULL);
    SQLExecute(hStmt);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    hStmt = NULL;
}