#pragma once

#include "./read_type.hpp"
#include <c_string.hpp>
#include <optional.hpp>

namespace class_file::method_descriptor {

	enum class reader_stage {
		parameters, ret
	};

	enum class read_error {
		expected_opening_brace
	};

	template<
		basic_iterator Iterator,
		reader_stage Stage = reader_stage::parameters
	>
	class reader {
		Iterator iterator_;
	public:

		constexpr reader(Iterator iterator) : iterator_{ iterator } {}

		template<typename Handler, typename ErrorHandler>
		optional<reader<Iterator, reader_stage::ret>>
		constexpr try_read_parameter_types_and_get_return_type_reader(
			Handler&& handler, ErrorHandler&& error_handler
		) const
		requires(Stage == reader_stage::parameters) {
			Iterator i = iterator_;
			if(*i != '(') {
				error_handler(read_error::expected_opening_brace);
			}
			++i;
			while(*i != ')') {
				bool error_happened = false;
				read_type_descriptor(
					i,
					handler,
					[&](auto error) {
						error_happened = true; error_handler(error);
					}
				);
				if(error_happened) { return {}; }
			}
			++i; // skip )
			return { reader<Iterator, reader_stage::ret>{ i } };
		}

		template<typename Handler, typename ErrorHandler>
		optional<reader<Iterator, reader_stage::ret>>
		constexpr try_read_parameter_type_names_and_get_return_type_reader(
			Handler&& handler, ErrorHandler&& error_handler
		) const
		requires(Stage == reader_stage::parameters) {
			Iterator i = iterator_;
			if(*i != '(') {
				error_handler(read_error::expected_opening_brace);
			}
			++i;
			while(*i != ')') {
				Iterator begin = i;
				bool error_happened = false;
				read_type_descriptor(
					i,
					[](auto){},
					[&](auto error) {
						error_happened = true; error_handler(error);
					}
				);
				if(error_happened) { return {}; }
				Iterator end = i;
				handler(c_string {
					(const char*) begin,
					iterator_and_sentinel{ begin, end }.distance()
				});
			}
			++i;
			return { { i } };
		}

		template<typename ErrorHandler>
		optional<reader<Iterator, reader_stage::ret>>
		constexpr try_skip_parameters_and_get_return_type_reader(
			ErrorHandler&& error_handler
		) const
		requires(Stage == reader_stage::parameters) {
			return try_read_parameter_types_and_get_return_type_reader(
				[](auto){}, forward<ErrorHandler>(error_handler)
			);
		}

		template<typename ErrorHandler>
		optional<uint8>
		constexpr try_read_parameters_count(
			ErrorHandler&& error_handler = []{}
		) const
		requires(Stage == reader_stage::parameters) {
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
		optional<Iterator>
		try_read_and_get_advanced_iterator(
			Handler&& handler, ErrorHandler&& error_handler
		) const
		requires(Stage == reader_stage::ret) {
			Iterator i = iterator_;
			bool error_happened = false;
			read_type_descriptor(
				i,
				handler,
				[&](auto error) { error_happened = true; error_handler(error); }
			);
			if(error_happened) {
				return {};
			}
			return { i };
		}

	};

	template<basic_iterator Iterator>
	reader(Iterator) -> reader<Iterator>;

	template<basic_iterator Iterator, typename Handler, typename ErrorHandler>
	void try_read_parameter_types(
		Iterator&& iterator, Handler&& handler, ErrorHandler&& error_handler
	) {
		reader r{ iterator };
		r.try_read_parameter_types_and_get_return_type_reader(
			forward<Handler>(handler), forward<ErrorHandler>(error_handler)
		);
	}

	template<basic_iterator Iterator, typename Handler, typename ErrorHandler>
	void try_read_return_type(
		Iterator&& iterator, Handler&& handler, ErrorHandler&& error_handler
	) {
		reader r{ iterator };
		auto possible_return_type_reader
			= r.try_skip_parameters_and_get_return_type_reader(error_handler);
		if(!possible_return_type_reader.has_value()) {
			return;
		}
		possible_return_type_reader.value()
			.try_read_and_get_advanced_iterator(handler, error_handler);
	}

	template<
		basic_iterator Iterator,
		typename ParameterTypesHandler,
		typename ReturnTypeHandler,
		typename ErrorHandler
	>
	void try_read_parameter_and_return_types(
		Iterator&& iterator,
		ParameterTypesHandler&& parameter_types_handler,
		ReturnTypeHandler&& return_type_handler,
		ErrorHandler&& error_handler
	) {
		reader reader{ iterator };
		auto possible_return_type_reader
			= reader.try_read_parameter_types_and_get_return_type_reader(
				parameter_types_handler,
				error_handler
			);
		if(!possible_return_type_reader.has_value()) {
			return;
		}
		possible_return_type_reader.value()
			.try_read_and_get_advanced_iterator(
				return_type_handler, error_handler
			);
	}

}