#include "cpu.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint8_t *memory;
uint8_t *registers;
uint16_t register_i = 0;
uint16_t program_counter = 0x200;
uint8_t stack_pointer = 0;
uint16_t *stack;
uint8_t delay_timer = 0;
uint8_t sound_timer = 0;
uint16_t opcode = 0;
void (*instrution_to_execute)();
bool *pixels;

void init_cpu() {
    memory = malloc(sizeof(uint8_t) * 0x1000); // 4096 bytes
    memset(memory, 0, sizeof(uint8_t) * 0x1000);
    registers = malloc(sizeof(uint8_t) * 16);
    memset(registers, 0, sizeof(uint8_t) * 16);
    stack = malloc(sizeof(uint16_t) * 16); // 8 o 16 bits??
    memset(stack, 0, sizeof(uint16_t) * 16);
    pixels = malloc(sizeof(bool) * 64 * 32);
    memset(pixels, 0, sizeof(bool) * 64 * 32);

    load_hex_sprites();

    // TEST
    pixels[0 + 0 * 64] = true;
    pixels[0 + 1 * 64] = true;
}

void load_hex_sprites() {
    const uint8_t hex_sprites[16][5] = {
        {0xF0, 0x90, 0x90, 0x90, 0xF0},
        {0x20, 0x60, 0x20, 0x20, 0x70},
        {0xF0, 0x10, 0xF0, 0x80, 0xF0},
        {0xF0, 0x10, 0xF0, 0x10, 0xF0},
        {0x90, 0x90, 0xF0, 0x10, 0x10},
        {0xF0, 0x80, 0xF0, 0x10, 0xF0},
        {0xF0, 0x80, 0xF0, 0x90, 0xF0},
        {0xF0, 0x10, 0x20, 0x40, 0x40},
        {0xF0, 0x90, 0xF0, 0x90, 0xF0},
        {0xF0, 0x90, 0xF0, 0x10, 0xF0},
        {0xF0, 0x90, 0xF0, 0x90, 0x90},
        {0xE0, 0x90, 0xE0, 0x90, 0xE0},
        {0xF0, 0x80, 0x80, 0x80, 0xF0},
        {0xE0, 0x90, 0x90, 0x90, 0xE0},
        {0xF0, 0x80, 0xF0, 0x80, 0xF0},
        {0xF0, 0x80, 0xF0, 0x80, 0x80}
    };

    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 5; y++) {
            memset(&memory[x*5 + y], hex_sprites[x][y], sizeof(uint8_t));
        }
    }

    //memset(memory, 0x);
}

void cpu_fetch() {
    memcpy(&opcode, &memory[program_counter], sizeof(opcode));
    opcode = opcode << 8 | opcode >> 8;

    printf("(opcode: %04X\tprogram_counter: %d)\n", opcode, program_counter);

    //printf("memory: %p\n", &memory);
    //printf("memory[1]: %hu\n", memory[1]);
    //printf("opcode: %8x\n", opcode);
}

