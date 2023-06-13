#pragma once

#include "./line_number.hpp"
#include "./reader_writer_stages.hpp"
#include "../../attribute/type.hpp"

#include <iterator_and_sentinel.hpp>

#include <tuple.hpp>
#include <read.hpp>

namespace class_file::line_numbers {

	template<basic_iterator Iterator, stage Stage = stage::count>
	class reader {
		const Iterator iterator_;
	public:

		static constexpr attribute::type attribute_type
			= attribute::type::line_numbers;

		reader(Iterator iterator) : iterator_{ iterator } {}

		tuple<uint16, reader<Iterator, stage::line_numbers>>
		read_count_and_get_line_numbers_reader()
		requires (Stage == stage::count) {
			Iterator i = iterator_;
			uint16 count = ::read<uint16, endianness::big>(i);
			return { count, { i } };
		}

		template<typename Handler>
		requires (Stage == stage::line_numbers)
		Iterator read_and_get_advanced_iterator(
			uint16 count, Handler&& handler
		) {
			Iterator i = iterator_;
			while(count > 0) {
				--count;
				uint16 start_pc {
					::read<uint16, endianness::big>(i)
				};
				class_file::line_number line_number {
					::read<uint16, endianness::big>(i)
				};
				handler(start_pc, line_number);
			}
			return i;
		}

	};

}