#pragma once

#include <core/integer.hpp>

namespace class_file::attribute::bootstrap::method {

	struct arguments_count {
		uint16 _;

		operator uint16 () { return _; }
	};

}