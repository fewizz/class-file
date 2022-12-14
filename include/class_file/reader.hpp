#pragma once

#include "./version.hpp"
#include "./constant.hpp"
#include "./access_flag.hpp"

#include "./field_reader.hpp"
#include "./method_reader.hpp"
#include "./constant_reader.hpp"

#include <tuple.hpp>
#include <array.hpp>

namespace class_file {

	enum class reader_stage {
		magic, version, constant_pool, access_flags,
		this_class, super_class,
		interfaces, fields, methods, attributes
	};

	template<typename Iterator, reader_stage Stage = reader_stage::magic>
	class reader {

		Iterator iterator_;

	public:

		reader(Iterator it) : iterator_{ it } {}

		tuple<bool, reader<Iterator, reader_stage::version>>
		check_for_magic_and_get_version_reader() const
		requires (Stage == reader_stage::magic) {
			Iterator i = iterator_;
			uint32 val = read<uint32, endianness::big>(i);
			bool result = val == 0xCAFEBABE;
			return { result, { i } };
		}

		tuple<
			class_file::version, reader<Iterator, reader_stage::constant_pool>
		>
		read_and_get_constant_pool_reader() const
		requires (Stage == reader_stage::version) {
			Iterator i = iterator_;
			uint16 minor = read<uint16, endianness::big>(i);
			uint16 major = read<uint16, endianness::big>(i);
			return { { minor, major }, { i } };
		}

		template<typename Handler>
		reader<Iterator, reader_stage::access_flags>
		read_and_get_access_flags_reader(Handler&& handler) const
		requires (Stage == reader_stage::constant_pool) {
			Iterator i = iterator_;
			 // "constant_pool_count is equal to the number of entries
			 // in the constant_pool table plus one"
			uint16 entries_count = read<uint16, endianness::big>(i);
			--entries_count; // minus one

			while(entries_count > 0) {
				i = constant::reader{ i }.read_and_get_advanced_iterator(
					[&](auto constant) {
						--entries_count;
						handler(constant);
					}
				);
			}

			return { i };
		}


		uint16 entries_count() const
		requires (Stage == reader_stage::constant_pool) {
			uint16 constant_pool_size = 0;
			read_and_get_access_flags_reader(
				[&](auto) { ++constant_pool_size; }
			);
			return constant_pool_size;
		}

		tuple<
			access_flags, reader<Iterator, reader_stage::this_class>
		>
		read_and_get_this_class_reader() const
		requires (Stage == reader_stage::access_flags) {
			Iterator i = iterator_;
			class_file::access_flags flags {
				read<access_flag, endianness::big>(i)
			};
			return { flags, { i } };
		}

		tuple<
			constant::class_index, reader<Iterator, reader_stage::super_class>
		>
		read_and_get_super_class_reader() const
		requires (Stage == reader_stage::this_class) {
			Iterator i = iterator_;
			constant::class_index this_class_index {
				read<uint16, endianness::big>(i)
			};
			return { this_class_index, { i } };
		}

		tuple<
			constant::class_index, reader<Iterator, reader_stage::interfaces>
		>
		read_and_get_interfaces_reader() const
		requires (Stage == reader_stage::super_class) {
			Iterator i = iterator_;
			constant::class_index super_class_index {
				read<uint16, endianness::big>(i)
			};
			return { super_class_index, { i } };
		}

		uint16 count() const
		requires (Stage == reader_stage::interfaces) {
			Iterator i = iterator_;
			return read<uint16, endianness::big>(i);
		}

		template<typename Handler>
		reader<Iterator, reader_stage::fields>
		read_and_get_fields_reader(Handler&& handler) const
		requires (Stage == reader_stage::interfaces) {
			Iterator i = iterator_;
			uint16 count = read<uint16, endianness::big>(i);
			for(uint16 x = 0; x < count; ++x) {
				constant::interface_index index {
					read<uint16, endianness::big>(i)
				};
				handler(index);
			}
			return { i };
		}

		uint16 count () const
		requires (Stage == reader_stage::fields) {
			Iterator i = iterator_;
			return read<uint16, endianness::big>(i);
		}

		template<typename Handler>
		reader<Iterator, reader_stage::methods>
		read_and_get_methods_reader(Handler&& handler) const
		requires (Stage == reader_stage::fields) {
			Iterator i = iterator_;
			uint16 count = read<uint16, endianness::big>(i);

			for(uint16 x = 0; x < count; ++x) {
				i = handler(field::reader{ i });
			}

			return { i };
		}

		uint16 count () const
		requires (Stage == reader_stage::methods) {
			Iterator i = iterator_;
			return read<uint16, endianness::big>(i);
		}

		template<typename Handler>
		reader<Iterator, reader_stage::attributes>
		read_and_get_attributes_reader(Handler&& handler) const
		requires (Stage == reader_stage::methods) {
			Iterator i = iterator_;
			uint16 count = read<uint16, endianness::big>(i);

			for(uint16 x = 0; x < count; ++x) {
				i = handler(method::reader{ i });
			}

			return { i };
		}

		template<typename Mapper, typename Handler>
		Iterator
		read_and_get_advanced_iterator(
			Mapper&& mapper, Handler&& handler
		) const
		requires (Stage == reader_stage::attributes) {
			Iterator i = iterator_;
			uint16 count = ::read<uint16, endianness::big>(i);
			while(count > 0) {
				--count;
				i = attribute::reader{ i }.read_and_get_advanced_iterator(
					forward<Mapper>(mapper),
					[&](auto attribute_reader) {
						handler(attribute_reader);
					}
				);
			}
			return i;
		}

	};

	template<typename Iterator, reader_stage Stage = reader_stage::magic>
	reader(Iterator) -> reader<Iterator, Stage>;

}