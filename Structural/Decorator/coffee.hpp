#pragma once

#include <iostream>
#include <string>
#include <memory>

class Coffee
{
public:
    virtual ~Coffee() = default;

    virtual float get_total_price() const = 0;
    virtual std::string get_description() const = 0;
    virtual void prepare() = 0;
};

class CoffeeBase : public Coffee
{
	float price_;
	std::string description_;
public:
    CoffeeBase(float price, const std::string& description) : price_{price}, description_{description}
	{
	}

    float get_total_price() const override
	{
		return price_;
	}

    std::string get_description() const override
	{
		return description_;
    }
};

class Espresso : public CoffeeBase
{
public:
	Espresso(float price = 4.0, const std::string& description = "Espresso")
        : CoffeeBase{price, description}
	{
	}

    void prepare() override
	{
		std::cout << "Making a perfect espresso: 7 g, 15 bar and 24 sec.\n";
	}
};

class Cappuccino : public CoffeeBase
{
public:
	Cappuccino(float price = 6.0, const std::string& description = "Cappuccino")
        : CoffeeBase{price, description}
	{
	}

    void prepare() override
	{
		std::cout << "Making a perfect cappuccino.\n";
	}
};

class Latte : public CoffeeBase
{
public:
	Latte(float price = 8.0, const std::string& description = "Latte")
        : CoffeeBase{price, description}
	{
	}

    void prepare() override
	{
		std::cout << "Making a perfect latte.\n";
	}
};

class CoffeeDecorator : public CoffeeBase
{
    std::unique_ptr<Coffee> coffee_;
public:
    CoffeeDecorator(std::unique_ptr<Coffee> coffee, float price, const std::string& description)
        : CoffeeBase{price, description}, coffee_{move(coffee)}
    {}

    float get_total_price() const override
    {
        return coffee_->get_total_price() + CoffeeBase::get_total_price();
    }

    std::string get_description() const override
    {
        return coffee_->get_description() + " + " + CoffeeBase::get_description();
    }

    void prepare() override
    {
        coffee_->prepare();
    }
};

class Whipped : public CoffeeDecorator
{
public:
    Whipped(std::unique_ptr<Coffee> coffee)
        : CoffeeDecorator{move(coffee), 2.5, "Whipped Cream"}
    {}

    void prepare() override
    {
        CoffeeDecorator::prepare();
        std::cout << "Adding a whipped cream..." << std::endl;
    }
};

class Whisky : public CoffeeDecorator
{
public:
    Whisky(std::unique_ptr<Coffee> coffee) : CoffeeDecorator{move(coffee), 6.0, "Whisky"}
    {}

    void prepare() override
    {
        CoffeeDecorator::prepare();
        std::cout << "Pouring 50cl of whisky..." << std::endl;
    }
};

class ExtraEspresso : public CoffeeDecorator
{
    Espresso espresso_;
public:
    ExtraEspresso(std::unique_ptr<Coffee> coffee) : CoffeeDecorator{move(coffee), 4.0, "Extra espresso"}
    {}

    void prepare() override
    {
        CoffeeDecorator::prepare();
        espresso_.prepare();
    }
};

