#pragma once

#include "./type.hpp"

#include <elements/of.hpp>
#include <c_string.hpp>
#include <expected.hpp>
#include <iterator_and_sentinel.hpp>
#include <types/common.hpp>
#include <on_scope_exit.hpp>
#include <optional.hpp>

namespace class_file::descriptor {

	enum class type_read_error {
		unknown_type
	};

	template<basic_iterator Iterator, typename Handler, typename ErrorHandler>
	void constexpr read_type(
		Iterator&& iterator, Handler&& handler, ErrorHandler&& error_handler
	) {
		using iterator_type = remove_reference<Iterator>;

		auto read_without_array = [&](
			iterator_type& iterator, auto&& handler, auto&& error_handler
		) {
			uint8 c = *iterator;
			++iterator;
			switch (c) {
			case 'V': handler(descriptor::v{}); return;
			case 'B': handler(descriptor::b{}); return;
			case 'C': handler(descriptor::c{}); return;
			case 'D': handler(descriptor::d{}); return;
			case 'F': handler(descriptor::f{}); return;
			case 'I': handler(descriptor::i{}); return;
			case 'J': handler(descriptor::j{}); return;
			case 'S': handler(descriptor::s{}); return;
			case 'Z': handler(descriptor::z{}); return;
			case 'L': {
				iterator_type class_name_begining = iterator;
				while(*iterator != ';') {
					++iterator;
				}
				iterator_type class_name_ending = iterator;
				handler(descriptor::object {
					.class_name {
						(const char*) class_name_begining,
						(uint16) iterator_and_sentinel {
							class_name_begining,
							class_name_ending
						}.distance()
					}
				});
				++iterator; // skip ;
				return;
			}
			default: error_handler(type_read_error::unknown_type); return;
			}
		};

		if(*iterator == '[') {
			++iterator;
			uint8 rank = 1;
			while(*iterator == '[') {
				++rank;
				++iterator;
			}
			iterator_type component_name_begining = iterator;
			bool error_happened = false;
			read_without_array(
				iterator,
				[](auto){},
				[&](auto error){ error_happened = true; error_handler(error); }
			);
			if(error_happened) {
				return;
			}

			iterator_type component_name_ending = iterator;
			handler(descriptor::array {
				.component {
					(const char*) component_name_begining,
					(uint16) iterator_and_sentinel {
						component_name_begining, component_name_ending
					}.distance()
				},
				.rank = rank
			});
		}
		else {
			read_without_array(
				iterator,
				forward<Handler>(handler),
				forward<ErrorHandler>(error_handler)
			);
		}
	}

}