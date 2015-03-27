#include "document.hpp"
#include "command.hpp"
#include "application.hpp"

#include <stack>
#include <string>

using namespace std;

class DocApplication : public Application
{
    Document doc_;
    CommandTracker cmd_tracker_;
public:
    DocApplication() : doc_{"Design Pattern: Command"}
    {
        unique_ptr<Command> cmd_paste(new PasteCmd(doc_, cmd_tracker_));
        unique_ptr<Command> cmd_undo(new UndoCmd(doc_, cmd_tracker_));
        unique_ptr<Command> cmd_print(new PrintCmd(doc_));
        unique_ptr<Command> cmd_toupper(new ToUpperCmd(doc_, cmd_tracker_));
        unique_ptr<Command> cmd_tolower(new ToLowerCmd(doc_, cmd_tracker_));
        unique_ptr<Command> cmd_copy(new CopyCmd(doc_));
        unique_ptr<Command> cmd_addText(new AddTextCmd(doc_, cmd_tracker_));

        auto menu_paste = MenuItem {"Paste", move(cmd_paste)};
        auto menu_toupper = MenuItem {"ToUpper", move(cmd_toupper)};
        auto menu_undo = MenuItem {"Undo", move(cmd_undo)};
        auto menu_print = MenuItem {"Print", move(cmd_print)};
        auto menu_copy = MenuItem {"Copy", move(cmd_copy)};
        auto menu_tolower = MenuItem {"ToLower", move(cmd_tolower)};
        auto menu_add = MenuItem {"Add", move(cmd_addText)};

        add_menu(move(menu_paste));
        add_menu(move(menu_toupper));
        add_menu(move(menu_print));
        add_menu(move(menu_undo));
        add_menu(move(menu_copy));
        add_menu(move(menu_tolower));
        add_menu(move(menu_add));

    }
};

int main()
{
    DocApplication app;

    const std::string exit_application = "end";

	std::string cmd;
	do
	{
        std::cout << "Enter command: ";
		std::cin >> cmd;
		app.execute_action(cmd);
    } while (cmd != exit_application);

    std::cout << "Goodbye!" << std::endl;
}

