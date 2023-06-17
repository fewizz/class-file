#pragma once

#include "./instruction.hpp"

#include <iterator_and_sentinel.hpp>
#include <output_stream.hpp>
#include <endianness.hpp>
#include <write.hpp>

namespace class_file::attribute::code::instruction {

	template<basic_output_stream<uint8> OS, typename Instruction>
	void write(
		OS&& os, Instruction instruction, basic_iterator auto begin
	) {
		::write<uint8>(os, Instruction::code);

		if constexpr(same_as_any<Instruction,
			nop, a_const_null, i_const_m1, i_const_0, i_const_1, i_const_2,
			i_const_3, i_const_4, i_const_5, l_const_0, l_const_1, f_const_0,
			f_const_1, f_const_2, d_const_0, d_const_1
		>) {
			// do nothing
		}
		else if constexpr(same_as<Instruction, bi_push>) {
			::write<int8>(instruction.value, os);
		}
		else if constexpr(same_as<Instruction, si_push>) {
			::write<int16, endianness::big>(instruction.value, os);
		}
		else if constexpr(same_as<Instruction, ldc>) {
			::write<uint8>(instruction.index, os);
		}
		else if constexpr(same_as<Instruction, ldc_w>) {
			::write<uint16, endianness::big>(instruction.index, os);
		}
		else if constexpr(same_as<Instruction, ldc_2_w>) {
			::write<uint16, endianness::big>(instruction.index, os);
		}
		else if constexpr(same_as_any<Instruction,
			i_load, l_load, f_load, d_load, a_load
		>) {
			::write<uint8>(instruction.index, os);
		}
		else if constexpr(same_as_any<Instruction,
			i_load_0, i_load_1, i_load_2, i_load_3,
			l_load_0, l_load_1, l_load_2, l_load_3,
			f_load_0, f_load_1, f_load_2, f_load_3,
			d_load_0, d_load_1, d_load_2, d_load_3,
			a_load_0, a_load_1, a_load_2, a_load_3,
			i_a_load, l_a_load, f_a_load, d_a_load,
			a_a_load, b_a_load, c_a_load, s_a_load
		>) {
			// do nothing
		}
		else if constexpr(same_as_any<Instruction,
			i_store, l_store, f_store, d_store, a_store
		>) {
			::write<uint8>(instruction.index, os);
		}
		else if constexpr(same_as_any<Instruction,
			i_store_0, i_store_1, i_store_2, i_store_3,
			l_store_0, l_store_1, l_store_2, l_store_3,
			f_store_0, f_store_1, f_store_2 ,f_store_3,
			d_store_0, d_store_1, d_store_2, d_store_3,
			a_store_0, a_store_1, a_store_2, a_store_3,
			i_a_store, l_a_store, f_a_store, d_a_store,
			a_a_store, b_a_store, c_a_store, s_a_store
		>) {
			// do nothing
		}
		else if constexpr(same_as_any<Instruction,
			pop, pop_2, dup, dup_x1, dup_x2, dup_2, dup_2_x1, dup_2_x2, swap
		>) {
			// do nothing
		}
		else if constexpr(same_as_any<Instruction,
			i_add, l_add, f_add, d_add,
			i_sub, l_sub, f_sub, d_sub,
			i_mul, l_mul, f_mul, d_mul,
			i_div, l_div, f_div, d_div,
			i_rem, l_rem, f_rem, d_rem,
			i_neg, l_neg, f_neg, d_neg,
			i_sh_l, l_sh_l, i_sh_r, l_sh_r,
			i_u_sh_r, l_u_sh_r,
			i_and, l_and,
			i_or, l_or,
			i_xor, l_xor
		>) {
			// do nothing
		}
		else if constexpr(same_as<Instruction, i_inc>){
			::write<uint8>(instruction.index, os);
			::write<int8>(instruction.value, os);
		}
		else if constexpr(same_as_any<Instruction,
			i_to_l, i_to_f, i_to_d,
			l_to_i, l_to_f, l_to_d,
			f_to_i, f_to_l, f_to_d,
			d_to_i, d_to_l, d_to_f,
			i_to_b, i_to_c, i_to_s
		>) {
			// do nothing
		}
		else if constexpr(same_as_any<Instruction,
			l_cmp,
			f_cmp_l, f_cmp_g,
			d_cmp_l, d_cmp_g
		>) {
			// do nothing
		}
		else if constexpr(same_as_any<Instruction,
			if_eq, if_ne, if_lt, if_ge, if_gt, if_le,
			if_i_cmp_eq, if_i_cmp_ne,
				if_i_cmp_lt, if_i_cmp_ge, if_i_cmp_gt, if_i_cmp_le,
			if_a_cmp_eq, if_a_cmp_ne,
			go_to, jmp_sr
		>) {
			::write<int16, endianness::big>(instruction.branch, os);
		}
		else if constexpr(same_as<Instruction, return_sr>) {
			::write<int8>(instruction.index, os);
		}
		// table_switch and lookup_switch are below
		else if constexpr(same_as<Instruction, table_switch>) {
			while((os - begin) % 4 != 0) {
				++os;
			}
			::write<int32, endianness::big>(instruction._default, os);
			::write<int32, endianness::big>(instruction.low, os);
			::write<int32, endianness::big>(instruction.high, os);
			for(int32 offset : instruction.offsets) {
				::write<int32, endianness::big>(offset, os);
			}
		}
		else if constexpr(same_as<Instruction, lookup_switch>) {
			while((os - begin) % 4 != 0) {
				++os;
			}
			::write<int32, endianness::big>(instruction._default, os);
			::write<int32, endianness::big>(instruction.pairs.size(), os);
			for(match_offset mo : instruction.match_offsets) {
				::write<int32, endianness::big>(mo.match, os);
				::write<int32, endianness::big>(mo.offset, os);
			}
		}
		else if constexpr(same_as_any<Instruction,
			i_return, l_return, f_return, d_return, a_return, _return
		>) {
			// do nothing
		}
		else if constexpr(same_as_any<Instruction,
			get_static, put_static,
			get_field, put_field,
			invoke_virtual, invoke_special, invoke_static
		>) {
			::write<uint16, endianness::big>(instruction.index, os);
		}
		else if constexpr(same_as<Instruction, invoke_interface>) {
			::write<uint16, endianness::big>(instruction.index, os);
			::write<uint8>(instruction.count, os);
			::write<uint8>(0, os);
		}
		else if constexpr(same_as<Instruction, invoke_dynamic>) {
			::write<uint16, endianness::big>(instruction.index, os);
			::write<uint8>(0, os);
			::write<uint8>(0, os);
		}
		else if constexpr(same_as<Instruction, _new>) {
			::write<uint16, endianness::big>(instruction.index, os);
		}
		else if constexpr(same_as<Instruction, new_array_type>) {
			::write<uint8>(instruction, os);
		}
		else if constexpr(same_as<Instruction, a_new_array>) {
			::write<uint16, endianness::big>(instruction.index, os);
		}
		else if constexpr(same_as_any<Instruction, array_length, a_throw>) {
			// do nothing
		}
		else if constexpr(same_as_any<Instruction, check_cast, instance_of>) {
			::write<uint16, endianness::big>(instruction.index, os);
		}
		else if constexpr(same_as_any<Instruction,
			monitor_enter, monitor_exit
		>) {
			// do nothing
		}
		else if constexpr(same_as_any<Instruction, wide_format_1>) {
			::write<uint8>(instruction.other_code, os);
			::write<uint16, endianness::big>(instruction.index, os);
		}
		else if constexpr(same_as_any<Instruction, wide_format_2>) {
			::write<uint8>(os, wide_format_2::other_code);
			::write<uint16, endianness::big>(instruction.index, os);
			::write<uint16, endianness::big>(instruction.value, os);
		}
		else if constexpr(same_as<Instruction, multi_new_array>) {
			::write<uint16, endianness::big>(instruction.index, os);
			::write<uint8>(instruction.dimensions, os);
		}
		else if constexpr(same_as_any<Instruction,
			if_null, if_non_null
		>) {
			::write<int16, endianness::big>(instruction.banch, os);
		}
		else if constexpr(same_as_any<Instruction, go_to_w, jmp_sr_w>) {
			::write<int32, endianness::big>(instruction.banch, os);
		}
		else {
			Instruction::UNKNOWN;
		}
	}

}