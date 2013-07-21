Json Lib
========
Json Lib is a light weight (2 files) parser for [Json] (http://www.json.org/).  It generates a tree of values and returns a `std::unique_ptr` to the root of the tree.

For Example:

```
Json::UniqueValue val = Json::parse("{\"key0\":\"a string one\",\"key1\":\"a string two\"}");

std::string one = val->get("key0");
std::string two = val->get("key1");
```

This project is _not ment for commercial use_.  It's just a hobby project and has a couple of known flaws.

The API is loosely based on the much more mature [JsonCpp] (http://jsoncpp.sourceforge.net/) and I would recommend that as the best Json parser for C++.