#pragma once
#include "ModelCore/ModelCoreTypes.h"
#include <string>
#include <stdint.h>
#include <cassert>

enum PropertyValueType
{
	PropertyNull = 0,
	PropertyBool,
	PropertyInt32,
	PropertyUInt32,	
	PropertyInt64,
	PropertyUInt64,
	PropertyFloat,
	PropertyDouble,	
	PropertyObject,	
	PropertyString,
};

class MODELCORE_API PropertyValue : public IObject
{
	IMPLEMENT_IOBJECT_THREADSAFE()
public:
	PropertyValue() : type_(PropertyNull) {}
	PropertyValue(bool v) { SetBoolValue(v); }
	PropertyValue(int32_t v) { SetInt32Value(v); }
	PropertyValue(uint32_t v) { SetUInt32Value(v); }
	PropertyValue(int64_t v) { SetInt64Value(v); }
	PropertyValue(uint64_t v) { SetUInt64Value(v); }
	PropertyValue(float v) { SetFloatValue(v); }
	PropertyValue(double v) { SetDoubleValue(v); }
	PropertyValue(const Ptr<IObject>& v) { SetObjectValue(v); }
	PropertyValue(const std::wstring& v) { SetStringValue(v); }
	PropertyValue(const std::string& v);

	inline bool IsBoolValue() const { return type_ == PropertyBool; }
	inline bool IsInt32Value() const { return type_ == PropertyInt32; }
	inline bool IsUInt32Value() const { return type_ == PropertyUInt32; }
	inline bool IsInt64Value() const { return type_ == PropertyInt64; }
	inline bool IsUInt64Value() const { return type_ == PropertyUInt64; }
	inline bool IsFloatValue() const { return type_ == PropertyFloat; }
	inline bool IsDoubleValue() const { return type_ == PropertyDouble; }
	inline bool IsObjectValue() const { return type_ == PropertyObject; }
	inline bool IsStringValue() const { return type_ == PropertyString; }

	inline bool GetBoolValue() const {assert(IsBoolValue());	return boolValue_;}
	inline int32_t GetInt32Value() const { assert(IsInt32Value());	return int32Value_; }
	inline uint32_t GetUInt32Value() const { assert(IsUInt32Value());	return uint32Value_; }
	inline int64_t GetInt64Value() const { assert(IsInt64Value());	return int64Value_; }
	inline uint64_t GetUInt64Value() const { assert(IsUInt64Value());	return uint64Value_; }
	inline float GetFloatValue() const { assert(IsFloatValue());	return floatValue_; }
	inline double GetDoubleValue() const { assert(IsDoubleValue());	return doubleValue_; }
	inline Ptr<IObject> GetObjectValue() const { assert(IsObjectValue());	return objectValue_; }
	inline std::wstring GetUTF16Value() const { assert(IsStringValue());	return *utf16Value_; }
	std::string GetUTF8Value() const;


	void SetBoolValue(bool v);
	void SetInt32Value(int32_t v);
	void SetUInt32Value(uint32_t v);
	void SetInt64Value(int64_t v);
	void SetUInt64Value(uint64_t v);
	void SetFloatValue(float v);
	void SetDoubleValue(double v);
	void SetObjectValue(const Ptr<IObject>& v);
	void SetStringValue(const std::wstring& s);
	void SetStringValue(const std::string& s);

	std::wstring ToUTF16() const;

private:
	void Clear();
	union {
		bool boolValue_;
		int32_t int32Value_;
		uint32_t uint32Value_;
		int64_t	int64Value_;
		uint64_t uint64Value_;
		float	floatValue_;
		double	doubleValue_;
		IObject* objectValue_;
		std::wstring* utf16Value_;
	};
	PropertyValueType type_;
};


class IPropertyObject : public IObject
{
public:
	virtual Ptr<PropertyValue> GetProperty(const std::string& propertyName) = 0;
};