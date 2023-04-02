#pragma once
#include "Plane.h"
enum Error_code { success, overflow, underflow };

const Error_code fail = overflow;

const int maxqueue = 500; //  Queue size
typedef Plane Queue_entry;

class Queue {
public:
	Queue();
	bool empty() const;
	Error_code serve();
	Error_code append(const Queue_entry& item);
	Error_code retrieve(Queue_entry& item) const;

protected:
	int count;
	int front, rear;
	Queue_entry entry[maxqueue];
};


class Extended_queue : public Queue {
public:
	bool full() const;
	int size() const;
	void clear();
	Error_code serve_and_retrieve(Queue_entry& item);
};


