#pragma once

#include <core/integer.hpp>

namespace class_file::attribute::code {

	struct exception_handler {
		uint16 start_pc;
		uint16 end_pc;
		uint16 handler_pc;
		uint16 catch_type;
	};

} // class_file