#pragma once

#include "./reader.hpp"

#include "./../code/reader.hpp"
#include "./../bootstrap_methods/reader.hpp"
#include "./../line_numbers/reader.hpp"
#include "./../source_file/reader.hpp"

template<basic_input_stream<uint8> IS>
template<
	typename IndexToUtf8Mapper,
	typename Handler
>
IS
class_file::attribute::reader<IS>::read_and_get_advanced_iterator(
	IndexToUtf8Mapper&& mapper, Handler&& handler
) {
	constant::name_index name_index {
		::read<uint16, endianness::big>(is_)
	};
	constant::utf8 name = mapper(name_index);

	uint32 length = ::read<uint32, endianness::big>(is_);

	remove_reference<IS> cpy = is_; // TODO
	if(
		name.has_equal_size_and_elements(c_string{ "Code" })
	) { handler(code::reader{ cpy }); }
	else if(
		name.has_equal_size_and_elements(c_string{ "BootstrapMethods" })
	) { handler(bootstrap_methods::reader{ cpy }); }
	else if(
		name.has_equal_size_and_elements(c_string{ "LineNumberTable" })
	) { handler(line_numbers::reader{ cpy }); }
	else if(
		name.has_equal_size_and_elements(c_string{ "SourceFile" })
	) { handler(source_file::reader{ cpy }); }

	is_ += length;
	return forward<IS>(is_);
}