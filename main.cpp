#include <iostream>
#include <vector>
#include <string>
using namespace std;
int currentYear, currentMonth, currentDay;
int monthsDays[] = {0, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};
int leftDaysCalculator(int year, int month, int day)
{
   int result = 0;
   // result += (year - currentYear)*365;
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
   int year, month, day, leftDays;
   Deadline(int y, int m, int d)
   {
      year = y;
      month = m;
      day = d;
      leftDays = leftDaysCalculator(y, m, d);
   }
};
class Subtask
{
public:
   string name, expressions, status;
   Subtask *next;
   Subtask *pre;
   Subtask(string name, string exp, string state, int y, int m, int d, Subtask *n, Subtask *p)
   {
      this->name = name;
      expressions = exp;
      status = state;
      next = n;
      pre = p;
      Deadline deadline(y, m, d);
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
   void insert(string name, string exp, string state, int y, int m, int d)
   {
      Subtask *n = new Subtask(name, exp, state, y, m, d, nullptr, head);
      if (head == nullptr)
      {
         head = n;
         tail = n;
         size++;
      }
      else
      {
         head->next = n;
         head = n;
         size++;
      }
   }
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
   string name, expressions;
   Linklist subtasks;
   Task(string name, string exp, int y, int m, int d)
   {
      this->name = name;
      expressions = exp;
      Deadline deadline(y, m, d);
   }
};
int main()
{
   cout << "Hi! At first please tell me today's date: ";
   cin >> currentYear >> currentMonth >> currentDay;
   string adsts;
   cout << "Enter task: ";
   cin.ignore();
   getline(cin, adsts);
   vector<string> ss = addTaskExtractInformations(adsts);
   Task t();
   return 0;
}
