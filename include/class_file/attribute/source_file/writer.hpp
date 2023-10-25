#pragma once

#include "../../constant.hpp"
#include "../../attribute/type.hpp"

#include <iterator_and_sentinel.hpp>

#include <write.hpp>

namespace class_file::source_file {

	template<basic_iterator Iterator>
	class writer {
		const Iterator iterator_;
	public:

		static constexpr attribute::type attribute_type
			= attribute::type::source_file;

		writer(Iterator iterator) : iterator_{ iterator } {}

		Iterator write_index_and_get_advanced_iterator(
			constant::utf8_index ci
		) {
			Iterator i = iterator_;
			::write<endianness::big, uint16>(ci, i);
			return i;
		}
	};

}