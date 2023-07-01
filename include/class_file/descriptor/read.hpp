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
constexpr auto read_non_void_non_array_descriptor(
	utf8::unit first_char,
	IS&& is,
	Handler&& handler,
	ErrorHandler&& error_handler
) -> decltype(handler(class_file::object{})) {
	switch (first_char) {
		case u8'B': return handler.template operator () <class_file::b> ();
		case u8'C': return handler.template operator () <class_file::c> ();
		case u8'D': return handler.template operator () <class_file::d> ();
		case u8'F': return handler.template operator () <class_file::f> ();
		case u8'I': return handler.template operator () <class_file::i> ();
		case u8'J': return handler.template operator () <class_file::j> ();
		case u8'S': return handler.template operator () <class_file::s> ();
		case u8'Z': return handler.template operator () <class_file::z> ();
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

				return handler.operator () (name);
			}
			else {
				[]<bool b = false>{ static_assert(b); }();
			}
		}
		default: {
			return error_handler(type_descriptor_read_error::unknown_type);
		}
	}
}

template<
	basic_input_stream<utf8::unit> IS,
	typename Handler,
	typename ErrorHandler
>
constexpr auto read_non_void_descriptor(
	utf8::unit first_char,
	IS&& is,
	Handler&& handler,
	ErrorHandler&& error_handler
) -> decltype(handler(class_file::object{})) {

	if(first_char != '[') {
		return read_non_void_non_array_descriptor(
			first_char,
			is,
			forward<Handler>(handler),
			forward<ErrorHandler>(error_handler)
		);
	}

	if constexpr(contiguous_iterator<IS>) {
		remove_reference<IS> beginning = is - 1;
		uint8 rank = 1;
		utf8::unit ch;
		while((ch = read<utf8::unit>(is)) == '[') {
			++rank;
		}
		bool error_happened = false;
		using error_handler_return_type = decltype(
			error_handler(type_descriptor_read_error{})
		);

		if constexpr(!same_as<error_handler_return_type, void>) {
			optional<error_handler_return_type> error_handler_result;
			read_non_void_non_array_descriptor(
				ch,
				is,
				[&]<typename...>(auto...) {},
				[&](auto error) {
					error_happened = true;
					error_handler_result = error_handler(error);
				}
			);
			if(error_happened) {
				return error_handler_result.move();
			}
		}
		else {
			read_non_void_non_array_descriptor(
				ch,
				is,
				[&]<typename...>(auto...) {},
				[&](auto error) {
					error_happened = true;
					error_handler(error);
				}
			);
			if(error_happened) {
				return;
			}
		}

		remove_reference<IS> ending = is;

		c_string_of_known_size<utf8::unit> name {
			iterator_and_sentinel{ beginning, ending }.as_range()
		};

		return handler.operator () (class_file::array{ name, rank });
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
constexpr auto read_possibly_void_descriptor(
	utf8::unit first_char,
	IS&& is,
	Handler&& handler,
	ErrorHandler&& error_handler
) -> decltype(handler(class_file::object{})) {
	if(first_char == 'V') {
		return handler.template operator () <class_file::v> ();
	}
	else {
		return read_non_void_descriptor(
			first_char,
			forward<IS>(is),
			forward<Handler>(handler),
			forward<ErrorHandler>(error_handler)
		);
	}
}

} // class_file