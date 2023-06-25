#pragma once

#include "./read.hpp"

#include <c_string.hpp>
#include <optional.hpp>

namespace class_file::method_descriptor {

	template<
		basic_input_stream<utf8::unit> IS,
		typename Handler,
		typename ErrorHandler
	>
	void constexpr read_return_descriptor(
		IS&& is,
		Handler&& handler,
		ErrorHandler&& error_handler
	) {
		utf8::unit first_char = read<utf8::unit>(is);
		read_possibly_void_descriptor(
			first_char,
			forward<IS>(is),
			forward<Handler>(handler),
			forward<ErrorHandler>(error_handler)
		);
	}

	enum class stage {
		parameters, ret
	};

	enum class read_error {
		expected_opening_brace
	};

	template<
		basic_input_stream<utf8::unit> IS,
		stage Stage = stage::parameters
	>
	class reader {
		IS is_;
	public:

		constexpr reader(IS&& is) : is_{ forward<IS>(is) } {}

		template<typename Handler, typename ErrorHandler>
		optional<reader<IS, stage::ret>>
		constexpr try_read_parameter_types_and_get_return_type_reader(
			Handler&& handler, ErrorHandler&& error_handler
		)
		requires(Stage == stage::parameters) {
			if(read<utf8::unit>(is_) != u8'(') {
				error_handler(read_error::expected_opening_brace);
			}
			utf8::unit first_char;
			while((first_char = read<utf8::unit>(is_)) != ')') {
				bool error_happened = false;
				read_non_void_descriptor(
					first_char,
					is_,
					handler,
					[&](auto error) {
						error_happened = true; error_handler(error);
					}
				);
				if(error_happened) { return {}; }
			}
			return { reader<IS, stage::ret>{ forward<IS>(is_) } };
		}

		template<typename Handler, typename ErrorHandler>
		optional<reader<IS, stage::ret>>
		constexpr try_read_parameter_type_names_and_get_return_type_reader(
			Handler&& handler, ErrorHandler&& error_handler
		)
		requires(Stage == stage::parameters) {
			if(read<utf8::unit>(is_) != '(') {
				error_handler(read_error::expected_opening_brace);
			}
			utf8::unit first_char;
			while((first_char = read<utf8::unit>(is_)) != ')') {
				IS begin = is_;
				bool error_happened = false;
				read_parameter_type_descriptor(
					first_char,
					is_,
					[](auto){},
					[&](auto error) {
						error_happened = true; error_handler(error);
					}
				);
				if(error_happened) { return {}; }
				IS end = is_;
				handler(c_string {
					(const utf8::unit*) begin,
					iterator_and_sentinel{ begin, end }.distance()
				});
			}
			return { { forward<IS>(is_) } };
		}

		template<typename ErrorHandler>
		optional<reader<IS, stage::ret>>
		constexpr try_skip_parameters_and_get_return_type_reader(
			ErrorHandler&& error_handler
		)
		requires(Stage == stage::parameters) {
			return try_read_parameter_types_and_get_return_type_reader(
				[]<typename...>(auto...){},
				forward<ErrorHandler>(error_handler)
			);
		}

		template<typename ErrorHandler>
		optional<uint8>
		constexpr try_read_parameters_count(
			ErrorHandler&& error_handler = []{}
		)
		requires(Stage == stage::parameters) {
			uint8 count = 0;
			bool error_happened = false;
			try_read_parameter_types_and_get_return_type_reader(
				[&](auto){ ++count; },
				[&](auto error) {
					error_happened = true; error_handler(error);
				}
			);
			if(error_happened) {
				return {};
			}
			return count;
		}

		template<typename Handler, typename ErrorHandler>
		optional<IS>
		try_read_and_get_advanced_iterator(
			Handler&& handler, ErrorHandler&& error_handler
		)
		requires(Stage == stage::ret) {
			bool error_happened = false;
			read_return_descriptor(
				is_,
				handler,
				[&](auto error) { error_happened = true; error_handler(error); }
			);
			if(error_happened) {
				return {};
			}
			return { is_ };
		}

	};

	template<basic_input_stream<utf8::unit> IS>
	reader(IS&&) -> reader<IS>;

	template<basic_input_stream<utf8::unit> IS, typename Handler, typename ErrorHandler>
	constexpr void try_read_parameter_types(
		IS&& is, Handler&& handler, ErrorHandler&& error_handler
	) {
		reader r{ forward<IS>(is) };
		r.try_read_parameter_types_and_get_return_type_reader(
			forward<Handler>(handler),
			forward<ErrorHandler>(error_handler)
		);
	}

	template<basic_iterator Iterator, typename ErrorHandler>
	optional<uint8> constexpr try_read_parameters_count(
		Iterator&& iterator, ErrorHandler&& error_handler
	) {
		reader r{ iterator };
		return r.try_read_parameters_count(
			forward<ErrorHandler>(error_handler)
		);
	}

	template<
		basic_input_stream<utf8::unit> IS,
		typename Handler,
		typename ErrorHandler
	>
	void try_read_return_type(
		IS&& is,
		Handler&& handler,
		ErrorHandler&& error_handler
	) {
		reader r { forward<IS>(is) };
		auto possible_return_type_reader
			= r.try_skip_parameters_and_get_return_type_reader(error_handler);
		if(possible_return_type_reader.has_no_value()) {
			return;
		}

		possible_return_type_reader.get()
			.try_read_and_get_advanced_iterator(handler, error_handler);
	}

	template<
		basic_input_stream<utf8::unit> IS,
		typename ParameterTypesHandler,
		typename ReturnTypeHandler,
		typename ErrorHandler
	>
	void try_read_parameter_and_return_types(
		IS&& is,
		ParameterTypesHandler&& parameter_types_handler,
		ReturnTypeHandler&& return_type_handler,
		ErrorHandler&& error_handler
	) {
		reader reader { forward<IS>(is) };
		auto possible_return_type_reader
			= reader.try_read_parameter_types_and_get_return_type_reader(
				parameter_types_handler,
				error_handler
			);
		if(possible_return_type_reader.has_no_value()) {
			return;
		}
		possible_return_type_reader.get()
			.try_read_and_get_advanced_iterator(
				return_type_handler, error_handler
			);
	}

}