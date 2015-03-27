#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <memory>

using namespace std;

#define WINDOWS

class Widget
{
public:
    virtual void draw() = 0;
    virtual ~Widget() = default;
};

class MotifButton : public Widget
{
public:
    void draw() override
    {
        cout << "MotifButton\n";
    }
};

class MotifMenu : public Widget
{
public:
    void draw() override
    {
        cout << "MotifMenu\n";
    }
};

class WindowsButton : public Widget
{
public:
    void draw() override
    {
        cout << "WindowsButton\n";
    }
};

class WindowsMenu : public Widget {
public:
    void draw() override
    {
        cout << "WindowsMenu\n";
    }
};

class WidgetFactory
{
public:
    virtual std::unique_ptr<Widget> create_menu() = 0;
    virtual std::unique_ptr<Widget> create_button() = 0;
    virtual ~WidgetFactory() = default;
};

class MotifWidgetFactory : public WidgetFactory
{
public:
    std::unique_ptr<Widget> create_menu() override
    {
        return std::unique_ptr<Widget>{new MotifMenu()};
    }

    std::unique_ptr<Widget> create_button() override
    {
        return std::unique_ptr<Widget>{new MotifButton()};
    }
};

class WindowsWidgetFactory : public WidgetFactory
{
public:
    std::unique_ptr<Widget> create_menu() override
    {
        return std::unique_ptr<Widget>{new WindowsMenu()};
    }

    std::unique_ptr<Widget> create_button() override
    {
        return std::unique_ptr<Widget>{new WindowsButton()};
    }
};

class Window
{
    std::vector<std::unique_ptr<Widget>> widgets;
public:
    void display() const
    {
        std::cout << "######################\n";
        for(const auto& w : widgets)
            w->draw();
        std::cout << "######################\n\n";
    }

    void add_widget(std::unique_ptr<Widget> widget)
    {
        widgets.push_back(move(widget));
    }
};

class WindowOne : public Window
{
public:
    WindowOne(WidgetFactory& widget_factory)
    {
        add_widget(widget_factory.create_button());
        add_widget(widget_factory.create_menu());
    }
};

class WindowTwo : public Window
{
public:
    WindowTwo(WidgetFactory& widget_factory)
    {
        add_widget(widget_factory.create_menu());
        add_widget(widget_factory.create_button());
        add_widget(widget_factory.create_button());
    }
};

int main(void)
{
#ifdef WINDOWS
    WindowsWidgetFactory widget_factory;
#else
    MotifWidgetFactory widget_factory;
#endif

    WindowOne w1(widget_factory);
    w1.display();

    WindowTwo w2(widget_factory);
    w2.display();
}
