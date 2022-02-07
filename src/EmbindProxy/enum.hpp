#pragma once

#include <string>
#include <vector>

namespace EmbindProxy {
class Enum {
public:
	Enum(std::string const& name, std::string const& fullyQualifiedName);

	/**
	* Creates a string corresponding to the pybind11 conversion of this enum.
	* E.g.
	*   py::enum_<Pet::Kind>(pet, "Kind", py::arithmetic())
	*/
	std::string getEmbind(std::string const& moduleOrClassName) const;

	/**
	* Add an enum value. Should be just the value name.
	* E.g.
	*   enum class MyEnum {
	*      MyValue;
	*   };
	*   Should only add the value 'MyValue' and not 'MyEnum::MyValue'
	*/
	void addValue(std::string const& value);

	void setScoped(bool isScoped);

private:
	// The user defined name of the enum
	std::string m_name;
	std::string m_fullyQualifiedName;
	std::vector<std::string> m_values;
	bool m_isScoped;
};
}    // namespace EmbindProxy
