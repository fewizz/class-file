#pragma once

#include "./count.hpp"
#include "./method_reader.hpp"
#include "../type.hpp"

#include <read.hpp>
#include <tuple.hpp>

namespace class_file::attribute::bootstrap_methods {

	enum class stage {
		count,
		methods
	};

	template<basic_input_stream<uint8> IS, stage Stage = stage::count>
	class reader {
		IS is_;
	public:

		reader(IS&& is) : is_{ forward<IS>(is) } {}

		static constexpr attribute::type attribute_type {
			type::bootstrap_methods
		};

		tuple<
			bootstrap_methods::count,
			reader<IS, stage::methods>
		>
		read_count_and_get_methods_reader()
		requires (Stage == stage::count) {
			bootstrap_methods::count count {
				::read<uint16, endianness::big>(is_)
			};
			return { count, { forward<IS>(is_) } };
		}

		template<typename Handler>
		requires (Stage == stage::methods)
		void read(
			bootstrap_methods::count count, Handler&& handler
		) {
			while(count > 0) {
				--count;
				handler(bootstrap::method::reader{ is_ });
			}
		}

	};

	template<basic_input_stream<uint8> IS>
	reader(IS&&) -> reader<IS>;

}