#pragma once

#include "./type.hpp"

#include <unicode/utf8.hpp>
#include <iterator_and_sentinel.hpp>
#include <overloaded.hpp>

namespace class_file {

enum class type_descriptor_read_error {
	unknown_type
};

template<
	basic_input_stream<utf8::unit> IS,
	typename Handler,
	typename ErrorHandler
>
constexpr void read_non_void_non_array_descriptor(
	utf8::unit first_char,
	IS&& is,
	Handler&& handler,
	ErrorHandler&& error_handler
) {
	switch (first_char) {
		case u8'B': handler.template operator () <class_file::b> (); return;
		case u8'C': handler.template operator () <class_file::c> (); return;
		case u8'D': handler.template operator () <class_file::d> (); return;
		case u8'F': handler.template operator () <class_file::f> (); return;
		case u8'I': handler.template operator () <class_file::i> (); return;
		case u8'J': handler.template operator () <class_file::j> (); return;
		case u8'S': handler.template operator () <class_file::s> (); return;
		case u8'Z': handler.template operator () <class_file::z> (); return;
		case u8'L': {
			if constexpr(
				contiguous_iterator<IS>
			) {
				remove_reference<IS> beginning = is;
				while(read<utf8::unit>(is) != ';');
				remove_reference<IS> ending = is - 1;

				class_file::object name {
					iterator_and_sentinel { beginning, ending }.as_range()
				};

				handler.operator () (name);
			}
			else {
				[]<bool b = false>{ static_assert(b); }();
			}
			return;
		}
		default: {
			error_handler(type_descriptor_read_error::unknown_type);
			return;
		}
	}
}

template<
	basic_input_stream<utf8::unit> IS,
	typename Handler,
	typename ErrorHandler
>
constexpr void read_non_void_descriptor(
	utf8::unit first_char,
	IS&& is,
	Handler&& handler,
	ErrorHandler&& error_handler
) {

	if(first_char != '[') {
		read_non_void_non_array_descriptor(
			first_char,
			is,
			forward<Handler>(handler),
			forward<ErrorHandler>(error_handler)
		);
		return;
	}

	if constexpr(contiguous_iterator<IS>) {
		remove_reference<IS> beginning = is - 1;
		uint8 rank = 1;
		utf8::unit ch;
		while((ch = read<utf8::unit>(is)) == '[') {
			++rank;
		}
		bool error_happened = false;
		read_non_void_non_array_descriptor(
			ch,
			is,
			[]<typename...>(auto...){},
			[&](auto error) {
				error_happened = true;
				error_handler(error);
			}
		);
		if(error_happened) {
			return;
		}

		remove_reference<IS> ending = is;

		c_string_of_known_size<utf8::unit> name {
			iterator_and_sentinel{ beginning, ending }.as_range()
		};

		handler.operator () (class_file::array{ name, rank });
	}
	else {
		[]<bool b = false>{ static_assert(b); }();
	}
}

template<
	basic_input_stream<utf8::unit> IS,
	typename Handler,
	typename ErrorHandler
>
constexpr void read_possibly_void_descriptor(
	utf8::unit first_char,
	IS&& is,
	Handler&& handler,
	ErrorHandler&& error_handler
) {
	if(first_char == 'V') {
		handler.template operator () <class_file::v> ();
	}
	else {
		read_non_void_descriptor(
			first_char,
			forward<IS>(is),
			forward<Handler>(handler),
			forward<ErrorHandler>(error_handler)
		);
	}
}

} // class_file