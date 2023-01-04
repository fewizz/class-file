#pragma once

#include "./arguments_count.hpp"
#include "../../constant.hpp"

#include <tuple.hpp>
#include <read.hpp>

namespace class_file::attribute::bootstrap::method {

	enum class reader_stage {
		reference_index,
		arguments_count,
		arguments
	};

	template<
		basic_iterator Iterator, reader_stage Stage = reader_stage::reference_index
	>
	class reader {
		const Iterator iterator_;
	public:

		reader(Iterator it) : iterator_{ it } {}

		tuple<
			constant::method_handle_index,
			reader<Iterator, reader_stage::arguments_count>
		>
		read_reference_index_and_get_arguments_count_reader()
		requires (Stage == reader_stage::reference_index)
		{
			Iterator i = iterator_;
			constant::method_handle_index reference_index {
				::read<uint16, endianness::big>(i)
			};
			return { reference_index, { i } };
		}

		tuple<
			arguments_count,
			reader<Iterator, reader_stage::arguments>
		>
		read_and_get_arguments_reader()
		requires (Stage == reader_stage::arguments_count)
		{
			Iterator i = iterator_;
			arguments_count arguments_count {
				::read<uint16, endianness::big>(i)
			};
			return { arguments_count, { i } };
		}

		template<typename Handler>
		requires (Stage == reader_stage::arguments)
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