#pragma once

#include <core/integer.hpp>

#include <class_file/constant.hpp>

namespace class_file::attribute::code {

	struct exception_handler {
		uint16 start_pc;
		uint16 end_pc;
		uint16 handler_pc;
		class_file::constant::class_index catch_type;
	};

} // class_file