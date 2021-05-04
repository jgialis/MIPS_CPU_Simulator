// //NEED TO CHECK FOR JUMP ADDRESS !@!@!@!@!@!@!@!@!@!@!@!@!@!@!@
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>
#include <map>
#include <vector>
#include <bitset>
#include <iomanip>

// //HEADER PROTOTYPES:
int arithmetic(std::string function); // 
std::string NOR(std::string value1, std::string value2); //
int binaryToDecimal(const std::string &binary);
std::string registerID_2_registerName(std::string rID); //
void fetch(); //
void decode(); //
void execute(); //
void mem(); //
void controlUnit(std::string opcode, std::string funct); //
void aluControl(std::string InstType, std::string funct); // 
void writeBack(); //

//GLOBAL VARIABLES FOR CONTROL SIGNALS.
bool RegWrite = false; // IDK
int RegDst = 0;        //If 1 rd is destination register, If 0 rt is destination register
bool branch = false;
int ALUSrc = 0; //SELECTOR TO DETERMINE SIGN EXTEND OR READ FROM REG
std::string InstType = "";
bool MemWrite = false; //if true, Write to address input with values given
int MemtoReg = 0;      //if false, value of reg write data comes from ALU, otherwise from data memory
bool MemRead = false;
bool jump = false;

//Other global variables
int PC = 0;            //PC for program counter
int next_pc = 0;
int branch_target = 0; // This variable will be updated by Execute() function and used by Fetch() function
                       // to decide the PC value of the next cycle. The first step for updating the
                       // branch_target variable is to shift-left-2 of the sign-extended offset input.
                       // The second step is to add the shift-left-2 output with the (PC + 4) value.

std::string jump_target = ""; // To update the jump_target variable, take the last 26 bits from the instruction.
                              // Run shift-left-2 (you may want to use multiplication) on it.
                              // And then, merge it with the first 4 bits of the next_pc variable.

bool alu_zero = false; // This variable will be updated by Execute() function and used when deciding
                       // PC value of the next cycle. The zero variable will be set by ALU when the
                       // computation result is zero and unset if otherwise.

int total_clock_cycles = 0;       // Increment by 1 whenever one instruction is finished, after WriteBack()
std::map<int, std::string> I_MEM; //We will use this map object to simulate the instruction memory (I_MEM).
std::string instruction = "";
std::string ALU_Op = "";
std::string ALU_res = "";
std::string destinationRegister = ""; 
std::string sourceRegister = "";
std::string tempRegister = "";
std::string opcode = "000000";
std::string funct = "000000";
std::string rs = "00000";
std::string rt = "00000";
std::string rd = "00000";
int rsVal = 0;
int rtVal = 0;
int rdVal = 0;
std::string shamt = "00000";
std::string immediate = "0000000000000000";
std::string address = "00000000000000000000000000";
int registerfile[32];
int d_mem[32];
int lwValue = 0;
int memoryAddress = 0;
std::string registersNames[32] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3",
                                  "$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
                                  "$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
                                  "$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};


//NEED TO TEST FOR JUMP ADDRESS
int main(){
    int index = 0;
    int i = 0;
    std::string fileName = "";
    std::ifstream reader;

    std::cout << "Enter the program file name to run:\n\n" << std::endl;
    std::cin >> fileName;

    //Open and read the file
    reader.open(fileName);

    //Initializing register file and d_mem array
    for (int i = 0; i < 32; i++){
        registerfile[i] = 0;
        d_mem[i] = 0;
    }
    
    // Initialize registerfile array like below 
    registerfile[9] = 32;
    registerfile[10] = 5;
    registerfile[16] = 112;
    
    // Initialize d_mem array like below 
    d_mem[28] = 5;
    d_mem[29] = 16;

    while (std::getline(reader, instruction)){
        I_MEM[index] = instruction; 
        // std::cout << "I_MEM[" << index << "] = " << I_MEM[index] << std::endl;
        index += 4;
    }

    //Using a for loop to simulate Single Cycle CPU execution (WILL NOT WORK BECAUSE # of times to execute is dependent on instruction and values.)
    while(I_MEM[PC] != ""){   
        fetch();
        decode();
        execute();
        mem();
        writeBack();
    }
    std::cout << "\nprogram terminated:" << std::endl;
    std::cout << "total execution time is " << total_clock_cycles <<  " cycles" << std::endl << std::endl;
}

