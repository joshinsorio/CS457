#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include<unistd.h>  


using namespace std;

//Parses lines from the testing input file and operates accordingly
void parseLine(string &input, vector<string> &commands);

int main(int argc, char const *argv[])
{
    string input = "";

    vector<string> commands;

    //Main loop
    while(input != ".EXIT")
    {
        cout << "> ";
        getline(cin, input);
        parseLine(input, commands);

    }

    return 0;
}

void parseLine(string &input, vector<string> &commands)
{
    string dbUse = "";
    if(input.find("CREATE DATABASE") != -1)
    {
        string name = input.substr(16, input.length() - 17);
	
        const int dirErr = system(("mkdir " + name).c_str());
		if (dirErr == 0)
        {
            cout << "Database " << name << " created." << endl;
        }
	
		else
		{
            cout << "!Failed to create database " << name << " because it already exists." << endl;
        }
    }

    else if(input.find("DROP DATABASE") != -1)
    {
        string name = input.substr(14, input.length() - 15);
	
        const int dirErr = system(("rmdir " + name).c_str());
		if (dirErr == 0)
        {
            cout << "Database " << name << " Deleted." << endl;
        }
	
		else
		{
            cout << "!Failed to delete database " << name << " because it does not exists." << endl;
        }
    }

    else if(input.find("USE") != -1)
    {
        string name = input.substr(4, input.length() - 5);
        dbUse = name;
        const int dirErr = chdir(name.c_str());
		if (dirErr == 0)
        {
            cout << "Using database " << name << endl;
        }
	
		else
		{
            cout << "!Failed to use database " << name << " because it does not exists." << endl;
        }
    }

    else if(input.find("CREATE TABLE") != -1)
    {
        string name = input.substr(13, input.length() - 14);
        const int dirErr = system(("touch " + name + ".txt").c_str());
		if (dirErr == 0)
        {
            cout << "Table " << name << " created." << endl;
        }
	
		else
		{
            cout << "!Failed to create table " << name << " because it does not exists." << endl;
        }        
    }

    else if(input.find("DROP TABLE") != -1)
    {
        string name = input.substr(11, input.length() - 12);
    }

    else if(input.find("SELECT *") != -1)
    {
        string name = input.substr(14, input.length() - 15);
    }

    else if(input.find(".EXIT") != -1)
    {
        
    }

    else
    {
        cout << "!Failed to find a valid command."  << endl;
    }

}