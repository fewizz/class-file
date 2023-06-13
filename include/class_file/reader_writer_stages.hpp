#pragma once

namespace class_file {

	enum class stage {
		magic, version, constant_pool, access_flags,
		this_class, super_class,
		interfaces, fields, methods, attributes
	};

}