#include "class_file/descriptor/method_reader.hpp"

#include <range.hpp>

// optional isn't constexpr yet.
int main() {
	using namespace class_file::descriptor;

	nuint param = 0;
	int last_result = 0;

	method::try_read_parameter_types(
		c_string{ "(IF[[[[I)V" }.iterator(),
		[&]<typename Type>(Type type) {
			if constexpr(same_as<Type, i>) {
				if(param++ != 0) { last_result = 1; }
			}
			else if constexpr(same_as<Type, f>) {
				if(param++ != 1) { last_result = 2; }
			}
			else if constexpr(same_as<Type, class_file::descriptor::array>) {
				if(param++ != 2) { last_result = 3; }
				if(
					!type.component.have_elements_equal_to(c_string{ "I" }) ||
					type.rank != 4
				) {
					last_result = 4;
				}
			}
			else {
				last_result = 5;
			}
		},
		[&](auto){ last_result = 6; }
	);

	if(last_result > 0) return last_result;

	method::try_read_return_type(
		c_string{ "(IF[J)D" }.iterator(),
		[&]<typename Type>(Type) {
			if constexpr(!same_as<Type, d>) {
				last_result = 10;
			}
		},
		[&](auto){ last_result = 11; }
	);

	if(last_result > 0) return last_result;
	method::try_read_parameter_and_return_types(
		c_string{ "(Lsmth;)[I" }.iterator(),
		[&]<typename ParamType>(ParamType) {
			if constexpr(!same_as<ParamType, class_file::descriptor::object>) {
				last_result = 20;
			}
		},
		[&]<typename RetType>(RetType) {
			if constexpr(!same_as<RetType, class_file::descriptor::array>) {
				last_result = 21;
			}
		},
		[&](auto) { last_result = 22; }
	);

	return last_result;
}