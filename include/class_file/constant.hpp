#pragma once

#include <integer.hpp>
#include <span.hpp>

#include <unicode/utf8.hpp>

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

	class method_or_interface_method_ref_index : public index {
		using index::index;

	public:

		method_or_interface_method_ref_index(
			method_ref_index index
		) :
			constant::index{ index }
		{}

		method_or_interface_method_ref_index(
			interface_method_ref_index index
		) :
			constant::index{ index }
		{}

		method_ref_index as_method_ref_index() const {
			return method_ref_index{ *this };
		}

		interface_method_ref_index as_interface_method_ref_index() const {
			return interface_method_ref_index{ *this };
		}
	};

	class name_and_type_index : public index {
		using index::index;
	};

	class method_handle_index : public index {
		using index::index;
	};

	class method_type_index : public index {
		using index::index;
	};

	class dynamic_index : public index {
		using index::index;
	};

	class invoke_dynamic_index : public index {
		using index::index;
	};

	class name_index : public utf8_index {
		using utf8_index::utf8_index;
	};

	class descriptor_index : public utf8_index {
		using utf8_index::utf8_index;
	};

	class module_index : public index {
		using index::index;
	};

	class package_index : public index {
		using index::index;
	};

	struct unknown {
		uint8 tag;
	};

	struct skip {};

	struct utf8 : span<const ::utf8::unit, uint16> {
		static constexpr uint8 tag = 1;
	private:
		using base_type = span<const ::utf8::unit, uint16>;
	public:
		using base_type::base_type;
		using index_type = utf8_index;
	};

	struct name : utf8 {
		using utf8::utf8;
		using index_type = name_index;
		explicit name(utf8 base) : utf8{ base } {}
	};

	struct descriptor : utf8 {
		using utf8::utf8;
		using index_type = descriptor_index;
	};

	struct _int {
		static constexpr uint8 tag = 3;
		::int32 value;
		using index_type = int_index;
	};

	struct _float {
		static constexpr uint8 tag = 4;
		float value;
		using index_type = float_index;
	};

	struct _long {
		static constexpr uint8 tag = 5;
		::int64 value;
		using index_type = long_index;
	};

	struct _double {
		static constexpr uint8 tag = 6;
		double value;
		using index_type = double_index;
	};

	struct _class {
		static constexpr uint8 tag = 7;
		name_index name_constant_index;
		using index_type = class_index;
	};

	struct string {
		static constexpr uint8 tag = 8;
		utf8_index utf8_constant_index;
		using index_type = class string_index;
	};

	struct field_ref {
		static constexpr uint8 tag = 9;
		class_index class_constant_index;
		name_and_type_index name_and_type_constant_index;
		using index_type = field_ref_index;
	};

	struct method_ref {
		static constexpr uint8 tag = 10;
		class_index class_constant_index;
		name_and_type_index name_and_type_constant_index;
		using index_type = method_ref_index;
	};

	struct interface_method_ref {
		static constexpr uint8 tag = 11;
		class_index class_constant_index;
		name_and_type_index name_and_type_constant_index;
		using index_type = interface_method_ref_index;
	};

	struct name_and_type {
		static constexpr uint8 tag = 12;
		name_index name_constant_index;
		descriptor_index descriptor_constant_index;
		using index_type = name_and_type_index;
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
		constant::index reference_constant_index;
		using index_type = method_handle_index;
	};

	struct method_type {
		static constexpr uint8 tag = 16;
		descriptor_index descriptor_constant_index;
		using index_type = method_type_index;
	};

	struct dynamic {
		static constexpr uint8 tag = 17;
		uint16 bootstrap_method_attr_index;
		name_and_type_index name_and_type_constant_index;
		using index_type = dynamic_index;
	};

	struct invoke_dynamic {
		static constexpr uint8 tag = 18;
		uint16 bootstrap_method_attr_index;
		name_and_type_index name_and_type_constant_index;
		using index_type = invoke_dynamic_index;
	};

	struct module {
		static constexpr uint8 tag = 19;
		name_index name_constant_index;
		using index_type = module_index;
	};

	struct package {
		static constexpr uint8 tag = 20;
		name_index name_constant_index;
		using index_type = package_index;
	};

}