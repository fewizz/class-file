#pragma once

#include "./count.hpp"
#include "./method_reader.hpp"
#include "../type.hpp"

#include <read.hpp>
#include <tuple.hpp>

namespace class_file::attribute::bootstrap_methods {

	enum class reader_stage {
		count,
		methods
	};

	template<basic_iterator Iterator, reader_stage Stage = reader_stage::count>
	class reader {
		const Iterator iterator_;
	public:

		reader(Iterator iterator) : iterator_{ iterator } {}

		static constexpr attribute::type attribute_type {
			type::bootstrap_methods
		};

		tuple<
			bootstrap_methods::count,
			reader<Iterator, reader_stage::methods>
		>
		read_count_and_get_methods_reader()
		requires (Stage == reader_stage::count)
		{
			Iterator i = iterator_;
			bootstrap_methods::count count {
				::read<uint16, endianness::big>(i)
			};
			return { count, { i } };
		}

		template<typename Handler>
		requires (Stage == reader_stage::methods)
		void
		read(
			bootstrap_methods::count count, Handler&& handler
		) {
			Iterator i = iterator_;

			while(count > 0) {
				--count;
				handler(bootstrap::method::reader{ i });
			}
		}

	};

}