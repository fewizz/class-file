#pragma once

#include "../../constant.hpp"
#include "../../attribute/type.hpp"

#include <iterator_and_sentinel.hpp>

#include <tuple.hpp>
#include <read.hpp>

namespace class_file::source_file {

	template<basic_input_stream<uint8> IS>
	class reader {
		IS is_;
	public:

		static constexpr attribute::type attribute_type
			= attribute::type::source_file;

		reader(IS&& is) : is_{ forward<IS>(is) } {}

		tuple<constant::utf8_index, IS>
		read_index_and_get_advanced_iterator() {
			constant::utf8_index index {
				::read<uint16, endianness::big>(is_)
			};
			return { index, forward<IS>(is_) };
		}
	};

	template<basic_input_stream<uint8> IS>
	reader(IS&&) -> reader<IS>;

}