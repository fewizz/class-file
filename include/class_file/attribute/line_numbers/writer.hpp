#pragma once

#include "./line_number.hpp"
#include "./reader_writer_stages.hpp"
#include "../../attribute/type.hpp"

#include <iterator_and_sentinel.hpp>

#include <tuple.hpp>
#include <write.hpp>

namespace class_file::line_numbers {

	template<basic_iterator Iterator, stage Stage = stage::count>
	class writer {
		const Iterator iterator_;
	public:

		static constexpr attribute::type attribute_type
			= attribute::type::line_numbers;

		writer(Iterator iterator) : iterator_{ iterator } {}

		writer<Iterator, stage::line_numbers>
		write_count_and_get_line_numbers_writer(uint16 count)
		requires (Stage == stage::count) {
			Iterator i = iterator_;
			::write<endianness::big, uint16>(count, i);
			return { i };
		}

		template<typename Handler>
		requires (Stage == stage::line_numbers)
		Iterator write_and_get_advanced_iterator(
			Handler&& handler
		) {
			Iterator i = iterator_;

			handler([](uint16 start_pc, class_file::line_number line_number) {
				::write<endianness::big>(start_pc, i);
				::write<endianness::big>(line_number, i);
			});

			return i;
		}

	};

}