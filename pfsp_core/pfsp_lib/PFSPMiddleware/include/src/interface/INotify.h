#ifndef INOTIFY_H_
#define INOTIFY_H_

class INotify {
public:
	virtual void MwCompNotify(void* pvArg) = 0;
};

#endif	/* INOTIFY_H_ */