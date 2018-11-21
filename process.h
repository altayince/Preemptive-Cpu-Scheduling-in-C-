//
// Created by altay on 16.11.2018.
//
#pragma once
#include "queue"
#include <string>

typedef struct instruction
{
    std::string ID;
    int burst_time;
}instruction_t;

typedef class process
{
public:
    std::string ID;
    int priority;
    std::string code;
    int arrival_time;
    int finished;
    int instruction_counter = 1;
    int execution_time = 0;
    std::queue<instruction> instructions;

/*
 * generates path to .txt file
 */
    std::string get_filename()
    {
        std::string temp;
        temp = code + ".txt";
        return temp;
    }
}process_t;

