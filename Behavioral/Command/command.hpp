#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <memory>

#include "document.hpp"

class Command
{
public:
	virtual void execute() = 0;
	virtual void undo() = 0;
    virtual std::unique_ptr<Command> clone() = 0;

	virtual ~Command() {}
};

class CommandTracker
{
    std::stack<std::unique_ptr<Command>> commands_;
public:
    void record_command(std::unique_ptr<Command> cmd)
    {
        commands_.push(move(cmd));
    }

    std::unique_ptr<Command> pop_last_command()
    {
        if (commands_.empty())
            throw std::out_of_range("No commands in history");

        std::unique_ptr<Command> last_cmd = move(commands_.top());
        commands_.pop();

        return last_cmd;
    }
};

class UndoableCommand : public Command
{
    CommandTracker& cmd_tracker_;
public:
    UndoableCommand(CommandTracker& cmd_tracker) : cmd_tracker_(cmd_tracker)
    {}

    virtual void execute() override
    {
        do_save_state();

        cmd_tracker_.record_command(std::unique_ptr<Command>(clone()));

        do_execute();
    }

protected:
    virtual void do_save_state() = 0;
    virtual void do_execute() = 0;

    CommandTracker& command_tracker() const
    {
        return cmd_tracker_;
    }
};

class PasteCmd : public UndoableCommand
{
    Document& document_;
	size_t prev_length_;

public:
    PasteCmd(Document& document, CommandTracker& cmd_tracker) : UndoableCommand(cmd_tracker), document_(document), prev_length_(0)
	{
	}

protected:
    void do_save_state() override
    {
        prev_length_ = document_.length();
    }

    void do_execute() override
	{
        document_.paste();
	}

public:
    void undo() override
	{
        size_t replacement_length = document_.length() - prev_length_;
        document_.replace(prev_length_, replacement_length, "");
	}

    std::unique_ptr<Command> clone() override
	{
        return std::unique_ptr<PasteCmd>(new PasteCmd(*this));
	}
};

class CopyCmd : public Command
{
    Document& document_;
public:
    CopyCmd(Document &document) : document_(document){}
    void execute()
    {
        document_.copy();
    }
    void undo()
    {
    }
    std::unique_ptr<Command> clone()
    {
        return std::unique_ptr<Command>(new CopyCmd(*this));
    }
};

class ToUpperCmd : public UndoableCommand
{
    Document& document_;
	Document::Memento memento_;

public:
    ToUpperCmd(Document& document, CommandTracker& cmd_tracker) :UndoableCommand(cmd_tracker), document_(document)
	{
	}

    void undo()
    {
        document_.set_memento(memento_);
    }


    std::unique_ptr<Command> clone() override
    {
        return std::unique_ptr<Command>(new ToUpperCmd(*this));
    }

protected:
    void do_save_state() override
    {
        memento_ = document_.create_memento();
    }

    void do_execute() override
    {
        document_.to_upper();
    }
};

class ToLowerCmd : public UndoableCommand
{
    Document& document_;
    Document::Memento memento_;
public:
    ToLowerCmd(Document & document, CommandTracker &cmd_tracker)
        : UndoableCommand(cmd_tracker), document_(document)
    {
    }

    void undo() { document_.set_memento(memento_); }
    std::unique_ptr<Command> clone() override
    {
        return std::unique_ptr<Command>(new ToLowerCmd(*this));
    }
protected:
    void do_save_state() override
    {
        memento_ = document_.create_memento();
    }
    void do_execute() override
    {
        document_.to_lower();
    }
};

class PrintCmd : public Command
{
    Document& document_;
public:
    PrintCmd(Document& document) : document_(document)
	{
	}

	void execute()
	{
        document_.print();
	}

	void undo()
	{
	}

    std::unique_ptr<Command> clone() override
	{
        return std::unique_ptr<Command>(new PrintCmd(*this));
	}
};

class AddTextCmd : public UndoableCommand
{
    Document& document_;
    size_t prev_length_ ;
public:
    AddTextCmd(Document & document, CommandTracker &cmd_tracker)
        : UndoableCommand(cmd_tracker), document_(document)
    {}
    void undo()
    {
        size_t replacement_lenght = document_.length() - prev_length_;
        document_.replace(prev_length_, replacement_lenght, "");
    }
    std::unique_ptr<Command> clone()
    {
        return std::unique_ptr<AddTextCmd>(new AddTextCmd(*this));
    }

protected:
    void do_save_state()
    {
        prev_length_ = document_.length();
    }
    void do_execute()
    {
        std::string input;
        std::cout << "Enter text: ";
        std::cin >> input;
        document_.add_text(input);
    }
};


class UndoCmd : public Command
{
    Document& document_;
    CommandTracker& cmd_tracker_;
public:
    UndoCmd(Document& doc, CommandTracker& cmd_tracker) : document_(doc), cmd_tracker_(cmd_tracker) {}

	void execute()
	{
		try
		{
            auto prevCmd = cmd_tracker_.pop_last_command();
			prevCmd->undo();
        }
		catch(const std::out_of_range& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

    void undo() override
	{
        // empty implementation
	}

    std::unique_ptr<Command> clone() override
    {
        return std::unique_ptr<Command>(new UndoCmd(*this));
	}
};

