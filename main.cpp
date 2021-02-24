#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <algorithm> 

using namespace std;

//Parses lines from the testing input file and operates accordingly
void parseLine(string &input, string &dbUse);

string getName(string line);
string getTblName(string line);
string getParse(string &input);
string getData(string line);
bool DoesFileExist (const string& name);

int main(int argc, char const *argv[])
{
    string input = "";
    vector<string> commands;
    string dbUse = "";

    //Main loop
    while(input != ".EXIT")
    {
        cout << "> ";
        getline(cin, input);
        //transform(input.begin(), input.end(), input.begin(), ::toupper);
        parseLine(input, dbUse);
    }

    return 0;
}

void parseLine(string &input, string &dbUse)
{
    string command = getParse(input);

    if(command.find("CREATE DATABASE") != -1)
    {
        string name = getName(command);
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

    else if(command.find("DROP DATABASE") != -1)
    {
        string name = getName(command);
        const int dirErr = system(("rmdir " + name).c_str());

		if(dirErr == 0)
        {
            cout << "Database " << name << " Deleted." << endl;
        }
	
		else
		{
            if(dbUse == "")
            {
                if(system(("rmdir " + name).c_str()) == 0)
                {
                    cout << "Database " << name << " Deleted." << endl;
                }
                
                else
                {
                    cout << "!Failed to delete database " << name << " because it does not exist." << endl;
                }
            }
            else
                cout << "!Failed to delete database " << name << " because you are not in the correct directory." << endl;
        }
    }

    else if(command.find("USE") != -1)
    {
        string name = getName(command);
        dbUse = name;

		if((chdir(name.c_str()) == 0))
        {
            cout << "Using database " << name << endl;
        }
    
        //Should the database not be found, go back a directory and search again
		else
		{
            if(dbUse == "")
            {
                cout << "!Failed to use database " << name << " because it does not exist." << endl;
            }

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
    }

    else if(command.find("CREATE TABLE") != -1)
    {
        fstream file;
        string name = getTblName(command);
        string data = getData(command);


        //Determine if the table already exists
        if(DoesFileExist((name + ".txt").c_str()))
        {
            cout << "!Failed to create table " << name << " because it already exists." << endl;
        }
        
        else
        {
            
            if (dbUse != "")
            {
                file.open((name + ".txt").c_str(), ios::out);
                file << data << " | ";
                cout << "Table " << name << " created." << endl;
                file.close();
            }
        
            else
            {
                cout << "!Failed to create table " << name << " because there is no database in use." << endl;
            }
        }

    }

    else if(command.find("DROP TABLE") != -1)
    {
        fstream file;
        string name = getTblName(command);

        //Determine if the table already exists
        if(!DoesFileExist((name + ".txt").c_str()))
        {
            cout << "!Failed to delete table " << name << " because the table does not exist." << endl;
        }

        else
        {
            if (dbUse != "")
            {
                const int dirErr = system(("rm " + name + ".txt").c_str());
                cout << "Table " << name << " deleted." << endl;
            }
        
            else
            {
                cout << "!Failed to delete table " << name << " because you are not using a database." << endl;
            }
        }
    }

    else if(command.find("SELECT * FROM") != -1)
    {
        fstream file;
        string line;
        string name = getName(command);

        //Determine if the table does not exist
        if(!DoesFileExist((name + ".txt").c_str()))
        {
            cout << "!Failed to query table " << name << " because the table does not exist." << endl;
        }
        
        else
        {
            if (dbUse != "")
            {
                file.open((name + ".txt").c_str(), ios::in);
                while(getline(file, line)) 
                {
                    cout << line << endl;
                }
                file.close();
            }
        
            else
            {
                cout << "!Failed to delete table " << name << " because you are not using a database." << endl;
            }
        }
        file.close();

    }

    else if(command.find("ALTER TABLE") != -1)
    {
        fstream file;
        string line;
        string data = command.substr(command.find("ADD") + 4, command.length() - (command.find("ADD") + 4));
        string name = getTblName(command);
        
        //Determine if the table does not exist
        if(!DoesFileExist((name + ".txt").c_str()))
        {
            cout << "!Failed to alter table " << name << " because the table does not exist." << endl;
        }

        else
        {
            if (dbUse != "")
            {
                file.open((name + ".txt").c_str(), ios::app);
                file << data << endl;
                file.close();

                file.open((name + ".txt").c_str(), ios::in);
                while(getline(file, line)) 
                {
                    cout << line << endl;
                }
                file.close();
            }
        
            else
            {
                cout << "!Failed to alter table " << name << " because you are not using a database." << endl;
            }
        }
    }

    else if(command.find(".EXIT") != -1)
    {
        cout << "Exiting now..." << endl;
    }

    else
    {
        cout << "!Failed to find a valid command."  << endl;
    }

}

string getData(string line)
{
	auto it = find(line.begin(), line.end(), '(');
	line = string(it, line.end());
	line = line.substr(1, line.size() - 2);
	replace(line.begin(), line.end(), ',', '|');

	return line;
}

string getName(string line)
{
	stringstream ss(line);
	string token;
	while (getline(ss, token, ' '))
	{
	}
	return token;
}

string getTblName(string line)
{
	stringstream ss(line);
	string token;
	int i = 0;
	while (getline(ss, token, ' ') && i < 2)
	{
		i++;
	}
	return token;
}

string getParse(string &input)
{
    stringstream ss(input);
	string token;
    getline(ss, token, ';');


    return token;
}

//imported from https://stackoverflow.com/questions/46292764/check-for-file-existence-in-c-without-creating-file
bool DoesFileExist (const string& name)
{
    return ( access( name.c_str(), F_OK ) != -1 );
}