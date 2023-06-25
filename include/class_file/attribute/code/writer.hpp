#pragma once

#include "./reader_writer_stages.hpp"
#include "./instruction.hpp"
#include "./write_instruction.hpp"
#include "./exception_handler.hpp"
#include "../../attribute/type.hpp"
#include "../../attribute/__writer/writer.hpp"

#include <loop_action.hpp>
#include <write.hpp>

namespace class_file::attribute::code {

	template<
		basic_iterator Iterator,
		stage Stage = stage::max_stack
	>
	class writer {
		const Iterator iterator_;
	public:

		static constexpr attribute::type attribute_type = type::code;

		writer(Iterator iterator) : iterator_{ iterator } {}

		writer<Iterator, stage::max_locals>
		write_and_get_max_locals_writer(uint16 max_stack) const
		requires (Stage == stage::max_stack) {
			Iterator i = iterator_;
			::write<uint16, endianness::big>(max_stack, i);
			return { i };
		}

		writer<Iterator, stage::code>
		write_and_get_code_writer(uint16 max_locals) const
		requires (Stage == stage::max_locals) {
			Iterator i = iterator_;
			::write<uint16, endianness::big>(max_locals, i);
			return { i };
		}

		template<typename Handler>
		writer<Iterator, stage::exception_table>
		write_and_get_exception_table_writer(
			uint32 length, Handler&& handler
		) const
		requires (Stage == stage::code) {
			Iterator i = iterator_;
			::write<uint32, endianness::big>(length, i);
			Iterator begin = i;

			handler([&](auto instruction) {
				instruction::write(i, instruction, begin);
			});

			return { i };
		}

		template<typename Handler>
		writer<Iterator, stage::attributes>
		write_and_get_attributes_writer(
			uint16 count, Handler&& handler
		) const
		requires (Stage == stage::exception_table) {
			Iterator i = iterator_;

			::write<uint16, endianness::big>(count, i);

			handler([&](
				uint16 start_pc,
				uint16 end_pc,
				uint16 handler_pc,
				constant::class_index catch_type
			) {
				::write<endianness::big, uint16>(start_pc, i);
				::write<endianness::big, uint16>(end_pc, i);
				::write<endianness::big, uint16>(handler_pc, i);
				::write<endianness::big, uint16>(catch_type, i);
			});

			return { i };
		}

		template<typename Mapper, typename Handler>
		Iterator write_and_get_advanced_iterator(
			uint16 count, Mapper&& mapper, Handler&& handler
		) const
		requires (Stage == stage::attributes) {
			Iterator i = iterator_;
			::write<uint16, endianness::big>(count, i);

			while(count > 0) {
				--count;
				i = attribute::writer{ i }
				.read_and_get_advanced_iterator(
					forward<Mapper>(mapper),
					handler
				);
			}

			return i;
		}

	};

	template<basic_iterator Iterator>
	writer(Iterator) -> writer<Iterator>;

}