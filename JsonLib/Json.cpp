#include "Json.h"

#include <assert.h>
#include <stdlib.h> 
#include <sstream>

namespace Json
{
	//////////////////////////////////////////////////////////////////////////////////////
	// null value class
	//////////////////////////////////////////////////////////////////////////////////////
	class NullValue : public Value
	{
	public:
		NullValue() {}
		virtual bool isNull() const override { return true; }
		virtual std::string toString() const;
	};
	std::string NullValue::toString() const
	{
		return "null";
	}

	// some default values
	namespace
	{
		typedef std::shared_ptr<Value> SharedValue;
		typedef std::map<std::string, SharedValue> Object;
		typedef std::vector<SharedValue> Array;

		Object emptyObject;
		Array emptyArray;
		std::string emptyString = "";

		NullValue theNullValue;	// singleton for null
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// Value base class
	// Provides defaults that reduce the complexity of the inherited class definitions.
	//////////////////////////////////////////////////////////////////////////////////////
	bool Value::isInt() const		{ return false; }
	bool Value::isFloat() const		{ return false; }
	bool Value::isString() const	{ return false; }
	bool Value::isBool() const		{ return false; }
	bool Value::isNull() const		{ return false; }
	bool Value::isObject() const	{ return false; }
	bool Value::isArray() const		{ return false; }

	// The accessors will assert if you try and access them incorrectly
	int Value::asInt() const										{ assert(false); return 0; }
	float Value::asFloat() const									{ assert(false); return 0.0f; }
	const std::string& Value::asString() const						{ assert(false); return emptyString; }
	bool Value::asBool() const										{ assert(false); return true; }
	void Value::add(const std::string& key, Value * val)			{ assert(false);}
	void Value::remove(const std::string& key)						{ assert(false); }
	Value& Value::get(const std::string& key)						{ assert(false); return theNullValue; }
	const Value& Value::get(const std::string& key) const			{ assert(false); return theNullValue; }
	Value& Value::operator[](const std::string& key)				{ assert(false); return theNullValue; }
	const Value& Value::operator[](const std::string& key) const	{ assert(false); return theNullValue; }
	void Value::add(Value * val)									{ assert(false);}
	Value& Value::operator[](unsigned int key)						{ assert(false); return theNullValue; }
	const Value& Value::operator[](unsigned int key) const			{ assert(false); return theNullValue; }
	unsigned int Value::size() const								{ assert(false); return 0; }

	//////////////////////////////////////////////////////////////////////////////////////
	// Integer value class
	//////////////////////////////////////////////////////////////////////////////////////
	class IntValue : public Value
	{
	public:
		IntValue(int value) : value(value) {}
		virtual bool isInt() const override { return true; }
		virtual int asInt() const override { return value; }
		virtual std::string toString() const;
	private:
		int value;
	};
	std::string IntValue::toString() const
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// Float value class
	//////////////////////////////////////////////////////////////////////////////////////
	class FloatValue : public Value
	{
	public:
		FloatValue(float value) : value(value) {}
		virtual bool isFloat() const override { return true; }
		virtual float asFloat() const override { return value; }
		virtual std::string toString() const override;
	private:
		float value;
	};
	std::string FloatValue::toString() const
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// String value class
	//////////////////////////////////////////////////////////////////////////////////////
	class StringValue : public Value
	{
	public:
		StringValue(const std::string& value) : value(value) {}
		virtual bool isString() const { return true; }
		virtual const std::string& asString() const override { return value; }
		virtual std::string toString() const override { return "\"" + value + "\""; }
	private:
		std::string value;
	};

