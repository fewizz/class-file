#pragma once

#include <core/integer.hpp>
#include <core/span.hpp>

namespace class_file::constant {

	struct unknown {
		uint8 tag;
	};

	struct skip {};

	// kind = Utf8
	struct utf8 : span<const char, uint16> {
		static constexpr uint8 tag = 1;
		using base_type = span<const char, uint16>;
		using base_type::base_type;
	};

	// kind = Integer
	struct _int {
		static constexpr uint8 tag = 3;
		::int32 value;
	};

	// kind = Float
	struct _float {
		static constexpr uint8 tag = 4;
		float value;
	};

	// kind = Long
	struct _long {
		static constexpr uint8 tag = 5;
		::int64 value;
	};

	// kind = Double
	struct _double {
		static constexpr uint8 tag = 6;
		double value;
	};

	// kind = Class
	struct _class {
		static constexpr uint8 tag = 7;
		uint16 name_index;
	};

	// kind = String
	struct string {
		static constexpr uint8 tag = 8;
		uint16 string_index;
	};

	// kind = Fieldref
	struct field_ref {
		static constexpr uint8 tag = 9;
		uint16 class_index;
		uint16 name_and_type_index;
	};

	// kind = Methodref
	struct method_ref {
		static constexpr uint8 tag = 10;
		uint16 class_index;
		uint16 name_and_type_index;
	};

	// kind = InterfaceMethodref
	struct interface_method_ref {
		static constexpr uint8 tag = 11;
		uint16 class_index;
		uint16 name_and_type_index;
	};

	// kind = NameAndType
	struct name_and_type {
		static constexpr uint8 tag = 12;
		uint16 name_index;
		uint16 descriptor_index;
	};

	// kind = MethodHandle
	struct method_handle {
		static constexpr uint8 tag = 15;
		uint8 reference_kind;
		uint16 reference_index;
	};

	// kind = MethodType
	struct method_type {
		static constexpr uint8 tag = 16;
		uint16 descriptor_index;
	};

	// kind = Dynamic
	struct dynamic {
		static constexpr uint8 tag = 17;
		uint16 bootstrap_method_attr_index;
		uint16 name_and_type_index;
	};

	// kind = InvokeDynamic
	struct invoke_dynamic {
		static constexpr uint8 tag = 18;
		uint16 bootstrap_method_attr_index;
		uint16 name_and_type_index;
	};

	// kind = Module
	struct module {
		static constexpr uint8 tag = 19;
		uint16 name_index;
	};

	// kind = Package
	struct package {
		static constexpr uint8 tag = 20;
		uint16 name_index;
	};

}