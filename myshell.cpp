#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

/*
  Function Declarations for builtin shell commands:
 */
int cd_command(const vector<string> &args);
int help_command(const vector<string> &args);
int exit_command(const vector<string> &args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
const vector<string> builtin_commands = {
    "cd",
    "help",
    "exit"};

const vector<int (*)(const vector<string> &)> builtin_functions = {
    &cd_command,
    &help_command,
    &exit_command};

int num_builtins()
{
    return builtin_commands.size();
}

/*
  Builtin function implementations.
*/
int cd_command(const vector<string> &args)
{
    if (args.size() < 2)
    {
        cerr << "lsh: expected argument to \"cd\"" << endl;
    }
    else
    {
        if (chdir(args[1].c_str()) != 0)
        {
            perror("lsh");
        }
    }
    return 1;
}

int help_command(const vector<string> &args)
{
    cout << "Liam Glenn's LSH" << endl;
    cout << "Type program names and arguments, and hit enter." << endl;
    cout << "The following are built in:" << endl;

    // Using range-based for loop instead of indexed loop
    for (const auto &cmd : builtin_commands)
    {
        cout << "  " << cmd << endl;
    }

    cout << "Use the man command for information on other programs." << endl;
    return 1;
}

int exit_command(const vector<string> &args)
{
    return 0;
}

int launchCommand(const vector<string> &args)
{
    //  var declarations
    pid_t pid, wpid;
    int status;

    // Check if args is empty before executing
    if (args.empty())
    {
        cerr << "lsh: empty command" << endl;
        return 1;
    }

    // create child process
    pid = fork();
    if (pid == 0)
    {
        // Child process
        // Convert vector<string> to char* array for execvp
        vector<char *> c_args;
        // loop thru args
        for (const auto &arg : args)
        {
            //  capture all args for execv
            c_args.push_back(const_cast<char *>(arg.c_str()));
        }
        // Add nullptr at the end as required by execvp
        c_args.push_back(nullptr);

        // execvp only returns if there's an error
        if (execvp(c_args[0], c_args.data()) == -1)
        {
            perror("lsh");
            exit(EXIT_FAILURE); // Using EXIT_FAILURE constant instead of 1
        }
        // No need for an explicit success exit - execvp never returns on success
    }
    else if (pid < 0)
    {
        // Error forking
        perror("lsh");
    }
    else
    {
        // Parent process
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED); // wait for child to finish
        } while (!WIFEXITED(status) && !WIFSIGNALED(status)); // keeps calling waitpid until the child proccess exits or is interrupted by a signal
    }

    return 1;
}

//  parse the line into tokens
vector<string> parseLine(const string &line)
{
    // var declars
    vector<string> tokens; // vector of tokens to be returned
    stringstream ss(line); // create a ss from the line variable
    string token;          // var for individual tokens

    try
    {
        // Reserve initial capacity (64 tokens)
        tokens.reserve(64);

        // Split the line by whitespace characters
        while (ss >> token)
        {
            tokens.push_back(token); //  add individual tokens to return vector
        }

        return tokens; // return all tokens
    }
    catch (const bad_alloc &e) //  check for allocation issues
    {
        //  if we have allocation issues, print error message and exit program
        cerr << "lsh: allocation error: " << e.what() << endl;
        exit(EXIT_FAILURE); // Using EXIT_FAILURE constant instead of 1
    }
}

//  read in a line of input from user
string readLine(void)
{
    try
    {
        // Simplified implementation using getline
        string line;
        getline(cin, line);
        return line; // return the line when finished
    }
    catch (const bad_alloc &e) // check if there are any issues allocating
    {
        // print error message and exit program if so
        cerr << "lsh: allocation error: " << e.what() << endl;
        exit(EXIT_FAILURE); // Using EXIT_FAILURE constant instead of 1
    }
}

//  execute the user's commands
int executeCommand(const vector<string> &args)
{
    //  if nothing was entered, nothing to do
    if (args.empty())
    {
        // An empty command was entered.
        return 1;
    }

    //  if it is a built in, use our built in methods
    for (int i = 0; i < num_builtins(); i++)
    {
        if (args[0] == builtin_commands[i])
        {
            return builtin_functions[i](args);
        }
    }

    //  else, use execv
    return launchCommand(args);
}

//  loop through until user is done
void command_loop(void)
{
    // var declarations
    string line;         // holds the whole line
    vector<string> args; // vector with each argument
    int status;          // tracks the status in order to determine when to leave loop

    do
    {
        cout << "> ";                  // print prompt
        line = readLine();             // read the input
        args = parseLine(line);        // parse the args
        status = executeCommand(args); // execute the command
    } while (status); // keep looping until user exits
}

int main(int argc, char **argv)
{
    // Load Config Files

    command_loop(); // Loop through commands while user is using it

    return EXIT_SUCCESS; // Using EXIT_SUCCESS constant instead of 0
}