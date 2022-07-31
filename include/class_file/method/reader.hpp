#pragma once

#include "../access_flag.hpp"

#include "../attribute/reader.hpp"

#include <core/meta/elements/of.hpp>
#include <core/read.hpp>

namespace class_file::method {

	enum class reader_stage {
		access_flags,
		name_index,
		descriptor_index,
		attributes
	};

	template<typename Iterator, reader_stage Stage = reader_stage::access_flags>
	class reader {
		Iterator iterator_;
	public:

		reader(Iterator it) : iterator_{ it } {}

		Iterator iterator_copy() {
			return iterator_;
		}

		elements::of<access_flags, reader<Iterator, reader_stage::name_index>>
		read_access_flags_and_get_name_index_reader() const
		requires (Stage == reader_stage::access_flags) {
			Iterator i = iterator_;
			access_flag flags {
				(access_flag) ::read<uint16, endianness::big>(i)
			};
			return { flags, { i } };
		}

		elements::of<
			constant::name_index,
			reader<Iterator, reader_stage::descriptor_index>
		>
		read_and_get_descriptor_index_reader() const
		requires (Stage == reader_stage::name_index) {
			Iterator i = iterator_;
			constant::name_index name_index {
				::read<uint16, endianness::big>(i)
			};
			return { name_index, { i } };
		}

		elements::of<
			constant::descriptor_index,
			reader<Iterator, reader_stage::attributes>
		>
		read_and_get_attributes_reader() const
		requires (Stage == reader_stage::descriptor_index) {
			Iterator i = iterator_;
			constant::descriptor_index desc_index {
				::read<uint16, endianness::big>(i)
			};
			return { desc_index, { i } };
		}

		template<typename Mapper, typename Handler>
		Iterator read_and_get_advanced_iterator(
			Mapper&& mapper, Handler&& handler
		) const
		requires (Stage == reader_stage::attributes) {
			Iterator i{ iterator_ };
			uint16 count{ ::read<uint16, endianness::big>(i) };
			while(count > 0) {
				--count;
				i = attribute::reader{i}.read_and_get_advanced_iterator(
					mapper,
					[&](auto attribute){
						handler(attribute);
					}
				);
			}
			return i;
		}

	};

	template<typename Iterator>
	reader(Iterator) -> reader<Iterator>;

}