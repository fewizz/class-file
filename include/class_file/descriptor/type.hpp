#pragma once

#include <span.hpp>

#include <unicode/utf8.hpp>

namespace class_file {

	struct v{};

	struct b{}; struct c{}; struct d{}; struct f{};
	struct i{}; struct j{}; struct s{}; struct z{};

	struct object : span<const utf8::unit, uint64> {

		constexpr object(const utf8::unit* ptr, nuint len) :
			span<const utf8::unit, uint64>{ ptr, len }
		{}

		constexpr object(const object&) = default;
		constexpr object& operator = (const object&) = default;
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