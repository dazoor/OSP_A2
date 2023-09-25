#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "types.h"
#include "pcb.h"

// selection function
pcb* selectNextProcessRR(std::vector<pcb>& processes, int& currentIndex, int quantum) {
    // looping through processes, each getting 1 quantum
    while (true) {
        // moving to next process
        currentIndex = (currentIndex + 1) % processes.size(); 
        pcb& process = processes[currentIndex];

        // check for remaining time in process
        if (process.getTotalTime() > 0) {
            // assign quantum time to process or remaining burst time if less
            int timeToRun = std::min(quantum, int(process.getTotalTime()));
            process.setQuantum(timeToRun);

            // returning selected process
            return &process;
        }
    }
}


// loading function for after selection
void loadPCB(pcb* selectedProcess) {
    // checking for null process (none left)
    if (selectedProcess != nullptr) {
        std::cout << "Loading Process ID: " << selectedProcess->getId() << std::endl;
    } else {
        std::cout << "No process selected to load." << std::endl;
    }
}

int main(int argc, char** argv) {
    if (argc != 3)
    {
        std::cout << "Incorrect number of args" << std::endl;
        return -1;
    }
    // vector of processes init
    std::vector<pcb> processes;

    // open and check data file
    std::ifstream inputFile(argv[2]);
    if (!inputFile.is_open()) {
        std::cout << "Failed to open data file." << std::endl;
        return 1;
    }

    // reading and creating proccesses objects
    std::string line;
    while (std::getline(inputFile, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != std::string::npos) {
            std::string processIdStr = line.substr(0, commaPos);
            std::string burstTimeStr = line.substr(commaPos + 1);

            osp2023::id_type processId = std::stoi(processIdStr);
            osp2023::time_type burstTime = std::stoll(burstTimeStr);
            
            // tempProcess for pushing to processes
            pcb tempProcess;
            
            // setting attributes
            tempProcess.setId(processId);
            tempProcess.setTotalTime(burstTime);
            tempProcess.setTimeUsed(0);
            tempProcess.setTotalWaitTime(0);
            
            processes.push_back(tempProcess);

        }
    }


    inputFile.close();

    
    // keeping track of current time
    osp2023::time_type currentTime = 0;
    osp2023::time_type totalTurnaround = 0;
    osp2023::time_type totalWait = 0;
    osp2023::time_type totalResponse = 0;
    int index = 0;

    while (!processes.empty()) {

        
        pcb* nextProcess = selectNextProcessRR(processes, index, std::stoi(argv[1]));
        loadPCB(nextProcess);

        
        // set base response time
        if (nextProcess->getTimeUsed() == 0) {
            nextProcess->setResponseTime(currentTime);
        }

        // simulating running the process by updating other process' times

        //time used += total time;
        nextProcess->setTotalTime(nextProcess->getTimeUsed() + nextProcess->getTotalTime());
        //total wait tim = current time, only if process has ended
        if (nextProcess->getTotalTime() <= 0) {
            nextProcess->setTotalWaitTime(currentTime);
        }
        
        // turnaround = current time + totaltime
        nextProcess->setTurnaroundTime(currentTime + nextProcess->getTotalTime());
        
        // subtract runtime by quantum
        nextProcess->setTotalTime(nextProcess->getTotalTime() - nextProcess->getQuantum());

        //printing values
        std::cout << "\tBurst time remaining: " << nextProcess->getTotalTime() << "ms\n";

        if (nextProcess->getTotalTime() <= 0) {
            std::cout << "\tTurnaround time: " << nextProcess->getTurnaroundTime() << "ms\n";
            std::cout << "\tWaiting time: " << nextProcess->getTotalWaitTime() << "ms\n";
            std::cout << "\tResponse time: " << nextProcess->getResponseTime() << "ms\n\n";
        }
        

        // update current time
        currentTime += nextProcess->getQuantum();

        if (nextProcess->getTotalTime() <= 0) {
            totalTurnaround += nextProcess->getTurnaroundTime();
            totalWait += nextProcess->getTotalWaitTime();
            totalResponse += nextProcess->getResponseTime();
        }
        

        // iterate through others to update wait times
        for (std::vector<pcb>::iterator it = processes.begin(); it != processes.end(); ++it) {
            pcb& process = *it;
            if (process.getId() != nextProcess->getId()) {
                process.setTotalWaitTime(process.getTotalTime() + nextProcess->getTotalTime());
            }
        }

        // process removed from vector once run simulation is completed

        // saftey checking to remove correct project (IF PROCESS IS COMPLETE, BURST TIME = 0)
        // in case first project is not selected like it should be
        if (nextProcess->getTotalTime() == 0) {
            size_t indexToRemove = 0;
            for (size_t i = 0; i < processes.size(); ++i) {
                if (processes[i].getId() == nextProcess->getId()) {
                    indexToRemove = i;
                    break;  
                }
            }
            if (indexToRemove < (processes.size() + 1)) {
                processes.erase(processes.begin() + indexToRemove);
            }
            index += 1;
        }
    }

    std::cout << "PCB Results: RR" << std::endl;

    std::cout << "\t Average Turnaround Time: ";
    double avgTurnaround = static_cast<double>(totalTurnaround) / index;
    std::cout << avgTurnaround << "ms\n";

    std::cout << "\t Average Waiting Time: ";
    double avgWait = static_cast<double>(totalWait) / index;
    std::cout << avgWait << "ms\n";

    std::cout << "\t Average Response Time: ";
    double avgResponse = static_cast<double>(totalResponse) / index;
    std::cout << avgResponse << "ms\n\n";

    return 0;
}