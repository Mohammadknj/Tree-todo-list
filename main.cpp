#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int currentYear, currentMonth, currentDay;
int monthsDays[] = {0, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};
int leftDaysCalculator(int year, int month, int day) {
    int result = 0;
    if (year > currentYear + 1) {
        result += (year - (currentYear + 1)) * 365;
        year = currentYear + 1;
    }
    while (year > currentYear || month > currentMonth) {
        result += monthsDays[--month];
        if (month == 0) {
            month = 13;
            year--;
        }
    }
    result += day - currentDay;
    return result;
}
// if it's 0 means wrong command
// if it's 1 means task command
// if it's 2 means subtask command
int isWrongOrTaskOrSubtask = 1;
bool checkCorrectAddingCommand(int i, string str) {
    if (isWrongOrTaskOrSubtask == 1) {
        string s = "add task ";
        for (int j = 0; j < i; ++j) {
            if (s[j] != str[j]) {
                isWrongOrTaskOrSubtask = 0;
                return false;
            }
        }
    } else {
        string s = "add subtask ";
        for (int j = 0; j < i; ++j) {
            if (s[j] != str[j]) {
                isWrongOrTaskOrSubtask = 0;
                return false;
            }
        }
    }
    return true;
}
vector<string> addTaskExtractInformations(string str) {
    vector<string> result;
    string word;
    int i = 9;
    if (isWrongOrTaskOrSubtask == 2) {
        i += 3;
    }
    if (!checkCorrectAddingCommand(i, str)) {
        cout << "Error! You Entered a wrong command\n";
        return {};
    }
    while (i < int(str.length())) {
        if (str[i] == ',') {
            result.push_back(word);
            word.clear();
            i += 2;
            continue;
        }
        word += str[i];
        i++;
    }
    result.push_back(word);
    return result;
}
// Predefinition
class Task;

