#pragma once

#include <core/span.hpp>

namespace class_file::code::instruction {

	struct nop { static constexpr uint8 code = 0; };

	struct a_const_null { static constexpr uint8 code = 1; };
	struct i_const_m1   { static constexpr uint8 code = 2; };
	struct i_const_0    { static constexpr uint8 code = 3; };
	struct i_const_1    { static constexpr uint8 code = 4; };
	struct i_const_2    { static constexpr uint8 code = 5; };
	struct i_const_3    { static constexpr uint8 code = 6; };
	struct i_const_4    { static constexpr uint8 code = 7; };
	struct i_const_5    { static constexpr uint8 code = 8; };
	struct l_const_0    { static constexpr uint8 code = 9; };
	struct l_const_1    { static constexpr uint8 code = 10; };
	struct f_const_0    { static constexpr uint8 code = 11; };
	struct f_const_1    { static constexpr uint8 code = 12; };
	struct f_const_2    { static constexpr uint8 code = 13; };

	struct bi_push {
		static constexpr uint8 code = 16;
		int8 value;
	};
	struct si_push {
		static constexpr uint8 code = 17;
		int16 value;
	};

	struct ldc {
		static constexpr uint8 code = 18;
		uint8 index;
	};
	struct ldc_w {
		static constexpr uint8 code = 19;
		uint16 index;
	};
	struct ldc_2_w {
		static constexpr uint8 code = 20;
		uint16 index;
	};

	struct i_load {
		static constexpr uint8 code = 21;
		uint8 index;
	};
	struct l_load {
		static constexpr uint8 code = 22;
		uint8 index;
	};
	struct f_load {
		static constexpr uint8 code = 23;
		uint8 index;
	};
	struct a_load {
		static constexpr uint8 code = 25;
		uint8 index;
	};
	struct i_load_0 { static constexpr uint8 code = 26; };
	struct i_load_1 { static constexpr uint8 code = 27; };
	struct i_load_2 { static constexpr uint8 code = 28; };
	struct i_load_3 { static constexpr uint8 code = 29; };
	struct l_load_0 { static constexpr uint8 code = 30; };
	struct l_load_1 { static constexpr uint8 code = 31; };
	struct l_load_2 { static constexpr uint8 code = 32; };
	struct l_load_3 { static constexpr uint8 code = 33; };
	struct f_load_0 { static constexpr uint8 code = 34; };
	struct f_load_1 { static constexpr uint8 code = 35; };
	struct f_load_2 { static constexpr uint8 code = 36; };
	struct f_load_3 { static constexpr uint8 code = 37; };
	struct d_load_0 { static constexpr uint8 code = 38; };
	struct d_load_1 { static constexpr uint8 code = 39; };
	struct d_load_2 { static constexpr uint8 code = 40; };
	struct d_load_3 { static constexpr uint8 code = 41; };
	struct a_load_0 { static constexpr uint8 code = 42; };
	struct a_load_1 { static constexpr uint8 code = 43; };
	struct a_load_2 { static constexpr uint8 code = 44; };
	struct a_load_3 { static constexpr uint8 code = 45; };
	struct i_a_load { static constexpr uint8 code = 46; };
	struct a_a_load  { static constexpr uint8 code = 50; };
	struct b_a_load  { static constexpr uint8 code = 51; };
	struct c_a_load  { static constexpr uint8 code = 52; };

	struct i_store {
		static constexpr uint8 code = 54;
		uint8 index;
	};
	struct l_store {
		static constexpr uint8 code = 55;
		uint8 index;
	};
	struct f_store {
		static constexpr uint8 code = 56;
		uint8 index;
	};
	struct a_store {
		static constexpr uint8 code = 58;
		uint8 index;
	};
	struct i_store_0 { static constexpr uint8 code = 59; };
	struct i_store_1 { static constexpr uint8 code = 60; };
	struct i_store_2 { static constexpr uint8 code = 61; };
	struct i_store_3 { static constexpr uint8 code = 62; };
	struct l_store_0 { static constexpr uint8 code = 63; };
	struct l_store_1 { static constexpr uint8 code = 64; };
	struct l_store_2 { static constexpr uint8 code = 65; };
	struct l_store_3 { static constexpr uint8 code = 66; };
	struct a_store_0 { static constexpr uint8 code = 75; };
	struct a_store_1 { static constexpr uint8 code = 76; };
	struct a_store_2 { static constexpr uint8 code = 77; };
	struct a_store_3 { static constexpr uint8 code = 78; };
	struct i_a_store { static constexpr uint8 code = 79; };
	struct a_a_store { static constexpr uint8 code = 83; };
	struct b_a_store { static constexpr uint8 code = 84; };
	struct c_a_store { static constexpr uint8 code = 85; };

	struct pop    {  static constexpr uint8 code = 87; };
	struct dup    {  static constexpr uint8 code = 89; };
	struct dup_x1 {  static constexpr uint8 code = 90; };
	struct dup_2  { static constexpr uint8 code = 92; };

