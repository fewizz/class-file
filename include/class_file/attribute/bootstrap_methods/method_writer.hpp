#pragma once

#include "./method_reader_writer_stages.hpp"
#include "./arguments_count.hpp"
#include "../../constant.hpp"

#include <tuple.hpp>
#include <write.hpp>

namespace class_file::attribute::bootstrap::method {

	template<
		basic_iterator Iterator,
		stage Stage = stage::reference_index
	>
	class writer {
		const Iterator iterator_;
	public:

		writer(Iterator iterator) : iterator_{ iterator } {}

		tuple<
			constant::method_handle_index,
			writer<Iterator, stage::arguments_count>
		>
		write_reference_index_and_get_arguments_count_writer(
			constant::method_handle_index handle_index
		)
		requires (Stage == stage::reference_index)
		{
			Iterator i = iterator_;
			constant::method_handle_index reference_index {
				::read<uint16, endianness::big>(i)
			};
			::write<endianness::big, uint16>(handle_index, i);
			return { reference_index, { i } };
		}

		writer<Iterator, stage::arguments>
		write_and_get_arguments_writer()
		requires (Stage == stage::arguments_count)
		{
			Iterator i = iterator_;
			arguments_count arguments_count {
				::read<uint16, endianness::big>(i)
			};
			return { i };
		}

		template<typename Handler>
		requires (Stage == stage::arguments)
		void
		write(Handler&& handler) {
			Iterator i = iterator_;

			handler([&](constant::index ci) {
				::write<endianness::big, uint16>(ci, i);
			});
		}

	};

}