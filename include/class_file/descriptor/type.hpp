#pragma once

#include <span.hpp>

namespace class_file {

	struct v{};

	struct b{}; struct c{}; struct d{}; struct f{};
	struct i{}; struct j{}; struct s{}; struct z{};

	struct object {
		span<const char, uint16> class_name;
	};

	struct array {
		span<const char, uint16> component;
		uint8 rank;
	};

}