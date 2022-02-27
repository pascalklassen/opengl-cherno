#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>

namespace test
{
    class Test
    {
    public:
        Test()
        {}

        virtual ~Test()
        {}

        virtual void OnUpdate(float deltaTime)
        {}

        virtual void OnRender()
        {}

        virtual void OnImGuiRender()
        {}
    };

    class TestMenu : public Test
    {
    private:
        std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
        Test*& m_CurrentTest;
    public:
        TestMenu(Test*& pCurrentTest);
        ~TestMenu()
        {}

        virtual void OnImGuiRender() override;

        template <typename T>
        void RegisterTest(const std::string& name)
        {
            std::cout << "Registering Test '" << name << "'." << std::endl;

            m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
        }
    };
}
