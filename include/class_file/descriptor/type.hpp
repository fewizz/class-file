#pragma once

#include <span.hpp>
#include <array.hpp>
#include <c_string.hpp>

#include <unicode/utf8.hpp>

namespace class_file {

	template<utf8::unit Term>
	struct base {
		static constexpr utf8::unit term_ = Term;

		static constexpr utf8::unit term() {
			return term_;
		}

		constexpr auto utf8_string() {
			return c_string_of_known_size<utf8::unit>{ &term_, 1 };
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

	struct object : span<const utf8::unit, uint64> {

		constexpr object(const utf8::unit* ptr, nuint len) :
			span<const utf8::unit, uint64>{ ptr, len }
		{}

		constexpr object(const object&) = default;
		constexpr object& operator = (const object&) = default;

		constexpr auto utf8_string() {
			return *this;
		}
	};

	struct array : span<const utf8::unit> {
		uint8 rank;

		constexpr array(span<const utf8::unit> name, uint8 rank) :
			span<const utf8::unit>{ name },
			rank{ rank }
		{}

		constexpr array(const array&) = default;
		constexpr array& operator = (const array&) = default;

		constexpr span<const utf8::unit, uint16> component() {
			return span{ iterator() + rank, size() - rank };
		};

		constexpr auto utf8_string() {
			return *this;
		}
	};

	template<typename Type>
	concept descriptor_type =
		same_as<Type, v> ||
		same_as<Type, b> || same_as<Type, c> ||
		same_as<Type, d> || same_as<Type, f> ||
		same_as<Type, i> || same_as<Type, j> ||
		same_as<Type, s> || same_as<Type, z> ||
		same_as<Type, object> || same_as<Type, array>;

}