#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <algorithm>
#include <cctype>
#include <stdexcept>

class Command;

class Clipboard
{
public:
	static std::string content;
};


class Document
{
	std::string title_;
	std::string text_;
public:
	Document(const std::string& title) : title_(title)
	{
	}

	void copy() const
	{
		Clipboard::content = text_;
	}

	void paste()
	{
		text_ += Clipboard::content;	
	}

	void to_upper()
	{
		std::transform(text_.begin(), text_.end(), text_.begin(), (int (*)(int))std::toupper);
	}

	void to_lower()
	{
		std::transform(text_.begin(), text_.end(), text_.begin(), (int (*)(int)) std::tolower);
	}

	void add_text(const std::string& txt)
	{
		text_ += txt;
	}

	void insert(size_t index, const std::string& txt)
	{
		text_.insert(index, txt);
	}

	void replace(size_t index, size_t num, const std::string& str)
	{
		text_.replace(index, num, str);
	}

	size_t length() const
	{
		return text_.length();
	}

	void print()
    {
        std::cout << "Document print: " << title_  << "\n" << text_ << std::endl;
    }

	class Memento
	{
		std::string snapshot_;
	public:
		Memento(std::string snapshot = "") : snapshot_(snapshot)
		{
		}

		friend class Document;
	};

	Memento create_memento()
	{
		return Memento(text_);
	}

	void set_memento(Memento& memento);
};
