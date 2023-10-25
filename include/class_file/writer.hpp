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

	template<basic_output_stream<uint8> OS, stage Stage = stage::magic>
	class writer {
		const OS os_;
	public:

		writer(OS&& os) : os_{ forward<OS>(os) } {}

		writer<OS, stage::version>
		write_magic_and_get_version_writer() const
		requires (Stage == stage::magic) {
			OS i = os_;
			::write<endianness::big, uint32>(0xCAFEBABE, i);
			return { i };
		}

		writer<OS, stage::constant_pool>
		write_and_get_constant_pool_writer(uint16 major, uint16 minor) const
		requires (Stage == stage::version) {
			OS i = os_;
			::write<endianness::big, uint16>(minor, i);
			::write<endianness::big, uint16>(major, i);
			return { i };
		}

		template<typename Handler>
		writer<OS, stage::access_flags>
		write_and_get_access_flags_writer(
			uint16 entries_count, Handler&& handler
		) const
		requires (Stage == stage::constant_pool) {
			// "constant_pool_count is equal to the number of entries
			//  in the constant_pool table plus one"
			++entries_count;
			::write<endianness::big, uint16>(entries_count, os_);

			constant::writer w{ os_ };
			handler(w);

			return { forward<OS>(os_) };
		}

		writer<OS, stage::this_class>
		write_and_get_this_class_writer(
			access_flags access_flags
		) const
		requires (Stage == stage::access_flags) {
			::write<endianness::big, access_flags>(access_flags, os_);
			return { forward<OS>(os_) };
		}

		writer<OS, stage::super_class>
		write_and_get_super_class_writer(
			constant::class_index class_index
		) const
		requires (Stage == stage::this_class) {
			OS i = os_;
			::write<endianness::big, uint16>(class_index, i);
			return { i };
		}

		writer<OS, stage::interfaces>
		write_and_get_interfaces_writer(
			constant::class_index class_index
		) const
		requires (Stage == stage::super_class) {
			OS i = os_;
			::write<endianness::big, uint16>(class_index, i);
			return { i };
		}

		template<typename Handler>
		writer<OS, stage::fields>
		write_and_get_fields_writer(uint16 count, Handler&& handler) const
		requires (Stage == stage::interfaces) {
			OS i = os_;
			::write<endianness::big, uint16>(count, i);

			handler([&](constant::class_index ci) {
				::write<uint16, endianness::big>(i, ci);
			});

			return { i };
		}

		template<typename Handler>
		writer<OS, stage::methods>
		write_and_get_methods_writer(uint16 count, Handler&& handler) const
		requires (Stage == stage::fields) {
			OS i = os_;
			::write<endianness::big, uint16>(count, i);

			handler([&]() {
				return field::writer{ i };
			});

			return { i };
		}

		template<typename Handler>
		writer<OS, stage::attributes>
		read_and_get_attributes_writer(uint16 count, Handler&& handler) const
		requires (Stage == stage::methods) {
			OS i = os_;
			::write<endianness::big, uint16>(count, i);
			handler([&]() {
				return method::writer{ i };
			});
			return { i };
		}

		template<typename Mapper, typename Handler>
		OS
		read_and_get_advanced_iterator(
			Mapper&& mapper, Handler&& handler
		) const
		requires (Stage == stage::attributes) {
			OS i = os_;
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

	template<basic_output_stream<uint8> OS, stage Stage = stage::magic>
	writer(OS&&) -> writer<OS, Stage>;

} // class_file