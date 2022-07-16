#pragma once

#include <core/span.hpp>

namespace class_file::descriptor {

	struct V{};

	struct B{}; struct C{}; struct D{}; struct F{};
	struct I{}; struct J{}; struct S{}; struct Z{};

	struct object_type : span<uint8, uint16> {
		using base_type = span<uint8, uint16>;
		using base_type::base_type;
	};

	struct array_type : span<uint8, uint16> {
		using base_type = span<uint8, uint16>;

		nuint rank;

		array_type(nuint rank, uint8* ptr, uint16 size) :
			base_type{ ptr, size },
			rank{ rank }
		{}

	};

};