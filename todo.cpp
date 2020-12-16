#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class task{
    public:
    string detail;
    string deadline;
    bool duedate;
    task(string m, bool b) :detail(m),deadline(""), duedate(b) {}
    task(string m) :detail(m), deadline(""), duedate(false) {}
    task(string m, string n) :detail(m), deadline(n), duedate(true) {}
};

vector<task> todo;

int read_log(void)
{
    ifstream log;
    int task_count = 0;
    log.open("todo_log.txt");
    if(log.is_open()){
        string temp, temp2;
        getline(log, temp);
        if(temp == "") return -1;
        task_count = stoi(temp);
        for(int i = 0; i < task_count; ++i){
            getline(log, temp);
            getline(log, temp2);
            if(temp2 == "") todo.push_back(task(temp, false));
            else todo.push_back(task(temp,temp2));
        }
    }
    else{
        cout << "No log file detected.\nCreating new one...\n";
        ofstream file("todo_log.txt");
    }
    log.close();
    return task_count;
}

void write_log(int task_count)
{
    ofstream log;
    log.open("todo_log.txt", ios::trunc);
    log << task_count << '\n';
    for(task i: todo){
        log << i.detail << '\n' << i.deadline << '\n';
    }
    log.close();
}

int read_input(int task_count)
{
    char c;
    cin >> c;
    if(c != '!'){
        cout << "Invalid command!\nEnter !help for a list of valid commands.\n";
        cin.sync();
        return 0;
    }
    string command;
    string temp;
    cin >> command;
    if(command == "add"){
        getline(cin, temp);
        temp.erase(temp.begin());
        todo.push_back(task(temp));
        return 1;
    }
    else if(command == "remove"){
        int index;
        cin >> index;
        if(index > todo.size()){
            cout << "Attempt to remove a task not present! Please enter a valid number to remove.\n";
            cin.sync();
            return 0;
        }
        todo.erase(todo.begin() + index - 1);
        return -1;
    }
    else if(command == "quit"){
        write_log(task_count);
        throw 0;
    }
    else if(command == "help"){
        cout << "Available commands are:\n!add [task] - to add a new task\n!remove [task number] - to remove the task at specific position\n!quit - to quit the program\n";
        return 0;
    }
    else if(command == "list"){
        for(int i = 0; i < todo.size(); ++i){
            cout << i+1 << ')' << todo[i].detail << '\n';
            if(todo[i].duedate == true) cout << "Deadline: " << todo[i].deadline << '\n';
        }
        return 0;
    }
    else {
        write_log(task_count);
        throw 3;
    }
}

int main()
try {
    int task_count = read_log();
    int additions = 0;
    if(task_count == -1){
        cout << "Looks like you are using the program for the first time.\nEnter !help to see available commands.\n";
        task_count = 0;
    }
    else cout << "Welcome to the todo planner.\nCurrently you have " << task_count << " task(s) left.\nEnter any valid command to continue.\n";
    while(true){
        task_count += read_input(task_count);
    }

}

catch(int i){
    if(i == 0) cout << "Exiting program...\n";
    else if(i == 3) cout << "Invalid command.\nExiting...\n";
    else cout << "Can't open log file!\nExiting...\n";
}