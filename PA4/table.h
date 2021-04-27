/*
 * @file table.h
 * @author Joshua Insorio (JoshuaInsorio@outlook.com)
 * @date April 6, 2021
 * @brief header file for the table class
 */


#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <cstring>

using namespace std;

class table
{
    public:
        table()
        {
            id = '\n';
            product[0] = '\n';
            price = '\n';
        }

        table(int pid, char pproduct[20], float pprice)
        {
            id = pid;
            strcpy(product, pproduct);
            price = pprice;
        }

        table(int intiger, char eemployee[10])
        {
            id = intiger;
            strcpy(employee, eemployee);
        }

        table(int eemployeeId, int ppid)
        {
            employeeId = eemployeeId;
            id = ppid;
        }

        table(string pid, string pproduct, string pprice)
        {
            holder1 = pid;
            holder2 = pproduct;
            holder3 = pprice;
        }

        table(string temp1, string temp2)
        {
            holder1 = temp1;
            holder2 = temp2;
        }

        string getHolder1()
        {
            return holder1;
        }
        string getHolder2()
        {
            return holder2;
        }
        string getHolder3()
        {
            return holder3;
        }

        int getID()
        {
            return id;
        }

        int getEID()
        {
            return employeeId;
        }
        
        char *getEmployee()
        {
            return employee;
        }

        char *getProduct()
        {
            return product;
        }

        float getPrice()
        {
            return price;
        }

        void setID(int pid)
        {
            id = pid;
        }

        void setProduct(char pproduct[20])
        {
            strcpy(product, pproduct);
        }

        void setPrice(float pprice)
        {
            price = pprice;
        }

        ~table()
        {
            id = '\n';
            //fix the destructor later
            //delete []product;
            price = '\n';
        }


    private:
        int id;
        int employeeId;
        char product[20];
        char employee[10];
        float price;
        string holder1;
        string holder2;
        string holder3;
};


#endif 