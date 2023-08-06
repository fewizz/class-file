#pragma once

#include "./instruction.hpp"

namespace class_file::attribute::code::instruction {

	template<
		basic_output_stream<uint8> OS,
		lookup_switch::stage Stage
	>
	struct lookup_switch::writer {
	private:
		OS os_;
		nuint count_;
	public:

		writer(OS&& os_) : os_{ forward<OS>(os_) } {}

		writer(OS&& os, nuint count) :
			os_{ forward<OS>(os) },
			count_{count}
		{}

		template<typename Handler>
		writer<OS, stage::range>
		align_and_get_info_writer(Handler&& handler)
		requires(Stage == stage::align)
		{
			handler(os_);
			return { forward<OS>(os_) };
		}

		writer<OS, stage::pairs>
		write_and_get_matches_and_offsets_writer(lookup_switch info)
		requires(Stage == stage::range)
		{
			::write<int32, endianness::big>(os_, info._default);
			::write<int32, endianness::big>(os_, info.pairs_count);
			return {
				forward<OS>(os_),
				(nuint) info.pairs_count
			};
		}

		template<typename Handler>
		void write(Handler&& handler)
		requires(Stage == stage::pairs)
		{
			while(count_ > 0) {
				lookup_switch::match_and_offset pair = handler();
				::write<int32, endianness::big>(os_, pair.match);
				::write<int32, endianness::big>(os_, pair.offset);
				--count_;
			}
		}

	};

}