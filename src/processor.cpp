#include "processor.h"
#include "linux_parser.h"

#include <string>

static float prev_idle{0}, prev_total{0};

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::string line, cpu_;
  int user_, nice_, system_, idle_, iowait_, irq_, softirq_, steal_;
  int idle, non_idle, total;
  int totald, idled;
  float cpu_per;
  
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu_ >> user_ >> nice_ >> system_ >> idle_ >> iowait_ >> irq_ >> softirq_ >> steal_;
    
    idle = idle_ + iowait_;
    non_idle = user_ + nice_ + system_ + irq_ + softirq_ + steal_;
    total = idle + non_idle;
    
    totald = total - prev_total;
    idled = idle - prev_idle;
    
    prev_total = total;
    prev_idle = idle;
    
    cpu_per = (float)(totald - idled) / (float)(totald);
  }

  return cpu_per;
}