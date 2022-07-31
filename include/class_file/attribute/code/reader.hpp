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
	class reader {
		Iterator iterator_;
	public:

		static constexpr attribute::type attribute_type = type::code;

		reader(Iterator it) : iterator_{ it } {}

		elements::of<reader<Iterator, reader_stage::max_locals>, uint16>
		operator () () const
		requires (Stage == reader_stage::max_stack) {
			Iterator i = iterator_;
			uint16 max_stack = read<uint16, endianness::big>(i);
			return { { i }, { max_stack } };
		}

		elements::of<reader<Iterator, reader_stage::code>, uint16>
		operator () () const
		requires (Stage == reader_stage::max_locals) {
			Iterator i = iterator_;
			uint16 max_locals = read<uint16, endianness::big>(i);
			return { { i }, { max_locals } };
		}

		reader<Iterator, reader_stage::exception_table>
		skip () const
		requires (Stage == reader_stage::code) {
			Iterator i = iterator_;
			uint32 length = read<uint32, endianness::big>(i);
			Iterator end = iterator_ + length;
			return { end };
		}

		elements::of<
			reader<Iterator, reader_stage::exception_table>,
			span<uint8, uint32>
		>
		as_span () const
		requires (Stage == reader_stage::code) {
			Iterator i = iterator_;
			uint32 length = read<uint32, endianness::big>(i);
			Iterator end = i + length;
			return {
				{ end },
				{ (uint8*) i, length }
			};
		}

		template<typename Handler>
		reader<Iterator, reader_stage::exception_table>
		operator () (Handler&& handler) const
		requires (Stage == reader_stage::code) {
			Iterator i = iterator_;
			uint32 length = read<uint32, endianness::big>(i);
			return (*this)(forward<Handler>(handler), length);
		}

		template<typename Handler>
		reader<Iterator, reader_stage::exception_table>
		operator () (Handler&& handler, uint32 length) const
		requires (Stage == reader_stage::code) {
			using namespace instruction;

			Iterator i = iterator_;
			Iterator end = iterator_ + length;
			Iterator src0 = iterator_;

			while(i - src0 < length) {
				uint8 instruction_code = read<uint8>(i);

				loop_action action;

				switch (instruction_code) {
					case 0:  action = handler(nop{}, i); break;
					case 1:  action = handler(a_const_null{}, i); break;
					case 2:  action = handler(i_const_m1{}, i); break;
					case 3:  action = handler(i_const_0{}, i); break;
					case 4:  action = handler(i_const_1{}, i); break;
					case 5:  action = handler(i_const_2{}, i); break;
					case 6:  action = handler(i_const_3{}, i); break;
					case 7:  action = handler(i_const_4{}, i); break;
					case 8:  action = handler(i_const_5{}, i); break;
					case 9:  action = handler(l_const_0{}, i); break;
					case 10: action = handler(l_const_1{}, i); break;
					case 11: action = handler(f_const_0{}, i); break;
					case 12: action = handler(f_const_1{}, i); break;
					case 13: action = handler(f_const_2{}, i); break;
					case 14: action = handler(d_const_0{}, i); break;
					case 15: action = handler(d_const_1{}, i); break;

					case 16: {
						int8 value = read<int8>(i);
						action = handler(bi_push{ value }, i); break;
					}
					case 17: {
						int16 value = read<int16, endianness::big>(i);
						action = handler(si_push{ value }, i); break;
					}

					case 18: {
						constant::index index { read<uint8>(i) };
						action = handler(ldc{ index }, i); break;
					}
					case 19: {
						constant::index index {
							read<uint16, endianness::big>(i)
						};
						action = handler(ldc_w{ index }, i); break;
					}
					case 20: {
						constant::index index {
							read<uint16, endianness::big>(i)
						};
						action = handler(ldc_2_w{ index }, i); break;
					}
					case 21: {
						uint8 index = read<uint8>(i);
						action = handler(i_load{ index }, i); break;
					}
					case 22: {
						uint8 index = read<uint8>(i);
						action = handler(l_load{ index }, i); break;
					}
					case 23: {
						uint8 index = read<uint8>(i);
						action = handler(f_load{ index }, i); break;
					}
					case 24: {
						uint8 index = read<uint8>(i);
						action = handler(d_load{ index }, i); break;
					}
					case 25: {
						uint8 index = read<uint8>(i);
						action = handler(a_load{ index }, i); break;
					}
					case 26: action = handler(i_load_0{}, i); break;
					case 27: action = handler(i_load_1{}, i); break;
					case 28: action = handler(i_load_2{}, i); break;
					case 29: action = handler(i_load_3{}, i); break;
					case 30: action = handler(l_load_0{}, i); break;
					case 31: action = handler(l_load_1{}, i); break;
					case 32: action = handler(l_load_2{}, i); break;
					case 33: action = handler(l_load_3{}, i); break;
					case 34: action = handler(f_load_0{}, i); break;
					case 35: action = handler(f_load_1{}, i); break;
					case 36: action = handler(f_load_2{}, i); break;
					case 37: action = handler(f_load_3{}, i); break;
					case 38: action = handler(d_load_0{}, i); break;
					case 39: action = handler(d_load_1{}, i); break;
					case 40: action = handler(d_load_2{}, i); break;
					case 41: action = handler(d_load_3{}, i); break;
					case 42: action = handler(a_load_0{}, i); break;
					case 43: action = handler(a_load_1{}, i); break;
					case 44: action = handler(a_load_2{}, i); break;
					case 45: action = handler(a_load_3{}, i); break;
					case 46: action = handler(i_a_load{}, i); break;
					case 47: action = handler(l_a_load{}, i); break;
					case 48: action = handler(f_a_load{}, i); break;
					case 49: action = handler(d_a_load{}, i); break;
					case 50: action = handler(a_a_load{}, i); break;
					case 51: action = handler(b_a_load{}, i); break;
					case 52: action = handler(c_a_load{}, i); break;
					case 53: action = handler(s_a_load{}, i); break;
					case 54: {
						uint8 index = read<uint8>(i);
						action = handler(i_store{ index }, i); break;
					}
					case 55: {
						uint8 index = read<uint8>(i);
						action = handler(l_store{ index }, i); break;
					}
					case 56: {
						uint8 index = read<uint8>(i);
						action = handler(f_store{ index }, i); break;
					}
					case 57: {
						uint8 index = read<uint8>(i);
						action = handler(d_store{ index }, i); break;
					}
					case 58: {
						uint8 index = read<uint8>(i);
						action = handler(a_store{ index }, i); break;
					}
					case 59: action = handler(i_store_0{}, i); break;
					case 60: action = handler(i_store_1{}, i); break;
					case 61: action = handler(i_store_2{}, i); break;
					case 62: action = handler(i_store_3{}, i); break;
					case 63: action = handler(l_store_0{}, i); break;
					case 64: action = handler(l_store_1{}, i); break;
					case 65: action = handler(l_store_2{}, i); break;
					case 66: action = handler(l_store_3{}, i); break;
					case 67: action = handler(f_store_0{}, i); break;
					case 68: action = handler(f_store_1{}, i); break;
					case 69: action = handler(f_store_2{}, i); break;
					case 70: action = handler(f_store_3{}, i); break;
					case 71: action = handler(d_store_0{}, i); break;
					case 72: action = handler(d_store_1{}, i); break;
					case 73: action = handler(d_store_2{}, i); break;
					case 74: action = handler(d_store_3{}, i); break;
					case 75: action = handler(a_store_0{}, i); break;
					case 76: action = handler(a_store_1{}, i); break;
					case 77: action = handler(a_store_2{}, i); break;
					case 78: action = handler(a_store_3{}, i); break;
					case 79: action = handler(i_a_store{}, i); break;
					case 80: action = handler(l_a_store{}, i); break;
					case 81: action = handler(f_a_store{}, i); break;
					case 82: action = handler(d_a_store{}, i); break;
					case 83: action = handler(a_a_store{}, i); break;
					case 84: action = handler(b_a_store{}, i); break;
					case 85: action = handler(c_a_store{}, i); break;
					case 86: action = handler(s_a_store{}, i); break;
					case 87: action = handler(pop{}, i); break;
					case 88: action = handler(pop_2{}, i); break;
					case 89: action = handler(dup{}, i); break;
					case 90: action = handler(dup_x1{}, i); break;
					case 91: action = handler(dup_x2{}, i); break;
					case 92: action = handler(dup_2{}, i); break;
					case 93: action = handler(dup_2_x1{}, i); break;
					case 94: action = handler(dup_2_x2{}, i); break;
					case 95: action = handler(swap{}, i); break;

					case 96: action = handler(i_add{}, i); break;
					case 97: action = handler(l_add{}, i); break;
					case 98: action = handler(f_add{}, i); break;
					case 99: action = handler(d_add{}, i); break;
					case 100: action = handler(i_sub{}, i); break;
					case 101: action = handler(l_sub{}, i); break;
					case 102: action = handler(f_sub{}, i); break;
					case 103: action = handler(d_sub{}, i); break;
					case 104: action = handler(i_mul{}, i); break;
					case 105: action = handler(l_mul{}, i); break;
					case 106: action = handler(f_mul{}, i); break;
					case 107: action = handler(d_mul{}, i); break;
					case 108: action = handler(i_div{}, i); break;
					case 109: action = handler(l_div{}, i); break;
					case 110: action = handler(f_div{}, i); break;
					case 111: action = handler(d_div{}, i); break;
					case 112: action = handler(i_rem{}, i); break;
					case 113: action = handler(l_rem{}, i); break;
					case 114: action = handler(f_rem{}, i); break;
					case 115: action = handler(d_rem{}, i); break;
					case 116: action = handler(i_neg{}, i); break;
					case 117: action = handler(l_neg{}, i); break;
					case 118: action = handler(f_neg{}, i); break;
					case 119: action = handler(d_neg{}, i); break;
					case 120: action = handler(i_sh_l{}, i); break;
					case 121: action = handler(l_sh_l{}, i); break;
					case 122: action = handler(i_sh_r{}, i); break;
					case 123: action = handler(l_sh_r{}, i); break;
					case 124: action = handler(i_u_sh_r{}, i); break;
					case 125: action = handler(l_u_sh_r{}, i); break;
					case 126: action = handler(i_and{}, i); break;
					case 127: action = handler(l_and{}, i); break;
					case 128: action = handler(i_or{}, i); break;
					case 129: action = handler(l_or{}, i); break;
					case 130: action = handler(i_xor{}, i); break;
					case 131: action = handler(l_xor{}, i); break;
					case 132: {
						uint8 index = read<uint8>(i);
						int8 value = read<int8>(i);
						action = handler(i_inc{ index, value }, i); break;
					}
					case 133: action = handler(i_to_l{}, i); break;
					case 134: action = handler(i_to_f{}, i); break;
					case 135: action = handler(i_to_d{}, i); break;
					case 136: action = handler(l_to_i{}, i); break;
					case 137: action = handler(l_to_f{}, i); break;
					case 138: action = handler(l_to_d{}, i); break;
					case 139: action = handler(f_to_i{}, i); break;
					case 140: action = handler(f_to_l{}, i); break;
					case 141: action = handler(f_to_d{}, i); break;
					case 142: action = handler(d_to_i{}, i); break;
					case 143: action = handler(d_to_l{}, i); break;
					case 144: action = handler(d_to_f{}, i); break;
					case 145: action = handler(i_to_b{}, i); break;
					case 146: action = handler(i_to_c{}, i); break;
					case 147: action = handler(i_to_s{}, i); break;

					case 148: action = handler(l_cmp{}, i); break;

					case 149: action = handler(f_cmp_l{}, i); break;
					case 150: action = handler(f_cmp_g{}, i); break;
					case 151: action = handler(d_cmp_l{}, i); break;
					case 152: action = handler(d_cmp_g{}, i); break;

					case 153: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_eq{ branch }, i); break;
					}
					case 154: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_ne{ branch }, i); break;
					}
					case 155: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_lt{ branch }, i); break;
					}
					case 156: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_ge{ branch }, i); break;
					}
					case 157: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_gt{ branch }, i); break;
					}
					case 158: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_le{ branch }, i); break;
					}
					case 159: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_i_cmp_eq{ branch }, i); break;
					}
					case 160: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_i_cmp_ne{ branch }, i); break;
					}
					case 161: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_i_cmp_lt{ branch }, i); break;
					}
					case 162: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_i_cmp_ge{ branch }, i); break;
					}
					case 163: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_i_cmp_gt{ branch }, i); break;
					}
					case 164: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_i_cmp_le{ branch }, i); break;
					}
					case 165: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_a_cmp_eq{ branch }, i); break;
					}
					case 166: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(if_a_cmp_ne{ branch }, i); break;
					}
					case 167: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(go_to{ branch }, i); break;
					}
					case 168: {
						int16 branch = read<int16, endianness::big>(i);
						action = handler(jmp_sr{ branch }, i); break;
					}
					case 169: {
						int8 index = read<int8>(i);
						action = handler(return_sr{ index }, i); break;
					}
					case 170: {
						while((i - src0) % 4 != 0) {
							++i;
						}
						int32 _default = read<int32, endianness::big>(i);
						int32 low = read<int32, endianness::big>(i);
						int32 high = read<int32, endianness::big>(i);
						auto n = high - low + 1;
						int32 storage[n];
						for(int32 z = 0; z < n; ++z) {
							int32 offset = read<int32, endianness::big>(i);
							storage[z] = offset;
						}
						span<int32, uint32> offsets{ storage, (uint32) n };
						action = handler(
							table_switch {
								_default,
								low,
								high,
								offsets
							}, i
						); break;
					}
					case 171: {
						while((i - src0) % 4 != 0) {
							++i;
						}
						int32 _default = read<int32, endianness::big>(i);
						uint32 n_pairs = read<uint32, endianness::big>(i);
						match_offset storage[n_pairs];
						for(nuint z = 0; z < n_pairs; ++z) {
							int32 match = read<int32, endianness::big>(i);
							int32 offset = read<int32, endianness::big>(i);
							storage[z] = match_offset{ match, offset };
						}
						span<match_offset, uint32> pairs{ storage, n_pairs };
						action = handler(
							lookup_switch{ _default, pairs }, i
						); break;
					}
					case 172: {
						action = handler(i_return{}, i); break;
					}
					case 173: {
						action = handler(l_return{}, i); break;
					}
					case 174: {
						action = handler(f_return{}, i); break;
					}
					case 175: {
						action = handler(d_return{}, i); break;
					}
					case 176: {
						action = handler(a_return{}, i); break;
					}
					case 177: {
						action = handler(_return{}, i); break;
					};

					case 178: {
						constant::field_ref_index index {
							read<uint16, endianness::big>(i)
						};
						action = handler(get_static{ index }, i); break;
					}
					case 179: {
						constant::field_ref_index index {
							read<uint16, endianness::big>(i)
						};
						action = handler(put_static{ index }, i); break;
					}
					case 180: {
						constant::field_ref_index index {
							read<uint16, endianness::big>(i)
						};
						action = handler(get_field{ index }, i); break;
					}
					case 181: {
						constant::field_ref_index index {
							read<uint16, endianness::big>(i)
						};
						action = handler(put_field{ index }, i); break;
					}
					case 182: {
						constant::method_ref_index index {
							read<uint16, endianness::big>(i)
						};
						action = handler(invoke_virtual{ index }, i); break;
					}
					case 183: {
						constant::method_ref_index index {
							read<uint16, endianness::big>(i)
						};
						action = handler(invoke_special{ index }, i); break;
					}
					case 184: {
						constant::method_ref_index index {
							read<uint16, endianness::big>(i)
						};
						action = handler(invoke_static{ index }, i); break;
					}
					case 185: {
						constant::interface_method_ref_index index {
							read<uint16, endianness::big>(i)
						};
						uint8 count = read<uint8>(i);
						read<uint8>(i); // skip 0
						action = handler(
							invoke_interface{ index, count }, i
						); break;
					}
					case 186: {
						uint16 index = read<uint16, endianness::big>(i);
						++i; // skip two zeros
						++i;
						action = handler(invoke_dynamic{ index }, i); break;
					}
					case 187: {
						constant::class_index index{
							read<uint16, endianness::big>(i)
						};
						action = handler(_new{ index }, i); break;
					}
					case 188: {
						new_array_type type{ (new_array_type) read<uint8>(i) };
						action = handler(new_array{ type }, i); break;
					}
					case 189: {
						constant::class_index index {
							read<uint16, endianness::big>(i)
						};
						action = handler(a_new_array{ index }, i); break;
					}
					case 190: action = handler(array_length{}, i); break;
					case 191: action = handler(a_throw{}, i); break;
					case 192: {
						constant::class_index index {
							read<uint16, endianness::big>(i)
						};
						action = handler(check_cast{ index }, i); break;
					}
					case 193: {
						constant::class_index index {
							read<uint16, endianness::big>(i)
						};
						action = handler(instance_of{ index }, i); break;
					}
					case 194: action = handler(monitor_enter{}, i); break;
					case 195: action = handler(monitor_exit{}, i); break;
					case 196: {
						uint8 other_code = read<uint8>(i);
						uint16 index = read<uint16, endianness::big>(i);
						if(other_code == i_inc::code) {
							uint16 value = read<uint16, endianness::big>(i);
							action = handler(
								wide_format_2{ index, value }, i
							);
						}
						else {
							action = handler(
								wide_format_1{ other_code, index }, i
							);
						}
						break;
					}
					case 197: {
						uint16 index = read<uint16, endianness::big>(i);
						uint8 dimensions = read<uint8>(i);
						action = handler(
							multi_new_array{ index, dimensions }, i
						); break;
					}
					case 198: {
						uint16 branch = read<uint16, endianness::big>(i);
						action = handler(if_null{ branch }, i); break;
					}
					case 199: {
						uint16 branch = read<uint16, endianness::big>(i);
						action = handler(if_non_null{ branch }, i); break;
					}
					case 200: {
						uint32 branch = read<uint32, endianness::big>(i);
						action = handler(goto_w{ branch }, i); break;
					}
					case 201: {
						uint16 branch = read<uint32, endianness::big>(i);
						action = handler(jmp_sr_w{ branch }, i); break;
					}

					default: action = handler(instruction_code, i);
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
				constant::class_index catch_type {
					read<uint16, endianness::big>(cpy)
				};

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