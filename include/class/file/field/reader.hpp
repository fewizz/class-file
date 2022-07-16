#pragma once

#include "../access_flag.hpp"

#include "../attribute/reader.hpp"

#include <core/meta/elements/of.hpp>
#include <core/read.hpp>

namespace class_file::field {

	enum class reader_stage {
		access_flags,
		name_index,
		descriptor_index,
		attributes,
		end
	};

	template<typename Iterator, reader_stage Stage = reader_stage::access_flags>
	struct reader {
		Iterator iterator_;

		elements::of<reader<Iterator, reader_stage::name_index>, access_flags>
		operator () () const
		requires (Stage == reader_stage::access_flags) {
			auto i = iterator_;
			uint16 flags = read<uint16, endianness::big>(i);
			return { { i }, { (access_flag) flags } };
		}

		elements::of<reader<Iterator, reader_stage::descriptor_index>, uint16>
		operator () () const
		requires (Stage == reader_stage::name_index) {
			auto i = iterator_;
			uint16 name_index = read<uint16, endianness::big>(i);
			return { { i }, { name_index } };
		}

		elements::of<reader<Iterator, reader_stage::attributes>, uint16>
		operator () () const
		requires (Stage == reader_stage::descriptor_index) {
			auto i = iterator_;
			uint16 desc_index = read<uint16, endianness::big>(i);
			return { { i }, { desc_index } };
		}

		template<typename Mapper, typename Handler>
		reader<Iterator, reader_stage::end>
		operator () (Mapper&& mapper, Handler&& handler) const
		requires (Stage == reader_stage::attributes) {
			auto i = iterator_;
			auto count = read<uint16, endianness::big>(i);
			while(count > 0) {
				--count;
				auto end = attribute::reader{i}(
					mapper,
					[&](auto attribute) {
						handler(attribute);
					}
				);
				i = end.iterator_;
			}
			return { i };
		}

	};

	template<typename Iterator>
	reader(Iterator) -> reader<Iterator>;

}