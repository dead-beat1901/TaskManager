#include "DatabaseRepository.h"
#include <algorithm>
#include <ctime>

using namespace std;

class TaskManager {
private:
    DatabaseRepository dbRepo;
    vector<TaskDTO> cachedTasks;

public:
    TaskManager() {
        dbRepo.Connect();
        ReloadTasks();
    }

    void ReloadTasks() {
        cachedTasks = dbRepo.LoadAllTasks();
    }


    void SortTasksSmart() {
        sort(cachedTasks.begin(), cachedTasks.end(), [](const TaskDTO& a, const TaskDTO& b) {

            if (a.Priority != b.Priority)
                return a.Priority < b.Priority;

            return a.Deadline < b.Deadline;
            });
    }

    void ChangeTaskStatus(int taskId, int newStatus) {

        dbRepo.UpdateStatus(taskId, newStatus);


        for (auto& t : cachedTasks) {
            if (t.Id == taskId) {
                t.StatusId = newStatus;
                break;
            }
        }
    }

    int CheckDeadlines() {
        time_t now = time(0);
        int overdueCount = 0;
        for (const auto& t : cachedTasks) {
            if (t.StatusId != Status_Done && t.Deadline < now) {
                overdueCount++;
            }
        }
        return overdueCount;
    }

    TaskDTO* GetTasksData(int* count) {
        *count = cachedTasks.size();
        return cachedTasks.data();
    }
};