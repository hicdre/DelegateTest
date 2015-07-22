#include "stdafx.h"
#include "ModelCore/Delegate.h"
#include <iostream>


class ClassA
{
public:
	void FuncA() {
		std::cout << "ClassA::FuncA" << std::endl;
	}
	void FuncB(int a) {
		std::cout << "ClassA::FuncB:"<< a << std::endl;
	}
	void FuncC(int a, int b) {
		std::cout << "ClassA::FuncC: " << a <<","<<b << std::endl;

	}
};

class ClassB : public SupportThreadLocalWeak<ClassB>
{
public:
	void FuncA() {
		std::cout << "ClassB::FuncA" << std::endl;
	}
	void FuncB(int a) {
		std::cout << "ClassB::FuncB:" << a << std::endl;
	}
	void FuncC(int a, int b) {
		std::cout << "ClassB::FuncC: " << a << "," << b << std::endl;
	}
};




void RunDelegateNoParamsTest()
{
	std::shared_ptr<ClassA> a(new ClassA);
	ClassB b;

	MultiDelegate_OneParam<int> delegate;
	delegate.Add(a, &ClassA::FuncB);
	delegate.Add(a, &ClassA::FuncC, 4);

	delegate.AddW(b.GetWeak(), &ClassB::FuncB);
	delegate.AddW(b.GetWeak(), &ClassB::FuncC, 5);

	delegate.AddF([](int a)
	{
		a++;
		std::cout << "lamdaA:" << a << std::endl;
	});
	int c = 6;
	delegate.AddF([&c](int a)
	{
		c = a;
		std::cout << "lamdaB:" << a << std::endl;
	});

	{
		ClassB d;
		delegate.AddW(d.GetWeak(), &ClassB::FuncB);
		delegate.Execute(66);
		std::cout << "======================" << std::endl;
	}
	delegate.Execute(66);
}