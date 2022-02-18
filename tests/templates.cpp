#include "Frontend/Wasm/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Specialized templates", "[templates]") {
	std::string moduleName = "defaultModule";
	auto stage =
	    TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <map>
#include <string>
#include <vector>

template <typename T>
T getSomething(T something) {
  return something;
}

template std::string getSomething(std::string something);
template int getSomething(int);
template std::vector<std::string> getSomething(std::vector<std::string>);

template <typename T>
class MyClass {
public:
T myFun(T type) {
	return type;
}
};

template class MyClass<int>;
template class MyClass<std::map<char, std::vector<int>>>;
)";

	auto pythonTestCode = fmt::format(R"(
hi = {moduleName}.getSomething("hi")
self.assertEqual(hi, "hi")

five = {moduleName}.getSomething(5)
self.assertEqual(five, 5)

l = {moduleName}.getSomething(["hi"])
self.assertEqual(l, ["hi"])

my_class_int = {moduleName}.MyClass_int()
self.assertEqual(my_class_int.myFun(25), 25)

my_class_map = {moduleName}.MyClass_map_char_vector_int()
self.assertEqual(my_class_map.myFun()",
	                                  fmt::arg("moduleName", moduleName)) +
	                      R"({'h': [1]}), {'h': [1]}))";

	auto errorCode = stage.runEmbindTest(cppCode, pythonTestCode, moduleName);
	REQUIRE(errorCode == 0);
}
