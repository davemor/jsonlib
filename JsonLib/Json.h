#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Json
{
	// abstract base class of all value classes
	// the only part of the hierarchy that is visible to the client code
	class Value
	{
	public:
		// query the type
		virtual bool isInt() const;
		virtual bool isFloat() const;
		virtual bool isString() const;
		virtual bool isBool() const;
		virtual bool isNull() const;
		virtual bool isObject() const;
		virtual bool isArray() const;

		// access the value
		virtual int asInt() const;
		virtual float asFloat() const;
		virtual const std::string& asString() const;
		virtual bool asBool() const;

		// access and append to object
		virtual void add(const std::string& key, Value * val);
		virtual void remove(const std::string& key);
		virtual Value& get(const std::string& key);
		virtual const Value& get(const std::string& key) const;
		virtual Value& operator[](const std::string& key);
		virtual const Value& operator[](const std::string& key) const;

		// access and append to array
		virtual void add(Value * val);
		virtual Value& operator[](unsigned int key);
		virtual const Value& operator[](unsigned int key) const;
		virtual unsigned int size() const;

		// stringify the value
		virtual std::string toString() const = 0;
	};

	// named construtors (e.g. Json::Value *obj = Json::newObject(); )
	Value * newObject();
	Value * newArray();
	Value * newInt(int value);
	Value * newFloat(float value);
	Value * newString(const std::string& value);
	Value * newBool(bool value);

	// helpful typedef
	typedef std::unique_ptr<Value> UniqueValue;

	// parser
	UniqueValue parse(const std::string& src);

	// tokeniser
	std::string listTokens(const std::string& src);
}