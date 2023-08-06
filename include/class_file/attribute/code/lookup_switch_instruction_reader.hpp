#pragma once

#include "./instruction.hpp"

namespace class_file::attribute::code::instruction {

	template<
		basic_input_stream<uint8> IS,
		lookup_switch::stage Stage
	>
	struct lookup_switch::reader {
	private:
		IS is_;
		nuint count_;
	public:

		reader(IS&& is) : is_{ forward<IS>(is) } {}

		reader(IS&& is, nuint count) :
			is_{ forward<IS>(is) },
			count_{count}
		{}

		template<typename Handler>
		reader<IS, stage::range>
		align_and_get_info_reader(Handler&& handler)
		requires(Stage == stage::align)
		{
			handler(is_);
			return { forward<IS>(is_) };
		}

		tuple<lookup_switch, reader<IS, stage::pairs>>
		read_and_get_matches_and_offsets_reader()
		requires(Stage == stage::range)
		{
			int32 _default    = ::read<int32, endianness::big>(is_);
			int32 pairs_count = ::read<int32, endianness::big>(is_);
			return {
				lookup_switch {
					._default = _default,
					.pairs_count = pairs_count
				},
				reader<IS, stage::pairs> {
					forward<IS>(is_),
					(nuint) pairs_count
				}
			};
		}

		void skip() {
			while(count_ > 0) {
				::read<int32, endianness::big>(is_);
				::read<int32, endianness::big>(is_);
				--count_;
			}
		}

		template<typename Handler, typename DefaultHandler = decltype([]{})>
		void read(
			Handler&& handler,
			DefaultHandler&& default_handler = {}
		)
		requires(Stage == stage::pairs)
		{
			while(count_ > 0) {
				int32 match = ::read<int32, endianness::big>(is_);
				int32 offset = ::read<int32, endianness::big>(is_);
				lookup_switch::match_and_offset pair{ match, offset };

				if constexpr(same_as<decltype(handler(pair)), loop_action>) {
					switch(handler(pair)) {
						case loop_action::stop:
							return;
						case loop_action::next:
							// nothin
					}
				}
				else {
					handler(pair);
				}
				--count_;
			}
			default_handler();
		}

	};
}