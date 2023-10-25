#pragma once

#include "./constant.hpp"

#include <integer.hpp>
#include <read.hpp>

#include <unicode/utf8.hpp>

#include <iterator_and_sentinel.hpp>

namespace class_file::constant {

	template<basic_output_stream<uint8> OS>
	class writer {
		const OS os_;
	public:

		writer(OS&& os) : os_{ forward<OS>(os) } {}
		writer(      writer&&) = delete;
		writer(const writer& ) = delete;

		void operator () (utf8 str) {
			::write<uint8>(os_, (uint8) utf8::tag);
			::write<uint16, endianness::big>(os_, str.size());
			str.copy_to(os_);
		}

		void operator () (_int v) {
			::write<uint8>(os_, (uint8) _int::tag);
			::write<int32, endianness::big>(os_, v.value);
		}

		void operator () (_float v) {
			::write<uint8>(os_, (uint8) _float::tag);
			::write<float, endianness::big>(os_, v.value);
		}

		void operator () (_long v) {
			::write<uint8>(os_, (uint8) _long::tag);
			::write<int64, endianness::big>(os_, v.value);
		}

		void operator () (_double v) {
			::write<uint8>(os_, (uint8) _double::tag);
			::write<double, endianness::big>(os_, v.value);
		}

		void operator () (_class c) {
			::write<uint8>(os_, (uint8) _class::tag);
			::write<uint16, endianness::big>(os_, c.name_constant_index);
		}

		void operator () (string s) {
			::write<uint8>(os_, (uint8) string::tag);
			::write<uint16, endianness::big>(os_, s.utf8_constant_index);
		}

		void operator () (field_ref fr) {
			::write<uint8>(os_, (uint8) field_ref::tag);
			::write<uint16, endianness::big>(os_, fr.class_constant_index);
			::write<uint16, endianness::big>(os_, fr.name_and_type_constant_index);
		}

		void operator () (method_ref mr) {
			::write<uint8>(os_, (uint8) method_ref::tag);
			::write<uint16, endianness::big>(os_, mr.class_constant_index);
			::write<uint16, endianness::big>(os_, mr.name_and_type_constant_index);
		}

		void operator () (interface_method_ref mr) {
			::write<uint8>(os_, (uint8) interface_method_ref::tag);
			::write<uint16, endianness::big>(os_, mr.class_constant_index);
			::write<uint16, endianness::big>(os_, mr.name_and_type_constant_index);
		}

		void operator () (name_and_type mr) {
			::write<uint8>(os_, (uint8) name_and_type::tag);
			::write<uint16, endianness::big>(os_, mr.name_constant_index);
			::write<uint16, endianness::big>(os_, mr.descriptor_constant_index);
		}

		void operator () (method_handle mh) {
			::write<uint8>(os_, (uint8) method_handle::tag);
			::write<uint8>(os_, (uint8) mh.kind);
			::write<uint16, endianness::big>(os_, mh.reference_constant_index);
		}

		void operator () (method_type mt) {
			::write<uint8>(os_, (uint8) method_type::tag);
			::write<uint16, endianness::big>(os_, mt.descriptor_constant_index);
		}

		void operator () (dynamic dyn) {
			::write<uint8>(os_, (uint8) dynamic::tag);
			::write<uint16, endianness::big>(
				os_,
				dyn.bootstrap_method_attr_index
			);
			::write<uint16, endianness::big>(
				os_,
				dyn.name_and_type_constant_index
			);
		}

		void operator () (invoke_dynamic id) {
			::write<uint8>(os_, (uint8) invoke_dynamic::tag);
			::write<uint16, endianness::big>(
				os_,
				id.bootstrap_method_attr_index
			);
			::write<uint16, endianness::big>(
				os_,
				id.name_and_type_constant_index
			);
		}

		void operator () (module m) {
			::write<uint8>(os_, (uint8) module::tag);
			::write<uint16, endianness::big>(os_, m.name_constant_index);
		}

		void operator () (package p) {
			::write<uint8>(os_, (uint8) package::tag);
			::write<uint16, endianness::big>(os_, p.name_constant_index);
		}

	};

	template<basic_output_stream<uint8> OS>
	writer(OS&& os) -> writer<OS&&>;

}