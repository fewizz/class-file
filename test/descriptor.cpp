#include "class_file/descriptor/method_reader.hpp"

#include <c_string.hpp>
#include <range.hpp>

bool consteval desc_type() {
	using namespace class_file::descriptor;

	auto throw_on_error = [](auto){ throw; };

	{
		read_type(
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
		read_type(
			c_string{ "Ljava/lang/String;" }.iterator(),
			[]<typename Type>(Type type) {
				if constexpr(types_are_same<Type, object>) {
					if(
						!range{ type.class_name }
						.equals_to(c_string{ "java/lang/String" })
					) { throw; }
				}
				else { throw; }
			},
			throw_on_error
		);
	}
	{
		read_type(
			c_string{ "[[[[Ljava/lang/String;" }.iterator(),
			[]<typename Type>(Type type) {
				if constexpr(same_as<Type, class_file::descriptor::array>) {
					if(
						!range{ type.component }
						.equals_to(c_string{ "Ljava/lang/String;" })
					) { throw; }
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