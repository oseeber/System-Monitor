#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() : OperatingSystem_(LinuxParser::OperatingSystem()), Kernel_(LinuxParser::Kernel()), totalProcs_(LinuxParser::TotalProcesses()), 
procsRunning_(LinuxParser::RunningProcesses()), upTime_(LinuxParser::UpTime()), mem_(LinuxParser::MemoryUtilization()), cpu_(Processor()) {}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_.clear();
    vector<int> pids = LinuxParser::Pids();
    for (int pid : pids) {
        Process process(pid);
        processes_.push_back(process);
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return Kernel_; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return mem_; }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return OperatingSystem_; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return procsRunning_; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return totalProcs_; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return upTime_; }