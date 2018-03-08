#pragma once

// to control parallel function
class Semaphore {
private:
	bool lock;
public:
	Semaphore() { lock = true; }
	virtual void signal() {
		lock = false;
	}
	virtual void wait() {
		while(lock) {}
		lock = true;
	}
};