#pragma once

#include "../../constant.hpp"

#include <integer.hpp>

namespace class_file::attribute::code {

	struct exception_handler {
		uint16 start_pc;
		uint16 end_pc;
		uint16 handler_pc;
		class_file::constant::class_index catch_type;
	};

} // class_file