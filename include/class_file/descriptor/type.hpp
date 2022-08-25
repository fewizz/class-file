#pragma once

#include <span.hpp>

namespace class_file::descriptor {

	struct V{};

	struct B{}; struct C{}; struct D{}; struct F{};
	struct I{}; struct J{}; struct S{}; struct Z{};

	struct object_type : span<const char, uint16> {
		using base_type = span<const char, uint16>;
		using base_type::base_type;
	};

	struct array_type : span<const char, uint16> {
		using base_type = span<const char, uint16>;

		nuint rank;

		array_type(nuint rank, char* ptr, uint16 size) :
			base_type{ ptr, size },
			rank{ rank }
		{}

	};

} // descriptor