	//////////////////////////////////////////////////////////////////////////////////////
	// Bool value class
	//////////////////////////////////////////////////////////////////////////////////////
	class BoolValue : public Value
	{
	public:
		BoolValue(bool value) : value(value) {}
		virtual bool isBool() const { return true; }
		virtual bool asBool() const override { return value; }
		virtual std::string toString() const override;
	private:
		bool value;
	};
	std::string BoolValue::toString() const
	{
		return value ? "true" : "false";
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// object value class
	//////////////////////////////////////////////////////////////////////////////////////
	class ObjectValue : public Value
	{
	public:
		ObjectValue() {}
		virtual bool isObject() const override { return true; }

		virtual std::string toString() const override;
		virtual void add(const std::string& key, Value * val)  override			{ value[key] = SharedValue(val); }
		virtual void remove(const std::string& key) override					{ value.erase(key); }
		virtual Value& get(const std::string& key) override						{ return const_cast<Value &>(static_cast<const Value &>(*this).get(key)); }
		virtual const Value& get(const std::string& key) const override;
		virtual Value& operator[](const std::string& key) override				{ return get(key); }
		virtual const Value& operator[](const std::string& key) const override	{ return get(key); }

	private:
		Object value;
	};
	const Value& ObjectValue::get(const std::string& key) const
	{
		if(value.find(key) == value.end())
		{
			return theNullValue; 
		}
		else 
		{
			return *(value.find(key)->second);
		}
	}
	std::string ObjectValue::toString() const
	{
		std::stringstream ss;
		ss << "{";
		const char * comma = "";
		auto end = value.end();
		for(auto itr = value.begin(); itr != end; ++itr)
		{
			ss << comma;
			ss << "\"" << itr->first << "\":" << itr->second->toString();
			comma = ",";
		}
		ss << "}";
		return ss.str();
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// array value class
	//////////////////////////////////////////////////////////////////////////////////////
	class ArrayValue : public Value
	{
	public:
		ArrayValue() {}
		virtual bool isArray() const override								{ return true; }
		virtual Array& asArray()											{ return value; }
		virtual const Array& asArray() const								{ return value; }
		virtual std::string toString() const override;
		virtual void add(Value * val) override								{ value.push_back(SharedValue(val)); }
		virtual Value& operator[](unsigned int key) override				{ return *value[key]; }
		virtual const Value& operator[](unsigned int key) const override	{ return *value[key]; }
		virtual unsigned int size() const override							{ return value.size(); }
	private:
		Array value;
	};
	std::string ArrayValue::toString() const
	{
		std::stringstream ss;
		ss << "[";
		const char * comma = "";
		auto end = value.end();
		for(auto itr = value.begin(); itr != end; ++itr)
		{
			ss << comma;
			ss << (*itr)->toString();
			comma = ",";
		}
		ss << "]";
		return ss.str();
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// public named construtors (e.g. Json::Value *obj = Json::newObject(); )
	//////////////////////////////////////////////////////////////////////////////////////
	Value * newInt(int value)					{ return new IntValue(value); }
	Value * newFloat(float value)				{ return new FloatValue(value); }
	Value * newString(const std::string& value)	{ return new StringValue(value); }
	Value * newBool(bool value)					{ return new BoolValue(value); }
	Value * newObject()							{ return new ObjectValue(); }
	Value * newArray()							{ return new ArrayValue(); }
	Value * newNull()							{ return &theNullValue; }

	//////////////////////////////////////////////////////////////////////////////////////
	// Implement the parser
	//////////////////////////////////////////////////////////////////////////////////////
	namespace
	{
		// lexer
		struct Token
		{
			enum Type
			{
				eOpenObject = 0,	// {
				eCloseObject,		// }
				eOpenArray,			// [
				eCloseArray,		// ]
				eColon,				// :
				eComma,				// ,
				eTrue,
				eFalse,
				eNull,
				eString,
				eInt,
				eFloat,
				eError,
				eEnd
			};

			Token() : type(eError) {}
			Token(Type type) : type(type) {}
			Token(Type type, const std::string& value) 
				: type(type), value(value) {}

			Type type;
			std::string value;

			std::string toString();
		};

		std::string Token::toString()
		{
			switch(type)
			{
			case	eOpenObject:	return "eOpenObject"; break;
			case	eCloseObject:	return "eCloseObject"; break;
			case	eOpenArray:		return "eOpenArray"; break;
			case	eCloseArray:	return "eCloseArray"; break;
			case	eColon:			return "eColon"; break;
			case	eComma:			return "eComma"; break;
			case	eTrue:			return "eTrue"; break;
			case	eFalse:			return "eFalse"; break;
			case	eNull:			return "eNull"; break;
			case	eString:		return "eString"; break;
			case	eInt:			return "eInt"; break;
			case	eFloat:			return "eFloat"; break;
			case	eError:			return "eError"; break;
			case	eEnd:			return "eEnd"; break;
			default: return "unknown"; break;
			}
		}

		// acts like a generator for the tokens
		class TokenStream
		{
		public:
			TokenStream(const std::string& str)
				: src(str)
				, itr(src.begin())
				, hasErrorMsg(false)
			{}
			bool isFinished() const;
			void advance();
			Token current();
			bool hasError() const;
			const std::string& getError() const;

		private:
			Token scanTrue();
			Token scanFalse();
			Token scanNull();
			Token scanString();
			Token scanNumber();
			bool isNumber();

			Token next();
			void error(const std::string& message);

			Token currentToken;
			std::string src;
			std::string::iterator itr; // current position
			bool hasErrorMsg;
			std::string errorMsg;
		};
		// public
		bool TokenStream::isFinished() const
		{
			return itr == src.end();
		}
		void TokenStream::advance()
		{
			if(!isFinished()) currentToken = next();
		}
		Token TokenStream::current()
		{
			return currentToken;
		}
		bool TokenStream::hasError() const
		{
			return hasErrorMsg;
		}
		const std::string& TokenStream::getError() const
		{
			return errorMsg;
		}
		// private
		Token TokenStream::next()
		{
			assert(!isFinished());

			Token rtn;
			while(isspace(*itr)) ++itr;
			switch(*itr)
			{
			case '{': rtn = Token(Token::eOpenObject); break;
			case '}': rtn = Token(Token::eCloseObject); break;
			case '[': rtn = Token(Token::eOpenArray); break;
			case ']': rtn = Token(Token::eCloseArray); break;
			case ':': rtn = Token(Token::eColon); break;
			case ',': rtn = Token(Token::eComma); break;
			case 't': rtn = scanTrue(); break;
			case 'f': rtn = scanFalse(); break;
			case 'n': rtn = scanNull(); break;
			case '"': rtn = scanString(); break;
			default:
				if(isNumber())
				{
					rtn = scanNumber();
				}
				else
				{
					// TODO: log some error information here
					rtn = Token(Token::eError);
				}
				break;
			}
			++ itr;
			return rtn;
		}
		Token TokenStream::scanTrue()
		{
			if(*++itr == 'r' && *++itr == 'u' && *++itr == 'e')
			{
				return Token(Token::eTrue);
			}
			else
			{
				error("Failed to parse token: 'true'");
				return Token(Token::eError);
			}
		}
		Token TokenStream::scanFalse()
		{
			if(*++itr == 'a' && *++itr == 'l' && *++itr == 's' && *++itr == 'e')
			{
				return Token(Token::eFalse);
			}
			else
			{
				error("Failed to parse token: 'false'");
				return Token(Token::eError);
			}
		}
		Token TokenStream::scanNull()
		{
			if(*++itr == 'u' && *++itr == 'l' && *++itr == 'l')
			{
				return Token(Token::eNull);
			}
			else
			{
				error("Failed to parse token: 'null'");
				return Token(Token::eError);
			}
		}
		Token TokenStream::scanString()
		{
			std::stringstream s;
			++itr;
			while(*itr != '"')
			{
				if(itr == src.end())
				{
					error("Reached end of characters while parsing string.");
					return Token(Token::eError);
				}
				else
				{
					s << *itr;
				}
				++itr;
			}
			return Token(Token::eString, s.str());
		}
		Token TokenStream::scanNumber()
		{
			// TODO: This methood does not handle the exponential part of a number
			bool hasDot = false;
			std::stringstream s;
			if(*itr == '-') 
			{ 
				s << '-';
				++itr;
			}
			while(isdigit(*itr) || (!hasDot && *itr == '.'))
			{
				if(!hasDot && *itr == '.')
				{
					hasDot = true;
					s << *itr;
				}
				else if(isdigit(*itr))
				{
					s << *itr;
				}
				else
				{
					error("Encountered illegal character while parsing number");
				}
				++itr;
			}
			--itr; // YUCK! try and get rid of this.
			if(hasError())
			{
				return Token(Token::eError);
			}
			if(hasDot)
			{
				return Token(Token::eFloat, s.str());
			}
			else
			{
				return Token(Token::eInt, s.str());
			}
		}
		bool TokenStream::isNumber()
		{
			return isdigit(*itr) != 0 || *itr == '-';
		}
		void TokenStream::error(const std::string& message)
		{
			errorMsg = message;
			hasErrorMsg = true;
		}

		// recursive parser
		class Parser
		{
		public:
			Parser(const std::string& str)
				: tokens(str) 
				, hasError(false) {}

			UniqueValue getValue();

		private:
			// helpers
			bool accept(Token::Type type);
			bool expect(Token::Type type);
			bool peek(Token::Type type);
			void next();
			void error(const std::string& message);

			// recursive decent functions
			void object();
			void members();
			void pair();
			void array();
			void elements();
			void value();

			// build helpers
			void push(Json::Value * val)	{ stack.push_back(val); }
			template<typename T> T* pop()	{ Json::Value * rtn = stack.back(); stack.pop_back(); return (T*) rtn; }
			template<typename T> T* top()	{ return (T*) stack.back(); }

			TokenStream tokens;			
			std::vector<Json::Value*> stack; // pointers to the current object that we are adding to

			bool hasError;
			std::string errorMsg;
		};
		UniqueValue Parser::getValue()
		{
			next();
			object();
			if(hasError)
			{
				// TODO: have error reporting as a switch or return somekind of error value or message on the null value
				fprintf(stderr, "Error: %s", errorMsg.c_str());
				return UniqueValue(newNull());
			}
			else
			{
				return UniqueValue(pop<ObjectValue>()); // the client code now owns the value
			}
		}
		bool Parser::accept(Token::Type type)
		{
			if(tokens.current().type == type)
			{
				next();
				return true;
			}
			return false;
		}
		bool Parser::expect(Token::Type type)
		{
			if(accept(type))
			{
				return true;
			}
			else
			{
				error("expect: unexpected symbol");
				return false;
			}
		}
		void Parser::next()
		{
			if(tokens.isFinished())
			{
				// TODO: Should there be an error if we call next and it's finished?
				//error("Reached end of token stream.  Document not valid.");
			}
			else
			{
				tokens.advance();
				if(tokens.hasError())
				{
					error("Lexical Error: " + tokens.getError());
				}
			}
		}
		bool Parser::peek(Token::Type type)
		{
			return tokens.current().type == type;
		}
		void Parser::error(const std::string& message)
		{
			hasError = true;
			errorMsg = message;
		}
		void Parser::object()
		{
			expect(Token::eOpenObject);
			push( newObject() );
			members();
			expect(Token::eCloseObject);
		}
		void Parser::members()
		{
			do {
				pair();
			} while(accept(Token::eComma));
		}
		void Parser::pair()
		{
			std::string key = tokens.current().value;
			if(accept(Token::eString))
			{
				expect(Token::eColon);
				value();
				Json::Value * newVal = pop<Value>();
				top<ObjectValue>()->add(key, newVal);
			}
		}
		void Parser::array()
		{
			expect(Token::eOpenArray);
			push(newArray());
			elements();
			expect(Token::eCloseArray);
		}
		void Parser::elements()
		{
			if(!peek(Token::eCloseArray))
			{
				do {
					value();
					Json::Value * newVal = pop<Value>();
					top<ArrayValue>()->add(newVal);
				} while(accept(Token::eComma));
			}
		}
		void Parser::value()
		{
			std::string val = tokens.current().value;
			if(accept(Token::eTrue))
			{
				push(newBool(true));
			}
			else if(accept(Token::eFalse))
			{
				push(newBool(false));
			}
			else if(accept(Token::eNull))
			{
				push(newNull());
			}
			else if(accept(Token::eString))
			{
				push(newString(val));
			}
			else if(accept(Token::eInt))
			{
				// TODO: what if this fails?
				int intVal = strtol (val.c_str(), NULL, 10);
				push(newInt(intVal));
			}
			else if(accept(Token::eFloat))
			{
				// TODO: what if this fails etc, also there is warning here?
				float floatVal = (float) atof(val.c_str());
				push(newFloat(floatVal));
			}
			else if(peek(Token::eOpenArray))
			{
				array();
			}
			else if(peek(Token::eOpenObject))
			{
				object();
			}
			else
			{
				error("Unexpected Token in stream.");
			}
		}
	}

	UniqueValue parse(const std::string& src)
	{
		return Parser(src).getValue();
	}

	std::string listTokens(const std::string& src)
	{
		std::stringstream ss;
		TokenStream tokens(src);
		while(!tokens.isFinished())
		{
			tokens.advance();
			ss << tokens.current().toString() << "\n";

			if(tokens.current().type == Token::eError)
			{
				ss << "Error: %s" << tokens.getError().c_str() << "\n";
			}
		}
		return ss.str();
	}
}