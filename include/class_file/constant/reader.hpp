#pragma once

#include "../constant.hpp"

#include <core/integer.hpp>
#include <core/read.hpp>

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
				case constant::utf8::tag : {
					uint16 len = read<uint16, endianness::big>(i);
					handler(constant::utf8{ (char*) i, len });
					i += len;
					break;
				}
				case constant::_int::tag : {
					int32 value = read<int32, endianness::big>(i);
					handler(constant::_int{ value });
					break;
				}
				case constant::_float::tag : {
					float value = read<float, endianness::big>(i);
					handler(constant::_float{ value });
					break;
				}
				case constant::_long::tag : {
					int64 val = read<uint64, endianness::big>(i);
					handler(constant::_long{ val });
					handler(constant::skip{});
					break;
				}
				case constant::_double::tag : {
					double value = read<double, endianness::big>(i);
					handler(constant::_double{ value });
					handler(constant::skip{});
					break;
				}
				case constant::_class::tag : {
					name_index name_index {
						read<uint16, endianness::big>(i)
					};
					handler(constant::_class{ name_index });
					break;
				}
				case constant::string::tag : {
					utf8_index string_content_index {
						read<uint16, endianness::big>(i)
					};
					handler(constant::string{ string_content_index });
					break;
				}
				case constant::field_ref::tag : {
					class_index class_index {
						read<uint16, endianness::big>(i)
					};
					name_and_type_index nat_index {
						read<uint16, endianness::big>(i)
					};
					handler(constant::field_ref{ class_index, nat_index });
					break;
				}
				case constant::method_ref::tag : {
					class_index class_index {
						read<uint16, endianness::big>(i)
					};
					name_and_type_index nat_index {
						read<uint16, endianness::big>(i)
					};
					handler(constant::method_ref{ class_index, nat_index });
					break;
				}
				case constant::interface_method_ref::tag : {
					interface_index class_index {
						read<uint16, endianness::big>(i)
					};
					name_and_type_index nat_index {
						read<uint16, endianness::big>(i)
					};
					handler(constant::interface_method_ref {
						class_index, nat_index
					});
					break;
				}
				case constant::name_and_type::tag : {
					name_index name_index {
						read<uint16, endianness::big>(i)
					};
					descriptor_index desc_index {
						read<uint16, endianness::big>(i)
					};
					handler(
						constant::name_and_type {
							name_index,
							desc_index
						});
					break;
				}
				case constant::method_handle::tag : {
					uint8 kind = *i++;
					uint16 index = read<uint16, endianness::big>(i);
					handler(constant::method_handle{ kind, index });
					break;
				}
				case constant::method_type::tag : {
					descriptor_index desc_index {
						read<uint16, endianness::big>(i)
					};
					handler(constant::method_type{ desc_index });
					break;
				}
				case constant::dynamic::tag : {
					uint16 method_attr_index {
						read<uint16, endianness::big>(i)
					};
					name_and_type_index name_and_type_index {
						read<uint16, endianness::big>(i)
					};
					handler(
						constant::dynamic {
							method_attr_index, name_and_type_index
						}
					);
					break;
				}
				case constant::invoke_dynamic::tag : {
					uint16 method_attr_index {
						read<uint16, endianness::big>(i)
					};
					name_and_type_index name_and_type_index {
						read<uint16, endianness::big>(i)
					};
					handler(
						constant::invoke_dynamic {
							method_attr_index,
							name_and_type_index
						});
					break;
				}
				case constant::module::tag : {
					name_index name_index {
						read<uint16, endianness::big>(i)
					};
					handler(constant::module{ name_index });
					break;
				}
				case constant::package::tag : {
					name_index name_index {
						read<uint16, endianness::big>(i)
					};
					handler(constant::package{ name_index });
					break;
				}
				default: handler(constant::unknown{ tag });
			}

			return i;
		}

	};

}