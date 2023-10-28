#pragma once

#include "./version.hpp"
#include "./constant.hpp"
#include "./access_flag.hpp"
#include "./reader_writer_stages.hpp"
#include "./field_reader.hpp"
#include "./method_reader.hpp"
#include "./constant_reader.hpp"

#include <tuple.hpp>
#include <array.hpp>

namespace class_file {

	template<
		basic_input_stream<uint8> IS,
		stage Stage = stage::magic
	>
	class reader {
		IS is_;
	public:

		reader(IS&& is) : is_{ forward<IS>(is) } {}

		tuple<
			bool,
			reader<IS, stage::version>
		>
		read_and_check_magic_and_get_version_reader()
		requires (Stage == stage::magic) {
			uint32 val = ::read<uint32, endianness::big>(is_);
			bool result = val == 0xCAFEBABE;
			return { result, { forward<IS>(is_) } };
		}

		tuple<
			class_file::version,
			reader<IS, stage::constant_pool>
		>
		read_and_get_constant_pool_reader()
		requires (Stage == stage::version) {
			uint16 minor = ::read<uint16, endianness::big>(is_);
			uint16 major = ::read<uint16, endianness::big>(is_);
			return { { major, minor }, { forward<IS>(is_) } };
		}

		
		reader<IS, stage::constant_pool>
		skip_and_get_constant_pool_reader()
		requires (Stage == stage::version) {
			::read<uint16, endianness::big>(is_);
			::read<uint16, endianness::big>(is_);
			return { forward<IS>(is_) };
		}

		template<typename Handler>
		reader<IS, stage::access_flags>
		read_and_get_access_flags_reader(Handler&& handler)
		requires (Stage == stage::constant_pool) {
			 // "constant_pool_count is equal to the number of entries
			 // in the constant_pool table plus one"
			uint16 entries_count = ::read<uint16, endianness::big>(is_);
			--entries_count; // minus one

			while(entries_count > 0) {
				is_ = constant::reader{ forward<IS>(is_) }
				.read_and_get_advanced_iterator(
					[&](auto constant) {
						--entries_count;
						handler(constant);
					}
				);
			}

			return { forward<IS>(is_) };
		}

		uint16 get_count() const
		requires (Stage == stage::constant_pool && contiguous_iterator<IS>) {
			remove_reference<IS> i = is_;
			uint16 entries_count = ::read<uint16, endianness::big>(i);
			--entries_count; // minus one
			return entries_count;
		}

		tuple<
			access_flags,
			reader<IS, stage::this_class>
		>
		read_and_get_this_class_reader()
		requires (Stage == stage::access_flags) {
			class_file::access_flags flags {
				::read<access_flag, endianness::big>(is_)
			};
			return { flags, { forward<IS>(is_) } };
		}

		
		reader<IS, stage::this_class>
		skip_and_get_this_class_reader()
		requires (Stage == stage::access_flags) {
			::read<access_flag, endianness::big>(is_);
			return { forward<IS>(is_) };
		}

		tuple<
			constant::class_index,
			reader<IS, stage::super_class>
		>
		read_and_get_super_class_reader()
		requires (Stage == stage::this_class) {
			constant::class_index this_class_index {
				::read<uint16, endianness::big>(is_)
			};
			return { this_class_index, { forward<IS>(is_) } };
		}

		reader<IS, stage::super_class>
		skip_and_get_super_class_reader()
		requires (Stage == stage::this_class) {
			::read<uint16, endianness::big>(is_);
			return { forward<IS>(is_) };
		}

		tuple<
			constant::class_index,
			reader<IS, stage::interfaces>
		>
		read_and_get_interfaces_reader()
		requires (Stage == stage::super_class) {
			constant::class_index super_class_index {
				::read<uint16, endianness::big>(is_)
			};
			return { super_class_index, { forward<IS>(is_) } };
		}

		reader<IS, stage::interfaces>
		skip_and_get_interfaces_reader()
		requires (Stage == stage::super_class) {
			::read<uint16, endianness::big>(is_);
			return { forward<IS>(is_) };
		}

		uint16 get_count() const
		requires (Stage == stage::interfaces) {
			remove_reference<IS> i = is_;
			return ::read<uint16, endianness::big>(i);
		}

		template<typename Handler>
		reader<IS, stage::fields>
		read_and_get_fields_reader(Handler&& handler)
		requires (Stage == stage::interfaces) {
			uint16 count = ::read<uint16, endianness::big>(is_);
			for(uint16 x = 0; x < count; ++x) {
				constant::class_index index {
					::read<uint16, endianness::big>(is_)
				};
				handler(index);
			}
			return { forward<IS>(is_) };
		}

		reader<IS, stage::fields>
		skip_and_get_fields_reader()
		requires (Stage == stage::interfaces) {
			return read_and_get_fields_reader([](auto){});
		}

		uint16 get_count () const
		requires (Stage == stage::fields && contiguous_iterator<IS>) {
			remove_reference<IS> i = is_;
			return ::read<uint16, endianness::big>(i);
		}

		template<typename Handler>
		reader<IS, stage::methods>
		read_and_get_methods_reader(Handler&& handler)
		requires (Stage == stage::fields) {
			uint16 count = ::read<uint16, endianness::big>(is_);

			for(uint16 x = 0; x < count; ++x) {
				is_ = handler(field::reader{ forward<IS>(is_) });
			}

			return { forward<IS>(is_) };
		}

		uint16 get_count () const
		requires (Stage == stage::methods) {
			remove_reference<IS> i = is_;
			return ::read<uint16, endianness::big>(i);
		}

		template<typename Handler>
		reader<IS, stage::attributes>
		read_and_get_attributes_reader(Handler&& handler)
		requires (Stage == stage::methods) {
			uint16 count = ::read<uint16, endianness::big>(is_);

			for(uint16 x = 0; x < count; ++x) {
				is_ = handler(method::reader{ forward<IS>(is_) });
			}

			return { forward<IS>(is_) };
		}

		template<typename Mapper, typename Handler>
		void read(
			Mapper&& mapper, Handler&& handler
		)
		requires (Stage == stage::attributes) {
			uint16 count = ::read<uint16, endianness::big>(is_);
			while(count > 0) {
				--count;
				is_ = attribute::reader{ forward<IS>(is_) }
				.read_and_get_advanced_iterator(
					forward<Mapper>(mapper),
					handler
				);
			}
		}

		template<typename Mapper, typename Handler>
		IS read_and_get_advanced_iterator(
			Mapper&& mapper, Handler&& handler
		)
		requires (Stage == stage::attributes) {
			read(
				forward<Mapper>(mapper),
				forward<Handler>(handler)
			);
			return forward<IS>(is_);
		}

	}; // reader

	template<basic_input_stream<uint8> IS, stage Stage = stage::magic>
	reader(IS&&) -> reader<IS, Stage>;

} // class_file