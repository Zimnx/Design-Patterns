#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <iterator>
#include <list>
#include <stdexcept>
#include <memory>

struct StatResult
{
	std::string description;
	double value;

	StatResult(const std::string& desc, double val) : description(desc), value(val)
	{
	}
};

typedef std::vector<StatResult> Results;

class Strategy
{
public:
    virtual void calculate_store(const std::vector<double> &p_data, Results &p_results) = 0;
    virtual ~Strategy() = default;
};

class StrategyAvg : public Strategy
{
public:
    void calculate_store(const std::vector<double> &p_data, Results &p_results)
    {
        double sum = std::accumulate(p_data.begin(), p_data.end(), 0.0);
        double avg = sum / p_data.size();

        StatResult result("AVG", avg);
        p_results.push_back(result);
    }
};

class StrategyMinMax : public Strategy
{
public:
    void calculate_store(const std::vector<double> &p_data, Results &p_results)
    {
        double min = *(std::min_element(p_data.begin(), p_data.end()));
        double max = *(std::max_element(p_data.begin(), p_data.end()));

        p_results.push_back(StatResult("MIN", min));
        p_results.push_back(StatResult("MAX", max));
    }
};

class StrategySum : public Strategy
{
public:
    void calculate_store(const std::vector<double> &p_data, Results &p_results)
    {
        double sum = std::accumulate(p_data.begin(), p_data.end(), 0.0);

        p_results.push_back(StatResult("SUM", sum));
    }
};

class DataAnalyzer
{
	std::vector<double> data_;
    std::shared_ptr<Strategy> strategy_;
public:
	DataAnalyzer()
	{
	}

	void load_data(const std::string& file_name)
	{
		data_.clear();

		std::ifstream fin(file_name.c_str());
		if (!fin)
			throw std::runtime_error("File not opened");

		double d;
		while (fin >> d)
		{
			data_.push_back(d);
		}

		std::cout << "File " << file_name << " has been loaded...\n";
	}

	void save_data(const std::string& file_name) const
	{ 
		std::ofstream fout(file_name.c_str());
		if (!fout)
			throw std::runtime_error("File not opened");

		for(std::vector<double>::const_iterator it = data_.begin(); it != data_.end(); ++it)
			fout << (*it) << std::endl;
	}

    void set_statistics(std::shared_ptr<Strategy> p_strategy)
	{
        strategy_ = p_strategy;
	}

	void calculate(Results& results)
	{
        strategy_->calculate_store(data_, results);
	}
};

void print_results(const Results& results)
{
    for(Results::const_iterator it = results.begin(); it != results.end(); ++it)
		std::cout << it->description << " = " << it->value << std::endl;
}

int main()
{
	Results results;

	DataAnalyzer da;
	da.load_data("data.dat");

    da.set_statistics(std::make_shared<StrategyAvg>());
	da.calculate(results);

    da.set_statistics(std::make_shared<StrategyMinMax>());
	da.calculate(results);

    da.set_statistics(std::make_shared<StrategySum>());
	da.calculate(results);

	print_results(results);

	std::cout << "\n\n";

	results.clear();
	da.load_data("new_data.dat");
	da.calculate(results);

	print_results(results);
}
