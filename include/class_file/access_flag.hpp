#pragma once

#include <core/flag_enum.hpp>

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

	struct access_flags : flag_enum<access_flag> {
		using base_type = flag_enum<access_flag>;
		using base_type::base_type;

		bool _public()    { return base_type::get(access_flag::_public);    }
		bool _private()   { return base_type::get(access_flag::_private);   }
		bool _protected() { return base_type::get(access_flag::_protected); }
		bool _static()    { return base_type::get(access_flag::_static);    }
		bool _final()     { return base_type::get(access_flag::_final);     }
		bool super()      { return base_type::get(access_flag::super);      }
		bool bridge()     { return base_type::get(access_flag::bridge);     }
		bool varargs()    { return base_type::get(access_flag::varargs);    }
		bool native()     { return base_type::get(access_flag::native);     }
		bool interface()  { return base_type::get(access_flag::interface);  }
		bool abstract()   { return base_type::get(access_flag::abstract);   }
		bool strict()     { return base_type::get(access_flag::strict);     }
		bool synthetic()  { return base_type::get(access_flag::synthetic);  }
		bool annotation() { return base_type::get(access_flag::annotation); }
		bool _enum()      { return base_type::get(access_flag::_enum);      }
		bool _module()    { return base_type::get(access_flag::_module);    }
	};

}