#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>

#define quit 1
#define invalid_command 2
#define absent -1

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

int read_log(void);

void write_log(int task_count);

int handle_commands(string s, int task_count);

int read_input(int task_count);

void help(void);

int main()
try {
    int task_count = read_log();
    if(task_count == absent){
        cout << "Looks like you are using the program for the first time.\nEnter !help to see available commands.\n";
        task_count = 0;
    }
    else cout << "Welcome to the todo planner.\nCurrently you have " << task_count << " task(s) left.\nEnter any valid command to continue.\n";
    while(true){
        task_count += read_input(task_count);
    }

}

catch(int i){
    if(i == quit) cout << "Exiting program...\n";
    else if(i == invalid_command) cout << "Invalid command.\nExiting...\n";
}

catch (const std::invalid_argument& ia){
	  std::cerr << "Invalid argument: " << ia.what() << '\n';
}

int read_log(void)
{
    ifstream log;
    int task_count = 0;
    
    log.open("todo_log.txt");
    if(log.is_open()){
        string temp, temp2;
        getline(log, temp);
       
       if(temp == "") return absent;
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
        return -1;
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

int handle_commands(string s, int task_count){
    if(s == "add"){
        getline(cin, s);
        s.erase(s.begin());
        todo.push_back(task(s));
        cout << "Task added successfully.\n";
        return 1;
    }
    else if(s == "remove"){
        int index;
        cin >> index;
        if(index > todo.size()){
            cout << "Attempt to remove a task not present! Please enter a valid number to remove.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return 0;
        }
        todo.erase(todo.begin() + index - 1);
        cout << "Removed task " << index << " successfully.\n";
        return -1;
    }
    else if(s == "quit"){
        write_log(task_count);
        throw quit;
    }
    else if(s == "help"){
        help();
        return 0;
    }
    else if(s == "list"){
        cout << "You have " << task_count << " tasks stored.\n";
        for(int i = 0; i < todo.size(); ++i){
            cout << i+1 << ") " << todo[i].detail << '\n';
            if(todo[i].duedate == true) cout << "Deadline: " << todo[i].deadline << "\n";
        }
        return 0;
    }    
    else if(s == "deadline"){
        int index;
        cin >> index;
        if(index > todo.size()){
            cout << "Attempt to add deadline to a task not present! Please enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return 0;
        }
        getline(cin, s);
        s.erase(s.begin());
        todo[index-1].deadline = s;
        todo[index-1].duedate = true;
        cout << "Added deadline to task " << index << " successfully!\n";
        return 0;
    }
    else if(s == "edit"){
        int index;
        cin >> index;
        getline(cin, s);
        s.erase(s.begin());
        todo[index-1].detail = s;
        cout << "Successfully edited task " << index << "\n";
        return 0;
    }
    write_log(task_count);
    throw invalid_command;
}

int read_input(int task_count)
{
    char c;
    cin >> c;
    if(c != '!'){
        cout << "Invalid command!\nEnter !help for a list of valid commands.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return 0;
    }
    string command;
    string temp;
    cin >> command;
    return handle_commands(command, task_count);
}

void help(void)
{
    string argument;
    getline(cin, argument);
    if(argument == ""){
        cout  << "Welcome to the help command.\nYou can enter !help [!command] without the brackets to know about a command or !help -all to view all available commands.\n";
    }
    else{
        argument.erase(argument.begin());
        if(argument == "-all"){
            cout << "The following commands are available for use.\n";
            cout << "`!help` to get the list of available commands.\n";
            cout << "`!add [task]` to add a new task to do.\n";
            cout << "`!list` to list the current tasks with their task numbers.\n";
            cout << "`!remove [task number]` to remove a specific task.\n";
            cout << "`!deadline [task number]` to add a deadline to a specific task.\n";
            cout << "`!edit [task number] [new task]` to edit a task description.\n";
        }
        else if(argument == "!add") cout << "`!add [task]` to add a new task to do.\n";
        else if(argument == "!list") cout << "`!list` to list the current tasks with their task numbers.\n";
        else if(argument == "!remove") cout << "`!remove [task number]` to remove a specific task.\n";
        else if(argument == "!deadline") cout << "`!deadline [task number]` to add a deadline to a specific task.\n";
        else if(argument == "!edit") cout << "`!edit [task number] [new task]` to edit a task description.\n";
        else cout << "Invalid argument!\n";
    }
}