#pragma once

// to control parallel function
class Semaphore {
private:
	bool lock;
public:
	Semaphore(bool lock = true) { this->lock = lock; }
	virtual void unlock() {
		lock = false;
	}
	virtual void wait() {
		while(lock) {}
	}
	virtual void inlock() {
		lock = true;
	}
};