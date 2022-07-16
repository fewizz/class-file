#pragma once

#include "../constant.hpp"
#include "code/reader.hpp"

#include <core/read.hpp>
#include <core/meta/elements/one_of.hpp>
#include <core/array.hpp>
#include <core/equals.hpp>

namespace class_file::attribute {

	enum class reader_stage {
		info,
		end
	};

	template<typename Iterator, reader_stage Stage = reader_stage::info>
	struct reader {
		Iterator iterator_;

		template<
			typename IndexToUtf8Mapper,
			typename Handler
		>
		reader<Iterator, reader_stage::end>
		operator () (IndexToUtf8Mapper&& mapper, Handler&& handler) const
		requires (Stage == reader_stage::info) {
			auto cpy = iterator_;
			uint16 name_index = read<uint16, endianness::big>(cpy);
			uint32 length = read<uint32, endianness::big>(cpy);
			constant::utf8 name = mapper(name_index);

			if(equals(name, array{ 'C', 'o', 'd', 'e' })) {
				handler(code::reader{ cpy });
			}

			cpy += length;
			return { cpy };
		}

		reader<Iterator, reader_stage::end> skip() const
		requires (Stage == reader_stage::info){
			auto cpy = iterator_;
			read<uint16, endianness::big>(cpy);
			uint32 length = read<uint32, endianness::big>(cpy);
			cpy += length;
			return { cpy };
		}
	};

	template<typename Iterator>
	reader(Iterator) -> reader<Iterator>;

} // class_file::attribute