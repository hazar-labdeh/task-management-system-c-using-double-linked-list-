#include <iostream>
#include <string>
using namespace std;

const int MAX_VALID_YR = 9999;
const int MIN_VALID_YR = 1800;


int extractYear(const string& dueDate, int i) {
    return (dueDate[i] - '0') * 1000 +
           (dueDate[i + 1] - '0') * 100 +
           (dueDate[i + 2] - '0') * 10 +
           (dueDate[i + 3] - '0');
}


int extractDayMonth(const string& dueDate, int i) {
    return (dueDate[i] - '0') * 10 +
           (dueDate[i + 1] - '0');
}


bool isLeap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}


bool isValid(int d, int m, int y) {
    if (y < MIN_VALID_YR || y > MAX_VALID_YR) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > 31) return false;

    if (m == 2)
        return isLeap(y) ? (d <= 29) : (d <= 28);

    if (m == 4 || m == 6 || m == 9 || m == 11)
        return (d <= 30);

    return true;
}


bool validDate(const string& date) {
    if (date.length() != 10) {
        cout << "Date format is wrong.\n";
        return false;
    }
    if (date[4] != '-' || date[7] != '-') {
        cout << "Date format is wrong.\n";
        return false;
    }

    int y = extractYear(date, 0);
    int m = extractDayMonth(date, 5);
    int d = extractDayMonth(date, 8);

    if (!isValid(d, m, y)) {
        cout << "Invalid date.\n";
        return false;
    }

    return true;
}


class Task {
public:
    string title;
    string description;
    string dueDate;
    int priority;
    string status;

    Task(string t, string d, string date, int p, string s) {
        title = t;
        description = d;
        dueDate = date;
        priority = p;
        status = s;
    }
};


Task* inputTask() {
    string title, description, dueDate, status;
    int priority;

    cin.ignore();
    cout << "Enter title: ";
    getline(cin, title);

    cout << "Enter description: ";
    getline(cin, description);

    do {
        cout << "Enter due date (YYYY-MM-DD): ";
        cin >> dueDate;
    } while (!validDate(dueDate));

    do {
        cout << "Enter priority (1=High, 2=Medium, 3=Low): ";
        cin >> priority;
    } while (!(priority == 1 || priority == 2 || priority == 3));
    
    cin.ignore();
    cout << "Enter status: ";
    getline(cin, status);

    return new Task(title, description, dueDate, priority, status);
}


class Node {
public:
    Task* task;
    Node* prev;
    Node* next;

    Node(Task* t) {
        task = t;
        prev = NULL;
        next = NULL;
    }
};


class DoublyLinkedList {
private:
    Node* first;
    Node* last;
    int count;

public:
    DoublyLinkedList() : first(NULL), last(NULL), count(0) {}
    
    ~DoublyLinkedList() {
    Node* current = first;
    while (current != NULL) {
        Node* next = current->next;
        delete current->task;  
        delete current;        
        current = next;
    }
}

    bool isEmpty() const {
        return (first == NULL);
    }

    void insertFirst(Task* task) {
        Node* newNode = new Node(task);
        if (isEmpty()) {
            first = last = newNode;
        } else {
            newNode->next = first;
            first->prev = newNode;
            first = newNode;
        }
        count++;
    }

    void insertLast(Task* task) {
        Node* newNode = new Node(task);
        if (isEmpty()) {
            first = last = newNode;
        } else {
            newNode->prev = last;
            last->next = newNode;
            last = newNode;
        }
        count++;
    }

    void remove(string title) {
        if (isEmpty()) {
            cout << "List is empty\n";
            return;
        }
        Node* temp = first;
        while (temp != NULL) {
            if (temp->task->title == title) {
                if (temp == first) {
                    first = temp->next;
                    if (first != NULL) first->prev = NULL;
                } else if (temp == last) {
                    last = temp->prev;
                    if (last != NULL) last->next = NULL;
                } else {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
                }
                delete temp->task;
                delete temp;
                count--;
                cout << "Task removed.\n";
                return;
            }
            temp = temp->next;
        }
        cout << "Task not found.\n";
    }

