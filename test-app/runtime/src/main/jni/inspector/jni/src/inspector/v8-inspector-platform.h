#ifndef V8_INSPECTOR_PLATFORM_H
#define V8_INSPECTOR_PLATFORM_H

#include "src/libplatform/default-platform.h"
#include "v8-platform.h"

using namespace std;
using namespace v8;
using namespace platform;

namespace v8_inspector {

class V8InspectorPlatform: public DefaultPlatform {
    public:
        explicit V8InspectorPlatform(
            IdleTaskSupport idle_task_support = IdleTaskSupport::kDisabled,
            unique_ptr<TracingController> tracing_controller = {}) {
        }

        void CallDelayedOnWorkerThread(unique_ptr<Task> task, double delay_in_seconds) override {
            DefaultPlatform::CallDelayedOnWorkerThread(move(task), 0);
        }

        static Platform* CreateDefaultPlatform() {
            return NewDefaultPlatform().release();
        }
    private:
        static unique_ptr<Platform> NewDefaultPlatform() {
            unique_ptr<DefaultPlatform> platform(new V8InspectorPlatform());
            platform->EnsureBackgroundTaskRunnerInitialized();
            return move(platform);
        }
};

}

#endif //V8_INSPECTOR_PLATFORM_H
