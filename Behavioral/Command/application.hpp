#pragma once

#include "document.hpp"
#include "command.hpp"
#include <map>
#include <boost/algorithm/string.hpp>

class MenuItem
{
	std::string caption_;
    std::shared_ptr<Command> cmd_;

public:
    MenuItem(const std::string& caption, std::shared_ptr<Command> cmd)
		: caption_(caption), cmd_(cmd)
	{
	}

    void action()
	{
        std::cout << "Command executed " << caption_ << "..." << std::endl;
		cmd_->execute();
	}

    std::string caption() const
    {
        return caption_;
    }
};


class Application
{
public:
    std::map<std::string, MenuItem> menu_;

public:
    void add_menu(MenuItem&& menu_item)
	{
        menu_.insert(std::make_pair(boost::to_lower_copy(menu_item.caption()), std::move(menu_item)));
	}

	bool execute_action(const std::string& action_name)
	{
        auto menu_item = menu_.find(boost::to_lower_copy(action_name));

        if (menu_item != menu_.end())
		{
            menu_item->second.action();

			return true;
		}

		return false;
	}
};
