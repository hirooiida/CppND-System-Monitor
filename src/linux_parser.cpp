#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>

#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

constexpr long hertz = 100.0;

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
  string os, kernel, version, build;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version >> build;
  }
  return version;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  string unit;
  float mem_total;
  float mem_free;
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::stringstream sstream;
      sstream << line;
      sstream >> key >> value >> unit;
      
      if (key == "MemTotal:") {
        mem_total = std::stof(value);
      } else if (key == "MemFree:") {
        mem_free = std::stof(value);
      }
    }
  }
  return (mem_total - mem_free) / mem_total ;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string systime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> systime >> idletime;
  }
  return std::stol(systime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Refactoring is needed
float LinuxParser::CpuUtilization(int pid) {
  float cpu_utilization = 0;

  string n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13;
  string utime, stime, cutime, cstime;
  string n18, n19, n20, n21;
  string starttime, rest;
    
  string uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  
  string statline;
  std::ifstream statstream(kProcDirectory + to_string(pid) + kStatFilename);
  if (statstream.is_open()) {
    std::getline(statstream, statline);
    std::istringstream statlinestream(statline);
    statlinestream >> n1 >> n2 >> n3 >> n4 >> n5 >> n6 >> n7 >> n8 >> n9 >> n10 >> n11 >> n12 >> n13
                  >> utime >> stime >> cutime >> cstime
                  >> n18 >> n19 >> n20 >> n21
                  >> starttime >> rest;

    long total_time = std::stol(utime) + std::stol(stime);
    total_time = std::stol(cutime) + std::stol(cstime);
    float seconds = std::stol(uptime) - std::stol(starttime) / hertz;
  
    cpu_utilization = (total_time / hertz) / seconds;
  }

  return cpu_utilization;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  string rest;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::stringstream sstream;
      sstream << line;
      sstream >> key >> value >> rest;
      
      if (key == "processes") {
        return std::stoi(value);
      }
    }
  }
  return 0 ;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  string rest;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::stringstream sstream;
      sstream << line;
      sstream >> key >> value >> rest;
      
      if (key == "procs_running") {
        return std::stoi(rest);
      }
    }
  }
  return 0 ;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) +  kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return "";
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  string unit;
  string memory;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) +  kStatusFilename);
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::stringstream sstream;
      sstream << line;
      sstream >> key >> value >> unit;
      
      if (key == "VmSize:") {
        int mem_mb = std::stof(value) / 1000;
        memory = to_string(mem_mb);
      } 
    }
  }
    return memory;
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  string rest;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) +  kStatusFilename);
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::stringstream sstream;
      sstream << line;
      sstream >> key >> value >> rest;
      
      if (key == "Uid:") {
        return value;
      } 
    }
  }
    return "";
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string name;
  string passwd;
  string key;
  string rest;
  
  std::ifstream filestream(kPasswordPath);
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::stringstream sstream;
      sstream << line;
      sstream >> name >> passwd >> key >> rest;
      
      if (key == Uid(pid)) {
        return name;
      } 
    }
  }
    return "";
}

// DONE: Read and return the uptime of a process
// TODO: Refactor the code
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {

  string n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13;
  string n14, n15, n16, n17, n18, n19, n20, n21;
  string starttime, rest;
  
  string statline;
  std::ifstream statstream(kProcDirectory + to_string(pid) + kStatFilename);
  if (statstream.is_open()) {
    std::getline(statstream, statline);
    std::istringstream statlinestream(statline);
    statlinestream >> n1 >> n2 >> n3 >> n4 >> n5 >> n6 >> n7 >> n8 >> n9 >> n10 >> n11 >> n12 >> n13
                  >> n14 >> n15 >> n16 >> n17 >> n18 >> n19 >> n20 >> n21
                  >> starttime >> rest;
  }

  return UpTime() - std::stol(starttime) / sysconf(_SC_CLK_TCK);
}
