#include "stdafx.h"
#include "PropertyChangedHandler.h"

PropertyChangedArgs::PropertyChangedArgs( const std::string& name ) : name_(name)
{

}

const std::string& PropertyChangedArgs::Name() const
{
	return name_;
}


