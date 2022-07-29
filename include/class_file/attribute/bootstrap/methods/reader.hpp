#pragma once

#include "./count.hpp"
#include "../method/reader.hpp"
#include "../../type.hpp"

#include <core/read.hpp>
#include <core/meta/elements/of.hpp>

namespace class_file::attribute::bootstrap::methods {

	enum class reader_stage {
		count,
		methods,
		end
	};

	template<typename Iterator, reader_stage Stage = reader_stage::count>
	class reader {
		Iterator iterator_;
	public:

		reader(Iterator it) : iterator_{ it } {}

		static constexpr attribute::type attribute_type {
			type::bootstrap_methods
		};

		elements::of<reader<Iterator, reader_stage::methods>, methods::count>
		operator () () requires (Stage == reader_stage::count) {
			Iterator cpy = iterator_;
			uint32 length = read<uint32>(cpy);
			return { { cpy }, { length } };
		}

		template<typename Handler>
		requires (Stage == reader_stage::methods)
		reader<Iterator, reader_stage::end>
		operator () (methods::count count, Handler&& handler) {
			Iterator cpy = iterator_;

			while(count > 0) {
				--count._;
				handler(bootstrap::method::reader{ cpy });
			}
		}

	};

} // bootstrap_methods