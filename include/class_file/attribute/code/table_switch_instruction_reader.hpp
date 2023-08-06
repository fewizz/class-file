#pragma once

#include "./instruction.hpp"

namespace class_file::attribute::code::instruction {

	template<
		basic_input_stream<uint8> IS,
		table_switch::stage Stage
	>
	struct table_switch::reader {
	private:
		IS is_;
		nuint count_;

	public:

		reader(IS&& is_, nuint count) :
			is_{ forward<IS>(is_) },
			count_{count}
		{}

		reader(IS&& is_) : is_{ forward<IS>(is_) } {}

		template<typename Handler>
		reader<IS, stage::range>
		align_and_get_info_reader(Handler&& handler)
		requires (Stage == stage::align)
		{
			handler(is_);
			return { forward<IS>(is_) };
		}

		tuple<table_switch, reader<IS, stage::offsets>>
		read_and_get_offsets_reader()
		requires (Stage == stage::range)
		{
			int32 _default = ::read<int32, endianness::big>(is_);
			int32 low      = ::read<int32, endianness::big>(is_);
			int32 high     = ::read<int32, endianness::big>(is_);
			return {
				table_switch {
					._default = _default,
					.low = low,
					.high = high
				},
				reader<IS, stage::offsets> {
					forward<IS>(is_),
					(nuint)(high + 1 - low)
				}
			};
		}

		template<typename Handler>
		IS read_and_get_advanced_input_stream(Handler&& handler)
		requires (Stage == stage::offsets)
		{
			while(count_ > 0) {
				handler(offset{ ::read<int32, endianness::big>(is_) });
				--count_;
			}
			return forward<IS>(is_);
		}

		void skip()
		requires (Stage == stage::offsets)
		{
			while(count_ > 0) {
				::read<int32, endianness::big>(is_);
				--count_;
			}
		}

		IS skip_and_get_advanced_input_stream()
		requires (Stage == stage::offsets)
		{
			skip();
			return forward<IS>(is_);
		}

		offset get_offset(uint32 index)
		requires (Stage == stage::offsets && random_access_iterator<IS>)
		{
			remove_reference<IS> is = is_ + index * sizeof(int32);
			offset off { ::read<int32, endianness::big>(is) };
			return off;
		}

		tuple<offset, remove_reference<IS>>
		get_offset_and_advanced_input_stream(uint32 index)
		requires (Stage == stage::offsets && random_access_iterator<IS>)
		{
			remove_reference<IS> is = is_ + index * sizeof(int32);
			offset off { ::read<int32, endianness::big>(is) };
			return {
				off,
				is
			};
		}
	};

}