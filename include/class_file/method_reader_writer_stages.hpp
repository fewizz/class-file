#pragma once

namespace class_file::method {

	enum class stage {
		access_flags,
		name_index,
		descriptor_index,
		attributes
	};

}