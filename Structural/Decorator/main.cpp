#include <memory>
#include <cassert>
#include "coffee.hpp"

void client(std::unique_ptr<Coffee> coffee)
{
    std::cout << "Description: " << coffee->get_description() << "; Price: " << coffee->get_total_price() << std::endl;
    coffee->prepare();
}

class CoffeeBuilder
{
    std::unique_ptr<Coffee> coffee_;
public:
    template <typename BaseCoffee>
    CoffeeBuilder& create_base()
    {
        coffee_.reset(new BaseCoffee);

        return *this;
    }

    template <typename Condiment>
    CoffeeBuilder& add()
    {
        assert(coffee_);

        coffee_ = std::make_unique<Condiment>(move(coffee_));

        return *this;
    }

    std::unique_ptr<Coffee> get_coffee()
    {
        return move(coffee_);
    }
};

int main()
{
    using namespace std;

    auto cf = make_unique<Whipped>(
                make_unique<Whisky>(
                    make_unique<Whisky>(
                        make_unique<ExtraEspresso>(
                            make_unique<Espresso>()))));
    client(move(cf));

    std::cout << "\n\n";

    CoffeeBuilder cb;

    cb.create_base<Espresso>();
    cb.add<ExtraEspresso>().add<Whisky>();
    cb.add<Whipped>();

    client(cb.get_coffee());
}
