#include "stdafx.h"
#include "PropertyObject.h"

static std::wstring MultiByteToWide(const std::string& mb, UINT32 code_page = CP_UTF8)
{
	if (mb.empty())
		return std::wstring();

	int mb_length = static_cast<int>(mb.length());
	// Compute the length of the buffer.
	int charcount = MultiByteToWideChar(code_page, 0,
		mb.data(), mb_length, NULL, 0);
	if (charcount == 0)
		return std::wstring();

	std::wstring wide;
	wide.resize(charcount);
	MultiByteToWideChar(code_page, 0, mb.data(), mb_length, &wide[0], charcount);

	return wide;
}

static std::string WideToMultiByte(const std::wstring& wide, UINT32 code_page = CP_UTF8)
{
	int wide_length = static_cast<int>(wide.length());
	if (wide_length == 0)
		return std::string();

	// Compute the length of the buffer we'll need.
	int charcount = WideCharToMultiByte(code_page, 0, wide.data(), wide_length,
		NULL, 0, NULL, NULL);
	if (charcount == 0)
		return std::string();

	std::string mb;
	mb.resize(charcount);
	WideCharToMultiByte(code_page, 0, wide.data(), wide_length,
		&mb[0], charcount, NULL, NULL);

	return mb;
}

PropertyValue::PropertyValue( const std::string& v )
{
	SetStringValue(MultiByteToWide(v));
}


void PropertyValue::SetBoolValue( bool v )
{
	Clear();  boolValue_ = v;  type_ = PropertyBool;
}

void PropertyValue::SetInt32Value( int32_t v )
{
	Clear();  int32Value_ = v;  type_ = PropertyInt32;
}

void PropertyValue::SetUInt32Value( uint32_t v )
{
	Clear();  uint32Value_ = v;  type_ = PropertyUInt32;
}

void PropertyValue::SetInt64Value( int64_t v )
{
	Clear();  int64Value_ = v;  type_ = PropertyInt64;
}

void PropertyValue::SetUInt64Value( uint64_t v )
{
	Clear();  uint64Value_ = v;  type_ = PropertyUInt64;
}

void PropertyValue::SetFloatValue( float v )
{
	Clear();  floatValue_ = v;  type_ = PropertyFloat;
}

void PropertyValue::SetDoubleValue( double v )
{
	Clear();  doubleValue_ = v;  type_ = PropertyDouble;
}

void PropertyValue::SetObjectValue( const Ptr<IObject>& v )
{
	Clear();  
	objectValue_ = v.get(); 
	if (objectValue_)
		objectValue_->AddRef();
	type_ = PropertyObject;
}


void PropertyValue::SetStringValue( const std::wstring& s )
{
	Clear();
	utf16Value_ = new std::wstring(s);
	type_ = PropertyString;
}

void PropertyValue::SetStringValue( const std::string& s )
{
	SetStringValue(MultiByteToWide(s));
}

std::wstring PropertyValue::ToUTF16() const
{
	switch (type_) {
	case PropertyNull:
		return L"(Null)";
	case PropertyBool:
		return boolValue_ ? L"true" : L"false";
	case PropertyInt32:
		{
			wchar_t buffer[2 * _MAX_INT_DIG];

			std::swprintf(buffer, sizeof (buffer) / sizeof (buffer[0]),
				L"%d", int32Value_);
			return std::wstring(buffer);
		}		
	case PropertyUInt32:
		{
			wchar_t buffer[2 * _MAX_INT_DIG];

			std::swprintf(buffer, sizeof (buffer) / sizeof (buffer[0]),
				L"%u", uint32Value_);
			return std::wstring(buffer);
		}
	case PropertyInt64:
		return std::to_wstring(int64Value_);
	case PropertyUInt64:
		return std::to_wstring(uint64Value_);
	case PropertyFloat:
		return std::to_wstring((long double)floatValue_);
	case PropertyDouble:
		return std::to_wstring((long double)doubleValue_);
	case PropertyObject:
		assert(0);
		return L"(Object)";
	case PropertyString:
		assert(utf16Value_);
		return *utf16Value_;
	default:
		return L"(Unknown)";
	}
}

void PropertyValue::Clear()
{
	if (IsObjectValue()) 
	{
		if (objectValue_)
			objectValue_->Release();			
	} 
	else if (IsStringValue())
	{
		if (utf16Value_)
			delete utf16Value_;
	}

	uint64Value_ = 0;
	type_ = PropertyNull;
}

std::string PropertyValue::GetUTF8Value() const
{
	return WideToMultiByte(GetUTF16Value());
}
