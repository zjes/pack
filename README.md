# pack

DTO Tools library

Pack is a data transfer object (DTO) library for converting data between `json`, `yaml`, `protobuf` representations and C++ structs.

##  Short description
Typical C++ struct looks like:

```cpp
struct MyData: public pack::Node
{
    pack::String     str  = FIELD("str-value");
    pack::Float      fl   = FIELD("float-value", pack::Default(10.f));
    pack::StringList list = FIELD("mylist");

    using pack::Node::Node;
    META(MyData, str, fl, list);
};
```
For example, to convert `MyData` to `yaml` just use:

```cpp
MyData data;
data.str = "string";
data.list = {"one", "two"};

if (auto ret = pack::yaml::serialize(data)) {
    std::cout << "Yaml content: " << *ret << std::endl;
} else {
    std::cerr << "Serialization error: " << ret.error() << std::endl;
}
```
or from `json` to `MyData`:
```cpp
MyData data;
if (auto ret = pack::json::deserialize(jsonContent, data)) {
    // Use data
} else {
    std::cerr << "Deserialization error: " << ret.error() << std::endl;
}
```
