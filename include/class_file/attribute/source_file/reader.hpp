#pragma once

#include "../../constant.hpp"
#include "../../attribute/type.hpp"

#include <iterator_and_sentinel.hpp>

#include <tuple.hpp>
#include <read.hpp>

namespace class_file::source_file {

	template<basic_iterator Iterator>
	class reader {
		const Iterator iterator_;
	public:

		static constexpr attribute::type attribute_type
			= attribute::type::source_file;

		reader(Iterator iterator) : iterator_{ iterator } {}

		tuple<constant::utf8_index, Iterator> read_and_get_advanced_iterator() {
			Iterator i = iterator_;
			constant::utf8_index index {
				::read<uint16, endianness::big>(i)
			};
			return { index, i };
		}
	};

}