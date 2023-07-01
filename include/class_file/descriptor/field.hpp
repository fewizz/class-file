#pragma once

#include "./read.hpp"

#include <input_stream.hpp>
#include <on_scope_exit.hpp>
#include <unicode/utf8.hpp>
#include <overloaded.hpp>

namespace class_file {

	template<
		basic_input_stream<utf8::unit> IS,
		typename Handler,
		typename ErrorHandler
	>
	constexpr auto read_field_descriptor(
		IS&& is,
		Handler&& handler,
		ErrorHandler&& error_handler
	) -> decltype(handler(class_file::object{})) {
		utf8::unit first_char = read<utf8::unit>(is);
		return read_non_void_descriptor(
			first_char,
			forward<IS>(is),
			forward<Handler>(handler),
			forward<ErrorHandler>(error_handler)
		);
	}

} // class_file