#pragma once

#include <core/integer.hpp>

namespace class_file::attribute::bootstrap::method {

	class arguments_count {
		uint16 value_;
	public:

		arguments_count(uint16 value) : value_{ value } {}

		operator uint16& () { return value_; }
	};

}