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
int isTaskOrSubtask=1;
bool checkCorrectAddingCommand(int i, string str){
    if (isTaskOrSubtask == 1) {
        string s = "add task ";
        for (int j = 0; j < i; ++j) {
            if (s[j] != str[j]) {
                return false;
            }
        }
    }else{
        string s = "add subtask ";
        for (int j = 0; j < i; ++j) {
            if (s[j] != str[j]) {
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
    if (isTaskOrSubtask == 2){
        i += 3;       
    }
    if(!checkCorrectAddingCommand(i,str)){
        cout<<"Error! You Entered a wrong command\n";
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
        date = to_string(year) + '/' + to_string(month) + '/' + to_string(day);
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
        cout << "Enter task: ";
        cin.ignore();
        getline(cin, str);
        vector<string> ss = addTaskExtractInformations(str);
        // if(ss.has_v)
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
    void insert(Subtask *sub, Task*t) {
        sub->parent = t;
        sub->previous = head;
        if (head == nullptr) {
            head = sub;
            tail = sub;
        } else {
            head->next = sub;
            head = sub;
        }
        size++;
    }
    void delNode(Subtask *subtask) {
        if (subtask == nullptr)
            return;
        if (subtask->previous != nullptr)
            subtask->previous->next = subtask->next;
        else
            tail = subtask->next;
        if (subtask->next != nullptr)
            subtask->next->previous = subtask->previous;
        else
            head = subtask->previous;
        size--;
        delete subtask;
    }
};
vector<string>ss;
class Task {
public:
    string name, description, status;
    Task *parent;
    Task *rightSibling;
    Subtask *leftChild;
    Linklist subtasks;
    Deadline deadline;
    void deleteIncorrectTask(Task *t){
        delete t;
        t = nullptr;
    }
    Task() {
        parent = nullptr;
        rightSibling = nullptr;
        leftChild = nullptr;
        string str;
        cout << "Enter task: ";
        cin.ignore();
        getline(cin, str);
        ss = addTaskExtractInformations(str);
        if(ss.size()<1){
            // ~Task();
            return;
        }
        name = ss[0];
        description = ss[1];
        status = ss[3];
        deadline.makeDeadline(ss[2]);
    }
    ~Task(){
        if(ss.size()<1)
        deleteIncorrectTask(this);
    }
    void addSubtask(Subtask *sub, Task*t) {
        subtasks.insert(sub,t);
        if(t->subtasks.size == 1)
            t->leftChild = sub;
    }
};
class TreeRoot {
public:
    Task *itself;
    Task *leftChild;
    TreeRoot() {
        itself = nullptr;
        leftChild = nullptr;
    }
    void addTask(Task *t) {
        if (leftChild == nullptr) {
            t->parent = itself;
            leftChild = t;
        } else {
            Task *x = leftChild;
            while (x->rightSibling != nullptr) {
                x = x->rightSibling;
            }
            x->rightSibling = t;
        }
    }
};
// Task *addTask() {
//     string str;
//     cout << "Enter task: ";
//     cin.ignore();
//     getline(cin, str);
//     vector<string> ss = addTaskExtractInformations(str);
//     return new Task t(ss);
// }
void menu(TreeRoot *Root) {
    int choice;
    do {
        cout << "menu:" << endl;
        cout << "1.Add task" << endl;
        cout << "2.Add subtask" << endl;
        // cout << "3.bazyabi etelaat" << endl;
        // cout << "4.namayesh jadval doroos va daneshjoo" << endl;
        // cout << "5.sabte etelaat dar file matni" << endl;
        // cout << "6.bazyabie etelaat az file matni" << endl;
        cout << "0.Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            isTaskOrSubtask = 1;
            Task *t = new Task;
            if(t!=nullptr)
            Root->addTask(t);
            break;
        }
        case 2:{
            isTaskOrSubtask = 2;
            Subtask *sub = new Subtask();
            Task* T = Root->leftChild;
            string currentTaskName = sub->taskName;
            while(T != nullptr && T->name != currentTaskName)
                T = T->rightSibling;
            if(T!=nullptr)
                T->addSubtask(sub,T);
            else delete sub;
            break;
        }
        // case 3:
        //    bazyabi_etelaat();
        //    break;
        // case 4:
        //    namayesh_jadvali();
        //    break;
        // case 5:
        //    sabt_dar_file();
        //    break;
        // case 6:
        //    bazyabi_az_file();
        //    break;
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
