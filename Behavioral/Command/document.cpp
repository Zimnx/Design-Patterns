#include "document.hpp"
#include "command.hpp"

std::string Clipboard::content = "Clipboard text...";

void Document::set_memento(Document::Memento& memento)
{
	text_ = memento.snapshot_;
}

