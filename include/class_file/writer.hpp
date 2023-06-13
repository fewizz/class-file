#pragma once

#include "./reader_writer_stages.hpp"
#include "./constant_writer.hpp"
#include "./version.hpp"
#include "./constant.hpp"
#include "./access_flag.hpp"
#include "./field_writer.hpp"
#include "./method_writer.hpp"

#include <write.hpp>

namespace class_file {

	template<basic_iterator Iterator, stage Stage = stage::magic>
	class writer {
		const Iterator iterator_;
	public:

		writer(Iterator iterator) : iterator_{ iterator } {}

		writer<Iterator, stage::version>
		write_magic_and_get_version_writer() const
		requires (Stage == stage::magic) {
			Iterator i = iterator_;
			::write<endianness::big, uint32>(0xCAFEBABE, i);
			return { i };
		}

		writer<Iterator, stage::constant_pool>
		write_and_get_constant_pool_writer(uint16 major, uint16 minor) const
		requires (Stage == stage::version) {
			Iterator i = iterator_;
			::write<endianness::big, uint16>(minor, i);
			::write<endianness::big, uint16>(major, i);
			return { i };
		}

		template<typename Handler>
		writer<Iterator, stage::access_flags>
		write_and_get_access_flags_writer(
			uint16 entries_count, Handler&& handler
		) const
		requires (Stage == stage::constant_pool) {
			Iterator i = iterator_;

			// "constant_pool_count is equal to the number of entries
			//  in the constant_pool table plus one"
			++entries_count;
			::write<endianness::big, uint16>(entries_count, i);

			constant::writer w{ i };
			handler(w);

			return { w.iterator_copy() };
		}

		writer<Iterator, stage::this_class>
		write_and_get_this_class_writer(
			access_flags access_flags
		) const
		requires (Stage == stage::access_flags) {
			Iterator i = iterator_;
			::write<endianness::big, access_flags>(access_flags, i);
			return { i };
		}

		writer<Iterator, stage::super_class>
		write_and_get_super_class_writer(
			constant::class_index class_index
		) const
		requires (Stage == stage::this_class) {
			Iterator i = iterator_;
			::write<endianness::big, uint16>(class_index, i);
			return { i };
		}

		writer<Iterator, stage::interfaces>
		write_and_get_interfaces_writer(
			constant::class_index class_index
		) const
		requires (Stage == stage::super_class) {
			Iterator i = iterator_;
			::write<endianness::big, uint16>(class_index, i);
			return { i };
		}

		template<typename Handler>
		writer<Iterator, stage::fields>
		write_and_get_fields_writer(uint16 count, Handler&& handler) const
		requires (Stage == stage::interfaces) {
			Iterator i = iterator_;
			::write<endianness::big, uint16>(count, i);

			handler([&](constant::class_index ci) {
				::write<uint16, endianness::big>(i, ci);
			});

			return { i };
		}

		template<typename Handler>
		writer<Iterator, stage::methods>
		write_and_get_methods_writer(uint16 count, Handler&& handler) const
		requires (Stage == stage::fields) {
			Iterator i = iterator_;
			::write<endianness::big, uint16>(count, i);

			handler([&]() {
				return field::writer{ i };
			});

			return { i };
		}

		template<typename Handler>
		writer<Iterator, stage::attributes>
		read_and_get_attributes_writer(uint16 count, Handler&& handler) const
		requires (Stage == stage::methods) {
			Iterator i = iterator_;
			::write<endianness::big, uint16>(count, i);
			handler([&]() {
				return method::writer{ i };
			});
			return { i };
		}

		template<typename Mapper, typename Handler>
		Iterator
		read_and_get_advanced_iterator(
			Mapper&& mapper, Handler&& handler
		) const
		requires (Stage == stage::attributes) {
			Iterator i = iterator_;
			uint16 count = ::read<uint16, endianness::big>(i);
			while(count > 0) {
				--count;
				i = attribute::writer{ i }.read_and_get_advanced_iterator(
					forward<Mapper>(mapper),
					handler
				);
			}
			return i;
		}

	}; // writer

	template<basic_iterator Iterator, stage Stage = stage::magic>
	writer(Iterator) -> writer<Iterator, Stage>;

} // class_file