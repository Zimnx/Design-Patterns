#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <memory>


class Stock;

class Observer
{
public:
    virtual void update(const Stock& stock) = 0;
    virtual ~Observer()
    {
    }
};


class Stock
{
private:
	std::string symbol_;
	double price_;
    std::vector<std::shared_ptr<Observer>> observers_;
public:
	Stock(const std::string& symbol, double price) : symbol_(symbol), price_(price)
	{

	}

	std::string get_symbol() const
	{
		return symbol_;
	}

	double get_price() const
	{
		return price_;
	}

    void subscribe(const std::shared_ptr<Observer> & observer)
    {
        observers_.push_back(observer);
    }

	void set_price(double price)
	{
		price_ = price;
        for(const auto& observer : observers_)
        {
            observer->update(*this);
        }
	}
};

class Investor : public Observer
{
    std::string name_;
public:
    Investor(const std::string& name) : name_(name)
    {
    }

    void update(const Stock& stock) override
    {
        std::cout << "Index change " << stock.get_symbol() << " to price:" << stock.get_price() << std::endl;
    }
};
