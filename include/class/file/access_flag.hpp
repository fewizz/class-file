#pragma once

#include <core/flag_enum.hpp>

namespace class_file {

	enum class access_flag {
		_public    = 0x0001,
		_private   = 0x0002,
		_protected = 0x0004,
		_static    = 0x0008,
		_final     = 0x0010,
		super      = 0x0020,
		bridge     = 0x0040,
		varargs    = 0x0040,
		native     = 0x0100,
		interface  = 0x0200,
		abstract   = 0x0400,
		strict     = 0x0800,
		synthetic  = 0x1000,
		annotation = 0x2000,
		_enum      = 0x4000,
		_module    = 0x8000
	};

	using access_flags = flag_enum<access_flag>;

}