#pragma once

#include "../../constant.hpp"

#include <write.hpp>
#include <array.hpp>
#include <range.hpp>
#include <c_string.hpp>

namespace class_file::attribute {

	template<basic_iterator Iterator>
	class writer {
		const Iterator iterator_;
	public:

		writer(Iterator iterator) : iterator_{ iterator } {}

		writer(const writer& ) = delete;
		writer(      writer&&) = delete;

		Iterator iterator_copy() const { return iterator_; }

		template<
			typename IndexToUtf8Mapper,
			typename Handler
		>
		void write(
			constant::utf8_index name_index,
			uint32 length,
			IndexToUtf8Mapper&& mapper,
			Handler&& handler
		) const;

	};

} // class_file::attribute