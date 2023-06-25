#pragma once

#include "./field_reader_writer_stages.hpp"
#include "./access_flag.hpp"
#include "./attribute/reader.hpp"

#include <tuple.hpp>
#include <read.hpp>

namespace class_file::field {

	template<
		basic_input_stream<uint8> IS, stage Stage = stage::access_flags
	>
	class reader {
		IS is_;
	public:

		reader(IS&& is) : is_{ forward<IS>(is) } {}

		tuple<
			access_flags,
			reader<IS, stage::name_index>
		>
		read_access_flags_and_get_name_index_reader()
		requires (Stage == stage::access_flags) {
			access_flags flags {
				::read<access_flag, endianness::big>(is_)
			};
			return { flags, { forward<IS>(is_) } };
		}

		tuple<
			constant::name_index,
			reader<IS, stage::descriptor_index>
		>
		read_and_get_descriptor_index_reader()
		requires (Stage == stage::name_index) {
			constant::name_index name_index {
				::read<uint16, endianness::big>(is_)
			};
			return { name_index, { forward<IS>(is_) } };
		}

		tuple<
			constant::descriptor_index,
			reader<IS, stage::attributes>
		>
		read_and_get_attributes_reader()
		requires (Stage == stage::descriptor_index) {
			constant::descriptor_index desc_index {
				::read<uint16, endianness::big>(is_)
			};
			return { desc_index, { forward<IS>(is_) } };
		}

		template<typename Mapper, typename Handler>
		IS read_and_get_advanced_iterator(
			Mapper&& mapper, Handler&& handler
		)
		requires (Stage == stage::attributes) {
			uint16 count{ ::read<uint16, endianness::big>(is_) };
			while(count > 0) {
				--count;
				is_ = attribute::reader{ forward<IS>(is_) }
					.read_and_get_advanced_iterator(
					mapper,
					[&](auto attribute) {
						handler(attribute);
					}
				);
			}
			return forward<IS>(is_);
		}

	};

	template<basic_input_stream<uint8> IS>
	reader(IS&&) -> reader<IS>;

}