#pragma once
#include <iostream>
#include <fstream>

using namespace std;

class table
{
    public:

        table(string name, string data)
        {
            dbName = name;
            dbData = data;
        };

        string getName()
        {
            return dbName;
        };

        string getData()
        {
            return dbData;
        };
        
        ~table()
        {
            dbName = "";
            dbData = "";
        };

    private:
        string dbName;
        string dbData;


};