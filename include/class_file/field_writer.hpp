#pragma once

#include "./field_reader_writer_stages.hpp"
#include "./constant.hpp"
#include "./access_flag.hpp"
#include "./attribute/__writer/writer.hpp"

#include <write.hpp>

namespace class_file::field {

	template<
		basic_iterator Iterator,
		stage Stage = stage::access_flags
	>
	class writer {
		const Iterator iterator_;
	public:

		writer(Iterator iterator) : iterator_{ iterator } {}

		Iterator iterator_copy() const { return iterator_; }

		writer<Iterator, stage::name_index>
		write_access_flags_and_get_name_index_writer(
			access_flags access_flags
		) const
		requires (Stage == stage::access_flags) {
			Iterator i = iterator_;
			::write<access_flags, endianness::big>(access_flags, i);
			return { i };
		}

		
		writer<Iterator, stage::descriptor_index>
		write_and_get_descriptor_index_writer(
			constant::name_index name_index
		) const
		requires (Stage == stage::name_index) {
			Iterator i = iterator_;
			::write<uint16, endianness::big>(name_index, i);
			return { i };
		}

		writer<Iterator, stage::attributes>
		write_and_get_attributes_writer(
			constant::descriptor_index descriptor_index
		) const
		requires (Stage == stage::descriptor_index) {
			Iterator i = iterator_;
			::write<uint16, endianness::big>(descriptor_index, i);
			return { i };
		}

		template<typename Handler>
		Iterator write_and_get_advanced_iterator(
			uint16 count, Handler&& handler
		) const
		requires (Stage == stage::attributes) {
			Iterator i = iterator_;
			::write<uint16, endianness::big>(count, i);

			attribute::writer attribute_writer{ i };
			handler(attribute_writer);
			return attribute_writer.iterator_copy();
		}

	}; // writer

	template<basic_iterator Iterator>
	writer(Iterator) -> writer<Iterator>;

} // class_file::field