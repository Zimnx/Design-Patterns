#pragma once

#include <iostream>

class Target
{
public:
	virtual void request() = 0;
    virtual ~Target() = default;
};

class Adaptee
{
public:
	void specific_request()
	{
		std::cout << "Called specific_request()" << std::endl;
	}
};

class ClassAdapter : public Target, private Adaptee
{
public:
    void request() override
	{
		specific_request();
	}
};

class ObjectAdapter : public Target
{
private:
	Adaptee& adaptee_;
public:
	ObjectAdapter(Adaptee& adaptee) : adaptee_(adaptee)
	{
	}
	
    void request() override
	{
		adaptee_.specific_request();
	}
};
