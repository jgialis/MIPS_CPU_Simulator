//  LIBRARIES USED

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>
#include <map>
#include <vector>
#include <bitset>
#include <iomanip>

// INCLUDING STAGE REGISTER CLASS HEADER FILES
#include "IF_ID.h"
#include "ID_EX.h"
#include "EX_MEM.h"
#include "MEM_WB.h"

//REQUIRED FUNCTIONS' PROTOTYPES:
void fetch();
void decode();
void writeBack();
void aluControl();
void controlUnit();
void hazardUnit();
void execute();
void mem();


//HELPER FUNCTIONS' PROTOTYPES
void modPC();
int arithmetic(std::string function);
int binaryToDecimal(const std::string &binary);
std::string registerID_2_registerName(std::string rID);
std::string NOR(std::string value1, std::string value2);

// MEMORY-OFFSET FLAGS
bool branch = false;
bool jump = false;

// HAZARD FLAGS
bool dataHazard = false;
bool controlHazard = false; 
bool firstNop = false;
bool secNop = false;


//  NEXT INSTRUCTION TARGET ADDRESS VARIABLES
int PC = 0;                     //PC for program counter 
int next_pc = 0;                // PC of the next instruction
int branch_target = 0;          // 16-bits from a 32-bit branch instruction; extended to 32-bits by shift left log
std::string jump_target = "";   // 26 bits from a 32-bit J-type instruction; extended to 32-bits by shift left log
bool alu_zero = false;          // Variable to be used when deciding PC value of the next cycle. Defined in ALU when result is 0, unused otherwise


//  VARIABLES FOR SIMPLIFICATION
int memoryAddress = 0;
int total_clock_cycles = 1;
std::string instruction = "";     
std::string ALU_Op = "";    
std::string ALU_res = "";  

// VARIABLES FOR DECODING 
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


// DATA STRUCTURES FOR MEM & FILES SIMULATION  I_MEM, and D_MEM
int d_mem[32];                      // Array to simulate Data Memory file
int registerfile[32];               // to correlate and assign names to a register file of values
std::map<int, std::string> I_MEM;   // Map to simulate I_MEM instruction file
std::string registersNames[32] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3",
                                  "$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
                                  "$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
                                  "$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};

// STAGE REGISTERS GLOBAL INSTANTIATIONS
IF_ID if_id;
ID_EX id_exe;
EX_MEM ex_mem;
MEM_WB mem_wb;

