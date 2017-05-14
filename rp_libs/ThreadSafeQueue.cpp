#include "stdafx.h"

#include "ThreadSafeQueue.h"

template <typename T>
ThreadSafeQueue<T>::ThreadSafeQueue(ThreadSafeQueue& other) {
	std::lock_guard<std::mutex> lk(other.dataMutex);
	dataQueue = other.dataQueue;
}

template <typename T>
void ThreadSafeQueue<T>::push(T new_value) {
	std::lock_guard<std::mutex> lk(dataMutex);
	dataQueue.push(new_value);
	dataConVar.notify_one();
}

template <typename T>
void ThreadSafeQueue<T>::waitAndPop(T& value) {
	std::unique_lock<std::mutex> lk(dataMutex);
	dataConVar.wait(lk, [this] { return !dataQueue.empty(); });
	value = dataQueue.front();
	dataQueue.pop();
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::waitAndPop() {
	std::unique_lock<std::mutex> lk(dataMutex);
	dataConVar.wait(lk, [this] { return !dataQueue.empty(); });
	std::shared_ptr<T> res(std::make_shared<T>(dataQueue.front()));
	dataQueue.pop();
	return res;
}

template <typename T>
bool ThreadSafeQueue<T>::tryPop(T& value) {
	std::lock_guard<std::mutex> lk(dataMutex);
	if (dataQueue.empty())
		return false;
	value = dataQueue.front();
	dataQueue.pop();
	return true;
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::tryPop() {
	std::lock_guard<std::mutex> lk(dataMutex);
	if (dataQueue.empty())
		return std::shared_ptr<T>();
	std::shared_ptr<T> res(std::make_shared<T>(dataQueue.front()));
	dataQueue.pop();
	return res;
}

template <typename T>
bool ThreadSafeQueue<T>::empty() {
	std::lock_guard<std::mutex> lk(dataMutex);
	return dataQueue.empty();
}