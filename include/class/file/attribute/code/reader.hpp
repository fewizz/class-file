#pragma once

#include "../type.hpp"
#include "../../code/instruction.hpp"

#include <core/meta/elements/of.hpp>
#include <core/read.hpp>

namespace class_file::attribute::code {

	enum class reader_stage {
		max_stack,
		max_locals,
		code,
		exception_table,
		attributes
	};

	template<typename Iterator, reader_stage Stage = reader_stage::max_stack>
	struct reader {
		static constexpr attribute::type type = type::code;

		Iterator iterator_;

		elements::of<reader<Iterator, reader_stage::max_locals>, uint16>
		operator () () const
		requires (Stage == reader_stage::max_stack) {
			Iterator cpy = iterator_;
			uint16 max_stack = read<uint16, endianness::big>(cpy);
			return { { cpy }, { max_stack } };
		}

		elements::of<reader<Iterator, reader_stage::code>, uint16>
		operator () () const
		requires (Stage == reader_stage::max_locals) {
			Iterator cpy = iterator_;
			uint16 max_locals = read<uint16, endianness::big>(cpy);
			return { { cpy }, { max_locals } };
		}

		template<typename Handler>
		reader<Iterator, reader_stage::exception_table>
		operator () (Handler&& handler) const
		requires (Stage == reader_stage::code) {
			Iterator cpy = iterator_;
			uint32 length = read<uint32, endianness::big>(cpy);
			return (*this)(forward<Handler>(handler), length);
		}

