#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

struct Task {
    string title;
    bool done;
};

class TodoList {
private:
    vector<Task> tasks;
    string filename;

public:
    explicit TodoList(const string& file) : filename(file) {
        load();
    }

    void add(const string& title) {
        tasks.push_back({ title, false });
        save();
    }

    void show() const {
        if (tasks.empty()) {
            cout << "\nNo tasks yet.\n";
            return;
        }
        cout << "\n--- Your tasks ---\n";
        for (size_t i = 0; i < tasks.size(); i++) {
            cout << i + 1 << ". [" << (tasks[i].done ? "x" : " ") << "] "
                << tasks[i].title << "\n";
        }
    }

    bool markDone(int number) {
        if (number < 1 || number > static_cast<int>(tasks.size())) return false;
        tasks[number - 1].done = true;
        save();
        return true;
    }

    bool remove(int number) {
        if (number < 1 || number > static_cast<int>(tasks.size())) return false;
        tasks.erase(tasks.begin() + (number - 1));
        save();
        return true;
    }

    int count() const {
        return static_cast<int>(tasks.size());
    }

private:
    void save() const {
        ofstream out(filename);
        for (const Task& t : tasks) {
            out << (t.done ? '1' : '0') << '|' << t.title << '\n';
        }
    }

    void load() {
        ifstream in(filename);
        if (!in) return;
        string line;
        while (getline(in, line)) {
            if (line.size() < 3) continue;
            tasks.push_back({ line.substr(2), line[0] == '1' });
        }
    }
};

void printMenu() {
    cout << "\n===== To-Do List =====\n"
        << "1. Add task\n"
        << "2. Show tasks\n"
        << "3. Mark task as done\n"
        << "4. Delete task\n"
        << "0. Exit\n"
        << "Choose: ";
}

int readNumber() {
    int n;
    while (!(cin >> n)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return n;
}

int main() {
    TodoList list("tasks.txt");
    int choice;

    do {
        printMenu();
        choice = readNumber();

        switch (choice) {
        case 1: {
            cout << "Task title: ";
            string title;
            getline(cin, title);
            if (title.empty()) {
                cout << "Title can't be empty.\n";
            }
            else {
                list.add(title);
                cout << "Task added.\n";
            }
            break;
        }
        case 2:
            list.show();
            break;
        case 3: {
            list.show();
            cout << "Task number to mark as done: ";
            int n = readNumber();
            cout << (list.markDone(n) ? "Done!\n" : "Invalid number.\n");
            break;
        }
        case 4: {
            list.show();
            cout << "Task number to delete: ";
            int n = readNumber();
            cout << (list.remove(n) ? "Deleted.\n" : "Invalid number.\n");
            break;
        }
        case 0:
            cout << "Bye!\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}