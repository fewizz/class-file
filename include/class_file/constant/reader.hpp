#pragma once

#include "../constant.hpp"

#include <integer.hpp>
#include <read.hpp>

namespace class_file::constant {

	template<typename Iterator>
	class reader {
		Iterator iterator_;
	public:

		reader(Iterator it) : iterator_{ it } {}
		
		template<typename Handler>
		Iterator read_and_get_advanced_iterator(Handler&& handler) {
			Iterator i = iterator_;
			uint8 tag = *i++;

			switch (tag) {
				case utf8::tag : {
					uint16 len = read<uint16, endianness::big>(i);
					handler(utf8{ (char*) i, len });
					i += len;
					break;
				}
				case _int::tag : {
					int32 value = read<int32, endianness::big>(i);
					handler(_int{ value });
					break;
				}
				case _float::tag : {
					float value = read<float, endianness::big>(i);
					handler(_float{ value });
					break;
				}
				case _long::tag : {
					int64 val = read<uint64, endianness::big>(i);
					handler(_long{ val });
					handler(skip{});
					break;
				}
				case _double::tag : {
					double value = read<double, endianness::big>(i);
					handler(_double{ value });
					handler(skip{});
					break;
				}
				case _class::tag : {
					name_index name_index {
						read<uint16, endianness::big>(i)
					};
					handler(_class{ name_index });
					break;
				}
				case string::tag : {
					utf8_index string_content_index {
						read<uint16, endianness::big>(i)
					};
					handler(string{ string_content_index });
					break;
				}
				case field_ref::tag : {
					class_index class_index {
						read<uint16, endianness::big>(i)
					};
					name_and_type_index nat_index {
						read<uint16, endianness::big>(i)
					};
					handler(field_ref{ class_index, nat_index });
					break;
				}
				case method_ref::tag : {
					class_index class_index {
						read<uint16, endianness::big>(i)
					};
					name_and_type_index nat_index {
						read<uint16, endianness::big>(i)
					};
					handler(method_ref{ class_index, nat_index });
					break;
				}
				case interface_method_ref::tag : {
					interface_index class_index {
						read<uint16, endianness::big>(i)
					};
					name_and_type_index nat_index {
						read<uint16, endianness::big>(i)
					};
					handler(interface_method_ref {
						class_index, nat_index
					});
					break;
				}
				case name_and_type::tag : {
					name_index name_index {
						read<uint16, endianness::big>(i)
					};
					descriptor_index desc_index {
						read<uint16, endianness::big>(i)
					};
					handler(name_and_type{ name_index, desc_index });
					break;
				}
				case method_handle::tag : {
					method_handle::behavior_kind kind {
						(method_handle::behavior_kind) read<uint8>(i)
					};
					uint16 index = read<uint16, endianness::big>(i);
					handler(method_handle{ kind, index });
					break;
				}
				case method_type::tag : {
					descriptor_index desc_index {
						read<uint16, endianness::big>(i)
					};
					handler(method_type{ desc_index });
					break;
				}
				case dynamic::tag : {
					uint16 method_attr_index {
						read<uint16, endianness::big>(i)
					};
					name_and_type_index name_and_type_index {
						read<uint16, endianness::big>(i)
					};
					handler(dynamic{ method_attr_index, name_and_type_index });
					break;
				}
				case invoke_dynamic::tag : {
					uint16 method_attr_index {
						read<uint16, endianness::big>(i)
					};
					name_and_type_index name_and_type_index {
						read<uint16, endianness::big>(i)
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
						read<uint16, endianness::big>(i)
					};
					handler(module{ name_index });
					break;
				}
				case package::tag : {
					name_index name_index {
						read<uint16, endianness::big>(i)
					};
					handler(package{ name_index });
					break;
				}
				default: handler(unknown{ tag });
			}

			return i;
		}

	};

}