int main(){

    int index = 0;
    std::string fileName = "sample_binary.txt";
    std::ifstream reader;

    std::cout << "Enter the program file name to run:\n\n" << std::endl;
    std::cin >> fileName;

    // //Open and read the file
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
        index += 4;
    }

    int size = I_MEM.size();
    if(size == 1){
        std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n";
        fetch();
        modPC();
        std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n";
        decode();
        std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n";
        execute();
        std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n";
        mem();
        std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n";
        writeBack();
        std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n"; 
    }
    else if(size == 2){
        for (int i = 0; i < size + 5 - 1; i++){
            std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n";
            if(i == 0){
                fetch();
            }
            else if(i == 1){
                decode();
                fetch();
                hazardUnit();  
            }
            else if(i == 2){
                execute();
                decode();
            }
            else if(i == 3){
                mem();
                execute();
            }
            else if(i == 4){
                writeBack();
                mem();
            }
            else if(i == 5){
               writeBack();
            }
            if(i < I_MEM.size()){
                modPC();
            }
        }
        std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n";  
    }
    else if(size == 3){
        for (int i = 0; i < size + 5 - 1; i++){
            std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n";
            if(i == 0){
                fetch();
            }
            else if(i == 1){
                decode();
                fetch();  
                hazardUnit();
            }
            else if(i == 2){
                execute();
                decode();
                fetch();
                hazardUnit(); 
            }
            else if(i == 3){
                mem();
                execute();
                decode();
            }
            else if(i == 4){
                writeBack();
                mem();
                execute();
            }
            else if(i == 5){
                writeBack();
                mem();
            }
            else if(i == 6){
                writeBack();
            }
            if(i < I_MEM.size()){
                modPC();
            }
        } 
        std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n"; 
    }
    else if(size == 4){
        for (int i = 0; i < size + 5 - 1; i++){
            std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n";
            if(i == 0){
                fetch();
            }
            else if(i == 1){
                decode();
                fetch();
                hazardUnit();
            }
            else if(i == 2){
               execute();
               decode();
               fetch();
               hazardUnit();
 
            }
            else if(i == 3){
               mem();
               execute();
               decode();
               fetch();
               hazardUnit(); 

            }
            else if(i == 4){
                writeBack();
                mem();
                execute();
                decode();
            }
            else if(i == 5){
                writeBack();
                mem();
                execute();
            }
            else if(i == 6){
               writeBack();
               mem();
            }
            else if(i == 7){
                writeBack();
            }
            if(i<I_MEM.size()){
                modPC();
            }

        }
        std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n"; 
    }
    else if(size >= 5){
        
        for (int i = 0; i < size + 5 - 1; i++){
        std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n";
        
            if(i == 0){
                fetch();
            }
            else if(i == 1){
                decode();
                fetch();
                hazardUnit();
            }
            else if(i == 2){
                execute();
                decode();
                fetch();
                hazardUnit();
            }
            else if(i == 3){
                // if(firstNop==false){
                //     mem();
                // }else{
                //     std::cout<<"Data Hazard Detected (nop1)"<<std::endl;
                // }
                mem();
                execute();
                decode();
                fetch();
                hazardUnit();
            }
            else if(i >= 4 && (i < ((size + 5 - 1) - 4))){
                // if(secNop==false){
                //     writeBack();
                // }else{
                //     std::cout<<"Data Hazard Detected (nop2)"<<std::endl;
                // }
                writeBack();
                mem();
                execute();
                decode();
                fetch();
                hazardUnit();
            }  
            else if(i == ((size + 5 - 1) - 4)){
                writeBack();
                mem();
                execute();
                decode();
            }
            else if(i == ((size + 5 - 1) - 3)){
                writeBack();
                mem();
                execute();
            }
            else if(i == ((size + 5 - 1) - 2)){
                writeBack();
                mem();
            }
            else if(i == ((size + 5 - 1) - 1)){
                writeBack();
            }
            if(i < I_MEM.size() && dataHazard == false){
                modPC();
            }
        }
        //std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n";
    }
 
    std::cout << "\nprogram terminated:" << std::endl;
    std::cout << "total execution time is " << (total_clock_cycles - 1) <<  " cycles" << std::endl << std::endl;
}

//PRINTING MODIFICATIONS OF PC
void modPC(){
    PC += 4;
    std::cout << "pc was modified to "; 
    printf("0x%x\n", PC);
    return;
        
}

//HELPER FUNCTION FOR BINARY TO DECIMAL TRANSLATION 
int binaryToDecimal(const std::string &binary){ 
    int number;
    number = std::strtoul(binary.c_str(), NULL, 2);
    return number;
}

//HELPER FUNCTION FOR ARITHMETIC OPERATIONS
int arithmetic(std::string function){
    if(function == "add")
        return (rsVal + rtVal);
    else if(function == "sub")
        return (rsVal - rtVal);
    return 0;
}

//HELPER FUNCTION TO SIMULATE NOR INSTRUCTION
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

//HELPER FUNCTION TO GET REGISTER NAME FROM REGISTER 5-BIT ID
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
        instruction = I_MEM[PC];
    else
        instruction = I_MEM[PC];
    
    next_pc = PC + 4;
    if_id.setNext_PC(next_pc);
    if_id.setInstruction(instruction);
}

