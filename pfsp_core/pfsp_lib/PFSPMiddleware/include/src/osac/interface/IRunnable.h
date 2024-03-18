#ifndef IRUNNABLE_H_
#define IRUNNABLE_H_

namespace OSAC {

class IRunnable {
public:
	virtual void* Run(void* pvArgs) = 0;
};

}

#endif	/* IRUNNABLE_H_ */