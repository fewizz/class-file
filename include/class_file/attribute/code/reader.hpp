#pragma once

#include "./reader_writer_stages.hpp"
#include "./instruction.hpp"
#include "./read_instruction.hpp"
#include "./exception_handler.hpp"
#include "../../attribute/type.hpp"
#include "../../attribute/__reader/reader.hpp"

#include <loop_action.hpp>
#include <tuple.hpp>
#include <read.hpp>

namespace class_file::attribute::code {

	template<
		basic_input_stream<uint8> IS,
		stage Stage = stage::max_stack
	>
	class reader {
		IS is_;
	public:

		static constexpr attribute::type attribute_type = type::code;

		reader(IS&& is) : is_{ ::forward<IS>(is) } {}

		tuple<
			uint16,
			reader<IS, stage::max_locals>
		>
		read_max_stack_and_get_max_locals_reader()
		requires (Stage == stage::max_stack) {
			uint16 max_stack = ::read<uint16, endianness::big>(is_);
			return { { max_stack }, { ::forward<IS>(is_) } };
		}

		tuple<uint16, reader<IS, stage::code>>
		read_and_get_code_reader()
		requires (Stage == stage::max_locals) {
			uint16 max_locals = ::read<uint16, endianness::big>(is_);
			return { { max_locals }, { ::forward<IS>(is_) } };
		}

		reader<IS, stage::exception_table>
		skip_and_get_exception_table_reader()
		requires (Stage == stage::code && contiguous_iterator<IS>) {
			uint32 length = ::read<uint32, endianness::big>(is_);
			while(length > 0) {
				::read<uint8>(is_);
				--length;
			}
			return { ::forward<IS>(is_) };
		}

		auto read_as_span() const
		requires (Stage == stage::code && contiguous_iterator<IS>) {
			IS i = is_;
			uint32 length = ::read<uint32, endianness::big>(i);
			return span{ i, length };
		}

		template<typename Handler>
		reader<IS, stage::exception_table>
		read_and_get_exception_table_reader(Handler&& handler)
		requires (Stage == stage::code && contiguous_iterator<IS>) {
			uint32 length = ::read<uint32, endianness::big>(is_);
			IS end = is_ + length;
			IS begin = is_;

			while(is_ - begin < length) {
				instruction::read(is_, begin, ::forward<Handler>(handler));
			}

			return { end };
		}

		uint16 read_count() const
		requires (Stage == stage::exception_table && contiguous_range<IS>) {
			IS i = is_;
			uint16 count = ::read<uint16, endianness::big>(i);
			return count;
		}

		template<typename Handler>
		reader<IS, stage::attributes>
		read_and_get_attributes_reader(Handler&& handler)
		requires (Stage == stage::exception_table && contiguous_range<IS>) {
			uint16 count = ::read<uint16, endianness::big>(is_);
			IS end = is_ + sizeof(uint16) * count * 4;

			while(count > 0) {
				uint16 start_pc = ::read<uint16, endianness::big>(is_);
				uint16 end_pc = ::read<uint16, endianness::big>(is_);
				uint16 handler_pc = ::read<uint16, endianness::big>(is_);
				constant::class_index catch_type {
					::read<uint16, endianness::big>(is_)
				};

				loop_action action = handler(exception_handler {
					start_pc, end_pc,
					handler_pc,
					catch_type
				});

				switch (action) {
					case loop_action::next: break;
					case loop_action::stop: return { end };
				}

				--count;
			}

			return { end };
		}

		template<typename Mapper, typename Handler>
		IS read_and_get_advanced_iterator(
			Mapper&& mapper, Handler&& handler
		)
		requires (Stage == stage::attributes) {
			uint16 count = ::read<uint16, endianness::big>(is_);
			while(count > 0) {
				--count;
				is_ =
					attribute::reader{ forward<IS>(is_) }
					.read_and_get_advanced_iterator(
						forward<Mapper>(mapper),
						handler
					);
			}
			return forward<IS>(is_);
		}

	};

	template<basic_input_stream<uint8> IS>
	reader(IS&&) -> reader<IS>;

} // class_file::attribute::code