void decode(){
    opcode = if_id.getInstruction().substr(0,6);
    rs = if_id.getInstruction().substr(6,5);  //ID
    rt = if_id.getInstruction().substr(11,5); //ID
    rd = if_id.getInstruction().substr(16,5); //ID
    immediate = if_id.getInstruction().substr(16,16);
    shamt = if_id.getInstruction().substr(21,5);
    funct = if_id.getInstruction().substr(26,6);
    address = if_id.getInstruction().substr(6,26);
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

    //Calculating Jump TA --> REVISIT SINCE PIPELINE MIGHT CHANGE THINGS A BIT
    std::string nextPCString = std::bitset<32>(if_id.getNext_PC()).to_string().substr(26,31); //Taking the first four bits of next_pc
    jump_target = nextPCString + address + "00"; //Concatenating: first four bits of next_pc + 26 bits of SLL + 00
    if(jump)
        PC = binaryToDecimal(jump_target); 
    
    controlUnit();
    //Pipeline buffers
    id_exe.setNext_pc(if_id.getNext_PC());
    id_exe.setRSVal(rsVal);
    id_exe.setRTVal(rtVal);
    id_exe.setSignExtendedVal(immediate);
    id_exe.setRTID(rt);
    id_exe.setRDID(rd);
    id_exe.setRSID(rs);
    id_exe.setFunct(funct);
}

void execute(){
    //Need to determine the proper ALU_OP value that is generated by aluControl.
    aluControl();
    //Pipeline WB control signals. Simply pass them to ex_mem pipeline register.
    ex_mem.setRegWrite(id_exe.getRegWrite());
    ex_mem.setMemToReg(id_exe.getMemToReg());
    //Pipeline M control signals. Simply pass them to ex_mem pipeline register.
    ex_mem.setBranch(id_exe.getBranch());
    ex_mem.setMemRead(id_exe.getMemRead());
    ex_mem.setMemWrite(id_exe.getMemWrite());
    if(id_exe.getRegDst() == 0){
        ex_mem.setRegDst(binaryToDecimal(id_exe.getRTID())); //If RegDst = 0, we will use rt # as register ID.
    }
    else if(id_exe.getRegDst() == 1){
        ex_mem.setRegDst(binaryToDecimal(id_exe.getRDID())); //If RegDst = 1, we will use rd # as register ID.
    }
    rsVal = id_exe.getRSVal();
    rtVal = id_exe.getRTVal();
    if(id_exe.getALUSrc() == 0){
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
        else if(ALU_Op == "0110" && id_exe.getInstType() == "01") //BEQ
            ALU_res = arithmetic("sub"); 
    }
    else if(id_exe.getALUSrc() == 1 && id_exe.getRegDst() == 0) { //LW, SW
        int temp = binaryToDecimal(id_exe.getSignExtendedVal()) + registerfile[binaryToDecimal(id_exe.getRSID())];  //tempRegister or destinationRegister?
        ALU_res = std::bitset<32>(temp).to_string();
    } 
    //Calculate the branch target address for ADDER component --> HANDLE THIS LATER.
    branch_target = id_exe.getNext_pc() + (binaryToDecimal(id_exe.getSignExtendedVal()) * 4); //REVISIT  
    if(binaryToDecimal(ALU_res) == 0){
        alu_zero = true;
        ex_mem.setZero(1);
        if (alu_zero && branch)
                PC = branch_target;
    }
    else
        ex_mem.setZero(0);
    //Forwarding control signals.
    ex_mem.setRegWrite(id_exe.getRegWrite());
    ex_mem.setMemToReg(id_exe.getMemToReg());
    ex_mem.setBranch(id_exe.getBranch());
    ex_mem.setMemRead(id_exe.getMemRead());
    ex_mem.setMemWrite(id_exe.getMemWrite());
    ex_mem.setSum(branch_target);
    //Forwarding values
    ex_mem.setALUResult(ALU_res);
    ex_mem.setRTVal(rtVal);
}

