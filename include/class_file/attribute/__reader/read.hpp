#pragma once

#include "./reader.hpp"

#include "./../code/reader.hpp"
#include "./../bootstrap_methods/reader.hpp"
#include "./../line_numbers/reader.hpp"
#include "./../source_file/reader.hpp"

template<typename Iterator>
template<
	typename IndexToUtf8Mapper,
	typename Handler
>
Iterator
class_file::attribute::reader<Iterator>::read_and_get_advanced_iterator(
	IndexToUtf8Mapper&& mapper, Handler&& handler
) const {
	Iterator i = iterator_;
	constant::name_index name_index {
		read<uint16, endianness::big>(i)
	};
	uint32 length = read<uint32, endianness::big>(i);
	constant::utf8 name = mapper(name_index);

	if(
		name.have_elements_equal_to(c_string{ "Code" })
	) { handler(code::reader{ i }); }
	else if(
		name.have_elements_equal_to(c_string{ "BootstrapMethods" })
	) { handler(bootstrap_methods::reader{ i }); }
	else if(
		name.have_elements_equal_to(c_string{ "LineNumberTable" })
	) { handler(line_numbers::reader{ i }); }
	else if(
		name.have_elements_equal_to(c_string{ "SourceFile" })
	) { handler(source_file::reader{ i }); }

	i += length;
	return i;
}