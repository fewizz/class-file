#pragma once

#include "../../constant.hpp"

#include <read.hpp>
#include <array.hpp>
#include <range.hpp>
#include <c_string.hpp>

namespace class_file::attribute {

	template<basic_input_stream<uint8> IS>
	class reader {
		IS is_;
	public:

		reader(IS&& is) : is_{ forward<IS>(is) } {}

		template<
			typename IndexToUtf8Mapper,
			typename Handler
		>
		IS read_and_get_advanced_iterator(
			IndexToUtf8Mapper&& mapper, Handler&& handler
		);

		IS skip_and_get_advanced_iterator() {
			read<uint16, endianness::big>(is_); // skip attribute index
			uint32 length = read<uint32, endianness::big>(is_);

			is_ += length;
			return forward<IS>(is_);
		}
	};

} // class_file::attribute