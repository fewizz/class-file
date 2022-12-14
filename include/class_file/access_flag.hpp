#pragma once

#include <enum_flags.hpp>
#include <bit_cast.hpp>

namespace class_file {

	enum class access_flag : uint16 {
		_public    = 0x0001,
		_private   = 0x0002,
		_protected = 0x0004,
		_static    = 0x0008,
		_final     = 0x0010,
		super      = 0x0020,
		bridge     = 0x0040,
		varargs    = 0x0080,
		native     = 0x0100,
		interface  = 0x0200,
		abstract   = 0x0400,
		strict     = 0x0800,
		synthetic  = 0x1000,
		annotation = 0x2000,
		_enum      = 0x4000,
		_module    = 0x8000
	};

	struct access_flags {
		bool _public   :1;
		bool _private  :1;
		bool _protected:1;
		bool _static   :1;
		bool _final    :1;
		bool super     :1;
		bool bridge    :1;
		bool varargs   :1;
		bool native    :1;
		bool interface :1;
		bool abstract  :1;
		bool strict    :1;
		bool synthetic :1;
		bool annotation:1;
		bool _enum     :1;
		bool _module   :1;

		access_flags(access_flag flag) {
			((access_flag&) *this) = flag; // ugh
		}

		operator uint16() const {
			return (uint16&) *this;
		}
	};

	static_assert(bits_in<access_flags> == 16);

}