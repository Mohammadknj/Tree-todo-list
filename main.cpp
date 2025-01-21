#include <iostream>
#include <vector>
#include <string>
using namespace std;
int currentYear, currentMonth, currentDay;
int monthsDays[] = {0, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};
int leftDaysCalculator(int year, int month, int day)
{
   int result = 0;
   if (year > currentYear + 1)
   {
      result += (year - (currentYear + 1)) * 365;
      year = currentYear + 1;
   }
   while (month > currentMonth)
   {
      result += monthsDays[month--];
      if (month == 0)
         month = 12;
   }
   result += day - currentDay;
   return result;
}
vector<string> addTaskExtractInformations(string str)
{
   vector<string> result;
   string word;
   int i = 8;
   while (i < str.length())
   {
      i++;
      if (str[i] == ',')
      {
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

class Deadline
{
public:
   int year, month, day, leftDays, hour, minute;
   Deadline(string str)
   {
      int arr[3];
      for (int i = 0; i < 3; i++)
      {
         arr[i] = 0;
      }

      int i = 0, j = 0;
      while (str[i] != ' ')
      {
         if (str[i] == '/')
         {
            i++;
            arr[j] /= 10;
            j++;
            continue;
         }
         arr[j] += str[i] - 48;
         arr[j] *= 10;
         i++;
      }
      while (str[i] != ':')
      {
         j += str[i] - 48;
         j *= 10;
         i++;
      }
      hour = j / 10;
      i++;
      minute = str[i++] * 10 + str[i];
      year = arr[0];
      month = arr[1];
      day = arr[2];
      leftDays = leftDaysCalculator(year, month, day);
   }
};
class Subtask
{
public:
   string name, description, status, taskName;
   Subtask *next;
   Subtask *pre;
   // Subtask(string name, string exp, string state, int y, int m, int d, Subtask *n, Subtask *p)
   Subtask(vector<string> str, Subtask *n, Subtask *p)
   {
      this->name = str[0];
      description = str[1];
      status = str[3];
      taskName = str[4];
      next = n;
      pre = p;
      Deadline deadline(str[2]);
   }
};
class Linklist
{
public:
   Subtask *head;
   Subtask *tail;
   int size;
   Linklist()
   {
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
   void delNode(Subtask *node)
   {
      if (node == nullptr)
         return;
      if (node->pre != nullptr)
         node->pre->next = node->next;
      else
         tail = node->next;
      if (node->next != nullptr)
         node->next->pre = node->pre;
      else
         head = node->pre;
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
class Task
{
public:
   string name, description, status;
   Linklist subtasks;
   Task(vector<string> str)
   {
      name = str[0];
      description = str[1];
      status = str[3];
      Deadline deadline(str[2]);
   }
};
void addTask()
{
   string str;
   cout << "Enter task: ";
   cin.ignore();
   getline(cin, str);
   vector<string> ss = addTaskExtractInformations(str);
   Task t(ss);
}
void menu()
{
   int choice;
   do
   {
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

      switch (choice)
      {
      case 1:
         addTask();
         break;
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
int main()
{
   cout << "Hi! At first please tell me today's date: ";
   cin >> currentYear >> currentMonth >> currentDay;
   menu();
   // Task t();
   return 0;
}
