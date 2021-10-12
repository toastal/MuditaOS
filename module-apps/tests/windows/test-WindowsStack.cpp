// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <catch2/catch.hpp>

#include <module-apps/apps-common/WindowsStack.hpp>
#include <module-apps/apps-common/windows/AppWindow.hpp>
#include <ostream>
#include <string>
#include <type_traits>
#include <sstream>

class TestWindow : public gui::AppWindow
{
  public:
    explicit TestWindow(const std::string &name) : gui::AppWindow(nullptr, name)
    {}
};

void add_dummy_windows(
    app::WindowsStack &ws, unsigned int count, std::function<std::string(unsigned int)> naming = [](unsigned int i) {
        return "window_name_" + std::to_string(i);
    })
{
    for (unsigned int i = 0; i < count; ++i) {
        std::string window_name = naming(i);
        ws.push(window_name, std::make_unique<TestWindow>(window_name));
    }
}

TEST_CASE("WindowsStack creation")
{
    app::WindowsStack stack;
    REQUIRE(true);
}

TEST_CASE("iteration")
{
    app::WindowsStack stack;

    SECTION("empty")
    {
        unsigned int i = 0;
        for (const auto &[name, window] : stack) {
            REQUIRE(!name.empty());
            REQUIRE(window != nullptr);
            ++i;
        }
        REQUIRE(i == 0);
    }

    SECTION("not empty")
    {
        unsigned int count = 1;
        add_dummy_windows(stack, count);

        unsigned int i = 0;
        for (const auto &[name, window] : stack) {
            REQUIRE(!name.empty());
            REQUIRE(window != nullptr);
            ++i;
        }
        REQUIRE(i == count);
    }
}

TEST_CASE("window getter by int")
{
    app::WindowsStack stack;

    SECTION("empty")
    {
        REQUIRE(!stack.get(0));
        REQUIRE(!stack.get(1));
        REQUIRE(!stack.get(100));
    }

    SECTION("not empty")
    {
        unsigned int count = 10;
        REQUIRE(count > 1);
        add_dummy_windows(stack, count);
        REQUIRE(stack.get());
        REQUIRE(stack.get(1));
        REQUIRE(stack.get(count - 1));
        REQUIRE(!stack.get(count));
    }
}

TEST_CASE("window getter by string")
{
    app::WindowsStack stack;
    SECTION("empty")
    {
        REQUIRE(!stack.get(""));
        REQUIRE(!stack.get("any_window_name"));
    }

    SECTION("not empty")
    {
        const unsigned int count = 10;
        std::string name_prefix  = "window";
        auto naming_foo          = [name_prefix](unsigned int i) { return name_prefix + std::to_string(i); };
        add_dummy_windows(stack, count, naming_foo);

        REQUIRE(stack.get(naming_foo(count / 2)));
        REQUIRE(!stack.get(naming_foo(count)));
    }
}

TEST_CASE("isEmpty")
{
    app::WindowsStack stack;
    SECTION("empty")
    {
        REQUIRE(stack.isEmpty());
    }

    SECTION("not empty")
    {
        unsigned int count = 10;
        REQUIRE(count > 1);
        add_dummy_windows(stack, count);
        REQUIRE(!stack.isEmpty());
    }
}

TEST_CASE("pop")
{
    app::WindowsStack stack;
    SECTION("empty")
    {
        REQUIRE(!stack.pop());
    }

    SECTION("not empty")
    {
        unsigned int count = 10;
        REQUIRE(count > 1);
        add_dummy_windows(stack, count);
        for (unsigned int i = 0; i < count; ++i) {
            REQUIRE(stack.pop());
        }
        REQUIRE(!stack.pop());
    }
}

TEST_CASE("popTo")
{
    app::WindowsStack stack;

    SECTION("empty")
    {
        REQUIRE(!stack.popTo("window"));
    }

    SECTION("not empty")
    {
        unsigned int count      = 10;
        std::string name_prefix = "window";
        auto naming_foo         = [name_prefix](unsigned int i) { return name_prefix + std::to_string(i); };
        REQUIRE(count > 1);
        add_dummy_windows(stack, count, naming_foo);
        // get back to half of stack
        REQUIRE(stack.popTo(naming_foo(count / 2)));
        // po the rest of stack
        for (unsigned int i = 0; i < count / 2 + 1; ++i) {
            REQUIRE(stack.pop());
        }
        REQUIRE(!stack.pop());
    }
}

TEST_CASE("clearLastWindow")
{
    app::WindowsStack stack;
    SECTION("empty")
    {
        REQUIRE(!stack.popLastWindow());
    }

    SECTION("not empty")
    {
        REQUIRE(!stack.popLastWindow());
        add_dummy_windows(stack, 1);
        REQUIRE(stack.popLastWindow());
        add_dummy_windows(stack, 10);
        REQUIRE(!stack.popLastWindow());
    }
}

TEST_CASE("drop")
{
    app::WindowsStack stack;
    SECTION("empty")
    {
        REQUIRE(!stack.drop(""));
        REQUIRE(!stack.drop("any_window_name"));
    }

    SECTION("not empty")
    {
        const unsigned int count = 10;
        std::string name_prefix  = "window";
        auto naming_foo          = [name_prefix](unsigned int i) { return name_prefix + std::to_string(i); };
        add_dummy_windows(stack, count, naming_foo);

        REQUIRE(stack.drop(naming_foo(count / 2)));
        for (unsigned int i = 0; i < count - 1; ++i) {
            REQUIRE(stack.pop());
        }
        REQUIRE(!stack.pop());
    }
}

TEST_CASE("logStack")
{
    app::WindowsStack stack;
    std::stringstream s;
    SECTION("no windows")
    {
        stack.logStack(s);
        REQUIRE(s.str().empty());
    }

    SECTION("no windows")
    {
        add_dummy_windows(stack, 10);
        stack.logStack(s);
        REQUIRE(!s.str().empty());
    }
}