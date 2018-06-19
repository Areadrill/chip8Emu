#include "Chip8.hpp"

Chip8::Chip8(char *filename){
    this->rom = this->loadROM(filename);
    srand (time(NULL));    
}

uint16_t Chip8::toShort(uint8_t msb, uint8_t lsb){
    return msb << 8 | lsb;
}

std::vector<uint16_t> Chip8::loadROM(char *filename){
    std::ifstream rom(filename, std::ios::binary);
    
    if(!rom){
        std::cerr << "Error: No such file found" << std::endl;
    }

    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(rom)),(std::istreambuf_iterator<char>()));
    std::vector<uint16_t> opcodes;

    for(std::vector<uint8_t>::iterator it = bytes.begin() ; it != bytes.end(); ++it){
        opcodes.push_back(toShort(*(1+it), *it));
        ++it;
    }

    return opcodes;                        
}


void Chip8::handleOpcode(uint16_t opcode){
    //special opcodes
    if(opcode == 0x00E0){
        clearScreen();
    }
    if(opcode == 0x00EE){
        ret();
    }

}

void Chip8::clearScreen(){
    for(int i = 0; i < 32; i++){
        for(int j = 0; j < 64; j++){
            this->screen[i][j] = false;
        }
    }
}

void Chip8::ret(){
    this->pc = this->addrStack->top();
    this->addrStack->pop();
}

void Chip8::goTo(uint16_t value){
    this->pc = value & 0x0FFF + ((value & 0xF000) >> 12 == 1)? 0 : this->registers[0];
}

void Chip8::call(uint16_t value){
    this->addrStack->push(this->pc);
    this->pc = value & 0x0FFF;
}

void Chip8::skipIf(uint16_t value){
    uint8_t instruction = (value & 0xF000) >> 12;

    if(this->registers[(value & 0x0F00) >> 8] == (value & 0x00FF)){
        if(instruction == 3){
            this->pc++;
        }
    }
    else if(instruction == 4){
        this->pc++;
    }    
}

void Chip8::skipIfReg(uint16_t value){
    uint8_t instruction = (value & 0xF000) >> 12;

    if(this->registers[(value & 0x0F00) >> 8] == this->registers[(value & 0x00F0) >> 4]){
        if(instruction == 5){
            this->pc++;
        }
    }
    else if(instruction == 9){
        this->pc++;
    }
}

void Chip8::skipIfKey(uint16_t value){
    uint8_t instruction = (value & 0x00FF);

    if(this->keyboard[this->registers[(value & 0x0F00) >> 8]]){
        if(instruction == 0x009E){
            this->pc++;
        }
    }
    else if(instruction == 0x00A1){
        this->pc++;
    }
}

void Chip8::setRegister(uint16_t value){
    this->registers[(value & 0x0F00) >> 8] = value & 0x00FF;
}

void Chip8::setI(uint16_t value){
    this->I = value & 0x0FFF;
}

void chip8:: addToI(uint16_t value){
    this->I += this->registers[(value & 0x0F00) >> 8];
}

void Chip8::add(uint16_t value){
    this->registers[(value & 0x0F00) >> 8] += value & 0x00FF;
}

void Chip8::handleRegisterOperations(uint16_t value){
    uint8_t regX = (value & 0x0F00) >> 8;
    uint8_t regY = (value & 0x00F0) >> 4;
    uint8_t currX = this->registers[regX];
    uint8_t currY = this->registers[regY];

    switch(value & 0x000F){
        case 0:
            this->registers[regX] = currY;
            break;
        case 1:
            this->registers[regX] |= currY;
            break;
        case 2:
            this->registers[regX] &= currY;
            break;
        case 3:
            this->registers[regX] ^= currY;
            break;
        case 4:
            this->registers[regX] += currY;
            this->registers[15] = (this->registers[regX] < currX)? 1 : 0;
            break;
        case 5:
            this->registers[regX] -= currY;
            this->registers[15] = (this->registers[regX] > currX)? 0 : 1;
            break;
        case 6:
            this->registers[15] = currY & 0x0001;
            this->registers[regX] = currY >> 1;
            break;
        case 7:
            this->registers[regX] = currY - currX;
            this->registers[15] = (this->registers[regX] > currX)? 0 : 1;
            break;
        case 15:
            this->registers[15] = currY & 0x0001;
            this->registers[regY] >>= 1;
            this->registers[regX] = this->registers[regY];
            break;
        default:
            break;
    }
}

void Chip8::randAnd(uint16_t value){
    this->registers[(value & 0x0F00) >> 8] = (rand() % 255) & (value & 0x00FF);
}

//architectural decisions pending
void Chip8::drawSprite(uint16_t value){
    uint8_t xOri = (value & 0x0F00) >> 8;
    uint8_t yOri = (value & 0x00F0) >> 4;
    uint8_t height = (value & 0x000F);
}

void Chip8::getDelay(uint16_t value){
    this->registers[(value & 0x0F00) >> 8] = this->delay;
}

//question
void Chip8::getKeyPress(uint16_t value){
    ;
}

void Chip8::setDelay(uint16_t value){
    this->delay = this->registers[(value & 0x0F00) >> 8]
}

void Chip8::setDelay(uint16_t value){
    this->sound = this->registers[(value & 0x0F00) >> 8]
}