void mem(){
    //Pipeline WB control signals. Simply pass them to ex_mem pipeline register.
    mem_wb.setRegWrite(ex_mem.getRegWrite());
    mem_wb.setMemToReg(ex_mem.getMemToReg());
    //Discard relevant M control signals
    if((ex_mem.getZero() == 1) && (ex_mem.getBranch() == true)) { //Double Check, probably wrong
        PC = ex_mem.getSum(); //Do something with it regarding PCSrc mux. if(PCSrc == 0)?????
    }
    //Passing from ex_mem pipeline register to mem_wb pipeline register.
    mem_wb.setALUResult(ex_mem.getALUResult());
    //Accessing memory address provided by ALU result.
    memoryAddress = binaryToDecimal(ex_mem.getALUResult());
    if(ex_mem.getMemRead()){
        mem_wb.setReadMemData(d_mem[memoryAddress / 4]);
    }
    //Load value into memory for SW given the TA.
    if(ex_mem.getMemWrite() ){
        d_mem[memoryAddress / 4] = ex_mem.getRTVal();
        std::cout << "memory "; printf("0x%x", memoryAddress); std::cout << " modified to "; 
        printf("0x%x\n", ex_mem.getRTVal());
    }
    //Passing from ex_mem pipeline register to mem_wb pipeline register.
    mem_wb.setRegDst(ex_mem.getRegDst());

}

void writeBack(){
    if ((mem_wb.getRegWrite()) && (mem_wb.getMemToReg() == 0)) {
        registerfile[mem_wb.getRegDst()] = binaryToDecimal(mem_wb.getALUResult()); 
            std::cout << registerID_2_registerName(std::bitset<5>(mem_wb.getRegDst()).to_string()) << " is modified to ";
            printf("0x%x\n", binaryToDecimal(mem_wb.getALUResult()));
    }
    if((mem_wb.getRegWrite()) && (mem_wb.getMemToReg() == 1)){ //LW instruction
        registerfile[mem_wb.getRegDst()] = mem_wb.getReadMemData();  
            std::cout << registerID_2_registerName(std::bitset<5>(mem_wb.getRegDst()).to_string()) << " is modified to "; 
            printf("0x%x\n", mem_wb.getReadMemData());
    }
    //RESETING DATA HAZARD DETECTION FLAGS & NOPS
    if(dataHazard == true && firstNop == true && secNop == true){
        dataHazard = false;
        firstNop = false;
        secNop = false;
    }
}