class Deadline {
public:
    int year, month, day, leftDays, hour, minute;
    string date;
    void makeDeadline(string str) {
        int arr[] = {0, 0, 0};
        int i = 0, j = 0;
        while (str[i] != ' ') {
            if (str[i] == '/') {
                i++;
                arr[j] /= 10;
                j++;
                continue;
            }
            arr[j] += str[i] - 48;
            arr[j] *= 10;
            i++;
        }
        arr[2] /= 10;
        i++;
        j = 0;
        while (str[i] != ':') {
            j += str[i] - 48;
            j *= 10;
            i++;
        }
        hour = j / 10;
        i++;
        minute = (str[i++] - 48) * 10 + (str[i] - 48);
        year = arr[0];
        month = arr[1];
        day = arr[2];
        leftDays = leftDaysCalculator(year, month, day);
        string min = minute < 10 ? "0" + to_string(minute) : to_string(minute);
        string hr = hour < 10 ? "0" + to_string(hour) : to_string(hour);
        hr += ":";
        date = to_string(year) + '/' + to_string(month) + '/' + to_string(day) +
               " " + hr + min;
    }
};
class Subtask {
public:
    string name, description, status, taskName;
    Subtask *next;
    Subtask *previous;
    Task *parent;
    Deadline deadline;
    Subtask() {
        string str;
        cout << "Enter subtask command: ";
        cin.ignore();
        getline(cin, str);
        vector<string> ss = addTaskExtractInformations(str);
        if (isWrongOrTaskOrSubtask == 0)
            return;
        name = ss[0];
        description = ss[1];
        status = ss[3];
        taskName = ss[4];
        next = nullptr;
        previous = nullptr;
        parent = nullptr;
        deadline.makeDeadline(ss[2]);
    }
};
class Linklist;
class Linklist {
public:
    Subtask *head;
    Subtask *tail;
    int size;
    Linklist() {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }
    void insert(Subtask *sub, Task *T) {
        sub->parent = T;
        sub->previous = head;
        if (head == nullptr) {
            head = sub;
            tail = sub;
        } else {
            // Head is the new node
            head->next = sub;
            head = sub;
        }
        size++;
    }
    void delNode(Subtask *subtask) {
        if (subtask == nullptr)
            return;
        // Checking for tail node
        if (subtask->previous != nullptr)
            subtask->previous->next = subtask->next;
        else
            tail = subtask->next;
        // Checking for head node
        if (subtask->next != nullptr)
            subtask->next->previous = subtask->previous;
        else
            head = subtask->previous;
        size--;
        delete subtask;
        subtask = nullptr;
    }
    void printList(string space, string taskName) {
        if (head == nullptr) {
            cout << space << "    There is not any subtask for " << taskName << endl;
            return;
        }
        Subtask *sub = tail;
        while (sub) {
            cout << space << "    Name: " << sub->name
                 << ", Description: " << sub->description
                 << ", Staus: " << sub->status << ", " << sub->taskName << endl;
            sub = sub->next;
        }
    }
};
class Task {
public:
    string name, description, status;
    Task *parent;
    Task *rightSibling;
    Subtask *leftChild;
    Linklist subtasks;
    Deadline deadline;
    Task() {
        parent = nullptr;
        rightSibling = nullptr;
        leftChild = nullptr;
        string str;
        cout << "Enter task command: ";
        cin.ignore();
        getline(cin, str);
        vector<string> ss = addTaskExtractInformations(str);
        if (isWrongOrTaskOrSubtask == 0)
            return;
        name = ss[0];
        description = ss[1];
        status = ss[3];
        deadline.makeDeadline(ss[2]);
    }
    void addSubtask(Subtask *sub, Task *T) {
        subtasks.insert(sub, T);
        if (T->subtasks.size == 1)
            T->leftChild = sub;
        sub->parent = T;
    }
    void deleteSubtasks() {
        Subtask *sub = leftChild, *subNxt = nullptr;
        while (sub) {
            subNxt = sub->next;
            subtasks.delNode(sub);
            sub = subNxt;
        }
    }
    // Another way of deleting subtasks
    // void deleteSubtasks1(){
    //     Subtask*sub=leftChild,*preSub=nullptr;
    //     while(sub){
    //         preSub=sub;
    //         sub=sub->next;
    //         delete preSub;
    //         preSub=nullptr;
    //         // sub->previous = nullptr;
    //     }
    //     subtasks.size=0;
    //     subtasks.head=nullptr;
    //     subtasks.tail=nullptr;
    // }
};
// Returns task with closer deadline
Task *closerTask(Task *T, Task *T1) {
    if (T->deadline.leftDays == T1->deadline.leftDays) {
        if (T->deadline.hour > T1->deadline.hour)
            return T1;
        else if (T->deadline.hour < T1->deadline.hour)
            return T;
        else
            return T->deadline.minute > T1->deadline.minute ? T1 : T;
    }
    if (T->deadline.leftDays > T1->deadline.leftDays)
        return T1;
    return T;
}
class TreeRoot {
public:
    Task *itself;
    Task *leftChild;
    int sizeOfTasks;
    TreeRoot() {
        itself = nullptr;
        leftChild = nullptr;
        sizeOfTasks = 0;
    }
    void addTask(Task *T) {
        T->parent = itself;
        sizeOfTasks++;
        if (leftChild == nullptr) {
            leftChild = T;
        } else {
            // Sorts when size is more than 1
            SortTasks(T);
        }
    }
    void deleteTask(Task *T) {
        if (T == leftChild) {
            leftChild = T->rightSibling;
        } else {
            // finding previous task and connecting previous to next task
            Task *preTask = leftChild;
            while (preTask->rightSibling != T)
                preTask = preTask->rightSibling;
            preTask->rightSibling = preTask->rightSibling->rightSibling;
        }
        delete T;
        T = nullptr;
        sizeOfTasks--;
        cout << "Task deleted successfully\n";
    }
    void SortTasks(Task *T) {
        Task *t = leftChild, *tPre;
        while (t != nullptr) {
            // finding task that current task is closer than it
            if (T == closerTask(t, T)) {
                insert(t, T);
                return;
            }
            tPre = t;
            t = t->rightSibling;
        }
        // Setting T in the right of last node. Now T is the last node
        tPre->rightSibling = T;
    }
    // Tlast will be the right task of Tnew
    void insert(Task *Tlast, Task *Tnew) {
        Tnew->rightSibling = Tlast;
        if (Tlast == leftChild) {
            leftChild = Tnew;
            return;
        }
        // finding left of Tlast
        Task *t = leftChild;
        while (t->rightSibling != Tlast)
            t = t->rightSibling;
        t->rightSibling = Tnew;
    }
};
// if all subtasks are done, task will delete
void checkDeletingTask(TreeRoot *Root, Task *T) {
    Subtask *sub = T->leftChild;
    while (sub) {
        if (sub->status != "done")
            return;
        sub = sub->next;
    }
    T->deleteSubtasks();
    Root->deleteTask(T);
}
vector<string> extractDeleteInformations(string str) {
    vector<string> result;
    string word;
    int i = 0;
    while (i < int(str.length())) {
        if (str[i] == ',') {
            result.push_back(word);
            word.clear();
            i += 2;
            continue;
        }
        word += str[i];
        i++;
    }
    result.push_back(word);
    return result;
}
void ChangingSubtaskStatus(TreeRoot *Root) {
    cout << "Enter your command: ";
    string str;
    cin.ignore();
    getline(cin, str);
    vector<string> ss = extractDeleteInformations(str);
    if (ss[2] != "done" && ss[2] != "to do") {
        cout << "Status should change to done or to do" << endl;
        ChangingSubtaskStatus(Root);
        return;
    }
    Task *T = Root->leftChild;
    while (T) {
        if (T->name == ss[0]) {
            Subtask *sub = T->leftChild;
            while (sub) {
                if (sub->name == ss[1]) {
                    sub->status = ss[2];
                    cout << "Status changed\n";
                    checkDeletingTask(Root, T);
                    return;
                }
                sub = sub->next;
            }
            cout << "Subtask wasn't found\n";
            return;
        }
        T = T->rightSibling;
    }
    cout << "Task wasn't found\n";
}
void showCloserTask(TreeRoot *Root) {
    Task *T = Root->leftChild;
    if (T == nullptr) {
        cout << "There isn't any tasks\n";
        return;
    }
    cout << "Task:\n    Name: " << T->name << ", Description: " << T->description
         << ", Status: " << T->status << endl;
    cout << "    It's " << T->deadline.leftDays
         << " days left. Deadline: " << T->deadline.date << endl;
    if (T->subtasks.size == 0)
        return;
    cout << "Subtasks:\n";
    Subtask *sub = T->subtasks.tail;
    while (sub != nullptr) {
        cout << "    " << sub->name << ", " << sub->description << ", "
             << sub->status << ", " << sub->deadline.date << endl;
        sub = sub->next;
    }
}
void showAllTasks(TreeRoot *Root) {
    Task *T = Root->leftChild;
    if (T == nullptr) {
        cout << "There isn't any tasks\n";
        return;
    }
    while (T) {
        cout << "Task:\n    Name: " << T->name
             << ", Description: " << T->description << ", Status: " << T->status
             << endl;
        cout << "    It's " << T->deadline.leftDays
             << " days left. Deadline: " << T->deadline.date << endl;
        T = T->rightSibling;
    }
}
void saveInTXTFile(TreeRoot *Root) {
    if(Root==nullptr){
        cout<<"There isn't any roots\n";
        return;
    }
    ofstream file("TreeDSproject.txt");
    if (file.is_open()) {
        file.clear();
        file << "Root: " << endl;
        file << "Name: " << Root->itself->name
             << ", Description: " << Root->itself->description
             << ", Status: " << Root->itself->status
             << ", Deadline: " << Root->itself->deadline.date << endl
             << endl;
        Task *T = Root->leftChild;
        while (T) {
            file << "Name: " << T->name << ", Description: " << T->description
                 << ", Status: " << T->status << ", Deadline: " << T->deadline.date
                 << endl;
            file << "Subtasks: ";
            if (T->leftChild == nullptr)
                file << "There isn't any subtasks\n" << endl;
            else {
                file << endl;
                Subtask *sub = T->leftChild;
                while (sub) {
                    file << "Name: " << sub->name << ", Description: " << sub->description
                         << ", Status: " << sub->status
                         << ", Deadline: " << sub->deadline.date << endl;
                    sub = sub->next;
                }
                file << endl;
            }
            T = T->rightSibling;
        }
        file.close();
        cout << "Tree added successfully!" << endl;
    } else
        cout << "Unable to open file!\n";
}
void retrieveFromTXTfile() {
    ifstream file("TreeDSproject.txt");
    if (file.is_open()) {
        string s;
        while (file) {
            getline(file, s);
            cout << endl << s;
        }
        file.close();
    } else
        cout << "Unable to open file\n";
}
void BFS(TreeRoot *Root) {
    cout << "\nRoot: " << endl;
    cout << "    Name: " << Root->itself->name
         << ", Description: " << Root->itself->description
         << ", Status: " << Root->itself->status << endl;
    cout << "Tasks: " << endl;
    Task *T = Root->leftChild;
    if (T == nullptr) {
        cout << "There isn't any tasks\n";
        return;
    }
    while (T) {
        cout << "    Name: " << T->name << ", Description: " << T->description
             << ", Status: " << T->status << endl;
        T = T->rightSibling;
    }
    cout << "Subtasks: " << endl;
    T = Root->leftChild;
    while (T) {
        T->subtasks.printList("", T->name);
        T = T->rightSibling;
    }
    cout << endl;
}
void DFS(TreeRoot *Root) {
    cout << "\nRoot: " << endl;
    cout << "    Name: " << Root->itself->name
         << ", Description: " << Root->itself->description
         << ", Status: " << Root->itself->status << endl;
    Task *T = Root->leftChild;
    if (T == nullptr) {
        cout << "There isn't any tasks\n\n";
        return;
    }
    while (T) {
        cout << endl;
        cout << "\tName: " << T->name << ", Description: " << T->description
             << ", Status: " << T->status << endl;
        cout << "\tSubtasks:\n";
        T->subtasks.printList("\t", T->name);
        T = T->rightSibling;
    }
}
void menu(TreeRoot *Root) {
    int choice;
    do {
        cout << "menu:" << endl;
        cout << "1.Add task" << endl;
        cout << "2.Add subtask" << endl;
        cout << "3.Change subtask status" << endl;
        cout << "4.Show task with higher priority" << endl;
        cout << "5.Show All Tasks sorted" << endl;
        cout << "6.Save tree in a txt file" << endl;
        cout << "7.Retrieve last tree from a txt file" << endl;
        cout << "8.BFS Traverse" << endl;
        cout << "9.DFS Traverse" << endl;
        cout << "0.Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            isWrongOrTaskOrSubtask = 1;
            Task *t = new Task;
            if (isWrongOrTaskOrSubtask == 1) {
                Root->addTask(t);
                cout << "Task successfully added :)\n";
            } else {
                delete t;
                t = nullptr;
            }
            break;
        }
        case 2: {
            isWrongOrTaskOrSubtask = 2;
            Subtask *sub = new Subtask();
            Task *T = Root->leftChild;
            string currentTaskName = sub->taskName;
            while (T != nullptr && T->name != currentTaskName)
                T = T->rightSibling;
            if (T != nullptr) {
                T->addSubtask(sub, T);
                cout << "Subtask successfully added :)\n";
            } else {
                if (isWrongOrTaskOrSubtask == 2)
                    cout << "Oops! Task wasn't found\n";
                delete sub;
                sub = nullptr;
            }
            break;
        }
        case 3:
            ChangingSubtaskStatus(Root);
            break;
        case 4:
            showCloserTask(Root);
            break;
        case 5:
            showAllTasks(Root);
            break;
        case 6:
            saveInTXTFile(Root);
            break;
        case 7:
            retrieveFromTXTfile();
            break;
        case 8:
            BFS(Root);
            break;
        case 9:
            DFS(Root);
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice! try again.\n";
        }
    } while (choice != 0);
}
int main() {
    cout << "Hi! At first please tell me today's date: ";
    cin >> currentYear >> currentMonth >> currentDay;
    cout << "First add root task\n";
    TreeRoot R;
    R.itself = new Task;
    menu(&R);
    return 0;
}
