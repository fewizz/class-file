#pragma once

#include "./method_reader_writer_stages.hpp"
#include "./arguments_count.hpp"
#include "../../constant.hpp"

#include <tuple.hpp>
#include <read.hpp>

namespace class_file::attribute::bootstrap::method {

	template<basic_iterator Iterator, stage Stage = stage::reference_index>
	class reader {
		const Iterator iterator_;
	public:

		reader(Iterator iterator) : iterator_{ iterator } {}

		tuple<
			constant::method_handle_index,
			reader<Iterator, stage::arguments_count>
		>
		read_reference_index_and_get_arguments_count_reader()
		requires (Stage == stage::reference_index)
		{
			Iterator i = iterator_;
			constant::method_handle_index reference_index {
				::read<uint16, endianness::big>(i)
			};
			return { reference_index, { i } };
		}

		tuple<
			arguments_count,
			reader<Iterator, stage::arguments>
		>
		read_and_get_arguments_reader()
		requires (Stage == stage::arguments_count)
		{
			Iterator i = iterator_;
			arguments_count arguments_count {
				::read<uint16, endianness::big>(i)
			};
			return { arguments_count, { i } };
		}

		template<typename Handler>
		requires (Stage == stage::arguments)
		void
		read(
			arguments_count arguments_count, Handler&& handler
		) {
			Iterator i = iterator_;

			while(arguments_count > 0) {
				--arguments_count;
				constant::index argument_index {
					::read<uint16, endianness::big>(i)
				};
				handler(argument_index);
			}
		}

	};

}