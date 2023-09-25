#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "types.h"
#include "pcb.h"

// selection function
pcb* selectNextProcessFIFO(std::vector<pcb>& processes) {
    // FIFO selects first project to arrive
    pcb* nextProcess = nullptr;
    for (std::vector<pcb>::iterator it = processes.begin(); it != processes.end(); ++it) {
        pcb& process = *it;
        if (nextProcess == nullptr) {
            nextProcess = &process;
        }
    }
    return nextProcess;
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
    if (argc != 2)
    {
        std::cout << "Incorrect number of args" << std::endl;
        return -1;
    }
    // vector of processes init
    std::vector<pcb> processes;

    // open and check data file
    std::ifstream inputFile(argv[1]);
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
    
    while (!processes.empty()) {

        
        pcb* nextProcess = selectNextProcessFIFO(processes);
        loadPCB(nextProcess);

        

        if (nextProcess->getTimeUsed() == 0) {
            nextProcess->setResponseTime(currentTime);
        }

        // simulating running the process by updating other process' times

        //time used += total time;
        nextProcess->setTotalTime(nextProcess->getTimeUsed() + nextProcess->getTotalTime());
        //total wait tim += (time used - total time);
        nextProcess->setTotalWaitTime(nextProcess->getTimeUsed() - nextProcess->getTotalTime());
        // turnaround = current time + totaltime
        nextProcess->setTurnaroundTime(currentTime + nextProcess->getTotalTime());

        //printing values
        std::cout << "\tBurst time: " << nextProcess->getTotalTime() << "ms\n";
        std::cout << "\tTurnaround time: " << nextProcess->getTurnaroundTime() << "ms\n";
        std::cout << "\tResponse time: " << nextProcess->getResponseTime() << "ms\n\n";

        // update current time
        currentTime += nextProcess->getTotalTime();

        // iterate through others to update wait times
        for (std::vector<pcb>::iterator it = processes.begin(); it != processes.end(); ++it) {
            pcb& process = *it;
            if (process.getId() != nextProcess->getId()) {
                process.setTotalWaitTime(process.getTotalTime() + nextProcess->getTotalTime());
            }
        }

        // process removed from vector once run simulation is completed

        // saftey checking to remove correct project
        // in case first project is not selected like it should be
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
    }

    return 0;
}




