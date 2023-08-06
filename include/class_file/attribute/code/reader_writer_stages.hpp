#pragma once

namespace class_file::attribute::code {

	enum class stage {
		max_stack,
		max_locals,
		code,
		exception_table,
		attributes
	};

}