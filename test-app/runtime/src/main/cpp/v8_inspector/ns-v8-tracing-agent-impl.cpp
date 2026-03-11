//
//  ns-v8-tracing-agent-impl.cpp
//  NativeScript
//
//  Created by Igor Randjelovic on 2023. 04. 03..
//  Copyright © 2023. Progress. All rights reserved.
//

//#include <iostream>
//#include <vector>
//#include <string>
#include <sstream>

#include "ns-v8-tracing-agent-impl.h"
#include "Runtime.h"
#include "NativeScriptException.h"
#include "ArgConverter.h"

namespace tns {
    namespace inspector {

        using v8::platform::tracing::TraceBuffer;
        using v8::platform::tracing::TraceBufferChunk;
        using v8::platform::tracing::TraceWriter;
        using v8::platform::tracing::TraceObject;
        using v8::platform::tracing::TracingController;
        using v8::platform::tracing::TraceConfig;
        using v8::platform::tracing::TraceRecordMode;

        void NSInMemoryTraceWriter::AppendTraceEvent(TraceObject *trace_event) {
            if(total_traces_ == 0) {
                // create a v8 JSON trace writer
                json_trace_writer_.reset(TraceWriter::CreateJSONTraceWriter(stream_));
            }

            ++total_traces_;
            json_trace_writer_->AppendTraceEvent(trace_event);
        }

        void NSInMemoryTraceWriter::Flush() {
            if(json_trace_writer_.get() != nullptr) {
                json_trace_writer_->Flush();
            }
        }

        std::string NSInMemoryTraceWriter::getTrace() {
            if(total_traces_ == 0) {
                return "";
            }

            // ensures the trace json is closed
            json_trace_writer_.reset();
            std::string str = stream_.str();

            // clear stream?
            stream_.str("");
            stream_.clear();

            return str;
        }


        TracingAgentImpl::TracingAgentImpl() {
            tracing_controller_ = reinterpret_cast<TracingController*>(Runtime::platform->GetTracingController());
        }

        bool TracingAgentImpl::start() {
            if(!tracing_) {
                tracing_ = true;

                // start tracing...
                current_trace_writer_ = new NSInMemoryTraceWriter();
                tracing_controller_->Initialize(
                        TraceBuffer::CreateTraceBufferRingBuffer(
                                TraceBuffer::kRingBufferChunks,
                                current_trace_writer_
                        )
                );
                // todo: create TraceConfig based on params.
                TraceConfig* config = TraceConfig::CreateDefaultTraceConfig();
                config->AddIncludedCategory("disabled-by-default-v8.cpu_profiler");
                config->SetTraceRecordMode(TraceRecordMode::RECORD_CONTINUOUSLY);
                tracing_controller_->StartTracing(config);
            }

            return true;
        }

        bool TracingAgentImpl::end() {
            if(tracing_) {
                tracing_controller_->StopTracing();

                if(current_trace_writer_ != nullptr) {
                    // store last trace on the agent.
                    lastTrace_ = current_trace_writer_->getTrace();
                    current_trace_writer_ = nullptr;
                }
                tracing_controller_->Initialize(nullptr);

                tracing_ = false;
            }

            return true;
        }

        void TracingAgentImpl::SendToDevtools(v8::Local<v8::Context> context, std::string jsonData) {
            std::string source = R"(
        (() => {
          function processTraceData(traceJSON) {

            try {
              // handle empty traces (e.g. when the user cancels the trace immediately)
              if(!traceJSON || traceJSON.trim().length === 0) {
                traceJSON = "{}";
              }
              const data = JSON.parse(traceJSON);

              for (let i = 0; i < data.traceEvents?.length; i = i + 20) {
                const chunk = data.traceEvents.slice(i, i + 20);

                __inspectorSendEvent(
                  JSON.stringify({
                    method: "Tracing.dataCollected",
                    params: {
                      value: chunk,
                    },
                  })
                );
              }

              __inspectorSendEvent(
                JSON.stringify({
                  method: "Tracing.tracingComplete",
                  params: {
                    dataLossOccurred: false,
                  },
                })
              );
            } catch (err) {
              console.log("[Inspector] Failed to parse trace data", err, traceJSON);
            }
          }
          return processTraceData;
        })();
    )";

            v8::Isolate* isolate = context->GetIsolate();
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolate_scope(isolate);
            v8::HandleScope handle_scope(isolate);

            v8::Local<v8::Script> script;
            bool success = v8::Script::Compile(context, tns::ArgConverter::ToV8String(isolate, source)).ToLocal(&script);
            assert(success && !script.IsEmpty());

            v8::Local<v8::Value> result;
            success = script->Run(context).ToLocal(&result);
            assert(success);

            v8::Local<v8::Function> processTraceData = result.As<v8::Function>();

            v8::Local<v8::Value> args[1] = { tns::ArgConverter::ToV8String(isolate, jsonData) };
            success = processTraceData->Call(context, processTraceData, 1, args).ToLocal(&result);
            assert(success);
        }


    } // namespace inspector
} // namespace tns
