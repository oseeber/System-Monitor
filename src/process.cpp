#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int n) : pid_(n), user_(LinuxParser::User(pid_)), cmd_(LinuxParser::Command(pid_)), 
    ram_(LinuxParser::Ram(pid_)) {}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 
    float hz = float(sysconf(_SC_CLK_TCK));
    float tot_time = (float)(LinuxParser::ActiveJiffies(pid_));
    float uptime = (float)(LinuxParser::UpTime());
    float starttime = (float)(LinuxParser::UpTime(pid_));
    float seconds = uptime - (starttime/hz);
    float usage = (tot_time/hz)/seconds;
    return usage;
 }

// TODO: Return the command that generated this process
string Process::Command() { return cmd_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    long starttime = LinuxParser::UpTime(pid_);
    long uptime = LinuxParser::UpTime();
    return uptime - starttime;
 }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return (this->CpuUtilization()) > (a.CpuUtilization());
 }