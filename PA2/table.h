/*
 * @file table.h
 * @author Joshua Insorio (JoshuaInsorio@outlook.com)
 * @date March 6, 2021
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

        int getID()
        {
            return id;
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
        char product[20];
        float price;
};


#endif 