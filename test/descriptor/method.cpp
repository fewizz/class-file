#include "class_file/descriptor/method.hpp"

#include <range.hpp>

// optional isn't constexpr yet.
int main() {
	nuint param = 0;

	class_file::method_descriptor::try_read_parameter_types(
		c_string{ u8"(IF[[[[I)V" }.iterator(),
		overloaded {
			[]<class_file::primitive_type>{ throw; },
			[&]<same_as<class_file::i>> {
				if(param++ != 0) { throw; }
			},
			[&]<same_as<class_file::f>> {
				if(param++ != 1) { throw; }
			},
			[](class_file::object) { throw; },
			[&](class_file::array a) {
				if(param++ != 2) { throw; }
				if(
					!a.element_type().has_equal_size_and_elements(
						c_string{u8"I"}
					) ||
					a.rank != 4
				) {
					throw;
				}
			}
		},
		[](auto){ throw; }
	);

	class_file::method_descriptor::try_read_return_type(
		c_string{ u8"(IF[J)D" }.iterator(),
		overloaded {
			[]<same_as<class_file::d>> {},
			[]<typename...>(auto...) { throw; }
		},
		[](auto){ throw; }
	);

	class_file::method_descriptor::try_read_parameter_and_return_types(
		c_string{ u8"(Lsmth;)[I" }.iterator(),
		overloaded {
			[]<typename...>(auto...){ throw; },
			[](class_file::object){ },
		},
		overloaded {
			[]<typename...>(auto...){ throw; },
			[](class_file::array a){
				if(
					!a.element_type()
					.has_equal_size_and_elements(c_string{ u8"I" }) ||
					a.rank != 1
				) {
					throw;
				}
			},
		},
		[&](auto) { throw; }
	);

	return 0;
}