void cpu_decode() {
    const uint8_t group = (opcode >> 12) & 0x0F;
    uint8_t subgroup;

    switch (group) {
        case 0x0:
            if (opcode == 0x00E0) { // (00E0) CLS
                instrution_to_execute = &clear;
            } else if (opcode == 0x00EE) { // (00EE) RET
                instrution_to_execute = &ret;
            } else {
                instrution_to_execute = &instruction_not_defined;
            }
            break;
        case 0x1: // (1nnn) JP addr
            instrution_to_execute = &jump;
            break;
        case 0x2: // (2nnn) CALL addr
            instrution_to_execute = &call;
            break;
        case 0x3: // (3xkk) SE Vx, byte
            instrution_to_execute = &skip_if_equals;
            break;
        case 0x4: // (4xkk) SNE Vx, byte
            instrution_to_execute = &skip_if_not_equals;
            break;
        case 0x5: // (5xy0) SE Vx, Vy
            instrution_to_execute = &skip_if_registers_equals;
            break;
        case 0x6: // (6xkk) LD Vx, byte
            instrution_to_execute = &load_byte_in_register_x;
            break;
        case 0x7: // (7xkk) ADD Vx, byte
            instrution_to_execute = &add_byte_to_register_x;
            break;
        case 0x8:
            subgroup = opcode & 0x000F;
            if (subgroup == 0x0) { // (8xy0) LD Vx, Vy
                instrution_to_execute = &load_register_y_in_register_x;
            } else if (subgroup == 0x1) { // (8xy1) OR Vx, Vy
                instrution_to_execute = &or_register_x_register_y;
            } else if (subgroup == 0x2) { // (8xy2) AND Vx, Vy
                instrution_to_execute = &and_register_x_register_y;
            } else if (subgroup == 0x3) { // (8xy3) XOR Vx, Vy
                instrution_to_execute = &xor_register_x_register_y;
            } else if (subgroup == 0x4) { // (8xy4) ADD Vx, Vy
                instrution_to_execute = &add_register_y_to_register_x;
            } else if (subgroup == 0x5) { // (8xy5) SUB Vx, Vy
                instrution_to_execute = &sub_register_y_to_register_x;
            } else if (subgroup == 0x6) { // (8xy6) SHR Vx {, Vy}
                instrution_to_execute = &shr;
            } else if (subgroup == 0x7) { // (8xy7) SUBN Vx, Vy
                instrution_to_execute = &sub_register_x_in_register_y;
            } else if (subgroup == 0xE) { // (8xyE) SHL Vx {, Vy}
                instrution_to_execute = &shl;
            } else {
                instrution_to_execute = &instruction_not_defined;
            }
            break;
        case 0x9: // (9xy0) SNE Vx, Vy
            instrution_to_execute = &skip_if_registers_not_equals;
            break;
        case 0xA: // (Annn) LD I, addr
            instrution_to_execute = &load_address_in_register_i;
            break;
        // TODO 0xB
        case 0xD: // (Dxyn) DRW Vx, Vy, nibble
            instrution_to_execute = &display_bytes;
            break;
        case 0xF:
            subgroup = opcode & 0x00FF;
            if (subgroup == 0x07) { // (8xy0) LD Vx, Vy
                instrution_to_execute = &instruction_not_defined;
            } else if (subgroup == 0x0A) { // (8xy1) OR Vx, Vy
                instrution_to_execute = &instruction_not_defined;
            } else if (subgroup == 0x15) { // (8xy2) AND Vx, Vy
                instrution_to_execute = &instruction_not_defined;
            } else if (subgroup == 0x18) { // (8xy3) XOR Vx, Vy
                instrution_to_execute = &instruction_not_defined;
            } else if (subgroup == 0x1E) { // (8xy4) ADD Vx, Vy
                instrution_to_execute = &add_register_x_to_register_i;
            } else if (subgroup == 0x29) { // (8xy5) SUB Vx, Vy
                instrution_to_execute = &instruction_not_defined;
            } else if (subgroup == 0x33) { // (8xy6) SHR Vx {, Vy}
                instrution_to_execute = &load_bcd_from_register;
            } else if (subgroup == 0x55) { // (Fx55) LD [I], Vx
                instrution_to_execute = &load_registers_to_memory;
            } else if (subgroup == 0x65) { // (Fx65) LD Vx, [I]
                instrution_to_execute = &load_registers_from_memory;
            } else {
                instrution_to_execute = &instruction_not_defined;
            }
            break;
        default:
            instrution_to_execute = &instruction_not_defined;
    }
}

void cpu_execute() {
    program_counter = program_counter + 2;
    instrution_to_execute();
}

void load_program() {
    int count = 0;
    FILE *file = fopen("/home/gabi/Downloads/1-chip8-logo.ch8", "rb");
    //FILE *file = fopen("/home/gabi/Downloads/2-ibm-logo.ch8", "rb");
    //FILE *file = fopen("/home/gabi/Downloads/3-corax+.ch8", "rb");
    //FILE *file = fopen("/home/gabi/Downloads/4-flags.ch8", "rb");
    if (file == NULL) {
        printf("The file is not opened. The program will "
               "now exit.");
        exit(0);
    }

    while (!feof(file)) {
        const uint8_t byte_to_save = fgetc(file);
        memset(&memory[program_counter + count], byte_to_save, sizeof(uint8_t));
        printf("memory address %d with data: %02X\n", program_counter + count, byte_to_save);
        count++;
    }
    fclose(file);
}

bool *get_pixels() {
    return pixels;
}

void close_cpu() {
    free(memory);
    free(registers);
    free(stack);
    free(pixels);
}

// INSTRUCTIONS

void clear() {
    printf("clear display\n");
    memset(pixels, 0, sizeof(bool) * 64 * 32);
}

