#pragma once

#include "./arguments_count.hpp"

#include <core/meta/elements/of.hpp>

namespace class_file::attribute::bootstrap::method {

	enum class reader_stage {
		reference_index,
		arguments_count,
		arguments,
		end
	};

	template<
		typename Iterator, reader_stage Stage = reader_stage::reference_index
	>
	struct reader {
		Iterator iterator_;

		elements::of<reader<Iterator, reader_stage::arguments_count>, uint16>
		operator () () requires (Stage == reader_stage::reference_index) {
			Iterator cpy = iterator_;
			uint16 reference_index = read<uint16>(cpy);
			return { { cpy }, reference_index };
		}

		elements::of<reader<Iterator, reader_stage::arguments>, arguments_count>
		operator () () requires (Stage == reader_stage::arguments_count) {
			Iterator cpy = iterator_;
			uint16 arguments_count = read<uint16>(cpy);
			return { { cpy }, { arguments_count } };
		}

		template<typename Handler>
		requires (Stage == reader_stage::arguments)
		reader<Iterator, reader_stage::end>
		operator () (arguments_count arguments_count, Handler&& handler) {
			Iterator cpy = iterator_;

			while(arguments_count > 0) {
				--arguments_count._;
				uint16 argument_index = read<uint16>(cpy);
				handler(argument_index);
			}

			return { cpy };
		}

	};

}