//HELPER FUNCTION
int binaryToDecimal(const std::string &binary){ 
    int number;
    number = std::strtoul(binary.c_str(), NULL, 2);
    return number;
}

//HELPER FUNCTION
int arithmetic(std::string function){
    if(function == "add")
        return (rsVal + rtVal);
    else if(function == "sub")
        return (rsVal - rtVal);
    return 0;
}

//HELPER FUNCTION
std::string NOR(int rsVal, int rtVal){
    std::string val = std::bitset<32>(rsVal | rtVal).to_string();
    for(int i = 0; i < 32; i++){
        if(val[i] == '1')
            val[i] = '0';
        else if(val[i] == '0')
            val[i] = '1';
    }
    return val;
}

//HELPER FUNCTION
std::string registerID_2_registerName(std::string rID){
    for (int i = 0; i < 32; i++){
        if(binaryToDecimal(rID) == i)
            return registersNames[i];
    }
    return "";
}

void fetch(){ 
    if (branch && alu_zero)
        instruction = I_MEM[PC];
    else if (jump)
        instruction = I_MEM[PC]; //REVISIT! 
    else
        instruction = I_MEM[PC];
    
    next_pc = PC + 4;
    PC = PC + 4;
}

void decode(){
    opcode = instruction.substr(0,6);
    rs = instruction.substr(6,5);
    rt = instruction.substr(11,5); 
    rd = instruction.substr(16,5);
    std::string shamt = instruction.substr(21,5);
    funct = instruction.substr(26,6);
    immediate = instruction.substr(16,16);
    address = instruction.substr(6,26);

    std::string tempImm = immediate;

    //Extracting Values at registers ID locations.
    rsVal = registerfile[binaryToDecimal(rs)];
    rdVal = registerfile[binaryToDecimal(rd)];
    rtVal = registerfile[binaryToDecimal(rt)];
    
    //SIGN EXTENSION UNIT to extend 16-bit imm to 32-bit.
    std::string negExt = "1111111111111111";
    std::string posExt = "0000000000000000";
    if(immediate[0] == '0')
        immediate = posExt + immediate;
    else if(immediate[0] == '1')
        immediate = negExt + immediate;

    controlUnit(opcode, funct);

    std::string nextPCString = std::bitset<32>(next_pc).to_string().substr(0,4); //Taking the first four bits of next_pc
    jump_target = nextPCString + address + "00"; //Concatenating: first four bits of next_pc + 26 bits of SLL + 00
    if(jump)
        PC = binaryToDecimal(jump_target); 
}

void execute(){
    if(ALUSrc == 0){
        if(ALU_Op == "0010")
            ALU_res = std::bitset<32>(arithmetic("add")).to_string();
        else if(ALU_Op == "0110") 
            ALU_res = std::bitset<32>(arithmetic("sub")).to_string();
        else if(ALU_Op == "0000") //AND R Type
            ALU_res = std::bitset<32>((rsVal & rtVal)).to_string();
        else if(ALU_Op == "0001") //OR R Type
            ALU_res = std::bitset<32>(rsVal | rtVal).to_string();
        else if(ALU_Op == "0111")//SLT R Type
            ALU_res = std::bitset<32>((rsVal < rtVal)).to_string();
        else if(ALU_Op == "1100") //NOR R Type
            ALU_res = NOR(rsVal, rtVal);
        else if(ALU_Op == "0110" && InstType == "01") //BEQ
            ALU_res = arithmetic("sub"); 
    }
    else if(ALUSrc == 1 && RegDst == 0) { //LW, SW
        int temp = binaryToDecimal(immediate) + registerfile[binaryToDecimal(rs)]; 
        ALU_res = std::bitset<32>(temp).to_string();
    }
        
    //Calculate the branch target address 
    branch_target = next_pc + (binaryToDecimal(immediate) * 4);  

    if(binaryToDecimal(ALU_res) == 0){
        alu_zero = true;
        if (alu_zero && branch){
                PC = branch_target;
        }
    }
}

void mem(){
    memoryAddress = binaryToDecimal(ALU_res);

    if(MemRead)
        lwValue = d_mem[memoryAddress / 4];
    
    //Load value into memory for SW given the TA.
    if(MemWrite)
        d_mem[memoryAddress / 4] = rtVal;
}