		template<typename Handler>
		reader<Iterator, reader_stage::exception_table>
		operator () (
			Handler&& handler,
			uint32 length
		) const
		requires (Stage == reader_stage::code) {
			using namespace class_file::code::instruction;

			auto cpy = iterator_;
			read<uint32, endianness::big>(cpy);
			auto src0 = cpy;

			while(cpy - src0 < length) {
				uint8 first = read<uint8>(cpy);

				switch (first) {
					case 0: handler(nop{}, cpy); break;

					case 1: handler(a_const_null{}, cpy); break;
					case 2: handler(i_const_m1{}, cpy); break;
					case 3: handler(i_const_0{}, cpy); break;
					case 4: handler(i_const_1{}, cpy); break;
					case 5: handler(i_const_2{}, cpy); break;
					case 6: handler(i_const_3{}, cpy); break;
					case 7: handler(i_const_4{}, cpy); break;
					case 8: handler(i_const_5{}, cpy); break;
					case 9: handler(l_const_0{}, cpy); break;
					case 10: handler(l_const_1{}, cpy); break;
					case 11: handler(f_const_0{}, cpy); break;
					case 12: handler(f_const_1{}, cpy); break;
					case 13: handler(f_const_2{}, cpy); break;

					case 16: {
						int8 value = read<int8>(cpy);
						handler(bi_push{ value }, cpy); break;
					}
					case 17: {
						int16 value = read<int16, endianness::big>(cpy);
						handler(si_push{ value }, cpy); break;
					}

					case 18: {
						uint8 index = read<uint8>(cpy);
						handler(ldc{ index }, cpy); break;
					}
					case 19: {
						uint16 index = read<uint16, endianness::big>(cpy);
						handler(ldc_w{ index }, cpy); break;
					}
					case 20: {
						uint16 index = read<uint16, endianness::big>(cpy);
						handler(ldc_2_w{ index }, cpy); break;
					}
					case 21: {
						uint8 index = read<uint8>(cpy);
						handler(i_load{ index }, cpy); break;
					}
					case 22: {
						uint8 index = read<uint8>(cpy);
						handler(l_load{ index }, cpy); break;
					}
					case 23: {
						uint8 index = read<uint8>(cpy);
						handler(f_load{ index }, cpy); break;
					}
					case 25: {
						uint8 index = read<uint8>(cpy);
						handler(a_load{ index }, cpy); break;
					}
					case 26: handler(i_load_0{}, cpy); break;
					case 27: handler(i_load_1{}, cpy); break;
					case 28: handler(i_load_2{}, cpy); break;
					case 29: handler(i_load_3{}, cpy); break;
					case 30: handler(l_load_0{}, cpy); break;
					case 31: handler(l_load_1{}, cpy); break;
					case 32: handler(l_load_2{}, cpy); break;
					case 33: handler(l_load_3{}, cpy); break;
					case 34: handler(f_load_0{}, cpy); break;
					case 35: handler(f_load_1{}, cpy); break;
					case 36: handler(f_load_2{}, cpy); break;
					case 37: handler(f_load_3{}, cpy); break;
					case 38: handler(d_load_0{}, cpy); break;
					case 39: handler(d_load_1{}, cpy); break;
					case 40: handler(d_load_2{}, cpy); break;
					case 41: handler(d_load_3{}, cpy); break;
					case 42: handler(a_load_0{}, cpy); break;
					case 43: handler(a_load_1{}, cpy); break;
					case 44: handler(a_load_2{}, cpy); break;
					case 45: handler(a_load_3{}, cpy); break;
					case 46: handler(i_a_load{}, cpy); break;

					case 50: handler(a_a_load{}, cpy); break;
					case 51: handler(b_a_load{}, cpy); break;
					case 52: handler(c_a_load{}, cpy); break;
					case 54: {
						uint8 index = read<uint8>(cpy);
						handler(i_store{ index }, cpy); break;
					}
					case 55: {
						uint8 index = read<uint8>(cpy);
						handler(l_store{ index }, cpy); break;
					}
					case 56: {
						uint8 index = read<uint8>(cpy);
						handler(f_store{ index }, cpy); break;
					}
					case 58: {
						uint8 index = read<uint8>(cpy);
						handler(a_store{ index }, cpy); break;
					}
					case 59: handler(i_store_0{}, cpy); break;
					case 60: handler(i_store_1{}, cpy); break;
					case 61: handler(i_store_2{}, cpy); break;
					case 62: handler(i_store_3{}, cpy); break;
					case 63: handler(l_store_0{}, cpy); break;
					case 64: handler(l_store_1{}, cpy); break;
					case 65: handler(l_store_2{}, cpy); break;
					case 66: handler(l_store_3{}, cpy); break;
					case 75: handler(a_store_0{}, cpy); break;
					case 76: handler(a_store_1{}, cpy); break;
					case 77: handler(a_store_2{}, cpy); break;
					case 78: handler(a_store_3{}, cpy); break;
					case 79: handler(i_a_store{}, cpy); break;
					case 83: handler(a_a_store{}, cpy); break;
					case 84: handler(b_a_store{}, cpy); break;
					case 85: handler(c_a_store{}, cpy); break;
					case 87: handler(pop{}, cpy); break;
					case 89: handler(dup{}, cpy); break;
					case 90: handler(dup_x1{}, cpy); break;
					case 92: handler(dup_2{}, cpy); break;

					case 96: handler(i_add{}, cpy); break;
					case 97: handler(l_add{}, cpy); break;
					case 98: handler(f_add{}, cpy); break;
					case 100: handler(i_sub{}, cpy); break;
					case 101: handler(l_sub{}, cpy); break;
					case 104: handler(i_mul{}, cpy); break;
					case 105: handler(l_mul{}, cpy); break;
					case 106: handler(f_mul{}, cpy); break;
					case 107: handler(d_mul{}, cpy); break;
					case 108: handler(i_div{}, cpy); break;
					case 110: handler(f_div{}, cpy); break;
					case 112: handler(i_rem{}, cpy); break;
					case 116: handler(i_neg{}, cpy); break;
					case 120: handler(i_sh_l{}, cpy); break;
					case 121: handler(l_sh_l{}, cpy); break;
					case 122: handler(i_sh_r{}, cpy); break;
					case 123: handler(l_sh_r{}, cpy); break;
					case 124: handler(i_ush_r{}, cpy); break;
					case 125: handler(l_ush_r{}, cpy); break;
					case 126: handler(i_and{}, cpy); break;
					case 127: handler(l_and{}, cpy); break;
					case 128: handler(i_or{}, cpy); break;
					case 129: handler(l_or{}, cpy); break;
					case 130: handler(i_xor{}, cpy); break;
					case 132: {
						uint8 index = read<uint8>(cpy);
						int8 value = read<int8>(cpy);
						handler(i_inc{ index, value }, cpy); break;
					}
					case 133: handler(i_to_l{}, cpy); break;
					case 134: handler(i_to_f{}, cpy); break;
					case 135: handler(i_to_d{}, cpy); break;
					case 136: handler(l_to_i{}, cpy); break;
					case 139: handler(f_to_i{}, cpy); break;
					case 141: handler(f_to_d{}, cpy); break;
					case 142: handler(d_to_i{}, cpy); break;
					case 145: handler(i_to_b{}, cpy); break;
					case 146: handler(i_to_c{}, cpy); break;

					case 148: handler(l_cmp{}, cpy); break;

					case 149: handler(f_cmp_l{}, cpy); break;
					case 150: handler(f_cmp_g{}, cpy); break;

					case 153: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_eq{ branch }, cpy); break;
					}
					case 154: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_ne{ branch }, cpy); break;
					}
					case 155: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_lt{ branch }, cpy); break;
					}
					case 156: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_ge{ branch }, cpy); break;
					}
					case 157: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_gt{ branch }, cpy); break;
					}
					case 158: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_le{ branch }, cpy); break;
					}
					case 159: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_i_cmp_eq{ branch }, cpy); break;
					}
					case 160: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_i_cmp_ne{ branch }, cpy); break;
					}
					case 161: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_i_cmp_lt{ branch }, cpy); break;
					}
					case 162: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_i_cmp_ge{ branch }, cpy); break;
					}
					case 163: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_i_cmp_gt{ branch }, cpy); break;
					}
					case 164: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_i_cmp_le{ branch }, cpy); break;
					}
					case 165: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_a_cmp_eq{ branch }, cpy); break;
					}
					case 166: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(if_a_cmp_ne{ branch }, cpy); break;
					}
					case 167: {
						int16 branch = read<int16, endianness::big>(cpy);
						handler(go_to{ branch }, cpy); break;
					}
					case 170: {
						while((cpy - src0) % 4 != 0) {
							++cpy;
						}
						int32 _default = read<int32, endianness::big>(cpy);
						int32 low = read<int32, endianness::big>(cpy);
						int32 high = read<int32, endianness::big>(cpy);
						auto n = high - low + 1;
						int32 storage[n];
						for(int32 z = 0; z < n; ++z) {
							int32 offset = read<int32, endianness::big>(cpy);
							storage[z] = offset;
						}
						span<int32, uint32> offsets{ storage, (uint32) n };
						handler(
							table_switch {
								_default,
								low,
								high,
								offsets
							}, cpy
						); break;
					}
					case 171: {
						while((cpy - src0) % 4 != 0) {
							++cpy;
						}
						int32 _default = read<int32, endianness::big>(cpy);
						uint32 n_pairs = read<uint32, endianness::big>(cpy);
						match_offset storage[n_pairs];
						for(nuint z = 0; z < n_pairs; ++z) {
							int32 match = read<int32, endianness::big>(cpy);
							int32 offset = read<int32, endianness::big>(cpy);
							storage[z] = match_offset{ match, offset };
						}
						span<match_offset, uint32> pairs{ storage, n_pairs };
						handler(lookup_switch{ _default, pairs }, cpy); break;
					}
					case 172: {
						if(handler(i_return{}, cpy)) {
							return{ src0 + length };
						}
						break;
					}
					case 173: {
						if(handler(l_return{}, cpy)) {
							return{ src0 + length };
						}
						break;
					}
					case 175: {
						if(handler(d_return{}, cpy)) {
							return{ src0 + length };
						}
						break;
					}
					case 176: {
						if(handler(a_return{}, cpy)) {
							return{ src0 + length };
						}
						break;
					}
					case 177: {
						if(handler(_return{}, cpy)) {
							return{ src0 + length };
						}
						break;
					};

					case 178: {
						uint16 index = read<uint16, endianness::big>(cpy);
						handler(get_static{ index }, cpy); break;
					}
					case 179: {
						uint16 index = read<uint16, endianness::big>(cpy);
						handler(put_static{ index }, cpy); break;
					}
					case 180: {
						uint16 index = read<uint16, endianness::big>(cpy);
						handler(get_field{ index }, cpy); break;
					}
					case 181: {
						uint16 index = read<uint16, endianness::big>(cpy);
						handler(put_field{ index }, cpy); break;
					}
					case 182: {
						uint16 index = read<uint16, endianness::big>(cpy);
						handler(invoke_virtual{ index }, cpy); break;
					}
					case 183: {
						uint16 index = read<uint16, endianness::big>(cpy);
						handler(invoke_special{ index }, cpy); break;
					}
					case 184: {
						uint16 index = read<uint16, endianness::big>(cpy);
						handler(invoke_static{ index }, cpy); break;
					}
					case 185: {
						uint16 index = read<uint16, endianness::big>(cpy);
						uint8 count = read<uint8>(cpy);
						read<uint8>(cpy); // skip 0
						handler(invoke_interface{ index, count }, cpy); break;
					}
					case 186: {
						uint16 index = read<uint16, endianness::big>(cpy);
						read<uint16>(cpy); // skip two zeros
						handler(invoke_dynamic{ index }, cpy); break;
					}
					case 187: {
						uint16 index = read<uint16, endianness::big>(cpy);
						handler(_new{ index }, cpy); break;
					}
					case 188: {
						uint8 type = read<uint8>(cpy);
						handler(new_array{ type }, cpy); break;
					}
					case 189: {
						uint16 index = read<uint16, endianness::big>(cpy);
						handler(a_new_array{ index }, cpy); break;
					}
					case 190: handler(array_length{}, cpy); break;
					case 191: handler(a_throw{}, cpy); break;
					case 192: {
						uint16 index = read<uint16, endianness::big>(cpy);
						handler(check_cast{ index }, cpy); break;
					}
					case 193: {
						uint16 index = read<uint16, endianness::big>(cpy);
						handler(instance_of{ index }, cpy); break;
					}
					case 194: handler(monitor_enter{}, cpy); break;
					case 195: handler(monitor_exit{}, cpy); break;
					case 198: {
						uint16 branch = read<uint16, endianness::big>(cpy);
						handler(if_null{ branch }, cpy); break;
					}
					case 199: {
						uint16 branch = read<uint16, endianness::big>(cpy);
						handler(if_non_null{ branch }, cpy); break;
					}

					default: handler(first, cpy);
				}
			}

			return { cpy };
		}

	};

	template<typename Iterator>
	reader(Iterator) -> reader<Iterator>;

}