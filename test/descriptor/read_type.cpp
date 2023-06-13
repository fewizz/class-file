#include "class_file/descriptor/read_type.hpp"

#include <c_string.hpp>
#include <range.hpp>
#include <types.hpp>

bool consteval desc_type() {
	using namespace class_file;

	auto throw_on_error = [](auto){ throw; };

	{
		read_type_descriptor<true>(
			c_string{ u8"I" }.iterator(),
			[]<typename Type>(Type) {
				if constexpr(!types_are_same<Type, i>) {
					throw;
				}
			},
			throw_on_error
		);
	}
	{
		read_type_descriptor<true>(
			c_string{ u8"[F" }.iterator(),
			[]<typename Type>(Type type) {
				if constexpr(same_as<Type, class_file::array>) {
					if(type.rank != 1) { throw; }
					if(type.size() != 2) { throw; }
					if(type.component().size() == 0) { throw; }
					if(
						!type.component().has_equal_size_and_elements(
							c_string{ u8"F" }
						)
					) { throw; }
				}
				else { throw; }
			},
			throw_on_error
		);
	}
	{
		read_type_descriptor<true>(
			c_string{ u8"Ljava/lang/String;" }.iterator(),
			[]<typename Type>(Type type) {
				if constexpr(types_are_same<Type, object>) {
					if(!type.has_equal_size_and_elements(
						c_string{ u8"java/lang/String" }
					)) { throw; }
				}
				else { throw; }
			},
			throw_on_error
		);
	}
	{
		read_type_descriptor<true>(
			c_string{ u8"[[[[Ljava/lang/String;" }.iterator(),
			[]<typename Type>(Type type) {
				if constexpr(same_as<Type, class_file::array>) {
					if(!type.component().has_equal_size_and_elements(
						c_string{ u8"Ljava/lang/String;" }
					)) { throw; }
					if(type.rank != 4) { throw; }
				}
				else { throw; }
			},
			throw_on_error
		);
	}

	return true;
}

static_assert(desc_type());

int main() {}