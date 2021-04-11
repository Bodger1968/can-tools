#pragma once
#include <condition_variable>
#include <queue>
#include <mutex>
#include <memory>

template <typename T>
class MesgFifo
{
public:
	explicit MesgFifo() {};
	~MesgFifo() {};
	explicit MesgFifo(const MesgFifo& other)
	{
		std::lock_guard lock(mutex_);
		fifo_ = other.fifo_;
	}

	void Push(T value)
	{
		std::lock_guard lock(mutex_);
		fifo_.push(value);
		condition_variable_.notify_one();
	}
	void WaitAndPop(T& value)
	{
		std::unique_lock lock(mutex_);
		condition_variable_.wait(lock, [this]() {return !fifo_.empty(); });
		value = fifo_.front();
		fifo_.pop();
	}

	std::shared_ptr<const T> WaitAndPop()
	{
		std::unique_lock lock(mutex_);
		condition_variable_.wait(lock, [this]() {return !fifo_.empty(); });
		std::shared_ptr<const T> res(std::make_shared<const T>(fifo_.front()));
		fifo_.pop();
		return res;
	}


	bool TryAndPop(T& value)
	{
		std::lock_guard lock(mutex_);
		if (fifo_.empty())
		{
			return false;
		}
		value = fifo_.front();
		fifo_.pop();
		return true;
	}

	std::shared_ptr<const T> TryAndPop()
	{
		std::lock_guard lock(mutex_);
		if (fifo_.empty())
		{
			return nullptr;
		}
		std::shared_ptr<const T> res(std::make_shared<const T>(fifo_.front()));
		fifo_.pop();
		return res;
	}

	bool Empty() const
	{
		std::lock_guard lock(mutex_);
		return fifo_.empty();
	}

	int Size() const
	{
		return fifo_.size();
	}

private:

	std::queue<T> fifo_;
	mutable std::mutex mutex_;
	std::condition_variable condition_variable_;

};

