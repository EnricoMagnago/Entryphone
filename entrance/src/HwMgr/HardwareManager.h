#ifndef ENTRYPHONE_HARDWAREMANAGER_H
#define ENTRYPHONE_HARDWAREMANAGER_H

#include <functional>
#include <atomic>

class HardwareManager {
public:
        typedef std::function<bool()> ringBellFun_t;

        HardwareManager() = delete;

        static bool initHardware(ringBellFun_t call_back);

        static void openDoor();

private:
        static std::atomic_flag already_handled;
        static ringBellFun_t ringBellFun;

        static void call_back();
};


#endif //ENTRYPHONE_HARDWAREMANAGER_H
