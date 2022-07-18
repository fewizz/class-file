#pragma once

#include "instruction.hpp"
#include "exception_handler.hpp"
#include "../type.hpp"

#include <core/meta/elements/of.hpp>
#include <core/loop_action.hpp>
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

		reader<Iterator, reader_stage::exception_table>
		skip () const
		requires (Stage == reader_stage::code) {
			Iterator cpy = iterator_;
			uint32 length = read<uint32, endianness::big>(cpy);
			auto end = iterator_ + length;
			return { end };
		}

		elements::of<
			reader<Iterator, reader_stage::exception_table>,
			span<uint8, uint32>
		>
		as_span () const
		requires (Stage == reader_stage::code) {
			Iterator cpy = iterator_;
			uint32 length = read<uint32, endianness::big>(cpy);
			auto end = cpy + length;
			return {
				{ end },
				{ (uint8*) cpy, length }
			};
		}

		template<typename Handler>
		reader<Iterator, reader_stage::exception_table>
		operator () (Handler&& handler) const
		requires (Stage == reader_stage::code) {
			auto cpy = iterator_;
			uint32 length = read<uint32, endianness::big>(cpy);
			return (*this)(forward<Handler>(handler), length);
		}

		template<typename Handler>
		reader<Iterator, reader_stage::exception_table>
		operator () (Handler&& handler, uint32 length) const
		requires (Stage == reader_stage::code) {
			using namespace instruction;

			auto cpy = iterator_;
			auto end = iterator_ + length;
			auto src0 = iterator_;

			while(cpy - src0 < length) {
				uint8 instruction_code = read<uint8>(cpy);

				loop_action action;

				switch (instruction_code) {
					case 0: action = handler(nop{}, cpy); break;
					case 1: action = handler(a_const_null{}, cpy); break;
					case 2: action = handler(i_const_m1{}, cpy); break;
					case 3: action = handler(i_const_0{}, cpy); break;
					case 4: action = handler(i_const_1{}, cpy); break;
					case 5: action = handler(i_const_2{}, cpy); break;
					case 6: action = handler(i_const_3{}, cpy); break;
					case 7: action = handler(i_const_4{}, cpy); break;
					case 8: action = handler(i_const_5{}, cpy); break;
					case 9: action = handler(l_const_0{}, cpy); break;
					case 10: action = handler(l_const_1{}, cpy); break;
					case 11: action = handler(f_const_0{}, cpy); break;
					case 12: action = handler(f_const_1{}, cpy); break;
					case 13: action = handler(f_const_2{}, cpy); break;
					case 14: action = handler(d_const_0{}, cpy); break;
					case 15: action = handler(d_const_1{}, cpy); break;

					case 16: {
						int8 value = read<int8>(cpy);
						action = handler(bi_push{ value }, cpy); break;
					}
					case 17: {
						int16 value = read<int16, endianness::big>(cpy);
						action = handler(si_push{ value }, cpy); break;
					}

					case 18: {
						uint8 index = read<uint8>(cpy);
						action = handler(ldc{ index }, cpy); break;
					}
					case 19: {
						uint16 index = read<uint16, endianness::big>(cpy);
						action = handler(ldc_w{ index }, cpy); break;
					}
					case 20: {
						uint16 index = read<uint16, endianness::big>(cpy);
						action = handler(ldc_2_w{ index }, cpy); break;
					}
					case 21: {
						uint8 index = read<uint8>(cpy);
						action = handler(i_load{ index }, cpy); break;
					}
					case 22: {
						uint8 index = read<uint8>(cpy);
						action = handler(l_load{ index }, cpy); break;
					}
					case 23: {
						uint8 index = read<uint8>(cpy);
						action = handler(f_load{ index }, cpy); break;
					}
					case 24: {
						uint8 index = read<uint8>(cpy);
						action = handler(d_load{ index }, cpy); break;
					}
					case 25: {
						uint8 index = read<uint8>(cpy);
						action = handler(a_load{ index }, cpy); break;
					}
					case 26: action = handler(i_load_0{}, cpy); break;
					case 27: action = handler(i_load_1{}, cpy); break;
					case 28: action = handler(i_load_2{}, cpy); break;
					case 29: action = handler(i_load_3{}, cpy); break;
					case 30: action = handler(l_load_0{}, cpy); break;
					case 31: action = handler(l_load_1{}, cpy); break;
					case 32: action = handler(l_load_2{}, cpy); break;
					case 33: action = handler(l_load_3{}, cpy); break;
					case 34: action = handler(f_load_0{}, cpy); break;
					case 35: action = handler(f_load_1{}, cpy); break;
					case 36: action = handler(f_load_2{}, cpy); break;
					case 37: action = handler(f_load_3{}, cpy); break;
					case 38: action = handler(d_load_0{}, cpy); break;
					case 39: action = handler(d_load_1{}, cpy); break;
					case 40: action = handler(d_load_2{}, cpy); break;
					case 41: action = handler(d_load_3{}, cpy); break;
					case 42: action = handler(a_load_0{}, cpy); break;
					case 43: action = handler(a_load_1{}, cpy); break;
					case 44: action = handler(a_load_2{}, cpy); break;
					case 45: action = handler(a_load_3{}, cpy); break;
					case 46: action = handler(i_a_load{}, cpy); break;
					case 47: action = handler(l_a_load{}, cpy); break;
					case 48: action = handler(f_a_load{}, cpy); break;
					case 49: action = handler(d_a_load{}, cpy); break;
					case 50: action = handler(a_a_load{}, cpy); break;
					case 51: action = handler(b_a_load{}, cpy); break;
					case 52: action = handler(c_a_load{}, cpy); break;
					case 53: action = handler(s_a_load{}, cpy); break;
					case 54: {
						uint8 index = read<uint8>(cpy);
						action = handler(i_store{ index }, cpy); break;
					}
					case 55: {
						uint8 index = read<uint8>(cpy);
						action = handler(l_store{ index }, cpy); break;
					}
					case 56: {
						uint8 index = read<uint8>(cpy);
						action = handler(f_store{ index }, cpy); break;
					}
					case 57: {
						uint8 index = read<uint8>(cpy);
						action = handler(d_store{ index }, cpy); break;
					}
					case 58: {
						uint8 index = read<uint8>(cpy);
						action = handler(a_store{ index }, cpy); break;
					}
					case 59: action = handler(i_store_0{}, cpy); break;
					case 60: action = handler(i_store_1{}, cpy); break;
					case 61: action = handler(i_store_2{}, cpy); break;
					case 62: action = handler(i_store_3{}, cpy); break;
					case 63: action = handler(l_store_0{}, cpy); break;
					case 64: action = handler(l_store_1{}, cpy); break;
					case 65: action = handler(l_store_2{}, cpy); break;
					case 66: action = handler(l_store_3{}, cpy); break;
					case 67: action = handler(f_store_0{}, cpy); break;
					case 68: action = handler(f_store_1{}, cpy); break;
					case 69: action = handler(f_store_2{}, cpy); break;
					case 70: action = handler(f_store_3{}, cpy); break;
					case 71: action = handler(d_store_0{}, cpy); break;
					case 72: action = handler(d_store_1{}, cpy); break;
					case 73: action = handler(d_store_2{}, cpy); break;
					case 74: action = handler(d_store_3{}, cpy); break;
					case 75: action = handler(a_store_0{}, cpy); break;
					case 76: action = handler(a_store_1{}, cpy); break;
					case 77: action = handler(a_store_2{}, cpy); break;
					case 78: action = handler(a_store_3{}, cpy); break;
					case 79: action = handler(i_a_store{}, cpy); break;
					case 80: action = handler(l_a_store{}, cpy); break;
					case 81: action = handler(f_a_store{}, cpy); break;
					case 82: action = handler(d_a_store{}, cpy); break;
					case 83: action = handler(a_a_store{}, cpy); break;
					case 84: action = handler(b_a_store{}, cpy); break;
					case 85: action = handler(c_a_store{}, cpy); break;
					case 86: action = handler(s_a_store{}, cpy); break;
					case 87: action = handler(pop{}, cpy); break;
					case 88: action = handler(pop_2{}, cpy); break;
					case 89: action = handler(dup{}, cpy); break;
					case 90: action = handler(dup_x1{}, cpy); break;
					case 91: action = handler(dup_x2{}, cpy); break;
					case 92: action = handler(dup_2{}, cpy); break;
					case 93: action = handler(dup_2_x1{}, cpy); break;
					case 94: action = handler(dup_2_x2{}, cpy); break;
					case 95: action = handler(swap{}, cpy); break;

					case 96: action = handler(i_add{}, cpy); break;
					case 97: action = handler(l_add{}, cpy); break;
					case 98: action = handler(f_add{}, cpy); break;
					case 99: action = handler(d_add{}, cpy); break;
					case 100: action = handler(i_sub{}, cpy); break;
					case 101: action = handler(l_sub{}, cpy); break;
					case 102: action = handler(f_sub{}, cpy); break;
					case 103: action = handler(d_sub{}, cpy); break;
					case 104: action = handler(i_mul{}, cpy); break;
					case 105: action = handler(l_mul{}, cpy); break;
					case 106: action = handler(f_mul{}, cpy); break;
					case 107: action = handler(d_mul{}, cpy); break;
					case 108: action = handler(i_div{}, cpy); break;
					case 109: action = handler(l_div{}, cpy); break;
					case 110: action = handler(f_div{}, cpy); break;
					case 111: action = handler(d_div{}, cpy); break;
					case 112: action = handler(i_rem{}, cpy); break;
					case 113: action = handler(l_rem{}, cpy); break;
					case 114: action = handler(f_rem{}, cpy); break;
					case 115: action = handler(d_rem{}, cpy); break;
					case 116: action = handler(i_neg{}, cpy); break;
					case 117: action = handler(l_neg{}, cpy); break;
					case 118: action = handler(f_neg{}, cpy); break;
					case 119: action = handler(d_neg{}, cpy); break;
					case 120: action = handler(i_sh_l{}, cpy); break;
					case 121: action = handler(l_sh_l{}, cpy); break;
					case 122: action = handler(i_sh_r{}, cpy); break;
					case 123: action = handler(l_sh_r{}, cpy); break;
					case 124: action = handler(i_u_sh_r{}, cpy); break;
					case 125: action = handler(l_u_sh_r{}, cpy); break;
					case 126: action = handler(i_and{}, cpy); break;
					case 127: action = handler(l_and{}, cpy); break;
					case 128: action = handler(i_or{}, cpy); break;
					case 129: action = handler(l_or{}, cpy); break;
					case 130: action = handler(i_xor{}, cpy); break;
					case 131: action = handler(l_xor{}, cpy); break;
					case 132: {
						uint8 index = read<uint8>(cpy);
						int8 value = read<int8>(cpy);
						action = handler(i_inc{ index, value }, cpy); break;
					}
					case 133: action = handler(i_to_l{}, cpy); break;
					case 134: action = handler(i_to_f{}, cpy); break;
					case 135: action = handler(i_to_d{}, cpy); break;
					case 136: action = handler(l_to_i{}, cpy); break;
					case 137: action = handler(l_to_f{}, cpy); break;
					case 138: action = handler(l_to_d{}, cpy); break;
					case 139: action = handler(f_to_i{}, cpy); break;
					case 140: action = handler(f_to_l{}, cpy); break;
					case 141: action = handler(f_to_d{}, cpy); break;
					case 142: action = handler(d_to_i{}, cpy); break;
					case 143: action = handler(d_to_l{}, cpy); break;
					case 144: action = handler(d_to_f{}, cpy); break;
					case 145: action = handler(i_to_b{}, cpy); break;
					case 146: action = handler(i_to_c{}, cpy); break;
					case 147: action = handler(i_to_s{}, cpy); break;

					case 148: action = handler(l_cmp{}, cpy); break;

					case 149: action = handler(f_cmp_l{}, cpy); break;
					case 150: action = handler(f_cmp_g{}, cpy); break;
					case 151: action = handler(d_cmp_l{}, cpy); break;
					case 152: action = handler(d_cmp_g{}, cpy); break;

					case 153: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_eq{ branch }, cpy); break;
					}
					case 154: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_ne{ branch }, cpy); break;
					}
					case 155: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_lt{ branch }, cpy); break;
					}
					case 156: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_ge{ branch }, cpy); break;
					}
					case 157: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_gt{ branch }, cpy); break;
					}
					case 158: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_le{ branch }, cpy); break;
					}
					case 159: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_i_cmp_eq{ branch }, cpy); break;
					}
					case 160: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_i_cmp_ne{ branch }, cpy); break;
					}
					case 161: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_i_cmp_lt{ branch }, cpy); break;
					}
					case 162: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_i_cmp_ge{ branch }, cpy); break;
					}
					case 163: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_i_cmp_gt{ branch }, cpy); break;
					}
					case 164: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_i_cmp_le{ branch }, cpy); break;
					}
					case 165: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_a_cmp_eq{ branch }, cpy); break;
					}
					case 166: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(if_a_cmp_ne{ branch }, cpy); break;
					}
					case 167: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(go_to{ branch }, cpy); break;
					}
					case 168: {
						int16 branch = read<int16, endianness::big>(cpy);
						action = handler(jmp_sr{ branch }, cpy); break;
					}
					case 169: {
						int8 index = read<int8>(cpy);
						action = handler(return_sr{ index }, cpy); break;
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
						action = handler(
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
						action = handler(
							lookup_switch{ _default, pairs }, cpy
						); break;
					}
					case 172: {
						action = handler(i_return{}, cpy); break;
					}
					case 173: {
						action = handler(l_return{}, cpy); break;
					}
					case 174: {
						action = handler(f_return{}, cpy); break;
					}
					case 175: {
						action = handler(d_return{}, cpy); break;
					}
					case 176: {
						action = handler(a_return{}, cpy); break;
					}
					case 177: {
						action = handler(_return{}, cpy); break;
					};

					case 178: {
						uint16 index = read<uint16, endianness::big>(cpy);
						action = handler(get_static{ index }, cpy); break;
					}
					case 179: {
						uint16 index = read<uint16, endianness::big>(cpy);
						action = handler(put_static{ index }, cpy); break;
					}
					case 180: {
						uint16 index = read<uint16, endianness::big>(cpy);
						action = handler(get_field{ index }, cpy); break;
					}
					case 181: {
						uint16 index = read<uint16, endianness::big>(cpy);
						action = handler(put_field{ index }, cpy); break;
					}
					case 182: {
						uint16 index = read<uint16, endianness::big>(cpy);
						action = handler(invoke_virtual{ index }, cpy); break;
					}
					case 183: {
						uint16 index = read<uint16, endianness::big>(cpy);
						action = handler(invoke_special{ index }, cpy); break;
					}
					case 184: {
						uint16 index = read<uint16, endianness::big>(cpy);
						action = handler(invoke_static{ index }, cpy); break;
					}
					case 185: {
						uint16 index = read<uint16, endianness::big>(cpy);
						uint8 count = read<uint8>(cpy);
						read<uint8>(cpy); // skip 0
						action = handler(
							invoke_interface{ index, count }, cpy
						); break;
					}
					case 186: {
						uint16 index = read<uint16, endianness::big>(cpy);
						read<uint16>(cpy); // skip two zeros
						action = handler(invoke_dynamic{ index }, cpy); break;
					}
					case 187: {
						uint16 index = read<uint16, endianness::big>(cpy);
						action = handler(_new{ index }, cpy); break;
					}
					case 188: {
						uint8 type = read<uint8>(cpy);
						action = handler(new_array{ type }, cpy); break;
					}
					case 189: {
						uint16 index = read<uint16, endianness::big>(cpy);
						action = handler(a_new_array{ index }, cpy); break;
					}
					case 190: action = handler(array_length{}, cpy); break;
					case 191: action = handler(a_throw{}, cpy); break;
					case 192: {
						uint16 index = read<uint16, endianness::big>(cpy);
						action = handler(check_cast{ index }, cpy); break;
					}
					case 193: {
						uint16 index = read<uint16, endianness::big>(cpy);
						action = handler(instance_of{ index }, cpy); break;
					}
					case 194: action = handler(monitor_enter{}, cpy); break;
					case 195: action = handler(monitor_exit{}, cpy); break;
					case 196: {
						uint8 other_code = read<uint8>(cpy);
						uint16 index = read<uint16, endianness::big>(cpy);
						if(other_code == i_inc::code) {
							uint16 value = read<uint16, endianness::big>(cpy);
							action = handler(
								wide_format_2{ index, value }, cpy
							);
						}
						else {
							action = handler(
								wide_format_1{ other_code, index }, cpy
							);
						}
						break;
					}
					case 197: {
						uint16 index = read<uint16, endianness::big>(cpy);
						uint8 dimensions = read<uint8>(cpy);
						action = handler(
							multi_new_array{ index, dimensions }, cpy
						); break;
					}
					case 198: {
						uint16 branch = read<uint16, endianness::big>(cpy);
						action = handler(if_null{ branch }, cpy); break;
					}
					case 199: {
						uint16 branch = read<uint16, endianness::big>(cpy);
						action = handler(if_non_null{ branch }, cpy); break;
					}
					case 200: {
						uint32 branch = read<uint32, endianness::big>(cpy);
						action = handler(goto_w{ branch }, cpy); break;
					}
					case 201: {
						uint16 branch = read<uint32, endianness::big>(cpy);
						action = handler(jmp_sr_w{ branch }, cpy); break;
					}

					default: action = handler(instruction_code, cpy);
				}

				switch (action) {
					case loop_action::next: break;
					case loop_action::stop: return { end };
				}
			}

			return { end };
		}

		uint16 count() const
		requires (Stage == reader_stage::exception_table) {
			Iterator cpy = iterator_;
			uint16 length = read<uint16, endianness::big>(cpy);
			return length;
		}

		template<typename Handler>
		reader<Iterator, reader_stage::attributes>
		operator () (Handler&& handler) const
		requires (Stage == reader_stage::exception_table) {
			Iterator cpy = iterator_;

			uint16 length = read<uint16, endianness::big>(cpy);

			Iterator end = cpy + sizeof(uint16) * length * 4;

			while(length > 0) {
				uint16 start_pc = read<uint16, endianness::big>(cpy);
				uint16 end_pc = read<uint16, endianness::big>(cpy);
				uint16 handler_pc = read<uint16, endianness::big>(cpy);
				uint16 catch_type = read<uint16, endianness::big>(cpy);

				loop_action action = handler(exception_handler {
					start_pc, end_pc,
					handler_pc,
					catch_type
				});

				switch (action) {
					case loop_action::next: break;
					case loop_action::stop: return { end };
				}

				--length;
			}

			return { end };
		}

	};

	template<typename Iterator>
	reader(Iterator) -> reader<Iterator>;

}