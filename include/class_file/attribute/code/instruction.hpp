#pragma once

#include "../../constant.hpp"

#include <span.hpp>

namespace class_file::attribute::code::instruction {

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
	struct d_const_0    { static constexpr uint8 code = 14; };
	struct d_const_1    { static constexpr uint8 code = 15; };

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
		constant::index index;
	};
	struct ldc_w {
		static constexpr uint8 code = 19;
		constant::index index;
	};
	struct ldc_2_w {
		static constexpr uint8 code = 20;
		constant::index index;
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
	struct d_load {
		static constexpr uint8 code = 24;
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
	struct l_a_load { static constexpr uint8 code = 47; };
	struct f_a_load { static constexpr uint8 code = 48; };
	struct d_a_load { static constexpr uint8 code = 49; };
	struct a_a_load  { static constexpr uint8 code = 50; };
	struct b_a_load  { static constexpr uint8 code = 51; };
	struct c_a_load  { static constexpr uint8 code = 52; };
	struct s_a_load  { static constexpr uint8 code = 53; };

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
	struct d_store {
		static constexpr uint8 code = 57;
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
	struct f_store_0 { static constexpr uint8 code = 67; };
	struct f_store_1 { static constexpr uint8 code = 68; };
	struct f_store_2 { static constexpr uint8 code = 69; };
	struct f_store_3 { static constexpr uint8 code = 70; };
	struct d_store_0 { static constexpr uint8 code = 71; };
	struct d_store_1 { static constexpr uint8 code = 72; };
	struct d_store_2 { static constexpr uint8 code = 73; };
	struct d_store_3 { static constexpr uint8 code = 74; };
	struct a_store_0 { static constexpr uint8 code = 75; };
	struct a_store_1 { static constexpr uint8 code = 76; };
	struct a_store_2 { static constexpr uint8 code = 77; };
	struct a_store_3 { static constexpr uint8 code = 78; };
	struct i_a_store { static constexpr uint8 code = 79; };
	struct l_a_store { static constexpr uint8 code = 80; };
	struct f_a_store { static constexpr uint8 code = 81; };
	struct d_a_store { static constexpr uint8 code = 82; };
	struct a_a_store { static constexpr uint8 code = 83; };
	struct b_a_store { static constexpr uint8 code = 84; };
	struct c_a_store { static constexpr uint8 code = 85; };
	struct s_a_store { static constexpr uint8 code = 86; };

	struct pop    {  static constexpr uint8 code = 87; };
	struct pop_2  {  static constexpr uint8 code = 88; };
	struct dup    {  static constexpr uint8 code = 89; };
	struct dup_x1 {  static constexpr uint8 code = 90; };
	struct dup_x2 {  static constexpr uint8 code = 91; };
	struct dup_2  { static constexpr uint8 code = 92; };
	struct dup_2_x1  { static constexpr uint8 code = 93; };
	struct dup_2_x2  { static constexpr uint8 code = 94; };
	struct swap  { static constexpr uint8 code = 95; };

	struct i_add {  static constexpr uint8 code = 96; };
	struct l_add {  static constexpr uint8 code = 97; };
	struct f_add {  static constexpr uint8 code = 98; };
	struct d_add {  static constexpr uint8 code = 99; };
	struct i_sub {  static constexpr uint8 code = 100; };
	struct l_sub {  static constexpr uint8 code = 101; };
	struct f_sub {  static constexpr uint8 code = 102; };
	struct d_sub {  static constexpr uint8 code = 103; };
	struct i_mul {  static constexpr uint8 code = 104; };
	struct l_mul {  static constexpr uint8 code = 105; };
	struct f_mul {  static constexpr uint8 code = 106; };
	struct d_mul {  static constexpr uint8 code = 107; };
	struct i_div {  static constexpr uint8 code = 108; };
	struct l_div {  static constexpr uint8 code = 109; };
	struct f_div {  static constexpr uint8 code = 110; };
	struct d_div {  static constexpr uint8 code = 111; };
	struct i_rem {  static constexpr uint8 code = 112; };
	struct l_rem {  static constexpr uint8 code = 113; };
	struct f_rem {  static constexpr uint8 code = 114; };
	struct d_rem {  static constexpr uint8 code = 115; };
	struct i_neg {  static constexpr uint8 code = 116; };
	struct l_neg {  static constexpr uint8 code = 117; };
	struct f_neg {  static constexpr uint8 code = 118; };
	struct d_neg {  static constexpr uint8 code = 119; };
	struct i_sh_l { static constexpr uint8 code = 120; };
	struct l_sh_l { static constexpr uint8 code = 121; };
	struct i_sh_r { static constexpr uint8 code = 122; };
	struct l_sh_r { static constexpr uint8 code = 123; };
	struct i_u_sh_r { static constexpr uint8 code = 124; };
	struct l_u_sh_r { static constexpr uint8 code = 125; };
	struct i_and { static constexpr uint8 code = 126; };
	struct l_and { static constexpr uint8 code = 127; };
	struct i_or {  static constexpr uint8 code = 128; };
	struct l_or {  static constexpr uint8 code = 129; };
	struct i_xor { static constexpr uint8 code = 130; };
	struct l_xor { static constexpr uint8 code = 131; };
	struct i_inc {
		static constexpr uint8 code = 132;
		uint8 index;
		int8 value;
	};
	struct i_to_l { static constexpr uint8 code = 133; };
	struct i_to_f { static constexpr uint8 code = 134; };
	struct i_to_d { static constexpr uint8 code = 135; };
	struct l_to_i { static constexpr uint8 code = 136; };
	struct l_to_f { static constexpr uint8 code = 137; };
	struct l_to_d { static constexpr uint8 code = 138; };
	struct f_to_i { static constexpr uint8 code = 139; };
	struct f_to_l { static constexpr uint8 code = 140; };
	struct f_to_d { static constexpr uint8 code = 141; };
	struct d_to_i { static constexpr uint8 code = 142; };
	struct d_to_l { static constexpr uint8 code = 143; };
	struct d_to_f { static constexpr uint8 code = 144; };
	struct i_to_b { static constexpr uint8 code = 145; };
	struct i_to_c { static constexpr uint8 code = 146; };
	struct i_to_s { static constexpr uint8 code = 147; };

	struct l_cmp { static constexpr uint8 code = 148; };

	struct f_cmp_l { static constexpr uint8 code = 149; };
	struct f_cmp_g { static constexpr uint8 code = 150; };
	struct d_cmp_l { static constexpr uint8 code = 151; };
	struct d_cmp_g { static constexpr uint8 code = 152; };

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
	struct jmp_sr {
		static constexpr uint8 code = 168;
		int16 branch;
	};
	struct return_sr {
		static constexpr uint8 code = 169;
		int8 index;
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
	struct f_return { static constexpr uint8 code = 174; };
	struct d_return { static constexpr uint8 code = 175; };
	struct a_return { static constexpr uint8 code = 176; };
	struct _return { static constexpr uint8 code = 177; };

	struct get_static {
		static constexpr uint8 code = 178;
		constant::field_ref_index index;
	};
	struct put_static {
		static constexpr uint8 code = 179;
		constant::field_ref_index index;
	};
	struct get_field {
		static constexpr uint8 code = 180;
		constant::field_ref_index index;
	};
	struct put_field {
		static constexpr uint8 code = 181;
		constant::field_ref_index index;
	};
	struct invoke_virtual {
		static constexpr uint8 code = 182;
		constant::method_ref_index index;
	};
	struct invoke_special {
		static constexpr uint8 code = 183;
		constant::method_ref_index index;
	};
	struct invoke_static {
		static constexpr uint8 code = 184;
		constant::method_ref_index index;
	};
	struct invoke_interface {
		static constexpr uint8 code = 185;
		constant::interface_method_ref_index index;
		uint8 count;
	};
	struct invoke_dynamic {
		static constexpr uint8 code = 186;
		constant::invoke_dynamic_index index;
	};

	struct _new {
		static constexpr uint8 code = 187;
		constant::class_index index;
	};
	enum class new_array_type : uint8 {
		_bool = 4, _char  = 5, _float = 6, _double = 7,
		_byte = 8, _short = 9, _int   = 10, _long  = 11
	};
	struct new_array {
		static constexpr uint8 code = 188;
		new_array_type type;
	};
	struct a_new_array {
		static constexpr uint8 code = 189;
		constant::class_index index;
	};
	struct array_length { static constexpr uint8 code = 190; };

	struct a_throw { static constexpr uint8 code = 191; };

	struct check_cast {
		static constexpr uint8 code = 192;
		constant::class_index index;
	};
	struct instance_of {
		static constexpr uint8 code = 193;
		constant::class_index index;
	};
	struct monitor_enter { static constexpr uint8 code = 194; };
	struct monitor_exit { static constexpr uint8 code = 195; };

	struct wide_format_1 {
		static constexpr uint8 code = 196;
		uint8 other_code;
		uint16 index;
	};

	struct wide_format_2 {
		static constexpr uint8 code = 196;
		static constexpr uint8 other_code = i_inc::code;
		uint16 index;
		uint16 value;
	};

	struct multi_new_array {
		static constexpr uint8 code = 197;
		uint16 index;
		uint8 dimensions;
	};

	struct if_null {
		static constexpr uint8 code = 198;
		uint16 branch;
	};
	struct if_non_null {
		static constexpr uint8 code = 199;
		uint16 branch;
	};

	struct goto_w {
		static constexpr uint8 code = 200;
		uint32 branch;
	};
	struct jmp_sr_w {
		static constexpr uint8 code = 200;
		uint32 branch;
	};

} // instruction