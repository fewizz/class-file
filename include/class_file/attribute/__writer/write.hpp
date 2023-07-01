#pragma once

#include "./writer.hpp"
#include "../type.hpp"
#include "../code/writer.hpp"
#include "../bootstrap_methods/writer.hpp"
#include "../line_numbers/writer.hpp"
#include "../source_file/writer.hpp"

#include <c_string.hpp>

template<basic_iterator Iterator>
template<
	typename IndexToUtf8Mapper,
	typename Handler
>
void class_file::attribute::writer<Iterator>::write(
	constant::utf8_index name_index,
	uint32 length,
	IndexToUtf8Mapper&& mapper,
	Handler&& handler
) const {
	Iterator i = iterator_;

	::write<uint16, endianness::big>(name_index, i);
	constant::utf8 name = mapper(name_index);

	::write<uint32, endianness::big>(length, i);

	if(
		name.has_equal_size_and_elements(c_string{ "Code" })
	) { handler(code::writer{ i }); }
	else if(
		name.has_equal_size_and_elements(c_string{ "BootstrapMethods" })
	) { handler(bootstrap_methods::writer{ i }); }
	else if(
		name.has_equal_size_and_elements(c_string{ "LineNumberTable" })
	) { handler(line_numbers::writer{ i }); }
	else if(
		name.has_equal_size_and_elements(c_string{ "SourceFile" })
	) { handler(source_file::reader{ i }); }

	i += length;
}