#include "class_file/descriptor/method_reader.hpp"

#include <range.hpp>

// optional isn't constexpr yet.
int main() {
	using namespace class_file;

	nuint param = 0;

	method_descriptor::try_read_parameter_types(
		c_string{ u8"(IF[[[[I)V" }.iterator(),
		[&]<typename Type>(Type type) {
			if constexpr(same_as<Type, i>) {
				if(param++ != 0) { throw 1; }
			}
			else if constexpr(same_as<Type, f>) {
				if(param++ != 1) { throw 2; }
			}
			else if constexpr(same_as<Type, class_file::array>) {
				if(param++ != 2) { throw 3; }
				if(
					!type.component()
					.has_equal_size_and_elements(c_string{ "I" })
					||
					type.rank != 4
				) {
					throw 4;
				}
			}
			else {
				throw 5;
			}
		},
		[&](auto){ throw 6; }
	);

	method_descriptor::try_read_return_type(
		c_string{ u8"(IF[J)D" }.iterator(),
		[&]<typename Type>(Type) {
			if constexpr(!same_as<Type, d>) {
				throw 10;
			}
		},
		[&](auto){ throw 11; }
	);

	method_descriptor::try_read_parameter_and_return_types(
		c_string{ u8"(Lsmth;)[I" }.iterator(),
		[&]<typename ParamType>(ParamType) {
			if constexpr(!same_as<ParamType, class_file::object>) {
				throw 20;
			}
		},
		[&]<typename RetType>(RetType) {
			if constexpr(!same_as<RetType, class_file::array>) {
				throw 21;
			}
		},
		[&](auto) { throw 22; }
	);
}