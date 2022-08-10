#include "processor.h"

// TODO: Return the aggregate CPU utilization
Processor::Processor() {}

float Processor::Utilization() {
    long active = LinuxParser::ActiveJiffies();
    long total = LinuxParser::Jiffies();
    float util = (active*1.0)/(total*1.0);
    return util;
}