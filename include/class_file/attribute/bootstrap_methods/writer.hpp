#pragma once

#include "./method_writer.hpp"
#include "./reader_writer_stages.hpp"
#include "../../attribute/type.hpp"

#include <iterator_and_sentinel.hpp>
#include <write.hpp>

namespace class_file::attribute::bootstrap_methods {

	template<basic_iterator Iterator, stage Stage = stage::count>
	class writer {
		const Iterator iterator_;
	public:

		static constexpr attribute::type attribute_type
			= type::bootstrap_methods;

		writer<Iterator, stage::methods>
		write_count_and_get_methods_writer(uint16 count)
		requires(Stage == stage::count)
		{
			Iterator i = iterator_;
			::write<endianness::big, uint16>(count, i);
			return { i };
		}

		template<typename Handler>
		Iterator
		write_and_get_advanced_iterator(Handler&& handler) {
			Iterator i = iterator_;

			handler([](){ return bootstrap::method::writer{ i }; });

			return i;
		}

	};

}