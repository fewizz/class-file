#pragma once

#include <core/integer.hpp>

namespace class_file::attribute::bootstrap::methods {

	struct count {
		uint32 _;
		operator uint32 () { return _; }
	};

}