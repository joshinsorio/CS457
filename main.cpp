#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <algorithm> 
#include "table.h"

using namespace std;


//Parses lines from the testing input file and operates accordingly
void parseLine(string &input, vector<table> &tables, string &dbUse);

int main(int argc, char const *argv[])
{
    string input = "";
    vector<string> commands;
    vector<table> tables;
    string dbUse = "";

    //Main loop
    while(input != ".EXIT")
    {
        cout << "> ";
        getline(cin, input);
        transform(input.begin(), input.end(), input.begin(), ::toupper);
        parseLine(input, tables, dbUse);
    }

    return 0;
}

void parseLine(string &input, vector<table> &tables, string &dbUse)
{

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

		if(dirErr == 0)
        {
            cout << "Database " << name << " Deleted." << endl;
        }
	
		else
		{
            //FIX FOR LATERRRRRRRRRRRRRR
            chdir("..");
            if(system(("rmdir " + name).c_str()) == 0)
            {
                cout << "Database " << name << " Deleted." << endl;
            }
            else
            {
                cout << "!Failed to delete database " << name << " because it does not exist." << endl;
            }
        }
    }

    else if(input.find("USE") != -1)
    {
        string name = input.substr(4, input.length() - 5);
        dbUse = name;
        //const int dirErr = system(("cd " + name).c_str());
		if (chdir(name.c_str()) == 0)
        {
            cout << "Using database " << name << endl;
        }
    
        //Should the database not be found, go back a directory and search again
		else
		{
            chdir("..");
            if (chdir(name.c_str()) == 0)
            {
                cout << "Using database " << name << endl;
            }
	
            else
            {
                cout << "!Failed to use database " << name << " because it does not exist." << endl;
            }
        }
    }

    else if(input.find("CREATE TABLE") != -1)
    {
        string name = input.substr(13, input.length() - 14);
        //const int dirErr = system(("touch " + dbUse + "/" + name + ".txt").c_str());
        //const int dirErr = system(("touch " + name + ".txt").c_str());
        
        //Determine if the table already exists
        for(int i = 0; tables.size(); i++)
        {
            if(tables[i].getName() == name)
            {
                cout << "!Failed to create table " << name << " because it does already exist." << endl;
                break;
            }
        }
		if (dbUse != "")
        {
            const int dirErr = system(("touch " + name + ".txt").c_str());
            if(dirErr == 0)
            {
                string data = input.substr(input.find("(") + 1, input.length() - 3 - input.find("("));
                table table(name, data);
                tables.push_back(table);
                cout << "Table " << name << " created." << endl;
            }
        }
	
		else
		{
            cout << "!Failed to create table " << name << " because you are not using a database." << endl;
        }
        
    }

    else if(input.find("DROP TABLE") != -1)
    {
        string name = input.substr(11, input.length() - 12);

		if (dbUse != "")
        {
            const int dirErr = system(("rm " + name + ".txt").c_str());
            if(dirErr == 0)
            {
                for(int i = 0; i < tables.size(); i++)
                {
                    if (tables[i].getName() == name)
                    {
                        tables.erase(tables.begin() + i);
                    }

                }
                cout << "Table " << name << " deleted." << endl;
            }
        }
	
		else
		{
            cout << "!Failed to delete table " << name << " because you are not using a database." << endl;
        }
    }

    else if(input.find("SELECT *") != -1)
    {
        string name = input.substr(14, input.length() - 15);
        

    }

    else if(input.find("ALTER TABLE") != -1)
    {
        string name = input.substr(12, input.find("ADD") - 13);
    }

    else if(input.find(".EXIT") != -1)
    {
        cout << "Exiting now..." << endl;
    }

    else
    {
        cout << "!Failed to find a valid command."  << endl;
    }
}