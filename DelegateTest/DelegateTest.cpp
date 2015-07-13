// DelegateTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ModelCore/Delegate/Delegate.h"


class FLogWriter
{
public:
	void WriteToLog( const std::string& str )
	{

	}
};

DECLARE_DELEGATE_OneParam( FStringDelegate, std::string );


int _tmain(int argc, _TCHAR* argv[])
{

	std::shared_ptr<FLogWriter> LogWriter(new FLogWriter());
	FStringDelegate WriteToLogDelegate;
	WriteToLogDelegate.BindSP( LogWriter, &FLogWriter::WriteToLog );

	WriteToLogDelegate.Execute( "Delegates are spiffy!");
	return 0;
}