void ret() {
    stack_pointer--;
    uint16_t address = stack[stack_pointer];
    program_counter = address;
    printf("exit subrutine, return to address %d\n", address);
}

void jump() {
    uint16_t address = opcode & 0x0FFF;
    printf("jump to address %d\n", address);
    program_counter = address;
}

void call() {
    uint16_t address = opcode & 0x0FFF;
    printf("call subrutine at address %d\n", address);
    stack[stack_pointer] = program_counter;
    //printf("stack[%d] = %d\n", stack_pointer, stack[stack_pointer]);
    stack_pointer++;
    program_counter = address;
}

void skip_if_equals() {
    uint8_t register_number = (opcode >> 8) & 0x0F;
    uint8_t byte = opcode & 0x00FF;
    printf("skip instruction if byte %02X is equals to register %d\n", byte, register_number);
    if (registers[register_number] == byte) {
        program_counter += 2;
    }
}

void skip_if_not_equals() {
    uint8_t register_number = (opcode >> 8) & 0x0F;
    uint8_t byte = opcode & 0x00FF;
    printf("skip instruction if byte %02X is not equals to register %d\n", byte, register_number);
    if (registers[register_number] != byte) {
        program_counter += 2;
    }
}

void skip_if_registers_equals() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    uint8_t register_number_y = (opcode >> 4) & 0x0F;
    printf("skip instruction if register %d is equals to register %d\n", register_number_x, register_number_y);
    if (registers[register_number_x] == registers[register_number_y]) {
        program_counter += 2;
    }
}

void load_byte_in_register_x() {
    uint8_t register_number = (opcode >> 8) & 0x0F;
    uint8_t byte = opcode & 0x00FF;
    printf("load byte %02X to register %d\n", byte, register_number);
    registers[register_number] = byte;
}

void add_byte_to_register_x() {
    uint8_t register_number = (opcode >> 8) & 0x0F;
    uint8_t byte = opcode & 0x00FF;
    printf("add byte %02X to register %d\n", byte, register_number);
    registers[register_number] += byte;
}

void load_register_y_in_register_x() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    uint8_t register_number_y = (opcode >> 4) & 0x0F;
    printf("load register y %d to register x %d\n", register_number_y, register_number_x);
    registers[register_number_x] = registers[register_number_y];
}

void or_register_x_register_y() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    uint8_t register_number_y = (opcode >> 4) & 0x0F;
    printf("OR between register x %d and register y %d\n", register_number_x, register_number_y);
    registers[register_number_x] |= registers[register_number_y];
}

void and_register_x_register_y() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    uint8_t register_number_y = (opcode >> 4) & 0x0F;
    printf("AND between register x %d and register y %d\n", register_number_x, register_number_y);
    registers[register_number_x] &= registers[register_number_y];
}

void xor_register_x_register_y() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    uint8_t register_number_y = (opcode >> 4) & 0x0F;
    printf("XOR between register x %d and register y %d\n", register_number_x, register_number_y);
    registers[register_number_x] ^= registers[register_number_y];
}

void add_register_y_to_register_x() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    uint8_t register_number_y = (opcode >> 4) & 0x0F;
    printf("add register_y %d to register_x %d\n", register_number_y, register_number_x);
    uint16_t result = registers[register_number_x] + registers[register_number_y];
    //printf("registers[%d] = %d\n", register_number_x, registers[register_number_x]);
    //printf("registers[%d] = %d\n", register_number_y, registers[register_number_y]);
    //printf("result = %d\n", result);
    registers[register_number_x] = result;
    //printf("final: registers[%d] = %d\n", register_number_x, registers[register_number_x]);
    registers[15] = (result >> 8) > 0;
    //printf("registers[F] = %d\n", registers[15]);
}

void sub_register_y_to_register_x() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    uint8_t register_number_y = (opcode >> 4) & 0x0F;
    printf("sub register_y %d to register_x %d\n", register_number_y, register_number_x);
    bool result = registers[register_number_x] >= registers[register_number_y];
    //printf("registers[%d] = %d\n", register_number_x, registers[register_number_x]);
    //printf("registers[%d] = %d\n", register_number_y, registers[register_number_y]);
    //printf("result = %d\n", result);
    registers[register_number_x] -= registers[register_number_y];
    //printf("final: registers[%d] = %d\n", register_number_x, registers[register_number_x]);
    registers[15] = result;
    //printf("registers[F] = %d\n", registers[15]);
}

