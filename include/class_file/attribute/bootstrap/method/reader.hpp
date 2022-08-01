#pragma once

#include "./arguments_count.hpp"
#include "./argument_index.hpp"

#include <core/meta/elements/of.hpp>
#include <core/read.hpp>

namespace class_file::attribute::bootstrap::method {

	enum class reader_stage {
		reference_index,
		arguments_count,
		arguments
	};

	template<
		typename Iterator, reader_stage Stage = reader_stage::reference_index
	>
	class reader {
		Iterator iterator_;
	public:

		reader(Iterator it) : iterator_{ it } {}

		elements::of<
			constant::method_handle_index,
			reader<Iterator, reader_stage::arguments_count>
		>
		read_reference_index_and_get_arguments_count_reader()
		requires (Stage == reader_stage::reference_index)
		{
			Iterator i = iterator_;
			constant::method_handle_index reference_index {
				::read<uint16>(i)
			};
			return { reference_index, { i } };
		}

		elements::of<
			arguments_count,
			reader<Iterator, reader_stage::arguments>
		>
		read_and_get_arguments_reader()
		requires (Stage == reader_stage::arguments_count)
		{
			Iterator i = iterator_;
			arguments_count arguments_count{ ::read<uint16>(i) };
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
				argument_index argument_index{ ::read<uint16>(i) };
				handler(argument_index);
			}
		}

	};

}