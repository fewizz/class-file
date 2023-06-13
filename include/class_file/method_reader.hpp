#pragma once

#include "./method_reader_writer_stages.hpp"
#include "./access_flag.hpp"
#include "./attribute/reader.hpp"

#include <tuple.hpp>
#include <read.hpp>

namespace class_file::method {

	template<
		basic_iterator Iterator, stage Stage = stage::access_flags
	>
	class reader {
		const Iterator iterator_;
	public:

		reader(Iterator iterator) : iterator_{ iterator } {}

		Iterator iterator_copy() const {
			return iterator_;
		}

		tuple<
			access_flags,
			reader<Iterator, stage::name_index>
		>
		read_access_flags_and_get_name_index_reader() const
		requires (Stage == stage::access_flags) {
			Iterator i = iterator_;
			access_flag flags {
				::read<access_flag, endianness::big>(i)
			};
			return { flags, { i } };
		}

		tuple<
			constant::name_index,
			reader<Iterator, stage::descriptor_index>
		>
		read_and_get_descriptor_index_reader() const
		requires (Stage == stage::name_index) {
			Iterator i = iterator_;
			constant::name_index name_index {
				::read<uint16, endianness::big>(i)
			};
			return { name_index, { i } };
		}

		tuple<
			constant::descriptor_index,
			reader<Iterator, stage::attributes>
		>
		read_and_get_attributes_reader() const
		requires (Stage == stage::descriptor_index) {
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
		requires (Stage == stage::attributes) {
			Iterator i = iterator_;
			uint16 count = ::read<uint16, endianness::big>(i);
			while(count > 0) {
				--count;
				i = attribute::reader{ i }.read_and_get_advanced_iterator(
					forward<Mapper>(mapper),
					handler
				);
			}
			return i;
		}

	};

	template<basic_iterator Iterator>
	reader(Iterator) -> reader<Iterator>;

}