void writeBack(){
    total_clock_cycles++;
    std::cout << "\ntotal_clock_cycles " << total_clock_cycles << " :\n";
    if(opcode == "100011"){ //LW instruction
        registerfile[binaryToDecimal(rt)] = lwValue;
        if(!branch){
            std::cout << registerID_2_registerName(rt) << " is modified to "; 
            printf("0x%x\n", lwValue);
        }
    }
    else if(opcode == "000000"){ //R Type instruction
        registerfile[binaryToDecimal(rd)] =  binaryToDecimal(ALU_res);
        if(!branch && RegDst == 1){
            std::cout << registerID_2_registerName(rd) << " is modified to ";
            printf("0x%x\n", binaryToDecimal(ALU_res));
        }
    }
    if (opcode == "101011"){
        std::cout << "memory "; printf("0x%x", memoryAddress); std::cout << " is modified to "; printf("0x%x\n", d_mem[memoryAddress / 4]);
    }
    std::cout << "PC is modified to "; printf("0x%x\n", PC);
}

void controlUnit(std::string opcode, std::string funct){
    if(opcode == "000000"){ //Considering R type instructions.
       RegWrite = true; //True - R does write to register.
       RegDst = 1; //RegDst MUX selector bit set to 1 since bin 15-11 is destination register.
       branch = false; //No branch required for R type.
       ALUSrc = 0; //ALUSrc MUX selector bit to 0 since no sign extension is necessary for R type.
       InstType = "10"; //R Type Instructions: ADD, SUB, AND, OR, SLT, NOR
       MemWrite = false; //R type instructions do NOT write to memory, leave as default 'false'.
       MemtoReg = 0; //MemToReg MUX selector bit is left as default 0 since the input value comes from ALU result. 
       MemRead = false; //MemRead is left as default value 'false' since no memory is read for R type.
       jump = false; //No jump for R-type instructions.
    }
    else if(opcode == "000010"){ //Considering J type
        jump =  true;
        RegWrite = false;
        branch = false;
        MemWrite = false;
        MemRead = false;
    }
    else{ //Considering I type instructions
        if(opcode == "100011") { //Considering LW instruction.
            RegWrite = true; //True - R does write to register.
            RegDst = 0; //RegDst MUX selector bit set to 0 since bin 20-16 is destination register for LW.
            branch = false; //No branch required for R type.
            ALUSrc = 1; //ALUSrc MUX selector bit to 1 since sign extension is necessary for LW type.
            InstType = "00"; 
            MemWrite = false; //LW instruction does NOT write to memory, leave as default 'false'.
            MemtoReg = 1; //MemToReg MUX selector bit is set to 1 since the input value comes from the memory. 
            MemRead = true; //MemRead is set to true since memory is read for LW.
            jump = false; //No jump for LW instructions.
        }
        else if (opcode == "000100") { //Considering beq instruction.
            RegWrite = false; //False - beq does not write to register.
            branch = true; //Brancing required for beq.
            ALUSrc = 0; //ALUSrc MUX selector bit to 0 since no sign extension is necessary for beq.
            InstType = "01"; 
            MemWrite = false; //Beq instruction does NOT write to memory, leave as default 'false'.
            MemRead = false; //MemRead is set to true since memory is not read for beq.
            jump = false; //No jump for beq instructions.
        }
        else if (opcode == "101011"){ //Considering SW instruction
            RegWrite = false; //False - SW does write to register.
            RegDst = 0;          
            branch = false; //No branch required for SW.
            ALUSrc = 1; //ALUSrc MUX selector bit to 1 since sign extension is necessary for SW.
            InstType = "00";
            MemWrite = true; //SW instructions do write to memory.
            // MemtoReg = X?
            MemRead = false; //MemRead is left as default value 'false' since no memory is read for SW.
            jump = false; //No jump for SW instruction.
        }
    }
    aluControl(InstType, funct);
}

void aluControl(std::string InstType, std::string funct){
    if (InstType == "00"){
        ALU_Op = "0010";
    }
    else if (InstType == "01"){
        ALU_Op = "0110";
    }
    else if (InstType == "10"){
        if (funct == "100000")
            ALU_Op = "0010"; //this is jump
            //pc = pc + immediate.
        else if (funct == "100010")
            ALU_Op = "0110";
        else if (funct == "100100")
            ALU_Op = "0000";
        else if (funct == "100101")
            ALU_Op = "0001";
        else if (funct == "101010"){
            ALU_Op = "0111";
        }
        else if (funct == "100111")
            ALU_Op = "1100";
    }
}