void controlUnit(){
    //std::cout<< "----------Control Unit Start----------\n";
    if(opcode == "000000"){ //Considering R type instructions.
        id_exe.setRegWrite(true); //True - R does write to register.
        id_exe.setRegDst(1); //RegDst MUX selector bit set to 1 since bin 15-11 is destination register.
        id_exe.setBranch(false); //No branch required for R type.
        id_exe.setALUSrc(0); //ALUSrc MUX selector bit to 0 since no sign extension is necessary for R type.
        id_exe.setInstType("10"); //R Type Instructions: ADD, SUB, AND, OR, SLT, NOR
        id_exe.setMemWrite(false); //R type instructions do NOT write to memory, leave as default 'false'.
        id_exe.setMemToReg(0); //MemToReg MUX selector bit is left as default 0 since the input value comes from ALU result. 
        id_exe.setMemRead(false); //MemRead is left as default value 'false' since no memory is read for R type.
        id_exe.setJump(false); //No jump for R-type instructions.
    }
    else if(opcode == "000010"){ //Considering J type
        id_exe.setJump(true); //No jump for R-type instructions.
        id_exe.setRegWrite(false);
        id_exe.setBranch(false);
        id_exe.setMemWrite(false);
        id_exe.setMemRead(false);
    }
    else{ //Considering I type instructions
        if(opcode == "100011") { //Considering LW instruction.
            id_exe.setRegWrite(true); //True - R does write to register.
            id_exe.setRegDst(0); //RegDst MUX selector bit set to 0 since bin 20-16 is destination register for LW.
            id_exe.setBranch(false); //No branch required for R type.
            id_exe.setALUSrc(1); //ALUSrc MUX selector bit to 1 since sign extension is necessary for LW type.
            id_exe.setInstType("00"); 
            id_exe.setMemWrite(false); //LW instruction does NOT write to memory, leave as default 'false'.
            id_exe.setMemToReg(1); //MemToReg MUX selector bit is set to 1 since the input value comes from the memory. 
            id_exe.setMemRead(true); //MemRead is set to true since memory is read for LW.
            id_exe.setJump(false); //No jump for LW instructions.
        }
        else if (opcode == "000100") { //Considering beq instruction.
            id_exe.setRegWrite(false); //False - beq does not write to register.
            id_exe.setBranch(true); //Brancing required for beq.
            id_exe.setALUSrc(0); //ALUSrc MUX selector bit to 0 since no sign extension is necessary for beq.
            id_exe.setInstType("01"); 
            id_exe.setMemWrite (false); //Beq instruction does NOT write to memory, leave as default 'false'.
            id_exe.setMemRead (false); //MemRead is set to true since memory is not read for beq.
            id_exe.setJump(false); //No jump for beq instructions.
        }
        else if (opcode == "101011"){ //Considering SW instruction
            id_exe.setRegWrite(false); //False - SW does write to register.
            id_exe.setRegDst(0);
            id_exe.setBranch(false); //No branch required for SW.
            id_exe.setALUSrc(1); //ALUSrc MUX selector bit to 1 since sign extension is necessary for SW.
            id_exe.setInstType("00");
            id_exe.setMemWrite(true); //SW instructions do write to memory.
            id_exe.setMemRead(false); //MemRead is left as default value 'false' since no memory is read for SW.
            id_exe.setJump(false); //No jump for SW instruction.

        }
    }
}


void hazardUnit(){
    int decExeSource = binaryToDecimal(id_exe.getRSID());
    int decExeTemp = binaryToDecimal(id_exe.getRTID());

    int memExeDest = ex_mem.getRegDst();
    int memExeTemp = ex_mem.getRTVal();

    if(!dataHazard){
        if(ex_mem.getRegWrite()){
            if((decExeSource == memExeDest || decExeTemp == memExeDest) ||
            (decExeSource == memExeTemp || decExeTemp == memExeTemp)){
                dataHazard = true;
                std::cout<<"Data Hazard Detected"<<std::endl;
                return;
            }
        }
    }
    else{
        if(firstNop == false){
            firstNop = true;
            mem();
            std::cout<<"Data Hazard Detected (nop1)" <<std::endl;

            std::cout<<"\ntotal_clock_cycles "<<total_clock_cycles++<<":"<<std::endl;
            secNop = true;
            writeBack();
            std::cout<<"Data Hazard Detected (nop2)" <<std::endl;
            std::cout<<"\ntotal_clock_cycles "<<total_clock_cycles++<<":"<<std::endl;
            return;
        }
        if(secNop == false){
        }
    }
}

//Opcode and funct field together determine ALU Op
void aluControl(){
    if (id_exe.getInstType() == "00")
        ALU_Op = "0010";
    else if (id_exe.getInstType() == "01")
        ALU_Op = "0110";
    else if (id_exe.getInstType() == "10"){
        if (id_exe.getFunct() == "100000")
            ALU_Op = "0010";
        else if (id_exe.getFunct() == "100010")
            ALU_Op = "0110";
        else if (id_exe.getFunct() == "100100")
            ALU_Op = "0000";
        else if (id_exe.getFunct() == "100101")
            ALU_Op = "0001";
        else if (id_exe.getFunct() == "101010")
            ALU_Op = "0111";
        else if (id_exe.getFunct() == "100111")
            ALU_Op = "1100";
    }
}