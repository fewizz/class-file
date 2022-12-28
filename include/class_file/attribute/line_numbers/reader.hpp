#pragma once

#include "../../attribute/type.hpp"

#include <iterator_and_sentinel.hpp>

#include <tuple.hpp>
#include <read.hpp>

namespace class_file {

	class line_number {
		uint16 value_;
	public:
		explicit line_number(uint16 value) : value_ { value } {}
		explicit operator uint16() { return value_; }
	};

}

namespace class_file::line_numbers {

	enum class reader_stage {
		count,
		line_numbers
	};

	template<basic_iterator Iterator, reader_stage Stage = reader_stage::count>
	class reader {
		Iterator iterator_;
	public:

		static constexpr attribute::type attribute_type
			= attribute::type::line_numbers;

		reader(Iterator iterator) : iterator_{ iterator } {}

		tuple<uint16, reader<Iterator, reader_stage::line_numbers>>
		read_and_get_line_numbers_reader()
		requires (Stage == reader_stage::count) {
			Iterator i = iterator_;
			uint16 count = ::read<uint16, endianness::big>(i);
			return { count, { i } };
		}

		template<typename Handler>
		requires (Stage == reader_stage::line_numbers)
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