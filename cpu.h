#ifndef CPU_H
#define CPU_H
#include <stdbool.h>

void init_cpu();
void load_hex_sprites();
void cpu_fetch();
void cpu_decode();
void cpu_execute();
void load_program();
bool *get_pixels();
void close_cpu();

void clear();
void ret();
void jump();
void call();
void skip_if_equals();
void skip_if_not_equals();
void skip_if_registers_equals();
void load_byte_in_register_x();
void add_byte_to_register_x();
void load_register_y_in_register_x();
void or_register_x_register_y();
void and_register_x_register_y();
void xor_register_x_register_y();
void add_register_y_to_register_x();
void sub_register_y_to_register_x();
void shr();
void sub_register_x_in_register_y();
void shl();
void skip_if_registers_not_equals();
void load_address_in_register_i();
void jump_plus_register();
void display_bytes();
void add_register_x_to_register_i();
void load_bcd_from_register();
void load_registers_to_memory();
void load_registers_from_memory();

void instruction_not_defined();

#endif
