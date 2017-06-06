//
// Created by Panayot Cankov on 26/05/2017.
//

#include "ManualInstrumentation.h"

bool tns::instrumentation::Frame::disabled = true;
const std::chrono::steady_clock::time_point tns::instrumentation::Frame::disabled_time = std::chrono::steady_clock::time_point();
