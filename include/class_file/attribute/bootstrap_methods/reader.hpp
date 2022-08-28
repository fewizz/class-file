#pragma once

#include "./count.hpp"
#include "./method_reader.hpp"
#include "../type.hpp"

#include <read.hpp>
#include <elements/of.hpp>

namespace class_file::attribute::bootstrap_methods {

	enum class reader_stage {
		count,
		methods
	};

	template<typename Iterator, reader_stage Stage = reader_stage::count>
	class reader {
		Iterator iterator_;
	public:

		reader(Iterator it) : iterator_{ it } {}

		static constexpr attribute::type attribute_type {
			type::bootstrap_methods
		};

		elements::of<
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

} // bootstrap_methods