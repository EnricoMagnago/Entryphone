
#ifndef ENTRYPHONE_HARDWAREMANAGER_H
#define ENTRYPHONE_HARDWAREMANAGER_H

#include <functional>

class HardwareManager {
public:
	typedef std::function<void()> ringBellFun_t;

	HardwareManager(const ringBellFun_t& call_back);

	bool initHardware() const;

	void openDoor();

private:
	const ringBellFun_t ringBellFun;
};


#endif //ENTRYPHONE_HARDWAREMANAGER_H
