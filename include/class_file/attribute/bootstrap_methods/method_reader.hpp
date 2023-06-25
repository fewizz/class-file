#pragma once

#include "./method_reader_writer_stages.hpp"
#include "./arguments_count.hpp"
#include "../../constant.hpp"

#include <tuple.hpp>
#include <read.hpp>

namespace class_file::attribute::bootstrap::method {

	template<basic_input_stream<uint8> IS, stage Stage = stage::reference_index>
	class reader {
		const IS is_;
	public:

		reader(IS&& is) : is_{ forward<IS>(is) } {}

		tuple<
			constant::method_handle_index,
			reader<IS, stage::arguments_count>
		>
		read_reference_index_and_get_arguments_count_reader()
		requires (Stage == stage::reference_index) {
			constant::method_handle_index reference_index {
				::read<uint16, endianness::big>(is_)
			};
			return { reference_index, { forward<IS>(is_) } };
		}

		tuple<
			arguments_count,
			reader<IS, stage::arguments>
		>
		read_and_get_arguments_reader()
		requires (Stage == stage::arguments_count) {
			arguments_count arguments_count {
				::read<uint16, endianness::big>(is_)
			};
			return { arguments_count, { forward<IS>(is_) } };
		}

		template<typename Handler>
		requires (Stage == stage::arguments)
		void
		read(
			arguments_count arguments_count, Handler&& handler
		) {
			while(arguments_count > 0) {
				--arguments_count;
				constant::index argument_index {
					::read<uint16, endianness::big>(is_)
				};
				handler(argument_index);
			}
		}

	};

}