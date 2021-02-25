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

//Functions to help assist the main line parser function to help parse the parsed commands
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

//Main function in terms parsing the commands lines
void parseLine(string &input, string &dbUse)
{
    string command = getParse(input);

    if(command.find("CREATE DATABASE") != -1)
    {
        string name = getName(command);
        
        //Create the database and determine if there was an error
        if (system(("mkdir " + name).c_str()) == 0)
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

        //Delete directory and determine if there was an error
		if(system(("rmdir " + name).c_str()) == 0)
        {
            cout << "Database " << name << " Deleted." << endl;
        }
	
		else
		{
            //Checking if in the correct directory
            if(dbUse == "")
            {
                //Delete table and determine if there was an error
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

        //Check and use the desired database, provided in the correct directory
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
            //Checking if in the correct directory
            if (dbUse != "")
            {
                file.open((name + ".txt").c_str(), ios::out);
                file << data << ",";
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
            //Checking if there is a database in use and in the correct directory
            if (dbUse != "")
            {
                //Delete the table and determine if there was an error
                if(system(("rm " + name + ".txt").c_str()) == 0)
                {
                    cout << "Table " << name << " deleted." << endl;
                }
                
                else
                {
                    cout << "!Failed to delete table " << name << " because it does not exist." << endl;
                }
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
            //Checking if there is a database in use and in the correct directory
            if (dbUse != "")
            {
                file.open((name + ".txt").c_str(), ios::in);
                while(getline(file, line)) 
                {
                    int pos = line.find(',');
                    while(pos > 0)
                    {
                        line.replace(pos, 2, " | ");
                        pos = line.find(',');
                    }
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
            //Checking if there is a database in use and in the correct directory
            if (dbUse != "")
            {
                file.open((name + ".txt").c_str(), ios::app);
                file << data << ",";
                file.close();

                cout << "Table " << name << " modified." << endl;
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

//Function dedicated to returning the extra data and ignore the '(' from the parsed line
string getData(string line)
{
	auto it = find(line.begin(), line.end(), '(');
	line = string(it, line.end());
	line = line.substr(1, line.size() - 2);

	return line;
}

//Function to retrieve the file/directory name from the parsed line
string getName(string line)
{
	stringstream ss(line);
	string token;
	while (getline(ss, token, ' '))
    {}

	return token;
}

//Function dedicated to retrieve the table name for creation from the parsed line
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

//Function to retrieve the input line up until the ';'
string getParse(string &input)
{
    stringstream ss(input);
	string token;
    getline(ss, token, ';');

    return token;
}

//Function to determine if a file exists
bool DoesFileExist (const string& name)
{
    return (access(name.c_str(), F_OK ) != -1 );
}