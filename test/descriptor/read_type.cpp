#include "class_file/descriptor/read_type.hpp"

#include <c_string.hpp>
#include <range.hpp>
#include <types.hpp>

bool consteval desc_type() {
	using namespace class_file;

	auto throw_on_error = [](auto){ throw; };

	{
		read_type_descriptor(
			c_string{ "I" }.iterator(),
			[]<typename Type>(Type) {
				if constexpr(!types_are_same<Type, i>) {
					throw;
				}
			},
			throw_on_error
		);
	}
	{
		read_type_descriptor(
			c_string{ "[F" }.iterator(),
			[]<typename Type>(Type type) {
				if constexpr(same_as<Type, class_file::array>) {
					if(type.rank != 1) { throw; }
					if(type.size() != 2) { throw; }
					if(type.component().size() == 0) { throw; }
					if(
						!type.component().have_elements_equal_to(c_string{"F"})
					) { throw; }
				}
				else { throw; }
			},
			throw_on_error
		);
	}
	{
		read_type_descriptor(
			c_string{ "Ljava/lang/String;" }.iterator(),
			[]<typename Type>(Type type) {
				if constexpr(types_are_same<Type, object>) {
					if(!type.have_elements_equal_to(
						c_string{ "java/lang/String" }
					)) { throw; }
				}
				else { throw; }
			},
			throw_on_error
		);
	}
	{
		read_type_descriptor(
			c_string{ "[[[[Ljava/lang/String;" }.iterator(),
			[]<typename Type>(Type type) {
				if constexpr(same_as<Type, class_file::array>) {
					if(!type.component().have_elements_equal_to(
						c_string{ "Ljava/lang/String;" }
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