#pragma once

#include "../constant.hpp"
#include "code/reader.hpp"
//#include "bootstrap/methods/reader.hpp"

#include <core/read.hpp>
#include <core/meta/elements/one_of.hpp>
#include <core/array.hpp>
#include <core/equals.hpp>
#include <core/c_string.hpp>

namespace class_file::attribute {

	template<typename Iterator>
	class reader {
		Iterator iterator_;
	public:

		reader(Iterator it) : iterator_{ it } {}

		template<
			typename IndexToUtf8Mapper,
			typename Handler
		>
		Iterator read_and_get_advanced_iterator(
			IndexToUtf8Mapper&& mapper, Handler&& handler
		) const {
			Iterator i = iterator_;
			constant::utf8_index name_index{ read<uint16, endianness::big>(i) };
			uint32 length = read<uint32, endianness::big>(i);
			constant::utf8 name = mapper(name_index);

			if(equals(name, c_string{ "Code" })) {
				handler(code::reader{ i });
			}
			//else if(equals(name, c_string{ "BootstrapMethods" })) {
			//	handler(bootstrap::methods::reader{ cpy });
			//}

			i += length;
			return i;
		}

		Iterator skip_and_get_advanced_iterator() const {
			Iterator i = iterator_;
			read<uint16, endianness::big>(i);
			uint32 length = read<uint32, endianness::big>(i);
			i += length;
			return i;
		}
	};

} // class_file::attribute