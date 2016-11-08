#ifndef STACKITY_H
#define STACKITY_H

#include <vector>
#include <chrono>
#include <string>
#include <stack>
#include "Tracer.h"

namespace tns {
    /**
     * The class outputs XML data of runtime method calls and time elapsed
     */
    class Stackity {
    public:
        struct FrameEntry {
            FrameEntry(std::string description, std::string message = std::string("")) :
                    m_start(std::chrono::steady_clock::now()),
                    m_entries(s_frames),
                    m_description(description),
                    m_message(message) {

                if (Tracer::isEnabled()) { // if benchmarking is enabled
                    timeval curTime;
                    gettimeofday(&curTime, NULL);
                    int milli = curTime.tv_usec / 1000;
                    auto tm = localtime(&curTime.tv_sec);
                    Tracer::Trace(DESCRIPTOR,
                                  "%s<FrameEntry callee=\"%s\" description=\"%s\" start=\"%02d:%02d:%03d\">",
                                  std::string(m_entries, '\t').c_str(), m_description.c_str(),
                                  m_message.c_str(),
                                  tm->tm_min, tm->tm_sec, milli);
                    ++s_frames;
                }
            }

            ~FrameEntry() {
                if (Tracer::isEnabled()) { // if benchmarking is enabled
                    --s_frames;
                    std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();

                    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
                            end - m_start).count();

                    auto difference = (float) microseconds / 1000;

                    Tracer::Trace(DESCRIPTOR, "%s<Duration time=\"%.2f\" />\n%s</FrameEntry>",
                                  std::string(m_entries + 1, '\t').c_str(), difference, std::string(m_entries, '\t').c_str());
                }
            }

            int m_entries;
            std::string m_description;
            std::string m_message;
            std::chrono::time_point<std::chrono::steady_clock> m_start;
        };

        static int s_frames;
        static const int DESCRIPTOR = Tracer::Descriptors::BENCHMARK;
    };
}

#endif //STACKITY_H
