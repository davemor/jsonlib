#include "Json.h"

#define CATCH_CONFIG_MAIN
#include "Catch.h"

bool test(const std::string& src)
{
	Json::UniqueValue val = Json::parse(src);
	std::string str = val->toString();
	return src.compare(str) == 0;
}

TEST_CASE( "Parse and stringify simple objects", "[json/stringify/simple]" ) 
{
	REQUIRE( test("{}"));
	REQUIRE( test("{\"key0\":true}"));
	REQUIRE( test("{\"key0\":false}"));
	REQUIRE( test("{\"key0\":null}"));
	REQUIRE( test("{\"key0\":\"astring\"}"));
	REQUIRE( test("{\"key0\":123456}"));
}

TEST_CASE( "Parse and stringify complex objects", "[json/stringify/objects]" ) 
{
	REQUIRE( test("{\"key0\":\"astringone\",\"key1\":\"astringtwo\"}"));
	REQUIRE( test("{\"key0\":\"astringone\",\"key1\":\"astringtwo\",\"key2\":\"astringthree\"}"));
	REQUIRE( test("{\"key\":{\"key0\":true}}"));
	REQUIRE( test("{\"key0\":{\"key1\":{\"key3\":\"astringone\"}}}"));
}

TEST_CASE( "Parse and stringify arrays", "[json/stringify/arrays]" ) 
{
	REQUIRE( test("{\"key0\":[\"astringtwo\",\"astringtwo\"]}"));
}

TEST_CASE( "Take a large object and query it", "[json/query]" ) 
{
	Json::UniqueValue val = Json::parse("{\"Name\":\"JohnDoe\",\"PermissionToCall\":true,\"PhoneNumbers\":[{\"Location\":\"Home\",\"Number\":\"555-555-1234\"},{\"Location\":\"Work\",\"Number\":\"555-555-9999Ext.123\"}]}"); 
	REQUIRE(val->isObject());
	REQUIRE(val->get("Name").isString());
	REQUIRE(val->get("name").isNull());
	REQUIRE((*val)["Name"].isString());
	REQUIRE((*val)["name"].isNull());
}
