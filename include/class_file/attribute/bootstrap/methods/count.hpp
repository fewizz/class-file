#pragma once

#include <integer.hpp>

namespace class_file::attribute::bootstrap::methods {

	class count {
		uint16 value_;
	public:

		count(uint16 value) : value_{ value } {}

		operator uint16& () { return value_; }
	};

}