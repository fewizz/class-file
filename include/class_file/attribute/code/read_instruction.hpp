#pragma once

#include "./instruction.hpp"

#include <iterator_and_sentinel.hpp>
#include <endianness.hpp>
#include <read.hpp>

namespace class_file::attribute::code::instruction {

	template<basic_iterator Iterator, typename Handler>
	decltype(auto) read(
		Iterator&& iterator, basic_iterator auto begin, Handler&& handler
	) {
		uint8 instruction_code = ::read<uint8>(iterator);

		switch (instruction_code) {
			case 0:  return handler(nop{});
			case 1:  return handler(a_const_null{});
			case 2:  return handler(i_const_m1{});
			case 3:  return handler(i_const_0{});
			case 4:  return handler(i_const_1{});
			case 5:  return handler(i_const_2{});
			case 6:  return handler(i_const_3{});
			case 7:  return handler(i_const_4{});
			case 8:  return handler(i_const_5{});
			case 9:  return handler(l_const_0{});
			case 10: return handler(l_const_1{});
			case 11: return handler(f_const_0{});
			case 12: return handler(f_const_1{});
			case 13: return handler(f_const_2{});
			case 14: return handler(d_const_0{});
			case 15: return handler(d_const_1{});

			case 16: {
				int8 value = ::read<int8>(iterator);
				return handler(bi_push{ value });
			}
			case 17: {
				int16 value = ::read<int16, endianness::big>(iterator);
				return handler(si_push{ value });
			}

			case 18: {
				constant::index index { ::read<uint8>(iterator) };
				return handler(ldc{ index });
			}
			case 19: {
				constant::index index {
					::read<uint16, endianness::big>(iterator)
				};
				return handler(ldc_w{ index });
			}
			case 20: {
				constant::index index {
					::read<uint16, endianness::big>(iterator)
				};
				return handler(ldc_2_w{ index });
			}
			case 21: {
				uint8 index = ::read<uint8>(iterator);
				return handler(i_load{ index });
			}
			case 22: {
				uint8 index = ::read<uint8>(iterator);
				return handler(l_load{ index });
			}
			case 23: {
				uint8 index = ::read<uint8>(iterator);
				return handler(f_load{ index });
			}
			case 24: {
				uint8 index = ::read<uint8>(iterator);
				return handler(d_load{ index });
			}
			case 25: {
				uint8 index = ::read<uint8>(iterator);
				return handler(a_load{ index });
			}
			case 26: return handler(i_load_0{});
			case 27: return handler(i_load_1{});
			case 28: return handler(i_load_2{});
			case 29: return handler(i_load_3{});
			case 30: return handler(l_load_0{});
			case 31: return handler(l_load_1{});
			case 32: return handler(l_load_2{});
			case 33: return handler(l_load_3{});
			case 34: return handler(f_load_0{});
			case 35: return handler(f_load_1{});
			case 36: return handler(f_load_2{});
			case 37: return handler(f_load_3{});
			case 38: return handler(d_load_0{});
			case 39: return handler(d_load_1{});
			case 40: return handler(d_load_2{});
			case 41: return handler(d_load_3{});
			case 42: return handler(a_load_0{});
			case 43: return handler(a_load_1{});
			case 44: return handler(a_load_2{});
			case 45: return handler(a_load_3{});
			case 46: return handler(i_a_load{});
			case 47: return handler(l_a_load{});
			case 48: return handler(f_a_load{});
			case 49: return handler(d_a_load{});
			case 50: return handler(a_a_load{});
			case 51: return handler(b_a_load{});
			case 52: return handler(c_a_load{});
			case 53: return handler(s_a_load{});
			case 54: {
				uint8 index = ::read<uint8>(iterator);
				return handler(i_store{ index });
			}
			case 55: {
				uint8 index = ::read<uint8>(iterator);
				return handler(l_store{ index });
			}
			case 56: {
				uint8 index = ::read<uint8>(iterator);
				return handler(f_store{ index });
			}
			case 57: {
				uint8 index = ::read<uint8>(iterator);
				return handler(d_store{ index });
			}
			case 58: {
				uint8 index = ::read<uint8>(iterator);
				return handler(a_store{ index });
			}
			case 59: return handler(i_store_0{});
			case 60: return handler(i_store_1{});
			case 61: return handler(i_store_2{});
			case 62: return handler(i_store_3{});
			case 63: return handler(l_store_0{});
			case 64: return handler(l_store_1{});
			case 65: return handler(l_store_2{});
			case 66: return handler(l_store_3{});
			case 67: return handler(f_store_0{});
			case 68: return handler(f_store_1{});
			case 69: return handler(f_store_2{});
			case 70: return handler(f_store_3{});
			case 71: return handler(d_store_0{});
			case 72: return handler(d_store_1{});
			case 73: return handler(d_store_2{});
			case 74: return handler(d_store_3{});
			case 75: return handler(a_store_0{});
			case 76: return handler(a_store_1{});
			case 77: return handler(a_store_2{});
			case 78: return handler(a_store_3{});
			case 79: return handler(i_a_store{});
			case 80: return handler(l_a_store{});
			case 81: return handler(f_a_store{});
			case 82: return handler(d_a_store{});
			case 83: return handler(a_a_store{});
			case 84: return handler(b_a_store{});
			case 85: return handler(c_a_store{});
			case 86: return handler(s_a_store{});
			case 87: return handler(pop{});
			case 88: return handler(pop_2{});
			case 89: return handler(dup{});
			case 90: return handler(dup_x1{});
			case 91: return handler(dup_x2{});
			case 92: return handler(dup_2{});
			case 93: return handler(dup_2_x1{});
			case 94: return handler(dup_2_x2{});
			case 95: return handler(swap{});

			case 96: return handler(i_add{});
			case 97: return handler(l_add{});
			case 98: return handler(f_add{});
			case 99: return handler(d_add{});
			case 100: return handler(i_sub{});
			case 101: return handler(l_sub{});
			case 102: return handler(f_sub{});
			case 103: return handler(d_sub{});
			case 104: return handler(i_mul{});
			case 105: return handler(l_mul{});
			case 106: return handler(f_mul{});
			case 107: return handler(d_mul{});
			case 108: return handler(i_div{});
			case 109: return handler(l_div{});
			case 110: return handler(f_div{});
			case 111: return handler(d_div{});
			case 112: return handler(i_rem{});
			case 113: return handler(l_rem{});
			case 114: return handler(f_rem{});
			case 115: return handler(d_rem{});
			case 116: return handler(i_neg{});
			case 117: return handler(l_neg{});
			case 118: return handler(f_neg{});
			case 119: return handler(d_neg{});
			case 120: return handler(i_sh_l{});
			case 121: return handler(l_sh_l{});
			case 122: return handler(i_sh_r{});
			case 123: return handler(l_sh_r{});
			case 124: return handler(i_u_sh_r{});
			case 125: return handler(l_u_sh_r{});
			case 126: return handler(i_and{});
			case 127: return handler(l_and{});
			case 128: return handler(i_or{});
			case 129: return handler(l_or{});
			case 130: return handler(i_xor{});
			case 131: return handler(l_xor{});
			case 132: {
				uint8 index = ::read<uint8>(iterator);
				int8 value = ::read<int8>(iterator);
				return handler(i_inc{ index, value });
			}
			case 133: return handler(i_to_l{});
			case 134: return handler(i_to_f{});
			case 135: return handler(i_to_d{});
			case 136: return handler(l_to_i{});
			case 137: return handler(l_to_f{});
			case 138: return handler(l_to_d{});
			case 139: return handler(f_to_i{});
			case 140: return handler(f_to_l{});
			case 141: return handler(f_to_d{});
			case 142: return handler(d_to_i{});
			case 143: return handler(d_to_l{});
			case 144: return handler(d_to_f{});
			case 145: return handler(i_to_b{});
			case 146: return handler(i_to_c{});
			case 147: return handler(i_to_s{});

			case 148: return handler(l_cmp{});

			case 149: return handler(f_cmp_l{});
			case 150: return handler(f_cmp_g{});
			case 151: return handler(d_cmp_l{});
			case 152: return handler(d_cmp_g{});

			case 153: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_eq{ branch });
			}
			case 154: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_ne{ branch });
			}
			case 155: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_lt{ branch });
			}
			case 156: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_ge{ branch });
			}
			case 157: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_gt{ branch });
			}
			case 158: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_le{ branch });
			}
			case 159: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_i_cmp_eq{ branch });
			}
			case 160: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_i_cmp_ne{ branch });
			}
			case 161: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_i_cmp_lt{ branch });
			}
			case 162: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_i_cmp_ge{ branch });
			}
			case 163: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_i_cmp_gt{ branch });
			}
			case 164: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_i_cmp_le{ branch });
			}
			case 165: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_a_cmp_eq{ branch });
			}
			case 166: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_a_cmp_ne{ branch });
			}
			case 167: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(go_to{ branch });
			}
			case 168: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(jmp_sr{ branch });
			}
			case 169: {
				int8 index = ::read<int8>(iterator);
				return handler(return_sr{ index });
			}
			case 170: {
				while((iterator - begin) % 4 != 0) {
					++iterator;
				}
				int32 _default = ::read<int32, endianness::big>(iterator);
				int32 low = ::read<int32, endianness::big>(iterator);
				int32 high = ::read<int32, endianness::big>(iterator);
				auto n = high - low + 1;
				int32 storage[n];
				for(int32 z = 0; z < n; ++z) {
					int32 offset = ::read<int32, endianness::big>(iterator);
					storage[z] = offset;
				}
				span<int32, uint32> offsets{ storage, (uint32) n };
				return handler(
					table_switch {
						_default,
						low,
						high,
						offsets
					}
				);
			}
			case 171: {
				while((iterator - begin) % 4 != 0) {
					++iterator;
				}
				int32 _default = ::read<int32, endianness::big>(iterator);
				uint32 n_pairs = ::read<uint32, endianness::big>(iterator);
				match_offset storage[n_pairs];
				for(nuint z = 0; z < n_pairs; ++z) {
					int32 match = ::read<int32, endianness::big>(iterator);
					int32 offset = ::read<int32, endianness::big>(iterator);
					storage[z] = match_offset{ match, offset };
				}
				span<match_offset, uint32> pairs{ storage, n_pairs };
				return handler(
					lookup_switch{ _default, pairs }
				);
			}
			case 172: return handler(i_return{});
			case 173: return handler(l_return{});
			case 174: return handler(f_return{});
			case 175: return handler(d_return{});
			case 176: return handler(a_return{});
			case 177: return handler(_return{});

			case 178: {
				constant::field_ref_index index {
					::read<uint16, endianness::big>(iterator)
				};
				return handler(get_static{ index });
			}
			case 179: {
				constant::field_ref_index index {
					::read<uint16, endianness::big>(iterator)
				};
				return handler(put_static{ index });
			}
			case 180: {
				constant::field_ref_index index {
					::read<uint16, endianness::big>(iterator)
				};
				return handler(get_field{ index });
			}
			case 181: {
				constant::field_ref_index index {
					::read<uint16, endianness::big>(iterator)
				};
				return handler(put_field{ index });
			}
			case 182: {
				constant::method_ref_index index {
					::read<uint16, endianness::big>(iterator)
				};
				return handler(invoke_virtual{ index });
			}
			case 183: {
				constant::method_or_interface_method_ref_index index {
					::read<uint16, endianness::big>(iterator)
				};
				return handler(invoke_special{ index });
			}
			case 184: {
				constant::method_or_interface_method_ref_index index {
					::read<uint16, endianness::big>(iterator)
				};
				return handler(invoke_static{ index });
			}
			case 185: {
				constant::interface_method_ref_index index {
					::read<uint16, endianness::big>(iterator)
				};
				uint8 count = ::read<uint8>(iterator);
				::read<uint8>(iterator); // skip 0
				return handler(invoke_interface{ index, count });
			}
			case 186: {
				constant::invoke_dynamic_index index {
					::read<uint16, endianness::big>(iterator)
				};
				++iterator; // skip two zeros
				++iterator;
				return handler(invoke_dynamic{ index });
			}
			case 187: {
				constant::class_index index{
					::read<uint16, endianness::big>(iterator)
				};
				return handler(_new{ index });
			}
			case 188: {
				new_array_type type{ (new_array_type) ::read<uint8>(iterator) };
				return handler(new_array{ type });
			}
			case 189: {
				constant::class_index index {
					::read<uint16, endianness::big>(iterator)
				};
				return handler(a_new_array{ index });
			}
			case 190: return handler(array_length{});
			case 191: return handler(a_throw{});
			case 192: {
				constant::class_index index {
					::read<uint16, endianness::big>(iterator)
				};
				return handler(check_cast{ index });
			}
			case 193: {
				constant::class_index index {
					::read<uint16, endianness::big>(iterator)
				};
				return handler(instance_of{ index });
			}
			case 194: return handler(monitor_enter{});
			case 195: return handler(monitor_exit{});
			case 196: {
				uint8 other_code = ::read<uint8>(iterator);
				uint16 index = ::read<uint16, endianness::big>(iterator);
				if(other_code == i_inc::code) {
					uint16 value = ::read<uint16, endianness::big>(iterator);
					return handler(wide_format_2{ index, value });
				}
				else {
					return handler(wide_format_1{ other_code, index });
				}
			}
			case 197: {
				uint16 index = ::read<uint16, endianness::big>(iterator);
				uint8 dimensions = ::read<uint8>(iterator);
				return handler(multi_new_array{ index, dimensions });
			}
			case 198: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_null{ branch });
			}
			case 199: {
				int16 branch = ::read<int16, endianness::big>(iterator);
				return handler(if_non_null{ branch });
			}
			case 200: {
				int32 branch = ::read<int32, endianness::big>(iterator);
				return handler(go_to_w{ branch });
			}
			case 201: {
				int16 branch = ::read<int32, endianness::big>(iterator);
				return handler(jmp_sr_w{ branch });
			}

			default: return handler(unknown{ instruction_code });
		}
	}

}