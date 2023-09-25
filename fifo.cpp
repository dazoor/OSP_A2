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
        std::cout << "incorrect number of args" << std::endl;
        return -1;
    }
    // vector of processes init
    std::vector<pcb> processes;

    // open and check data file
    std::ifstream inputFile(argv[0]);
    if (!inputFile.is_open()) {
        std::cout << "Failed to open data file." << std::endl;
        return 1;
    }

    // reading and creating proccesses objects
    std::string line;
    while (std::getline(inputFile, line)) {
        int commaPos = line.find(',');
        if (commaPos != std::string::npos) {
            std::string processIdStr = line.substr(0, commaPos);
            std::string burstTimeStr = line.substr(commaPos + 1);

            std::cout << processIdStr << " " << burstTimeStr << std::endl;

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

    
    while (!processes.empty()) {
        pcb* nextProcess = selectNextProcessFIFO(processes);
        loadPCB(nextProcess);

        // simulating running the process by updating other process' times

        //time used += total time;
        nextProcess->setTotalTime(nextProcess->getTimeUsed() + nextProcess->getTotalTime());
        //total wait tim += (time used - total time);
        nextProcess->setTotalWaitTime(nextProcess->getTimeUsed() - nextProcess->getTotalTime());

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
        int indexToRemove = 0;
        for (int i = 0; i < processes.size(); ++i) {
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




