#pragma once

#include "./instruction.hpp"
#include "./read_instruction.hpp"
#include "./exception_handler.hpp"
#include "../../attribute/type.hpp"
#include "../../attribute/__reader/reader.hpp"

#include <loop_action.hpp>
#include <tuple.hpp>
#include <read.hpp>

namespace class_file::attribute::code {

	enum class reader_stage {
		max_stack,
		max_locals,
		code,
		exception_table,
		attributes
	};

	template<
		basic_iterator Iterator, reader_stage Stage = reader_stage::max_stack
	>
	class reader {
		const Iterator iterator_;
	public:

		static constexpr attribute::type attribute_type = type::code;

		reader(Iterator it) : iterator_{ it } {}

		Iterator iterator_copy() const { return iterator_; }

		tuple<uint16, reader<Iterator, reader_stage::max_locals>>
		read_and_get_max_locals_reader() const
		requires (Stage == reader_stage::max_stack) {
			Iterator i = iterator_;
			uint16 max_stack = read<uint16, endianness::big>(i);
			return { { max_stack }, { i } };
		}

		tuple<uint16, reader<Iterator, reader_stage::code>>
		read_and_get_code_reader() const
		requires (Stage == reader_stage::max_locals) {
			Iterator i = iterator_;
			uint16 max_locals = read<uint16, endianness::big>(i);
			return { { max_locals }, { i } };
		}

		reader<Iterator, reader_stage::exception_table>
		skip_and_get_exception_table_reader() const
		requires (Stage == reader_stage::code) {
			Iterator i = iterator_;
			uint32 length = read<uint32, endianness::big>(i);
			Iterator end = i + length;
			return { end };
		}

		auto read_as_span() const
		requires (Stage == reader_stage::code) {
			Iterator i = iterator_;
			uint32 length = read<uint32, endianness::big>(i);
			return span{ i, length };
		}

		template<typename Handler>
		reader<Iterator, reader_stage::exception_table>
		read_and_get_exception_table_reader(Handler&& handler) const
		requires (Stage == reader_stage::code) {
			Iterator i = iterator_;
			uint32 length = read<uint32, endianness::big>(i);
			Iterator end = i + length;
			Iterator begin = i;

			while(i - begin < length) {
				instruction::read(i, begin, forward<Handler>(handler));
			}

			return { end };
		}

		uint16 read_count() const
		requires (Stage == reader_stage::exception_table) {
			Iterator i = iterator_;
			uint16 count = read<uint16, endianness::big>(i);
			return count;
		}

		template<typename Handler>
		reader<Iterator, reader_stage::attributes>
		read_and_get_attributes_reader(Handler&& handler) const
		requires (Stage == reader_stage::exception_table) {
			Iterator i = iterator_;
			uint16 count = read<uint16, endianness::big>(i);
			Iterator end = i + sizeof(uint16) * count * 4;

			while(count > 0) {
				uint16 start_pc = read<uint16, endianness::big>(i);
				uint16 end_pc = read<uint16, endianness::big>(i);
				uint16 handler_pc = read<uint16, endianness::big>(i);
				constant::class_index catch_type {
					read<uint16, endianness::big>(i)
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
		Iterator read_and_get_advanced_iterator(
			Mapper&& mapper, Handler&& handler
		) const
		requires (Stage == reader_stage::attributes) {
			Iterator i = iterator_;
			uint16 count{ ::read<uint16, endianness::big>(i) };
			while(count > 0) {
				--count;
				i = attribute::reader{ i }
				.read_and_get_advanced_iterator(
					forward<Mapper>(mapper),
					handler
				);
			}
			return i;
		}

	};

	template<typename Iterator>
	reader(Iterator) -> reader<Iterator>;

}