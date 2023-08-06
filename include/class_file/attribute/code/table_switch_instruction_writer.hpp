#pragma once

#include "./instruction.hpp"

namespace class_file::attribute::code::instruction {

	template<
		basic_output_stream<uint8> OS,
		table_switch::stage Stage
	>
	struct table_switch::writer {
	private:
		OS os_;
		nuint count_;

	public:

		writer(OS&& os_, nuint count) :
			os_{ forward<OS>(os_) },
			count_{count}
		{}

		writer(OS&& os_) : os_{ forward<OS>(os_) } {}

		template<typename Handler>
		writer<OS, stage::range>
		align_and_get_info_writer(Handler&& handler)
		requires (Stage == stage::align)
		{
			handler(os_);
			return { forward<OS>(os_) };
		}

		reader<OS, stage::offsets>
		write_and_get_offsets_writer(table_switch info)
		requires (Stage == stage::range)
		{
			::write<int32, endianness::big>(os_, info._default);
			::write<int32, endianness::big>(os_, info.low);
			::write<int32, endianness::big>(os_, info.high);
			return {
				forward<OS>(os_),
				(nuint)(info.high + 1 - info.low)
			};
		}

		template<typename Handler>
		OS write_and_get_advanced_input_stream(Handler&& handler)
		requires (Stage == stage::offsets)
		{
			while(count_ > 0) {
				offset off = handler();
				::write<int32, endianness::big>(os_, off.value);
				--count_;
			}
			return forward<OS>(os_);
		}

	};

}