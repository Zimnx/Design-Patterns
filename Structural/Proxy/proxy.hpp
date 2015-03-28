#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <mutex>

class Subject
{
public:
	virtual void request() = 0;
    virtual ~Subject() = default;
};

class RealSubject : public Subject
{
	std::string data_;
public:
	RealSubject(const std::string& data) : data_ {data}
	{
		std::cout << "RealSubject's creation" << std::endl;
	}

    RealSubject(const RealSubject&) = delete;
    RealSubject& operator=(const RealSubject&) = delete;

	~RealSubject()
	{
		std::cout << "RealSubject's clean-up" << std::endl;
	}
	
    void request() override
	{
		std::cout << "Called RealSubject.request() with " << data_ << std::endl;
	}
};

class Proxy : public Subject
{
	std::string data_;
    std::unique_ptr<RealSubject> real_subject_;
    std::once_flag init_flag_;
public:
    Proxy(const std::string& data) : data_ {data}, real_subject_{nullptr}
	{
		std::cout << "Proxy's creation" << std::endl;
	}

	void request()
	{
        std::call_once(init_flag_, [this] { real_subject_.reset(new RealSubject(data_));});

		real_subject_->request();
	}
};

class SynchronizingProxy : public Subject
{
    RealSubject real_subject_;
    std::mutex mtx_;
public:
    SynchronizingProxy(const std::string& data) : real_subject_{data}
    {
        std::cout << "Proxy's creation" << std::endl;
    }

    void request()
    {
        std::lock_guard<std::mutex> lk {mtx_};

        real_subject_.request();
    }
};
