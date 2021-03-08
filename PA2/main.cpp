/*
 * @file main.cpp
 * @author Joshua Insorio (JoshuaInsorio@outlook.com)
 * @date March 6, 2021
 * @brief main program
 */

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
bool DoesFileExist (const string &name);


int main(int argc, char const *argv[])
{
    string input = "";
    string dbUse = "";

    //Main loop
    while(input != ".EXIT")
    {
        cout << "> ";
        getline(cin, input);
        parseLine(input, dbUse);
    }

    return 0;
}

/*
 *  Function:  parseLine 
 *  --------------------
 *  Parses the lines from the script/command line 
 * 
 *  input: Provides direct access to the original input string to parse
 *  dbUse: Provides direct access to the string keeping track of the current directory
 *
 *  returns: Does not return anything, but rather determines the actions to correspond to the commands
 * 
 */
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
            //Checking if in the correct directory
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
                //Create a table with the name provided and insert the given data
                file.open((name + ".txt").c_str(), ios::out);
                file << data << ", ";
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
                //Open table file and print contents, replacing "," with " | "
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
                cout << "!Failed to select from table " << name << " because you are not using a database." << endl;
            }
        }
        file.close();
    }

    else if(command.find("ALTER TABLE") != -1)
    {
        fstream file;
        string line;
        //Specific parsing for after the ADD keyword to retrieve the data
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
                //Open the table with the name provided and append the given data
                file.open((name + ".txt").c_str(), ios::app);
                file << data << ", ";
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
        exit(0);
    }

    else if((command.find("--") != -1) || (command.find("") != -1))
    {
        //Do nothing when coming across SQL comments and blank space
    }

    else
    {
        //Default response to any unrecognized commands(was mainly used for manual testing)
        cout << "!Failed to find a valid command."  << endl;
    }

}

/*
 *  Function:  getData 
 *  --------------------
 *  Helper function dedicated to returning the extra data and ignore the '(' from the parsed line
 * 
 *  line: Provides a copy of command line string input to be parsed specifically for the data part
 *
 *  returns: String of the data to be written into the table txt file
 * 
 */
string getData(string line)
{
	auto it = find(line.begin(), line.end(), '(');
	line = string(it, line.end());
	line = line.substr(1, line.size() - 2);

	return line;
}

/*
 *  Function:  getName 
 *  --------------------
 *  Helper function to retrieve the file/directory name from the parsed line
 * 
 *  line: Provides a copy of command line string input to be parsed specifically for the name of a table/directory desired
 *
 *  returns: String of the name of the table/directory used for the fstream library
 * 
 */
string getName(string line)
{
	stringstream ss(line);
	string token;
	while (getline(ss, token, ' '))
    {}

	return token;
}

/*
 *  Function:  getTblName 
 *  --------------------
 *  Helper function dedicated to retrieve the table name for creation from the parsed line
 * 
 *  line: Provides a copy of command line string input to be parsed specifically for the name of a table
 *
 *  returns: String of the name of the table used for the fstream library
 * 
 */
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

/*
 *  Function:  getParse 
 *  --------------------
 *  Helper function to retrieve the input line up until the ';'
 * 
 *  input: Provides direct access to the original input string to parse
 *
 *  returns: String of the command without the ';' to determine the function and to be further parsed as needed
 * 
 */
string getParse(string &input)
{
    stringstream ss(input);
	string token;
    getline(ss, token, ';');

    return token;
}

/*
 *  Function:  DoesFileExist 
 *  --------------------
 *  Function to determine if a file exists
 * 
 *  name: Provides direct access to the original string containing the name of the file
 *
 *  returns: Boolean determining if the file exists in the current directory
 * 
 */
bool DoesFileExist(const string &name)
{
    return (access(name.c_str(), F_OK ) != -1 );
}