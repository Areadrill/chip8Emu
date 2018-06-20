#include "Chip8.hpp"

typedef  void (Chip8::*handlerFn)(uint16_t);

Chip8::Chip8(char *filename){
    this->rom = this->loadROM(filename);
    uint8_t* ptr = &(this->rom)[0];

    this->screen = new bool*[32];
    for (int i = 0; i < 32; ++i){
        this->screen[i] = new bool[64];
    }

    this->clearScreen();
    this->I = 0;
    this->pc = 0x200;

    std::fill(this->registers, this->registers+16, 0);
    std::copy(ptr, ptr + this->rom.size(), this->memory + 512);
    std::copy(this->fontset, this->fontset + 80, this->memory);

    this->handlerMap.emplace(1, &Chip8::goTo);
    this->handlerMap.emplace(2, &Chip8::call);
    this->handlerMap.emplace(3, &Chip8::skipIf);
    this->handlerMap.emplace(4, &Chip8::skipIf);
    this->handlerMap.emplace(5, &Chip8::skipIfReg);
    this->handlerMap.emplace(6, &Chip8::setRegister);
    this->handlerMap.emplace(7, &Chip8::add);
    this->handlerMap.emplace(8, &Chip8::handleRegisterOperations);
    this->handlerMap.emplace(9, &Chip8::skipIfReg);
    this->handlerMap.emplace(10, &Chip8::setI);
    this->handlerMap.emplace(11, &Chip8::goTo);
    this->handlerMap.emplace(12, &Chip8::randAnd);
    this->handlerMap.emplace(13, &Chip8::drawSprite);
    this->handlerMap.emplace(14, &Chip8::skipIfKey);
    //missing some opcodes

    srand (time(NULL));    
}

void Chip8::step(){
    if(this->handleOpcode(this->toShort(this->memory[pc], this->memory[pc+1]))){
        this->pc += 2;
    }

    if(this->delay){
        this->delay--;
    }
    if(this->sound){
        this->sound--;
    }
    else{
        ; //make a sound
    }
    
}

uint16_t Chip8::toShort(uint8_t msb, uint8_t lsb){
    return msb << 8 | lsb;
}

std::vector<uint8_t> Chip8::loadROM(char *filename){
    std::ifstream rom(filename, std::ios::binary);
    
    if(!rom){
        std::cerr << "Error: No such file found" << std::endl;
    }

    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(rom)),(std::istreambuf_iterator<char>()));
    /*std::vector<uint16_t> opcodes;

    for(std::vector<uint8_t>::iterator it = bytes.begin() ; it != bytes.end(); ++it){
        opcodes.push_back(toShort(*(1+it), *it));
        ++it;
    }*/

    return bytes;                        
}

bool Chip8::handleOpcode(uint16_t opcode){
    //special opcodes
    if(opcode == 0x00E0){
        clearScreen();
        return true;
    }
    if(opcode == 0x00EE){
        ret();
        return false;
    }

    handlerFn fn = this->handlerMap.at((opcode & 0xF000) >> 12);
    (this->*fn)(opcode);

    return (std::find(std::begin(this->pcOpcodes), std::end(this->pcOpcodes), opcode) == std::end(this->pcOpcodes))? true : false;
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
    this->pc = value & 0x0FFF + ((value & 0xF000) >> 11 == 1)? 0 : this->registers[0];
}

void Chip8::call(uint16_t value){
    this->addrStack->push(this->pc);
    this->pc = value & 0x0FFF;
}

void Chip8::skipIf(uint16_t value){
    uint8_t instruction = (value & 0xF000) >> 12;

    if(this->registers[(value & 0x0F00) >> 8] == (value & 0x00FF)){
        if(instruction == 3){
            this->pc+=2;
        }
    }
    else if(instruction == 4){
        this->pc+=2;
    }    
}

void Chip8::skipIfReg(uint16_t value){
    uint8_t instruction = (value & 0xF000) >> 12;

    if(this->registers[(value & 0x0F00) >> 8] == this->registers[(value & 0x00F0) >> 4]){
        if(instruction == 5){
            this->pc+=2;
        }
    }
    else if(instruction == 9){
        this->pc+=2;
    }
}

void Chip8::skipIfKey(uint16_t value){
    uint8_t instruction = (value & 0x00FF);

    if(this->keyboard[this->registers[(value & 0x0F00) >> 8]]){
        if(instruction == 0x009E){
            this->pc+=2;
        }
    }
    else if(instruction == 0x00A1){
        this->pc+=2;
    }
}

void Chip8::setRegister(uint16_t value){
    this->registers[(value & 0x0F00) >> 8] = value & 0x00FF;
}

void Chip8::setI(uint16_t value){
    this->I = value & 0x0FFF;
}

void Chip8::addToI(uint16_t value){
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

void Chip8::drawSprite(uint16_t value){
    uint8_t xOri = this->registers[(value & 0x0F00) >> 8];
    uint8_t yOri = this->registers[(value & 0x00F0) >> 4];
    uint8_t height = (value & 0x000F);
    bool collisionDetected = false;

    for(uint8_t y = 0; y < height; y++){
        uint8_t spriteLine = this->memory[this->I + y];
        for(uint8_t x = 0; x < 8; x++){
            bool oldPixel = this->screen[yOri + y][xOri + x];
            this->screen[yOri + y][xOri + x] ^= (spriteLine & (uint8_t)pow(2, 8 - (x+1))) >> (8 - (x+1));

            if(!collisionDetected && oldPixel && !this->screen[yOri + y][xOri + x]){
                collisionDetected = true;
            }
        }
    }

    if(collisionDetected){
        this->registers[15] = 1;
    }
}

void Chip8::getDelay(uint16_t value){
    this->registers[(value & 0x0F00) >> 8] = this->delay;
}

void Chip8::getKeyPress(uint16_t value){
    ;
}

void Chip8::setDelay(uint16_t value){
    this->delay = this->registers[(value & 0x0F00) >> 8];
}

void Chip8::setSound(uint16_t value){
    this->sound = this->registers[(value & 0x0F00) >> 8];
}

//getters
bool** Chip8::getScreen(){
    return this->screen;
}