#include "class_file/descriptor/field.hpp"

#include <c_string.hpp>
#include <range.hpp>
#include <types.hpp>
#include <overloaded.hpp>

bool consteval desc_type() {
	using namespace class_file;

	{
		c_string descr{ u8"I" };
		read_field_descriptor(
			descr.iterator(),
			overloaded {
				[]<same_as<class_file::i>> {},
				[]<typename...>(auto...) { throw; }
			},
			[](auto){ throw; }
		);
	}
	{
		c_string f_arr{ u8"[F" };

		read_field_descriptor(
			f_arr.iterator(),
			overloaded {
				[]<primitive_type> { throw; },
				[](class_file::object) { throw; },
				[](class_file::array arr) {
					if(arr.rank != 1) { throw; }
					if(arr.size() != 2) { throw; }
					if(arr.element_type().size() != 1) { throw; }
					if(!arr.element_type().has_equal_size_and_elements(
						c_string{ u8"F" }
					)) { throw; }
				}
			},
			[](auto){ throw; }
		);

		read_field_descriptor(
			f_arr.iterator(),
			overloaded {
				[]<typename>(auto...) { throw; },
				[](class_file::object ref) {
					if(!ref.has_equal_size_and_elements(
						c_string{ u8"[F" }
					)) { throw; }
				}
			},
			[](auto){ throw; }
		);
	}
	{
		read_field_descriptor(
			c_string{ u8"Ljava/lang/String;" }.iterator(),
			overloaded {
				[]<primitive_type>{ throw; },
				[](class_file::object obj) {
					if(!obj.has_equal_size_and_elements(
						c_string{ u8"java/lang/String" }
					)) { throw; }
				}
			},
			[](auto){ throw; }
		);
	}
	{
		read_field_descriptor(
			c_string{ u8"[[[[Ljava/lang/String;" }.iterator(),
			overloaded {
				[]<primitive_type>{ throw; },
				[](class_file::object){ throw; },
				[](class_file::array arr) {
					if(!arr.element_type().has_equal_size_and_elements(
						c_string{ u8"Ljava/lang/String;" }
					)) { throw; }
					if(arr.rank != 4) { throw; }
				}
			},
			[](auto){ throw; }
		);
	}

	return true;
}

static_assert(desc_type());

int main() {}