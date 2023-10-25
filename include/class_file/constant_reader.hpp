#pragma once

#include "./constant.hpp"

#include <integer.hpp>
#include <read.hpp>

#include <unicode/utf8.hpp>

namespace class_file::constant {

	template<basic_input_stream<uint8> IS>
	class reader {
		IS is_;
	public:

		reader(IS&& is) : is_{ forward<IS>(is) } {}

		template<typename Handler>
		IS read_and_get_advanced_iterator(Handler&& handler) {
			uint8 tag = ::read<uint8>(is_);

			switch (tag) {
				case utf8::tag : {
					uint16 len = read<uint16, endianness::big>(is_);
					handler(utf8{ (::utf8::unit*) is_, len });
					is_ += len;
					break;
				}
				case _int::tag : {
					int32 value = read<int32, endianness::big>(is_);
					handler(_int{ value });
					break;
				}
				case _float::tag : {
					float value = read<float, endianness::big>(is_);
					handler(_float{ value });
					break;
				}
				case _long::tag : {
					int64 val = read<uint64, endianness::big>(is_);
					handler(_long{ val });
					handler(skip{});
					break;
				}
				case _double::tag : {
					double value = read<double, endianness::big>(is_);
					handler(_double{ value });
					handler(skip{});
					break;
				}
				case _class::tag : {
					name_index name_index {
						read<uint16, endianness::big>(is_)
					};
					handler(_class{ name_index });
					break;
				}
				case string::tag : {
					utf8_index string_content_index {
						read<uint16, endianness::big>(is_)
					};
					handler(string{ string_content_index });
					break;
				}
				case field_ref::tag : {
					class_index class_index {
						read<uint16, endianness::big>(is_)
					};
					name_and_type_index nat_index {
						read<uint16, endianness::big>(is_)
					};
					handler(field_ref{ class_index, nat_index });
					break;
				}
				case method_ref::tag : {
					class_index class_index {
						read<uint16, endianness::big>(is_)
					};
					name_and_type_index nat_index {
						read<uint16, endianness::big>(is_)
					};
					handler(method_ref{ class_index, nat_index });
					break;
				}
				case interface_method_ref::tag : {
					class_index class_index {
						read<uint16, endianness::big>(is_)
					};
					name_and_type_index nat_index {
						read<uint16, endianness::big>(is_)
					};
					handler(interface_method_ref {
						class_index, nat_index
					});
					break;
				}
				case name_and_type::tag : {
					name_index name_index {
						read<uint16, endianness::big>(is_)
					};
					descriptor_index desc_index {
						read<uint16, endianness::big>(is_)
					};
					handler(name_and_type{ name_index, desc_index });
					break;
				}
				case method_handle::tag : {
					method_handle::behavior_kind kind {
						(method_handle::behavior_kind) read<uint8>(is_)
					};
					constant::index index{ read<uint16, endianness::big>(is_) };
					handler(method_handle{ kind, index });
					break;
				}
				case method_type::tag : {
					descriptor_index desc_index {
						read<uint16, endianness::big>(is_)
					};
					handler(method_type{ desc_index });
					break;
				}
				case dynamic::tag : {
					uint16 method_attr_index {
						read<uint16, endianness::big>(is_)
					};
					name_and_type_index name_and_type_index {
						read<uint16, endianness::big>(is_)
					};
					handler(dynamic{ method_attr_index, name_and_type_index });
					break;
				}
				case invoke_dynamic::tag : {
					uint16 method_attr_index {
						read<uint16, endianness::big>(is_)
					};
					name_and_type_index name_and_type_index {
						read<uint16, endianness::big>(is_)
					};
					handler(
						invoke_dynamic {
							method_attr_index,
							name_and_type_index
						});
					break;
				}
				case module::tag : {
					name_index name_index {
						read<uint16, endianness::big>(is_)
					};
					handler(module{ name_index });
					break;
				}
				case package::tag : {
					name_index name_index {
						read<uint16, endianness::big>(is_)
					};
					handler(package{ name_index });
					break;
				}
				default: handler(unknown{ tag });
			}

			return forward<IS>(is_);
		}

	};

	template<basic_input_stream<uint8> IS>
	reader(IS&&) -> reader<IS>;

}