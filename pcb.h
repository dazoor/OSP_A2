#include <iostream>
#include "types.h"

#pragma once  // tells the compiler to only include this file once
              /**
 * the process control block - represents a process that needs to be executed in
 * our simulator. Please note the I used some custom types of osp2023::time_type
 * and osp2023::id_type. these are just aliases to long integers but they are
 * more meaningful type names, hinting at how they will be used. Please see
 * their definitions in types.h.
 **/
class pcb {
    // the unique process id
    osp2023::id_type id;
    // the total time that a process / job should run for
    osp2023::time_type total_time;
    // the time that has been used so far for this process
    osp2023::time_type time_used;
    // the priority level for this process
    // how much time has this process spent waiting for the cpu?
    osp2023::time_type total_wait_time;
    // what time was this process last on the cpu?

    osp2023::time_type turnaround_time;
    osp2023::time_type response_time;

    osp2023::time_type quantum_time;

   public:
    // max and min duration for a process in our system.
    static constexpr osp2023::time_type MAX_DURATION = 100;
    static constexpr osp2023::time_type MIN_DURATION = 10;

    // getters 
     osp2023::id_type getId() const {
        return id;
    }

    osp2023::time_type getTotalTime() const {
        return total_time;
    }

    osp2023::time_type getTimeUsed() const {
        return time_used;
    }

    osp2023::time_type getTotalWaitTime() const {
        return total_wait_time;
    }

    osp2023::time_type getTurnaroundTime() const {
        return turnaround_time;
    }

    osp2023::time_type getResponseTime() const {
        return response_time;
    }

    osp2023::time_type getQuantum() {
        return quantum_time;
    }

    // setters
    void setId(osp2023::id_type newId) {
        id = newId;
    }

    void setTotalTime(osp2023::time_type newTotalTime) {
        total_time = newTotalTime;
    }

    void setTimeUsed(osp2023::time_type newTimeUsed) {
        time_used = newTimeUsed;
    }

    void setTotalWaitTime(osp2023::time_type newTotalWaitTime) {
        total_wait_time = newTotalWaitTime;
    }

    void setTurnaroundTime(osp2023::time_type newTurnaround_time) {
        turnaround_time = newTurnaround_time;
    }

    void setResponseTime(osp2023::time_type newResponse_time) {
        response_time = newResponse_time;
    }

    void setQuantum(int quantum) {
        quantum_time = quantum;
    }
};
