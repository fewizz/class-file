#pragma once

#include "./line_number.hpp"
#include "./reader_writer_stages.hpp"
#include "../../attribute/type.hpp"

#include <iterator_and_sentinel.hpp>

#include <tuple.hpp>
#include <read.hpp>

namespace class_file::line_numbers {

	template<basic_input_stream<uint8> IS, stage Stage = stage::count>
	class reader {
		IS is_;
	public:

		static constexpr attribute::type attribute_type
			= attribute::type::line_numbers;

		reader(IS&& is) : is_{ forward<IS>(is) } {}

		tuple<uint16, reader<IS, stage::line_numbers>>
		read_count_and_get_line_numbers_reader()
		requires (Stage == stage::count) {
			uint16 count = ::read<uint16, endianness::big>(is_);
			return { count, { forward<IS>(is_) } };
		}

		template<typename Handler>
		requires (Stage == stage::line_numbers)
		IS read_and_get_advanced_iterator(
			uint16 count, Handler&& handler
		) {
			while(count > 0) {
				--count;
				uint16 start_pc {
					::read<uint16, endianness::big>(is_)
				};
				class_file::line_number line_number {
					::read<uint16, endianness::big>(is_)
				};
				handler(start_pc, line_number);
			}
			return forward<IS>(is_);
		}

	};

	template<basic_input_stream<uint8> IS>
	reader(IS&&) -> reader<IS>;

}