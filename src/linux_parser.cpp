#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float memTotal, memFree;
  string value, key, line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal") {
        memTotal = stof(value);
      } else if (key == "MemFree") {
        memFree = stof(value);
      } 
    }
  }
  return (memTotal - memFree)/memTotal;
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, uptime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return stol(uptime);
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line, value;
  long utime, stime, cutime, cstime, tot_time;
  int i = 0;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (i == 13) {
        utime = stol(value);
      } else if (i == 14) {
        stime = stol(value);
      } else if (i == 15) {
        cutime = stol(value);
      } else if (i == 16) {
        cstime = stol(value);
      }
      i++;
    }
    tot_time = utime + stime + cutime + cstime;
    return tot_time;
  }
  return tot_time;
 }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> jifs = LinuxParser::CpuUtilization();
  long ajifs = stol(jifs[kUser_]) + stol(jifs[kNice_]) + stol(jifs[kSystem_]) + 
    stol(jifs[kSoftIRQ_]) + stol(jifs[kSteal_]) + stol(jifs[kGuest_]) + 
    stol(jifs[kGuestNice_]);
  return ajifs;
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jifs = LinuxParser::CpuUtilization();
  long ijifs = stol(jifs[kIdle_]) + stol(jifs[kIOwait_]);
  return ijifs;
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, value, key;
  vector<string> cpu_vec {};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
    while (linestream >> value) {
      cpu_vec.push_back(value);
    }
  }
  return cpu_vec;
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value);
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value);
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    
    return line;
  }
  return line;

 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize") {
        int val = stoi(value);
        val = val/1000;
        value = std::to_string(val);
        return value;
      }
    }
  }
  return value;
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid") {
        return value;
      } 
    }
  }
  return value;
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string name, filler, proc_uid, line;
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> filler >> proc_uid;
      if (proc_uid == uid) {
        return name;
      }
    }
  }
  return name;
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, uptime;
  int i = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> uptime) {
      if (i == 21) {
        long time = stol(uptime);
        time = time/sysconf(_SC_CLK_TCK);
        return time;
      }
      i++;
    }
  }
  return stol(uptime);
 }