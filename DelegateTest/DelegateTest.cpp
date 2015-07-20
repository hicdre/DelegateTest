// DelegateTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "ModelCore/Delegate/Delegate.h"
#include "ModelCore/Delegate_NoParams.h"


class FLogWriter
{
public:
	void WriteToLog( const std::string& str )
	{

	}

	void WriteToLogs( std::string str )
	{

	}
	void Func_NoVar()
	{

	}

	void Func_TwoVar(int a, int b)
	{

	}
};

//DECLARE_DELEGATE_OneParam( FStringDelegate, const std::string& );


int _tmain(int argc, _TCHAR* argv[])
{

	std::shared_ptr<FLogWriter> LogWriter(new FLogWriter());
// 	FSimpleDelegate WriteToLogDelegate;
// 	WriteToLogDelegate.BindSP( LogWriter, &FLogWriter::WriteToLog, "xxx" );
// 
// 	WriteToLogDelegate.Execute( "Delegates are spiffy!");
// 
// 	LogWriter = nullptr;
// 
// 	WriteToLogDelegate.Execute("Delegates are spiffy!");
	MultiDelegate_NoParams delegate;
	delegate.Add(LogWriter, &FLogWriter::Func_NoVar);	

	delegate.AddF([]()
	{
		int i;
		i = 0;
	});	

	int k = 3;
	delegate.AddF([&k]()
	{
		k++;
	});


	//SPMethodDelegateBase_NoParams_OneVar<FLogWriter, std::string>::FMethodPtr s =  &FLogWriter::WriteToLog;
	//SPMethodDelegateBase_NoParams_OneVar<FLogWriter, const std::string&> s(LogWriter, &FLogWriter::WriteToLog, "xxx");
	delegate.Add(LogWriter, &FLogWriter::WriteToLogs, std::string("xxx"));

	delegate.Add(LogWriter, &FLogWriter::Func_TwoVar, 5, 6);	

	delegate.Execute();

	LogWriter = nullptr;

	delegate.Execute();




	return 0;
}

