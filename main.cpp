#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <queue>
#include "process.h"
#include "algorithm"


/*
 *  Stolen from CMPE250 templates.
 *  Will be used as parser.
 */
template <class Container>
void split(const std::string& str, Container& cont)
{
    std::istringstream iss(str);
    copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         back_inserter(cont));
}

/*
 * compare structs are used to sort according to custom criterion.
 */
struct compare
{
    bool operator() (const process_t& p1,const process_t& p2 )
    {
        return p1.arrival_time>p2.arrival_time;
    }
};

struct compare2
{
    bool operator() (const process_t& p1,const process_t& p2 )
    {
        return p1.priority>p2.priority;
    }
};

struct compare3
{
    inline bool operator() (const process_t& p1, const process_t& p2)
    {
        return (p1.ID < p2.ID);
    }
};
/*
 *  Function print_queue prints the current status of ready queue.
 */
void print_queue(std::priority_queue<process,std::vector<process>, compare2> temp, int time, std::ofstream & out)
{

    out << time << ":HEAD-";
    while(!temp.empty())
    {
        out << temp.top().ID << "[" << temp.top().instruction_counter << "]" ;
        temp.pop();
        if(!temp.empty())
        {
            out << "-";
        }
    }
    if(temp.empty())
    {
        out << "-";
    }
    out << "TAIL" << std::endl;
}
/*
 *  Function get_turnaround calculates the turnaround time
 *  which is finishing time - arrival time.
 */
int get_turnaround(process_t p)
{
    return p.finished - p.arrival_time;
}
/*
 *  Function get_waiting time calculates the total waiting
 *  time of the process which is the turnaround time
 *  minus the execution time.
 */
int get_waitingtime(process_t p)
{
    return get_turnaround(p) - p.execution_time;
}
/*
 *  Function print_stats prints the statistics for
 *  every single process, sorted according to their IDs.
 */
void print_stats( std::vector<process_t> temp,std::ofstream & out)
{
    std::sort(temp.begin(),temp.end(),compare3());
    for(int i=0;i<temp.size();i++)
    {
        out << temp[i].ID << " turnaround = " << get_turnaround(temp[i]) << std::endl;
        out << "Waiting time for " << temp[i].ID << " = " << get_waitingtime(temp[i]) << std::endl;
    }
}

/*
 *  Function get_processes returns a priority queue which includes
 *  all processes that are defined in definition.txt. Processes are
 *  sorted according to their arrival time.
 */

void get_processes(std::priority_queue<process,std::vector<process>, compare>* pq)
{


    std::ifstream infile("definition.txt");
    std::string line;
    //std::priority_queue<process,std::vector<process>, compare> pq;

    while(getline(infile,line))
    {
        std::vector<std::string>input;
        split(line,input);
        process_t process;

        process.ID = input[0];
        process.priority = std::atoi(input[1].c_str());
        process.code = input[2];
        process.arrival_time = std::atoi(input[3].c_str());

        std::ifstream buf;
        buf.open(process.get_filename().c_str());
        while(getline(buf,line))
        {
            std::vector<std::string> itr;
            split(line,itr);
            instruction_t instruction;
            instruction.ID = itr[0];
            instruction.burst_time = atoi(itr[1].c_str());
            process.execution_time += instruction.burst_time;
            process.instructions.push(instruction);

        }
        pq->push(process);

    }

}


int main()
{
    std::ofstream outfile;
    outfile.open("Output.txt");

    int pcount = 0;
    std::vector<process_t> terminated_processes;
    int actual_time = 0; // this variable will store the time.


    std::priority_queue<process,std::vector<process>, compare> a_pq;
    // arrival pq is a heap to store processes according to their arrival time.
    std::priority_queue<process,std::vector<process>, compare2> ready_queue;
    // ready queue is a heap to store processes according to their priority.


    get_processes(&a_pq);        //load all processes into arrival queue.
    pcount = (int)a_pq.size();   //number of processes.


    outfile << "0:HEAD--TAIL" << std::endl;

    /*
     *  Outer loop checks if anything new arrives.
     *  Inner loop executes arrived processes.
     */
    while(!a_pq.empty() || terminated_processes.size() != pcount)
    {
        //std::cout << "a_pq size is : " << a_pq.size() << std::endl;

        /*
         *  This if check is needed because otherwise
         *  while loop terminates when everything arrives
         *  without waiting for termination.
         */
        if(!a_pq.empty())
        {
            process_t arriving = a_pq.top();

            if(ready_queue.empty())
            {
                /*
                 * If ready queue is empty, push the next arriving one.
                 * adjust the time according to arrival time.
                 */
                actual_time = arriving.arrival_time;
                ready_queue.push(arriving);
                print_queue(ready_queue,actual_time,outfile);

                a_pq.pop();
            }
                /*
                 * Since instructions are atomic we may miss
                 * the arrival of a process so we have to check it.
                 */
            else if(arriving.arrival_time <= actual_time)
            {
                ready_queue.push(arriving);
                print_queue(ready_queue,actual_time,outfile);
                a_pq.pop();
            }
            else
            {
                //Nothing else matters.
            }
        }
        if(!ready_queue.empty())
        {
            process_t executing = ready_queue.top();
            ready_queue.pop();

            instruction_t instruction = executing.instructions.front();
            /*
             * If the instruction ID is exit,
             * it is time to terminate the process
             * and store the finishing time.
             */
            if(instruction.ID == "exit")
            {
                actual_time += instruction.burst_time;
                executing.instructions.pop();
                executing.finished = actual_time;
                terminated_processes.push_back(executing);
                print_queue(ready_queue,actual_time,outfile);
            }
            else
            {
                actual_time += instruction.burst_time;
                executing.instructions.pop(); executing.instruction_counter++;
                ready_queue.push(executing);

            }
        }
    }

    /*
     *  Scheduling is done. Write the stats in the output file.
     */

    print_stats(terminated_processes,outfile);
    outfile.close();

    return 0;
}
