#pragma once

#include <enum_flags.hpp>

namespace class_file {

	enum class access_flag : uint16 {
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

	struct access_flags : enum_flags<access_flag> {
		using base_type = enum_flags<access_flag>;
		using base_type::base_type;

		bool _public()    const {
			return base_type::get(access_flag::_public);
		}

		bool _private()   const {
			return base_type::get(access_flag::_private);
		}

		bool _protected() const {
			return base_type::get(access_flag::_protected);
		}

		bool _static()    const {
			return base_type::get(access_flag::_static);
		}

		bool _final()     const {
			return base_type::get(access_flag::_final);
		}

		bool super()      const {
			return base_type::get(access_flag::super);
		}

		bool bridge()     const {
			return base_type::get(access_flag::bridge);
		}

		bool varargs()    const {
			return base_type::get(access_flag::varargs);
		}

		bool native()     const {
			return base_type::get(access_flag::native);
		}

		bool interface()  const {
			return base_type::get(access_flag::interface);
		}

		bool abstract()   const {
			return base_type::get(access_flag::abstract);
		}

		bool strict()     const {
			return base_type::get(access_flag::strict);
		}

		bool synthetic()  const {
			return base_type::get(access_flag::synthetic);
		}

		bool annotation() const {
			return base_type::get(access_flag::annotation);
		}

		bool _enum()      const {
			return base_type::get(access_flag::_enum);
		}

		bool _module()    const {
			return base_type::get(access_flag::_module);
		}

	};

}