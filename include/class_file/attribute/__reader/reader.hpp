#pragma once

#include "../../constant.hpp"

#include <read.hpp>
#include <array.hpp>
#include <range.hpp>
#include <c_string.hpp>

namespace class_file::attribute {

	template<typename Iterator>
	class reader {
		Iterator iterator_;
	public:

		reader(Iterator it) : iterator_{ it } {}

		template<
			typename IndexToUtf8Mapper,
			typename Handler
		>
		Iterator read_and_get_advanced_iterator(
			IndexToUtf8Mapper&& mapper, Handler&& handler
		) const;

		Iterator skip_and_get_advanced_iterator() const {
			Iterator i = iterator_;
			read<uint16, endianness::big>(i); // skip attribute index
			uint32 length = read<uint32, endianness::big>(i);

			i += length;
			return i;
		}
	};

} // class_file::attribute