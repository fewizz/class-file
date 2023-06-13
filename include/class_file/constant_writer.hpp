#pragma once

#include "./constant.hpp"

#include <integer.hpp>
#include <read.hpp>

#include <unicode/utf8.hpp>

#include <iterator_and_sentinel.hpp>

namespace class_file::constant {

	template<basic_iterator Iterator>
	class writer {
		const Iterator iterator_;
	public:

		writer(Iterator it) : iterator_{ it } {}
		writer(      writer&&) = delete;
		writer(const writer& ) = delete;

		Iterator iterator_copy() const {
			return iterator_;
		}

		void operator () (utf8 str) {
			write<uint16, endianness::big>(iterator_, str.size());
			for(int i = 0; i < str.size(); ++i) {
				write(str[i], iterator_);
			}
		}

		void operator () (_int v) {
			write<int32, endianness::big>(iterator_, v.value);
		}

		void operator () (_float v) {
			write<float, endianness::big>(iterator_, v.value);
		}

		void operator () (_long v) {
			write<int64, endianness::big>(iterator_, v.value);
		}

		void operator () (_double v) {
			write<double, endianness::big>(iterator_, v.value);
		}

		void operator () (_class c) {
			write<uint16, endianness::big>(iterator_, c.name_index);
		}

		void operator () (string s) {
			write<uint16, endianness::big>(iterator_, s.string_index);
		}

		void operator () (field_ref fr) {
			write<uint16, endianness::big>(iterator_, fr.class_index);
			write<uint16, endianness::big>(iterator_, fr.name_and_type_index);
		}

		void operator () (method_ref mr) {
			write<uint16, endianness::big>(iterator_, mr.class_index);
			write<uint16, endianness::big>(iterator_, mr.name_and_type_index);
		}

		void operator () (interface_method_ref mr) {
			write<uint16, endianness::big>(iterator_, mr.class_index);
			write<uint16, endianness::big>(iterator_, mr.name_and_type_index);
		}

		void operator () (name_and_type mr) {
			write<uint16, endianness::big>(iterator_, mr.name_index);
			write<uint16, endianness::big>(iterator_, mr.descriptor_index);
		}

		void operator () (method_handle mh) {
			write<uint8>(iterator_, mh.kind);
			write<uint16, endianness::big>(iterator_, mh.reference_index);
		}

		void operator () (method_type mt) {
			write<uint16, endianness::big>(iterator_, mt.descriptor_index);
		}

		void operator () (dynamic dyn) {
			write<uint16, endianness::big>(
				iterator_,
				dyn.bootstrap_method_attr_index
			);
			write<uint16, endianness::big>(
				iterator_,
				dyn.name_and_type_index
			);
		}

		void operator () (invoke_dynamic id) {
			write<uint16, endianness::big>(
				iterator_,
				id.bootstrap_method_attr_index
			);
			write<uint16, endianness::big>(
				iterator_,
				id.name_and_type_index
			);
		}

		void operator () (module m) {
			write<uint16, endianness::big>(iterator_, m.name_index);
		}

		void operator () (package p) {
			write<uint16, endianness::big>(iterator_, p.name_index);
		}

	};

}