void shr() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    uint8_t register_number_y = (opcode >> 4) & 0x0F;
    printf("set register x %d (register_y %d) shr 1\n", register_number_x, register_number_y);
    bool result = registers[register_number_x] & 0x01;
    registers[register_number_x] >>= 1;
    registers[15] = result;
}

void sub_register_x_in_register_y() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    uint8_t register_number_y = (opcode >> 4) & 0x0F;
    printf("sub register_x %d in register_y %d\n", register_number_y, register_number_x);
    bool result = registers[register_number_x] <= registers[register_number_y];
    registers[register_number_x] = registers[register_number_y] - registers[register_number_x];
    registers[15] = result;
}

void shl() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    uint8_t register_number_y = (opcode >> 4) & 0x0F;
    printf("set register x %d (register_y %d) shl 1\n", register_number_x, register_number_y);
    bool result = (registers[register_number_x] >> 7) & 0x01;
    registers[register_number_x] <<= 1;
    registers[15] = result;
}

void skip_if_registers_not_equals() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    uint8_t register_number_y = (opcode >> 4) & 0x0F;
    printf("skip instruction if register %d is not equals to register %d\n", register_number_x, register_number_y);
    if (registers[register_number_x] != registers[register_number_y]) {
        program_counter += 2;
    }
}

void load_address_in_register_i() {
    uint16_t address = opcode & 0x0FFF;
    printf("load address %d to register I\n", address);
    register_i = address;
}

void display_bytes() { // TODO revisar lo de que un sprite se imprima por fuera de las coordenadas (en principio ya esta hecho)
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    uint8_t register_number_y = (opcode >> 4) & 0x00F;
    uint8_t coordinate_x = registers[register_number_x];
    uint8_t coordinate_y = registers[register_number_y];
    uint8_t n_bytes = opcode & 0x000F;
    registers[15] = 0;
    printf("display %d bytes in address %d at position (%d,%d)\n", n_bytes, register_i, coordinate_x, coordinate_y);
    for (uint16_t byte_readed = 0; byte_readed < n_bytes; byte_readed++) {

        uint8_t draw_at_y = coordinate_y + byte_readed < 32 ? coordinate_y + byte_readed : coordinate_y + byte_readed - 32;

        for (int x = 0; x < 8; x++) {
            uint8_t draw_at_x = coordinate_x + x < 64 ? coordinate_x + x : coordinate_x + x - 64;

            bool bit_to_draw = (memory[register_i + byte_readed] >> (7 - x)) & 0x01;
            //bool pixel_old_value =pixels[draw_at_x + draw_at_y * 64];

            pixels[draw_at_x + draw_at_y * 64] = pixels[draw_at_x + draw_at_y * 64] ^ bit_to_draw;
            if (!pixels[draw_at_x + draw_at_y * 64]) {
                registers[15] = 1;
            }
        }
    }
}

void add_register_x_to_register_i() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    printf("add register_x %d to register_i\n", register_number_x);
    register_i += registers[register_number_x];
}

void load_bcd_from_register() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    printf("load BCD from register_x %d to memory location in register_i %d)\n", register_number_x,
        register_i);
    memory[register_i] = (registers[register_number_x] / 100) % 100;
    memory[register_i + 1] = (registers[register_number_x] / 10) % 10;
    memory[register_i + 2] = registers[register_number_x] % 10;
    //printf("registers[%d] = %d\n", register_number_x, registers[register_number_x]);
    //printf("memory[%d] = %d\n", register_i, memory[register_i]);
    //printf("memory[%d] = %d\n", register_i + 1, memory[register_i + 1]);
    //printf("memory[%d] = %d\n", register_i + 2, memory[register_i + 2]);
}

void load_registers_to_memory() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    printf("load to memory location in register I %d from registers (at register_x %d)\n", register_i,
        register_number_x);
    for (int i = 0; i <= register_number_x; i++) {
        memory[register_i + i] = registers[i];
    }
}

void load_registers_from_memory() {
    uint8_t register_number_x = (opcode >> 8) & 0x0F;
    printf("load to registers (at register_x %d) from memory location in register I %d\n", register_number_x,
        register_i);
    for (int i = 0; i <= register_number_x; i++) {
        registers[i] = memory[register_i + i];
    }
}

void instruction_not_defined() {
    printf("instruction not found\n");
}
