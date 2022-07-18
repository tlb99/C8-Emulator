#include "chip8.hpp"
#include <iostream>

void chip8::initialize()
{
    pc = 0x200;  // Program counter starts at 0x200
    opcode = 0;      // Reset current opcode	
    I = 0;      // Reset index register
    sp = 0;      // Reset stack pointer

    memset(gfx, 0x00, 64 * 32);        // Clear display	
    memset(stack, 0x0000, 16);         // Clear stack
    memset(V, 0x00, 16);        // Clear registers V0-VF
    memset(memory, 0x00, 64 * 32);     // Clear memory

    // Load fontset
    for (int i = 0; i < 80; ++i)
        memory[i] = chip8_fontset[i];

    // Reset timers
    delay_timer = 60;
    sound_timer = 60;
}

void chip8::emulateCycle()
{
    // Fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    //std::cout << std::hex << opcode << std::endl;

    // Decode opcode
    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode & 0x000F)
        {
        case 0x0000: // 0x00E0: Clears the screen        
          // Execute opcode
            break;

        case 0x000E: // 0x00EE: Returns from subroutine          
          // Execute opcode
            break;

        default:
            printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
        }

    // Jump to address 0x0NNN
    case 0x1000:
        pc = opcode & 0x0FFF;
        break;

    // Call subroutine at NNN
    case 0x2000:
        stack[sp] = pc;
        ++sp;
        pc = opcode & 0x0FFF;
        break;

    // Skip the next instruction if VX == NN
    case 0x3000:
        if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
            pc += 4;
        else
            pc += 2;
        break;

    // Skip the next instruction if VX != NN
    case 0x4000:
        if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
            pc += 4;
        else
            pc += 2;
        break;

    // Skip the next instruction if VX == VY
    case 0x5000:
        if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
            pc += 4;
        else
            pc += 2;
        break;

    // Set VX to NN
    case 0x6000:
        V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        pc += 2;
        break;

    // Add NN to VX
    case 0x7000:
        V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
        pc += 2;
        break;

    case 0x8000:
    {
        switch (opcode & 0x000F) 
        {
            // Set VX to VY
            case 0x0000:
                V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                pc += 2;
                break;
            // Set VX to VX or VY
            case 0x0001:
                V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                pc += 2;
                break;
            // Set VX to VX and VY
            case 0x0002:
                V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                pc += 2;
                break;
            // Set VX to VX xor VY
            case 0x0003:
                V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                pc += 2;
                break;
            // Add VY to VX. VF is set to 1 when there's a carry, and to 0 when there is not
            case 0x0004:
                if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
                    V[0xF] = 1; //carry
                else
                    V[0xF] = 0;
                V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                pc += 2;
                break;
            // Subtract VY from VX. VF is set to 0 when there's a borrow, and 1 when there is not.
            case 0x0005:
                if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
                    V[0xF] = 0; //borrow
                else
                    V[0xF] = 1;
                V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                pc += 2;
                break;
            // Stores the least significant bit of VX in VF and then shifts VX to the right by 1
            case 0x0006:
                V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x0001;
                V[(opcode & 0x0F00) >> 8] >>= 1;
                pc += 2;
                break;
            // Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not
            case 0x0007:
                if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
                    V[0xF] = 0; //borrow
                else
                    V[0xF] = 1;
                V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                pc += 2;
                break;
            // Stores the most significant bit of VX in VF and then shifts VX to the left by 1
            case 0x000E:
                V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x8000;
                V[(opcode & 0x0F00) >> 8] <<= 1;
                pc += 2;
                break;
        }
    }

    // Skip the next instruction if VX != VY
    case 0x9000:
        if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
            pc += 4;
        else
            pc += 2;
        break;

    // Set I to the address NNN
    case 0xA000: 
      // Execute opcode
        I = opcode & 0x0FFF;
        pc += 2;
        break;

    // Jump to the address NNN plus V0
    case 0xB000:
        pc = (opcode & 0x0FFF) + V[0x0];
        break;

    // Set VX to NN and a random number in the range of 0-255
    case 0xC000:
        V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) & (std::rand() % 256);
        pc += 2;
        break;

    case 0xD000:
    {
        unsigned short x = V[(opcode & 0x0F00) >> 8];
        unsigned short y = V[(opcode & 0x00F0) >> 4];
        unsigned short height = opcode & 0x000F;
        unsigned short pixel;

        V[0xF] = 0;
        for (int yline = 0; yline < height; yline++)
        {
            pixel = memory[I + yline];
            for (int xline = 0; xline < 8; xline++)
            {
                if ((pixel & (0x80 >> xline)) != 0)
                {
                    if (gfx[(x + xline + ((y + yline) * 64))] == 1)
                        V[0xF] = 1;
                    gfx[x + xline + ((y + yline) * 64)] ^= 1;
                }
            }
        }

        drawFlag = true;
        pc += 2;
    }
    break;

    case 0xE000:
        switch (opcode & 0x00FF)
        {
            // EX9E: Skips the next instruction 
            // if the key stored in VX is pressed
        case 0x009E:
            if (key[V[(opcode & 0x0F00) >> 8]] != 0)
                pc += 4;
            else
                pc += 2;
            break;
        }

    case 0xF000:
        switch (opcode & 0x00FF)
        {
        case 0x0033:
            memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
            memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
            memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
            pc += 2;
            break;
        }

    default:
        printf("Unknown opcode: 0x%X\n", opcode);
    }

    // Update timers
    if (delay_timer > 0)
        --delay_timer;

    if (sound_timer > 0)
    {
        if (sound_timer == 1)
            printf("BEEP!\n");
        --sound_timer;
    }
}

void chip8::loadGame(const char* file_path)
{
    //Open file
    FILE* fp = fopen(file_path,"rb");
    //Get file size by going to end and calling ftell 
    fseek(fp, 0L, SEEK_END);
    const int bufferSize = ftell(fp);
    //Set fp back to beginning
    fseek(fp, 0L, SEEK_SET);

    //Declare dynamic array and read file
    unsigned char* buffer = new unsigned char[bufferSize];
    fread(buffer,bufferSize,1,fp);

    //Use fopen to load game into buffer
    for(int i = 0; i < bufferSize; ++i)
        memory[i + 512] = buffer[i];

    //Close file and release array memory
    fclose(fp);
    delete[] buffer;
}

unsigned char* chip8::getGfx()
{
    return this->gfx;
}