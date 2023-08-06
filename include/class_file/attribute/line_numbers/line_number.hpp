#pragma once

#include <integer.hpp>

namespace class_file {

	class line_number {
		uint16 value_;
	public:
		explicit line_number(uint16 value) : value_ { value } {}
		explicit operator uint16() { return value_; }
	};

}