	struct i_add {  static constexpr uint8 code = 96; };
	struct l_add {  static constexpr uint8 code = 97; };
	struct f_add {  static constexpr uint8 code = 98; };
	struct i_sub {  static constexpr uint8 code = 100; };
	struct l_sub {  static constexpr uint8 code = 101; };
	struct i_mul {  static constexpr uint8 code = 104; };
	struct l_mul {  static constexpr uint8 code = 105; };
	struct f_mul {  static constexpr uint8 code = 106; };
	struct d_mul {  static constexpr uint8 code = 107; };
	struct i_div {  static constexpr uint8 code = 108; };
	struct f_div {  static constexpr uint8 code = 110; };
	struct i_rem {  static constexpr uint8 code = 112; };
	struct i_neg {  static constexpr uint8 code = 116; };
	struct i_sh_l { static constexpr uint8 code = 120; };
	struct l_sh_l { static constexpr uint8 code = 121; };
	struct i_sh_r { static constexpr uint8 code = 122; };
	struct l_sh_r { static constexpr uint8 code = 123; };
	struct i_ush_r { static constexpr uint8 code = 124; };
	struct l_ush_r { static constexpr uint8 code = 125; };
	struct i_and { static constexpr uint8 code = 126; };
	struct l_and { static constexpr uint8 code = 127; };
	struct i_or {  static constexpr uint8 code = 128; };
	struct l_or {  static constexpr uint8 code = 129; };
	struct i_xor { static constexpr uint8 code = 130; };
	struct i_inc {
		static constexpr uint8 code = 132;
		uint8 index;
		int8 value;
	};
	struct i_to_l { static constexpr uint8 code = 133; };
	struct i_to_f { static constexpr uint8 code = 134; };
	struct i_to_d { static constexpr uint8 code = 135; };
	struct l_to_i { static constexpr uint8 code = 136; };
	struct f_to_i { static constexpr uint8 code = 139; };
	struct f_to_d { static constexpr uint8 code = 141; };
	struct d_to_i { static constexpr uint8 code = 142; };
	struct i_to_b { static constexpr uint8 code = 145; };
	struct i_to_c { static constexpr uint8 code = 146; };

	struct l_cmp { static constexpr uint8 code = 148; };

	struct f_cmp_l { static constexpr uint8 code = 149; };
	struct f_cmp_g { static constexpr uint8 code = 150; };

	struct if_eq {
		static constexpr uint8 code = 153;
		int16 branch;
	};
	struct if_ne {
		static constexpr uint8 code = 154;
		int16 branch;
	};
	struct if_lt {
		static constexpr uint8 code = 155;
		int16 branch;
	};
	struct if_ge {
		static constexpr uint8 code = 156;
		int16 branch;
	};
	struct if_gt {
		static constexpr uint8 code = 157;
		int16 branch;
	};
	struct if_le {
		static constexpr uint8 code = 158;
		int16 branch;
	};
	struct if_i_cmp_eq {
		static constexpr uint8 code = 159;
		int16 branch;
	};
	struct if_i_cmp_ne {
		static constexpr uint8 code = 160;
		int16 branch;
	};
	struct if_i_cmp_lt {
		static constexpr uint8 code = 161;
		int16 branch;
	};
	struct if_i_cmp_ge {
		static constexpr uint8 code = 162;
		int16 branch;
	};
	struct if_i_cmp_gt {
		static constexpr uint8 code = 163;
		int16 branch;
	};
	struct if_i_cmp_le {
		static constexpr uint8 code = 164;
		int16 branch;
	};
	struct if_a_cmp_eq {
		static constexpr uint8 code = 165;
		int16 branch;
	};
	struct if_a_cmp_ne {
		static constexpr uint8 code = 166;
		int16 branch;
	};
	struct go_to {
		static constexpr uint8 code = 167;
		int16 branch;
	};

	struct table_switch {
		static constexpr uint8 code = 170;
		int32 _default;
		int32 low;
		int32 hight;
		span<int32, uint32> offsets;
	};
	struct match_offset{ int32 match; int32 offset; };
	struct lookup_switch {
		static constexpr uint8 code = 171;
		int32 _default;
		span<match_offset, uint32> pairs;
	};

	struct i_return { static constexpr uint8 code = 172; };
	struct l_return { static constexpr uint8 code = 173; };
	struct d_return { static constexpr uint8 code = 175; };
	struct a_return { static constexpr uint8 code = 176; };
	struct _return { static constexpr uint8 code = 177; };

	struct get_static {
		static constexpr uint8 code = 178;
		uint16 index;
	};
	struct put_static {
		static constexpr uint8 code = 179;
		uint16 index;
	};
	struct get_field {
		static constexpr uint8 code = 180;
		uint16 index;
	};
	struct put_field {
		static constexpr uint8 code = 181;
		uint16 index;
	};
	struct invoke_virtual {
		static constexpr uint8 code = 182;
		uint16 index;
	};
	struct invoke_special {
		static constexpr uint8 code = 183;
		uint16 index;
	};
	struct invoke_static {
		static constexpr uint8 code = 184;
		uint16 index;
	};
	struct invoke_interface {
		static constexpr uint8 code = 185;
		uint16 index; uint8 count;
	};
	struct invoke_dynamic {
		static constexpr uint8 code = 186;
		uint16 index;
	};

	struct _new {
		static constexpr uint8 code = 187;
		uint16 index;
	};
	struct new_array {
		static constexpr uint8 code = 188;
		uint8 type;
	};
	struct a_new_array {
		static constexpr uint8 code = 189;
		uint16 index;
	};
	struct array_length { static constexpr uint8 code = 190; };

	struct a_throw { static constexpr uint8 code = 191; };

	struct check_cast {
		static constexpr uint8 code = 192;
		uint16 index;
	};
	struct instance_of {
		static constexpr uint8 code = 193;
		uint16 index;
	};
	struct monitor_enter { static constexpr uint8 code = 194; };
	struct monitor_exit { static constexpr uint8 code = 195; };

	struct if_null {
		static constexpr uint8 code = 198;
		uint16 branch;
	};
	struct if_non_null {
		static constexpr uint8 code = 199;
		uint16 branch;
	};

}