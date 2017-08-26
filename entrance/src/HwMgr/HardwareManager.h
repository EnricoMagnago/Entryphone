
#ifndef ENTRYPHONE_HARDWAREMANAGER_H
#define ENTRYPHONE_HARDWAREMANAGER_H

#include <functional>
#include <atomic>

class HardwareManager {
public:
	typedef std::function<bool()> ringBellFun_t;

	HardwareManager(const ringBellFun_t call_back);

	bool initHardware() const;

	void openDoor();

private:
	static std::atomic_flag already_handled;
	const ringBellFun_t ringBellFun;

	void call_back();
};


#endif //ENTRYPHONE_HARDWAREMANAGER_H
