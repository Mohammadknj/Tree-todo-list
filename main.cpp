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
vector<string> addTaskExtractInformations(string str) {
    vector<string> result;
    string word;
    int i = 8;
    while (i < int(str.length())) {
        i++;
        if (str[i] == ',') {
            result.push_back(word);
            word.clear();
            i++;
            continue;
        }
        word += str[i];
    }
    result.push_back(word);
    return result;
}
class Deadline {
public:
    int year, month, day, leftDays, hour, minute;
    Deadline(string str) {
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
        day = arr[2] / 10;
        leftDays = leftDaysCalculator(year, month, day);
    }
};
class Task;
class Subtask {
public:
    string name, description, status, taskName;
    Subtask *next;
    Subtask *previous;
    Task *parent;
    // Subtask(string name, string exp, string state, int y, int m, int d, Subtask
    // *nxt, Subtask *pre)
    Subtask(vector<string> str, Subtask *nxt, Subtask *pre, Task *p) {
        this->name = str[0];
        description = str[1];
        status = str[3];
        taskName = str[4];
        next = nxt;
        previous = pre;
        parent = p;
        Deadline deadline(str[2]);
    }
};
// class Queue {
// public:
//     vector<Subtask> subtasks[10];
//     Subtask *Qhead;
//     Subtask *Qtail;
//     int QSize;
//     int Qlen = sizeof subtasks/sizeof subtasks[0];
//     Queue() {
//         Qhead = nullptr;
//         Qtail = nullptr;
//         QSize = 0;
//     }
//     int qSize() {
//         return QSize;
//     }
//     // void enqueue(int Q[], int node) {
//     //     if (QSize == Qlen)
//     //         cout << "Overflow!" << endl;
//     //     else {
//     //         *Qtail = node;
//     //         QSize++;
//     //         if(Qtail == &Q[Qlen-1]){
//     //             Qtail = &Q[0];
//     //         }
//     //         else Qtail++;
//     //     }
//     //     // پیچیدگی: O(1)
//     // }
//     bool QIsEmpty() {
//         if (qSize() == 0)
//             return true;
//         return false;
//         // پیچیدگی: O(1)
//     }
//     Subtask dequeue() {
//         if (QIsEmpty()) {
//             cout << "Underflow!" << endl;
//         } else {
//             Subtask x = *Qhead;
//             // *Qhead = 0;
//             QSize--;
//             // if(&Qhead == &subtasks[Qlen-1]){
//             //     Qhead = *subtasks[0];
//             // }
//             // else Qhead++;
//             return x;
//         }
//         // پیچیدگی: O(1)
//     }
// };

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
    // پیچیدگی حافظه = O(1)
    //  پیچیدگی زمانی = O(1)
    // void insert(string name, string exp, string state, int y, int m, int d)
    // {
    //    Subtask *n = new Subtask(name, exp, state, y, m, d, nullptr, head);
    //    if (head == nullptr)
    //    {
    //       head = n;
    //       tail = n;
    //       size++;
    //    }
    //    else
    //    {
    //       head->next = n;
    //       head = n;
    //       size++;
    //    }
    // }
    // پیچیدگی حافظه ندارد
    //  پیچیدگی زمانی = O(1)
    void delNode(Subtask *node) {
        if (node == nullptr)
            return;
        if (node->previous != nullptr)
            node->previous->next = node->next;
        else
            tail = node->next;
        if (node->next != nullptr)
            node->next->previous = node->previous;
        else
            head = node->previous;
        size--;
        delete node;
    }
    // پیچیدگی حافظه = O(1)
    //  پیچیدگی زمانی = O(n)
    // subtask *search(int key){
    //     subtask * node = tail;
    //     while(node != nullptr && node->key != key)
    //         node = node->next;
    //     return node;
    // }
};
class Task {
public:
    string name, description, status;
    Task* parent;
    Task *rightSibling;
    Task *leftChild;
    Linklist subtasks;
    Task() {
        string str;
        cout << "Enter task: ";
        cin.ignore();
        getline(cin, str);
        vector<string> ss = addTaskExtractInformations(str);
        name = ss[0];
        description = ss[1];
        status = ss[3];
        Deadline deadline(ss[2]);
    }
};
class TreeRoot {
public:
    Task *root;
    Task *leftChild;
    TreeRoot() {
        root = nullptr;
        leftChild = nullptr;
    }
    void addTask(Task *t){
        if(leftChild == nullptr){
            t->parent = root;
            leftChild = t;
        }else{
            Task *x = leftChild;
            while(x->rightSibling != nullptr){
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
void menu(TreeRoot *R) {
    int choice;
    do {
        cout << "menu:" << endl;
        cout << "1.sabte etelaat" << endl;
        cout << "2.entekhab vahed" << endl;
        cout << "3.bazyabi etelaat" << endl;
        cout << "4.namayesh jadval doroos va daneshjoo" << endl;
        cout << "5.sabte etelaat dar file matni" << endl;
        cout << "6.bazyabie etelaat az file matni" << endl;
        cout << "0.3bar 0 ra bezanid ta kharej shavad" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:{
            Task *t = new Task;
            R->addTask(t);
            break;
        }
        // case 2:
        //    entekhab_vahed();
        //    break;
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
    cout<<"First add root task\n";
    TreeRoot R;
    R.root = new Task;
    menu(&R);
    return 0;
}
