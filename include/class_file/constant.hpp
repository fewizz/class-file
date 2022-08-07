#pragma once

#include <integer.hpp>
#include <span.hpp>

namespace class_file::constant {

	class index {
		uint16 value_;
	public:
		index() = default;
		explicit index(uint16 value) : value_{ value } {}
		operator uint16& () { return value_; }
		operator const uint16& () const { return value_; }
	};

	class utf8_index : public index {
		using index::index;
	};

	class int_index : public index {
		using index::index;
	};

	class float_index : public index {
		using index::index;
	};

	class long_index : public index {
		using index::index;
	};

	class double_index : public index {
		using index::index;
	};

	class class_index : public index {
		using index::index;
	};

	class string_index : public index {
		using index::index;
	};

	class field_ref_index : public index {
		using index::index;
	};

	class method_ref_index : public index {
		using index::index;
	};

	class interface_method_ref_index : public index {
		using index::index;
	};

	class name_and_type_index : public index{
		using index::index;
	};

	class method_handle_index : public index{
		using index::index;
	};

	class invoke_dynamic_index : public index{
		using index::index;
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
		name_index name_index;
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
		interface_index interface_index;
		name_and_type_index name_and_type_index;
	};

	struct name_and_type {
		static constexpr uint8 tag = 12;
		name_index name_index;
		descriptor_index descriptor_index;
	};

	struct method_handle {
		static constexpr uint8 tag = 15;
		enum class behavior_kind : uint8 {
			get_field = 1,
			get_static,
			put_field,
			put_static,
			invoke_virtual,
			invoke_static,
			invoke_special,
			new_invoke_special,
			invoke_interface
		} kind;
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