    void display() {
        if (isEmpty()) {
            cout << "Task list is empty.\n";
            return;
        }
        Node* temp = first;
        while (temp != NULL) {
            cout << "Title: " << temp->task->title
                 << ", Description: " << temp->task->description
                 << ", Due Date: " << temp->task->dueDate
                 << ", Priority: " << temp->task->priority
                 << ", Status: " << temp->task->status << endl;
            temp = temp->next;
        }
    }

    void displayByPriority() {
        if (isEmpty()) {
            cout << "Task list is empty.\n";
            return;
        }
        for (int p = 1; p <= 3; p++) {
            Node* temp = first;
            while (temp != NULL) {
                if (temp->task->priority == p) {
                    cout << "Title: " << temp->task->title
                         << ", Description: " << temp->task->description
                         << ", Due Date: " << temp->task->dueDate
                         << ", Priority: " << temp->task->priority
                         << ", Status: " << temp->task->status << endl;
                }
                temp = temp->next;
            }
        }
    }

    void updateStatus(string title, string newStatus) {
        if (isEmpty()) {
            cout << "Task list is empty.\n";
            return;
        }
        Node* temp = first;
        while (temp != NULL) {
            if (temp->task->title == title) {
                temp->task->status = newStatus;
                cout << "Task status updated.\n";
                return;
            }
            temp = temp->next;
        }
        cout << "Task not found.\n";
    }

    void searchTask(int ch, string words) {
        if (isEmpty()) {
            cout << "List is empty\n";
            return;
        }
        bool found = false;
        Node* temp = first;
        while (temp != NULL) {
            if ((ch == 1 && temp->task->title == words) ||
                (ch == 2 && temp->task->description == words)) {
                cout << "Title: " << temp->task->title
                     << ", Description: " << temp->task->description
                     << ", Due Date: " << temp->task->dueDate
                     << ", Priority: " << temp->task->priority
                     << ", Status: " << temp->task->status << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found) cout << "No tasks found.\n";
    }

    bool compareDates(const string& date1, const string& date2) {
        return date1 < date2;
    }

    void sortByDueDate() {
        if (isEmpty()) return;

        for (Node* i = first; i != NULL; i = i->next) {
            Node* minNode = i;
            for (Node* j = i->next; j != NULL; j = j->next) {
                if (compareDates(j->task->dueDate, minNode->task->dueDate)) {
                    minNode = j;
                }
            }
            if (minNode != i) {
                Task* temp = i->task;
                i->task = minNode->task;
                minNode->task = temp;
            }
        }

        cout << "Tasks sorted by due date.\n";
    }
};


void showMenu() {
    cout << "\n--- Task Management System ---\n";
    cout << "1. Add Task to Start\n";
    cout << "2. Add Task to End\n";
    cout << "3. Remove Task by Title\n";
    cout << "4. Display All Tasks\n";
    cout << "5. Display Tasks by Priority\n";
    cout << "6. Update Task Status\n";
    cout << "7. Search Tasks\n";
    cout << "8. Sort Tasks by Due Date\n";
    cout << "9. Exit\n";
    cout << "Choose an option: ";
}


int main() {
    DoublyLinkedList dl;
    int choice;
    string title, status, keyword;

    while (true) {
        showMenu();
        cin >> choice;

        if (choice == 1) {
            Task* t = inputTask();
            dl.insertFirst(t);
        } else if (choice == 2) {
            Task* t = inputTask();
            dl.insertLast(t);
        } else if (choice == 3) {
            cout << "Enter title to remove: ";
            cin >> title;
            dl.remove(title);
        } else if (choice == 4) {
            dl.display();
        } else if (choice == 5) {
            dl.displayByPriority();
        } else if (choice == 6) {
            cout << "Enter title to update status: ";
            cin >> title;
            cout << "Enter new status: ";
            cin >> status;
            dl.updateStatus(title, status);
        } else if (choice == 7) {
            int ch;
            string words;
            cout << "Enter 1 for title search or 2 for description search: ";
            cin >> ch;
            cout << "Enter what you want to search : ";
            cin.ignore();
            getline(cin, words);
            dl.searchTask(ch, words);
        } else if (choice == 8) {
            dl.sortByDueDate();
        } else if (choice == 9) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}