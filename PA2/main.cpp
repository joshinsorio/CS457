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
#include "table.h"

using namespace std;

//Parses lines from the testing input file and operates accordingly
void parseLine(string &input, string &dbUse, vector<table> &tempTable);

//Functions to help assist the main line parser function to help parse the parsed commands
string getName(string line);
string getTblName(string line);
string getParse(string &input);
string getData(string line);
bool DoesFileExist (const string &name);

//Functions to properly manage(modify/update) tuples in the database
void insertTemp(string name, vector<table> &tempTable);
void saveTable(string name, vector<table> &tempTable);
void rmReturn(string &input);
string getSet(string line, int temp);



int main(int argc, char const *argv[])
{
    string input = "";
    string dbUse = "";
    string appendInput = "";
    vector<table> tempTable;

    //Main loop
    while(input != ".EXIT")
    {
        cout << "> ";
        getline(cin, input);

        //Keep reading input from terminal until ';', '.', "", or "--", then append to the end of the orginal input
        while(input.find(';') == -1 && input.find('.') != 0 && input != "" && input.find("--") == -1)
		{
			getline(cin, appendInput);
			rmReturn(input);
			if (appendInput != "")
			{
				input += appendInput;
			}
		}
        parseLine(input, dbUse, tempTable);
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
void parseLine(string &input, string &dbUse, vector<table> &tempTable)
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
        {
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
                file << data << endl;
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

    else if((command.find("SELECT * FROM") != -1) || (command.find("select * from") != -1))
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
                        line.replace(pos, 1, "|");
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

    else if(command.find("insert into") != -1)
    {
        fstream file;
        string name = getTblName(command);
        string data = getData(command);

        //Simple command to remove whitespace from the original input
        data.erase(remove_if(data.begin(), data.end(), ::isspace), data.end());

        //Determine if the table already exists
        if(!DoesFileExist((name + ".txt").c_str()))
        {
            cout << "!Failed to create table " << name << " because it already exists." << endl;
        }
        
        else
        {
            //Checking if in the correct directory
            if (dbUse != "")
            {
                //Open the table with the name provided and append the given data
                file.open((name + ".txt").c_str(), ios::app);
                file << data << endl;
                cout << "1 new record inserted." << endl;
                file.close();
            }

            else
            {
                cout << "!Failed to create table " << name << " because there is no database in use." << endl;
            }
        }
    }

    else if(command.find("update") != -1)
    {
        int recordMCount = 0;
        string name = getSet(command, 1);
        string updateLocation = getSet(command, 3);
        string update = getSet(command, 5);
        string originalLocation = getSet(command, 9);
        insertTemp(name, tempTable);
        
        if(updateLocation == "price")
        {
            //Find the location of the tuple to be modified
            for (int i = 0; i < tempTable.size(); i++)
            {
                if(tempTable[i].getProduct() == originalLocation)
                {
                    tempTable[i].setPrice(stof(update));
                    ++recordMCount;
                }
            }
            cout << recordMCount << " record(s) modified." << endl;
        }
        
        else if(updateLocation == "name")
        {
            char temp[20];

            //Find the location of the tuple to be modified
            for (int i = 0; i < tempTable.size(); i++)
            {
                if(tempTable[i].getProduct() == originalLocation)
                {
                    tempTable[i].setProduct(strcpy(temp, update.c_str()));
                    ++recordMCount;
                }
            }
            cout << recordMCount << " record(s) modified." << endl;
        }
        saveTable(name, tempTable);
    }

    else if(command.find("delete") != -1)
    {
        int recordDCount = 0;
        string name = getSet(command, 2);
        string update = getSet(command, 4);
        string operand = getSet(command, 5);
        string updateLocation = getSet(command, 6);

        insertTemp(name, tempTable);

        if(update == "name")
        {
            if(operand == "=")
            {
                //Find the location of the tuple to be modified
                for (int i = 0; i < tempTable.size(); i++)
                {
                    if(tempTable[i].getProduct() == updateLocation)
                    {
                        tempTable.erase(tempTable.begin() + i);
                        ++recordDCount;
                    }
                }
                cout << recordDCount << " record(s) deleted." << endl;
            }
        }
        
        else if(update == "price")
        {
            if(operand == ">")
            {
                //Find the location of the tuple to be modified
                for (int i = 0; i < tempTable.size(); i++)
                {
                    if(tempTable[i].getPrice() > stof(updateLocation))
                    {
                        tempTable.erase(tempTable.begin() + i);
                        ++recordDCount;
                    }
                }
                cout << recordDCount << " record(s) deleted." << endl;
            }
        }

        //SPACE FOR MORE CASES

        saveTable(name, tempTable);
    }

    else if(command.find("select") != -1)
    {
        int selectionAdjust = 0;
        string selection1 = getSet(command, 1);
        string selection2 = "";
        string operand = "";
        string updateLocation = "";
        string update = "";
        string name = "";

        //Check if there is another selection
        if(selection1.find(",") != -1)
        {
            selection1.pop_back();
            selection2 = getSet(command, 2);
            selectionAdjust++;

            //Determine file name after determining how many selection are desired
            name = getSet(command, 3 + selectionAdjust);
            update = getSet(command, 5 + selectionAdjust);
            operand = getSet(command, 6 + selectionAdjust);
            updateLocation = getSet(command, 7 + selectionAdjust);

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
                    insertTemp(name, tempTable);

                    if(operand == "!=")
                    {
                        //Find the location of the tuple to be modified
                        for (int i = 1; i < tempTable.size(); i++)
                        {
                            if(update == "pid")
                            {
                                if(selection1 == "name")
                                {
                                    if(selection2 == "price")
                                    {                                        
                                        if(tempTable[i].getID() != stoi(updateLocation))
                                        {
                                            cout << tempTable[0].getHolder2() << "|" << tempTable[0].getHolder3() << endl;
                                            cout << tempTable[i].getProduct() << "|" << tempTable[i].getPrice() << endl;
                                        }
                                    }
                                }
                            }

                            //else if (update == "name")
                            //{
                            //}

                            //SPOT FOR EXTRA CASES

                        }
                    }
                }

                else
                {
                    cout << "!Failed to select from table " << name << " because you are not using a database." << endl;
                }

            }
        }
    }
    
    else if((command.find(".EXIT") != -1) || (command.find(".exit") != -1))
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
 *  Function:  getSet 
 *  --------------------
 *  Helper function dedicated to retrieve the desired value to be modified
 * 
 *  line: Provides a copy of command line string input to be parsed specifically for the name of a table
 *  temp: Provide the specific location of the command
 *
 *  returns: String of the name of the desired tuple to be modified
 * 
 */
string getSet(string line, int temp)
{
    stringstream ss(line);
	string token;
	int i = 0;
	while (getline(ss, token, ' ') && i < temp)
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
 *  Function:  rmReturn 
 *  --------------------
 *  Helper function to remove the return lines from the input
 * 
 *  line: Provides a copy of command line string input to be parsed specifically for the name of a table
 *
 *  returns: String of the command without the return carriages
 * 
 */
void rmReturn(string &input)
{
    string myString;

    //Removing return carriage from the input
	if (input.find('\r') == string::npos)
	{
		return;
	}

	else
	{
		for(int i = 0; i < input.size(); i++)
		{
			if (input[i] == '\r')
			{
				continue;
			}
			else
			{
				myString.push_back(input[i]);
			}
		}
		input = myString;
		return;
	}

}

/*
 *  Function:  insertTemp 
 *  --------------------
 *  Helper function to read the tuples from the database txt file and insert them into a temporary vector class for modification/updates
 * 
 *  name: name of the database txt file to be accessed with fstream
 * 
 *  tempTable: class table to hold the tuples
 * 
 */
void insertTemp(string name, vector<table> &tempTable)
{
    fstream file;

    //Open the table with the name provided
    file.open((name + ".txt").c_str(), ios::in);

    string token;
    string token2;
    string temp;
    vector<string> array;
    char product[20];
    int i = 0;
    int j = 0;

    //Clear temptable
    tempTable.clear();

    //Read from the fstream file and insert into a temporary array
    while(getline(file, token, '\n'))
    {
        stringstream ss(token);
        while(getline(ss, token2, ','))
        {
            array.push_back(token2);
            //cout << token2 <<  "---" << endl;
        }
        //token.erase(std::remove(token.begin(), token.end(), '\n'), token.end());
    }

    while(i < array.size())
    {
        //cout << array[i] << "," << array[i+1] << "," <<  array[i+2] << endl;
        if(i != 0)
        {
            //Convert string to int, char, and float to be inserted into table class for modifications
            table table(stoi(array[i]), strcpy(product, array[i+1].c_str()), stof(array[i+2]));
            tempTable.push_back(table);
        }
        else
        {
            table table(array[i], array[i+1], array[i+2]);
            tempTable.push_back(table);
        }
        i += 3;
    }
    
    file.close();
}

/*
 *  Function:  saveTable 
 *  --------------------
 *  Function to save the modified table into the table txt file
 * 
 *  name: Provides direct access to the original string containing the name of the file
 * 
 *  tempTable: class table that holds all the tuples
 * 
 */
void saveTable(string name, vector<table> &tempTable)
{
    fstream file;

    //Create a table with the name provided and insert the given data
    file.open((name + ".txt").c_str(), ios::out);
    for(int i = 0; i < tempTable.size(); i++)
    {
        if(i != 0)
        {
            file << tempTable[i].getID() << "," << tempTable[i].getProduct() << "," << tempTable[i].getPrice() << endl;
        }
        else
        {
            file << tempTable[i].getHolder1() << "," << tempTable[i].getHolder2() << "," << tempTable[i].getHolder3() << endl;
        }
    }
    file.close();
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