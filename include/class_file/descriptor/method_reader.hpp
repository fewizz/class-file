#pragma once

#include "./read_type.hpp"
#include <optional.hpp>

namespace class_file::descriptor::method {

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
		using common_read_error_type = elements::one_of<
			descriptor::type_read_error,
			descriptor::method::read_error
		>;
	public:

		reader(Iterator iterator) : iterator_{ iterator } {}

		template<typename Handler>
		expected<reader<Iterator, reader_stage::ret>, common_read_error_type>
		try_read_parameter_types_and_get_return_type_reader(
			Handler&& handler
		) const
		requires(Stage == reader_stage::parameters) {
			Iterator i = iterator_;
			if(*i != '(') {
				return { read_error::expected_opening_brace };
			}
			++i;
			while(*i != ')') {
				auto result = read_type(i, handler);
				if(result.is_unexpected()) {
					return { result.get_unexpected() };
				}
			}
			++i;
			return { { i } };
		}

		template<typename Handler>
		expected<reader<Iterator, reader_stage::ret>, common_read_error_type>
		try_read_parameter_type_names_and_get_return_type_reader(
			Handler&& handler
		) const
		requires(Stage == reader_stage::parameters) {
			Iterator i = iterator_;
			if(*i != '(') {
				return { read_error::expected_opening_brace };
			}
			++i;
			while(*i != ')') {
				Iterator begin = i;
				auto result = read_type(i, [](auto){ return true; });
				if(result.is_unexpected()) {
					return { result.get_unexpected() };
				}
				Iterator end = i;
				handler(c_string {
					(const char*) begin,
					iterator_and_sentinel{ begin, end }.distance()
				});
			}
			++i;
			return { { i } };
		}

		expected<reader<Iterator, reader_stage::ret>, common_read_error_type>
		try_skip_parameters_and_get_return_type_reader() const
		requires(Stage == reader_stage::parameters) {
			return operator()([](auto){ return true; });
		}

		expected<uint8, common_read_error_type>
		try_read_parameters_count() const
		requires(Stage == reader_stage::parameters) {
			uint8 count = 0;
			auto[ret_reader, result] =
				operator()([&](auto){ ++count; return true; });
			return result;
		}

		template<typename Handler>
		expected<Iterator, common_read_error_type>
		try_read_and_get_advanced_iterator(Handler&& handler) const
		requires(Stage == reader_stage::ret) {
			Iterator i = iterator_;
			auto result = read_type(i, handler);
			if(result.is_unexpected()) {
				return { result.get_unexpected() };
			}
			return { { i } };
		}

	};

	template<basic_iterator Iterator>
	reader(Iterator) -> reader<Iterator>;

	template<basic_iterator Iterator, typename Handler>
	auto try_read_parameter_types(Iterator&& iterator, Handler&& handler) {
		reader r{ iterator };
		return r.try_read_parameter_types_and_get_return_type_reader(
			forward<Handler>(handler)
		);
	}

	template<basic_iterator Iterator, typename Handler>
	auto try_read_return_type(Iterator&& iterator, Handler&& handler) {
		reader r{ iterator };
		auto possible_return_type_reader
			= r.try_skip_parameters_and_get_return_type_reader();
		if(possible_return_type_reader.is_unexpected()) {
			return possible_return_type_reader.get_unexpected();
		}
		return possible_return_type_reader
			.get_expected()
			.try_read_and_get_advanced_iterator(forward<Handler>(handler));
	}

	template<
		basic_iterator Iterator,
		typename ParameterTypesHandler,
		typename ReturnTypeHandler
	>
	auto try_read_parameters_and_return_type(
		Iterator&& iterator,
		ParameterTypesHandler&& parameter_types_handler,
		ReturnTypeHandler&& return_type_handler
	) {
		reader reader{ iterator };
		auto possible_return_type_reader
			= reader.try_read_parameter_types_and_get_return_type_reader(
				forward<ParameterTypesHandler>(parameter_types_handler)
			);
		if(possible_return_type_reader.is_unexpected()) {
			return possible_return_type_reader.get_unexpected();
		}
		return possible_return_type_reader
			.get_expected()
			.try_read_and_get_advanced_iterator(
				forward<ReturnTypeHandler>(return_type_handler)
			);
	}

}