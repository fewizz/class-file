#pragma once

#include <core/integer.hpp>
#include <core/span.hpp>

namespace class_file {

	class utf8_index {
		uint16 value_;
	public:
		explicit utf8_index(uint16 value) : value_{ value } {}
		operator uint16& () { return value_; }
	};

	class class_index {
		uint16 value_;
	public:
		explicit class_index(uint16 value) : value_{ value } {}
		operator uint16& () { return value_; }
	};

	class name_and_type_index {
		uint16 value_;
	public:
		explicit name_and_type_index(uint16 value) : value_{ value } {}
		operator uint16& () { return value_; }
	};

	class name_index : public utf8_index {
		using utf8_index::utf8_index;
	};

	class descriptor_index : public utf8_index {
		using utf8_index::utf8_index;
	};

	class interface_index : public class_index {
		using class_index::class_index;
	};

}

namespace class_file::constant {

	struct unknown {
		uint8 tag;
	};

	struct skip {};

	struct utf8 : span<const char, uint16> {
		static constexpr uint8 tag = 1;
		using base_type = span<const char, uint16>;
		using base_type::base_type;
	};

	struct _int {
		static constexpr uint8 tag = 3;
		::int32 value;
	};

	struct _float {
		static constexpr uint8 tag = 4;
		float value;
	};

	struct _long {
		static constexpr uint8 tag = 5;
		::int64 value;
	};

	struct _double {
		static constexpr uint8 tag = 6;
		double value;
	};

	struct _class {
		static constexpr uint8 tag = 7;
		utf8_index name_index;
	};

	struct string {
		static constexpr uint8 tag = 8;
		utf8_index string_index;
	};

	struct field_ref {
		static constexpr uint8 tag = 9;
		class_index class_index;
		name_and_type_index name_and_type_index;
	};

	struct method_ref {
		static constexpr uint8 tag = 10;
		class_index class_index;
		name_and_type_index name_and_type_index;
	};

	struct interface_method_ref {
		static constexpr uint8 tag = 11;
		class_index interface_index;
		name_and_type_index name_and_type_index;
	};

	struct name_and_type {
		static constexpr uint8 tag = 12;
		name_index name_index;
		descriptor_index descriptor_index;
	};

	struct method_handle {
		static constexpr uint8 tag = 15;
		uint8 reference_kind;
		uint16 reference_index;
	};

	struct method_type {
		static constexpr uint8 tag = 16;
		descriptor_index descriptor_index;
	};

	struct dynamic {
		static constexpr uint8 tag = 17;
		uint16 bootstrap_method_attr_index;
		name_and_type_index name_and_type_index;
	};

	struct invoke_dynamic {
		static constexpr uint8 tag = 18;
		uint16 bootstrap_method_attr_index;
		name_and_type_index name_and_type_index;
	};

	struct module {
		static constexpr uint8 tag = 19;
		name_index name_index;
	};

	struct package {
		static constexpr uint8 tag = 20;
		name_index name_index;
	};

}