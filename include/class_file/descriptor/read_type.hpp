#pragma once

#include "./type.hpp"

#include <iterator_and_sentinel.hpp>
#include <on_scope_exit.hpp>

namespace class_file {

	enum class type_descriptor_read_error {
		unknown_type
	};

	template<basic_iterator Iterator, typename Handler, typename ErrorHandler>
	void constexpr read_type_descriptor(
		Iterator&& iterator, Handler&& handler, ErrorHandler&& error_handler
	) {
		using iterator_type = remove_reference<Iterator>;

		auto read_without_array = [&](
			iterator_type& iterator, auto&& handler, auto&& error_handler
		) {
			uint8 c = *iterator;
			++iterator;
			switch (c) {
				case 'V': handler(class_file::v{}); return;
				case 'B': handler(class_file::b{}); return;
				case 'C': handler(class_file::c{}); return;
				case 'D': handler(class_file::d{}); return;
				case 'F': handler(class_file::f{}); return;
				case 'I': handler(class_file::i{}); return;
				case 'J': handler(class_file::j{}); return;
				case 'S': handler(class_file::s{}); return;
				case 'Z': handler(class_file::z{}); return;
				case 'L': {
					iterator_type class_name_begining = iterator;
					while(*iterator != ';') {
						++iterator;
					}
					iterator_type class_name_ending = iterator;
					handler(object {
						.class_name {
							(const char*) class_name_begining,
							(uint16) iterator_and_sentinel {
								class_name_begining,
								class_name_ending
							}.get_or_compute_distance()
						}
					});
					++iterator; // skip ;
					return;
				}
				default: {
					error_handler(type_descriptor_read_error::unknown_type);
					return;
				}
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
			handler(class_file::array {
				.component {
					(const char*) component_name_begining,
					(uint16) iterator_and_sentinel {
						component_name_begining, component_name_ending
					}.get_or_compute_distance()
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