#pragma once

#include <span.hpp>
#include <array.hpp>
#include <c_string.hpp>

#include <unicode/utf8.hpp>

namespace class_file {

	template<utf8::unit Term>
	struct base {
		static constexpr utf8::unit term = Term;

		constexpr auto utf8_string() {
			return c_string_of_known_size<utf8::unit>{ &term, 1 };
		}
	};

	struct v : base<u8'V'>{};

	struct b : base<u8'B'>{};
	struct c : base<u8'C'>{};
	struct d : base<u8'D'>{};
	struct f : base<u8'F'>{};
	struct i : base<u8'I'>{};
	struct j : base<u8'J'>{};
	struct s : base<u8'S'>{};
	struct z : base<u8'Z'>{};

	struct object : c_string_of_known_size<utf8::unit> {
		using base_type = c_string_of_known_size<utf8::unit>;
		using base_type::base_type;

		constexpr object(base_type str) : base_type{ str } {}
	};

	struct array : object {
		uint8 rank;

		constexpr array(c_string_of_known_size<utf8::unit> str, uint8 rank) :
			object{ str }, rank{ rank }
		{}

		constexpr object element_type() {
			return { iterator() + rank, size() - rank };
		}
	};

	template<typename Type>
	concept primitive_type =
		same_as<Type, b> || same_as<Type, c> ||
		same_as<Type, d> || same_as<Type, f> ||
		same_as<Type, i> || same_as<Type, j> ||
		same_as<Type, s> || same_as<Type, z>;

	template<typename Type>
	concept reference_type =
		same_as<Type, object> || same_as<Type, array>;

	template<typename Type>
	concept field_descriptor_type =
		primitive_type<Type> || reference_type<Type>;

	template<typename Type>
	concept parameter_descriptor_type = field_descriptor_type<Type>;
	
	template<typename Type>
	concept return_descriptor_type =
		field_descriptor_type<Type> || same_as<Type, v>;

}