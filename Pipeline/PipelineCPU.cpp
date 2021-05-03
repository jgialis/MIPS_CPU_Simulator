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
int arithmetic(std::string function);
std::string NOR(std::string value1, std::string value2);
int binaryToDecimal(const std::string &binary);
std::string registerID_2_registerName(std::string rID);
void fetch();
void decode();
void controlUnit();
void aluControl();
void execute();
void mem();
void writeBack();

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

int total_clock_cycles = 1;       // Increment by 1 whenever one instruction is finished, after WriteBack()
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

/////// CLASS DECLARATIONS TO BE MOVED LATER WHEN DEBUGGING IS FINISHED /////////////////
/////// CLASS DECLARATIONS TO BE MOVED LATER WHEN DEBUGGING IS FINISHED /////////////////
/////// CLASS DECLARATIONS TO BE MOVED LATER WHEN DEBUGGING IS FINISHED /////////////////
/////// CLASS DECLARATIONS TO BE MOVED LATER WHEN DEBUGGING IS FINISHED /////////////////
/////// CLASS DECLARATIONS TO BE MOVED LATER WHEN DEBUGGING IS FINISHED /////////////////
class IF_ID
{
private:
   std::string instruction;
   int next_pc;
public:
    IF_ID(){};
    std::string getInstruction(){return instruction;}
    int getNext_PC(){return next_pc;}
    void setInstruction(std::string instruction){this->instruction = instruction;}
    void setNext_PC(int next_pc){this->next_pc = next_pc;}
    // void printPipeline(){
    //     std::
    // }
};
IF_ID if_id;

class ID_EX
{
private:
    //---------------Control Signals---------------//
    //EXE
    int RegDst;    
    std::string InstType;;
    int ALUSrc; 
    //MEM
    bool branch;
    bool MemRead;
    bool MemWrite;
    //WB
    bool RegWrite; 
    int MemtoReg;     
    bool jump;

    int next_pc;
    int rsVal;
    int rtVal;
    std::string signExtendedVal;
    std::string funct;
    std::string rsID;
    std::string rtID;
    std::string rdID;
    
public:
    //Default Constructor
    ID_EX(){};
    //Getters
    int getNext_pc(){return next_pc;}
    int getRSVal(){return rsVal;}
    int getRTVal(){return rtVal;}
    std::string getFunct(){return funct;}
    std::string getRSID(){return rsID;}
    std::string getRTID(){return rtID;}
    std::string getRDID(){return rdID;}
    std::string getSignExtendedVal(){return signExtendedVal;}

    //EX getters
    int getRegDst(){return RegDst;}
    std::string getInstType(){return InstType;}
    int getALUSrc(){return ALUSrc;}
    //MEM getters
    bool getBranch(){return branch;}
    bool getMemRead(){return MemRead;}
    bool getMemWrite(){return MemWrite;}
    //WB getters
    bool getRegWrite(){return RegWrite;}
    int getMemToReg(){return MemtoReg;}
    bool getJump(){return jump;}

    //Setters
    void setNext_pc(int next_pc){ this->next_pc= next_pc;}
    void setRSVal(int rsVal){this->rsVal = rsVal;}
    void setRTVal(int rtVal){this-> rtVal = rtVal;}
    void setFunct(std::string funct){this-> funct = funct;}
    void setRSID(std::string rsID){this-> rsID = rsID;}
    void setRTID(std::string rtID){this->rtID = rtID;}
    void setRDID(std::string rdID){this->rdID = rdID;}
    void setSignExtendedVal(std::string signExtendedVal){this->signExtendedVal = signExtendedVal;}

    //EXE setters
    void setRegDst(int RegDst){this->RegDst = RegDst;}
    void setInstType(std::string InstType){this->InstType = InstType;}
    void setALUSrc(int ALUSrc){this->ALUSrc = ALUSrc;}
    //MEM setters
    void setBranch(bool branch){this->branch = branch;}
    void setMemRead(bool MemRead){this->MemRead = MemRead;}
    void setMemWrite(bool MemWrite){this->MemWrite = MemWrite;}
    //WB setters
    void setRegWrite(bool RegWrite){this->RegWrite = RegWrite;}
    void setMemToReg(int MemtoReg){this->MemtoReg = MemtoReg;}
    void setJump(bool jump){this->jump = jump;}

};
ID_EX id_exe;

class EX_MEM
{
private:
    //---------------Control Signals---------------//
    //MEM
    bool branch;
    bool MemRead;
    bool MemWrite;
    //WB
    bool RegWrite; 
    int MemtoReg;     
    bool jump;

    int sum;
    int zero;
    std::string ALUresult;
    int rtVal;
    int regDST;
public:
    //Default Constructor
    EX_MEM(){};
    //Getters
    int getSum(){return sum;}
    int getZero(){return zero;}
    std::string getALUResult(){return ALUresult;}
    int getRTVal(){return rtVal;}
    int getRegDst(){return regDST;} //Gets register ID pipelined from execute stage.
    //MEM getters
    bool getBranch(){return branch;}
    bool getMemRead(){return MemRead;}
    bool getMemWrite(){return MemWrite;}
    //WB getters
    bool getRegWrite(){return RegWrite;}
    int getMemToReg(){return MemtoReg;}
    bool getJump(){return jump;}

    //Setters
    void setSum(int sum){ this->sum= sum;}
    void setZero(int zero){this->zero = zero;}
    void setALUResult(std::string ALUresult){this-> ALUresult = ALUresult;}
    void setRTVal(int rtVal){this->rtVal = rtVal;}
    void setRegDst(int regDST){this-> regDST = regDST;} //Sets register ID to be pipelined according to RegDst mux.
    //MEM setters
    void setBranch(bool branch){this->branch = branch;}
    void setMemRead(bool MemRead){this->MemRead = MemRead;}
    void setMemWrite(bool MemWrite){this->MemWrite = MemWrite;}
    //WB setters
    void setRegWrite(bool RegWrite){this->RegWrite = RegWrite;}
    void setMemToReg(int MemtoReg){this->MemtoReg = MemtoReg;}
    void setJump(bool jump){this->jump = jump;}
};
EX_MEM ex_mem;

class MEM_WB{
private:
//---------------Control Signals---------------//
    //WB
    bool RegWrite; 
    int MemtoReg;     
    bool jump;

    std::string ALUresult;
    int readMemData;
    int regDST;
public:
    MEM_WB(){}
    std::string getALUResult(){return ALUresult;}
    int getReadMemData(){return readMemData;}
    int getRegDst(){return regDST;}
    //WB getters
    bool getRegWrite(){return RegWrite;}
    int getMemToReg(){return MemtoReg;}
    bool getJump(){return jump;}

    void setALUResult(std::string ALUresult){this-> ALUresult = ALUresult;}
    void setReadMemData(int readMemData){this-> readMemData = readMemData;}
    void setRegDst(int regDST){this-> regDST = regDST;}
    //WB setters
    void setRegWrite(bool RegWrite){this->RegWrite = RegWrite;}
    void setMemToReg(int MemtoReg){this->MemtoReg = MemtoReg;}
    void setJump(bool jump){this->jump = jump;}
};

MEM_WB mem_wb;
/////// CLASS DECLARATIONS TO BE MOVED LATER WHEN DEBUGGING IS FINISHED /////////////////
/////// CLASS DECLARATIONS TO BE MOVED LATER WHEN DEBUGGING IS FINISHED /////////////////
/////// CLASS DECLARATIONS TO BE MOVED LATER WHEN DEBUGGING IS FINISHED /////////////////
/////// CLASS DECLARATIONS TO BE MOVED LATER WHEN DEBUGGING IS FINISHED /////////////////
/////// CLASS DECLARATIONS TO BE MOVED LATER WHEN DEBUGGING IS FINISHED /////////////////

//NEED TO TEST FOR JUMP ADDRESS
int main(){
    int index = 0;
    int i = 0;
    std::string fileName = "";
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
        // //std::cout << "I_MEM[" << index << "] = " << I_MEM[index] << std::endl;
        index += 4;
    }

  

    int size = I_MEM.size();
    if(size == 1){
        std::cout << "\ntotal_clock_cycles " << total_clock_cycles++ << " :\n";
        fetch();
        PC += 4;
        decode();
        execute();
        mem();
        writeBack();
        std::cout << "pc was modified to "; 
        printf("0x%x\n", PC);
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
        }
        else if(i == 2){
            execute();
            decode();
            fetch();
        }
        else if(i == 3){
            mem();
            execute();
            decode();
            fetch();
        }
        else if(i >= 4 && (i < ((size + 5 - 1) - 4))){
            writeBack();
            mem();
            execute();
            decode();
            fetch();
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
            if(i < I_MEM.size()){
                PC += 4;
                std::cout << "pc was modified to "; 
                printf("0x%x\n", PC);
            }
        }
    }
 
    std::cout << "\nprogram terminated:" << std::endl;
    std::cout << "total execution time is " << (total_clock_cycles - 1) <<  " cycles" << std::endl << std::endl;
    
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
    //std::cout<< "----------Fetch Start----------\n";
    if (branch && alu_zero)
        instruction = I_MEM[PC];
    else if (jump)
        instruction = I_MEM[PC];
    else
        instruction = I_MEM[PC];
    
    next_pc = PC + 4;
    //std::cout << << std::endl;
    //std::cout << "if_id.setNext_PC("<< next_pc<<");" << std::endl;
    //std::cout <<"if_id.setInstruction("<<instruction<<");" << std::endl;
    if_id.setNext_PC(next_pc);
    if_id.setInstruction(instruction);
    //std::cout<< "----------Fetch End----------\n";
}

void decode(){
    //std::cout<< "----------Decode Start----------\n";
    opcode = if_id.getInstruction().substr(0,6);
    //std::cout << "opcode: " << opcode << std::endl;
    rs = if_id.getInstruction().substr(6,5);  //ID
    //std::cout << "rs: " << binaryToDecimal(rs) << std::endl;
    rt = if_id.getInstruction().substr(11,5); //ID
    //std::cout << "rt: " << binaryToDecimal(rt) << std::endl;
    rd = if_id.getInstruction().substr(16,5); //ID
    //std::cout << "rd: " << binaryToDecimal(rd) << std::endl;
    immediate = if_id.getInstruction().substr(16,16);
    //std::cout << "immediate: " << immediate << std::endl;
    std::string shamt = if_id.getInstruction().substr(21,5);
    //std::cout << "shamt: " << shamt << std::endl;
    funct = if_id.getInstruction().substr(26,6);
    //std::cout << "funct: " << funct << std::endl;
    
    address = if_id.getInstruction().substr(6,26);
    //std::cout << "address: " << address << std::endl;

    std::string tempImm = immediate;
    //std::cout << "tempImm: " << tempImm << std::endl;

    //Extracting Values at registers ID locations.
    rsVal = registerfile[binaryToDecimal(rs)];
    rdVal = registerfile[binaryToDecimal(rd)];
    rtVal = registerfile[binaryToDecimal(rt)];
    //std::cout << "rsVal: " << rsVal << std::endl;
    //std::cout << "rtVal: " << rtVal << std::endl;
    //std::cout << "rdVal: " << rdVal << std::endl;
    
    //SIGN EXTENSION UNIT to extend 16-bit imm to 32-bit.
    std::string negExt = "1111111111111111";
    std::string posExt = "0000000000000000";
    if(immediate[0] == '0')
        immediate = posExt + immediate;
    else if(immediate[0] == '1')
        immediate = negExt + immediate;
    //std::cout << "immediate: " << immediate << std::endl;

    //Calculating Jump TA --> REVISIT SINCE PIPELINE MIGHT CHANGE THINGS A BIT
    std::string nextPCString = std::bitset<32>(if_id.getNext_PC()).to_string().substr(0,4); //Taking the first four bits of next_pc
    jump_target = nextPCString + address + "00"; //Concatenating: first four bits of next_pc + 26 bits of SLL + 00
    if(jump)
        PC = binaryToDecimal(jump_target); 
    
    controlUnit();

    //Pipeline buffers
    id_exe.setNext_pc(if_id.getNext_PC());
    //std::cout << "id_exe.setNext_pc("<<if_id.getNext_PC()<<")" << std::endl;
    id_exe.setRSVal(rsVal);
    //std::cout << "id_exe.setRSVal("<<rsVal<<")" << std::endl;
    id_exe.setRTVal(rtVal);
    //std::cout << "id_exe.setRTVal("<<rtVal<<")" << std::endl;
    id_exe.setSignExtendedVal(immediate);
    //std::cout << "id_exe.setSignExtendedVal("<<immediate<<")" << std::endl;
    id_exe.setRTID(rt);
    //std::cout << "id_exe.setRTID("<<rt<<")" << std::endl;
    id_exe.setRDID(rd);
    //std::cout << "id_exe.setRDID("<<rd<<")" << std::endl;
    id_exe.setRSID(rs);
    //std::cout << "id_exe.setRSID("<<rs<<")" << std::endl;
    id_exe.setFunct(funct);
    //std::cout << "id_exe.setFunct("<<funct<<")" << std::endl;

    //std::cout<< "----------Decode End----------\n";
    // total_clock_cycles++;
}

void execute(){
    //std::cout<< "----------Execute Start----------\n";
    //Need to determine the proper ALU_OP value that is generated by aluControl.
    aluControl();

    //Pipeline WB control signals. Simply pass them to ex_mem pipeline register.
    ex_mem.setRegWrite(id_exe.getRegWrite());
    //std::cout <<"ex_mem.setRegWrite("<<id_exe.getRegWrite()<<")"<< std::endl;
    ex_mem.setMemToReg(id_exe.getMemToReg());
    //std::cout <<"ex_mem.setMemToReg("<<id_exe.getMemToReg()<<")"<< std::endl;
    //Pipeline M control signals. Simply pass them to ex_mem pipeline register.
    ex_mem.setBranch(id_exe.getBranch());
    //std::cout <<"ex_mem.setBranch("<<id_exe.getBranch()<<")"<< std::endl;
    ex_mem.setMemRead(id_exe.getMemRead());
    //std::cout <<"ex_mem.setMemRead("<<id_exe.getMemRead()<<")"<< std::endl;
    ex_mem.setMemWrite(id_exe.getMemWrite());
    //std::cout <<"ex_mem.setMemWrite("<<id_exe.getMemWrite()<<")"<< std::endl;
    //Discard relevant EX control signals
    //Decide to pass rtID or rdID
    if(id_exe.getRegDst() == 0){
        ex_mem.setRegDst(binaryToDecimal(id_exe.getRTID())); //If RegDst = 0, we will use rt # as register ID.
        //std::cout << "ex_mem.setRegDst(" <<binaryToDecimal(id_exe.getRTID())<< ")\n";
    }
    else if(id_exe.getRegDst() == 1){
        ex_mem.setRegDst(binaryToDecimal(id_exe.getRDID())); //If RegDst = 1, we will use rd # as register ID.
        //std::cout << "ex_mem.setRegDst(" <<binaryToDecimal(id_exe.getRDID())<< ")\n";
    }
        
    rsVal = id_exe.getRSVal();
    //std::cout << "rsVal: " << rsVal << std::endl;
    rtVal = id_exe.getRTVal();
    //std::cout << "rtVal: " << rtVal << std::endl;
    //std::cout<< "ALU_Op: " << ALU_Op << std::endl;
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
        // std::cout <<"LW received.\n";
        int temp = binaryToDecimal(id_exe.getSignExtendedVal()) + registerfile[binaryToDecimal(id_exe.getRSID())];  //tempRegister or destinationRegister?
        ALU_res = std::bitset<32>(temp).to_string();
        // std::cout <<"ALU in ex: " << ALU_res << "\n";
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
    
    //END BRANCH TA DETERMINATION CODE --> HANDLE BRANCH LATER!
    // total_clock_cycles++;

    //Forwarding control signals.
    ex_mem.setRegWrite(id_exe.getRegWrite());
    //std::cout << "ex_mem.setRegWrite("<<id_exe.getRegWrite()<<")" << std::endl;
    ex_mem.setMemToReg(id_exe.getMemToReg());
    ex_mem.setBranch(id_exe.getBranch());
    //std::cout<< "ex_mem.setMemToReg("<<id_exe.getMemToReg()<<")" << std::endl;
    ex_mem.setMemRead(id_exe.getMemRead());
    //std::cout<< "ex_mem.setMemRead("<<id_exe.getMemRead()<<")" << std::endl;
    ex_mem.setMemWrite(id_exe.getMemWrite());
    //std::cout<< "ex_mem.setMemWrite("<<id_exe.getMemWrite()<<")" << std::endl;

    ex_mem.setSum(branch_target);
    //std::cout<< "ex_mem.setSum("<<branch_target<<")" << std::endl;  
    //Forwarding values
    ex_mem.setALUResult(ALU_res);
    //std::cout<< "ex_mem.setALUResult("<<ALU_res<<")" << std::endl;  
    ex_mem.setRTVal(rtVal);
    //std::cout<< "ex_mem.setRTVal("<<rtVal<<")" << std::endl;  
    // ex_mem.setJump(id_exe.getJump()) --> BROKEN FIX LATER.
    //std::cout<< "----------Execute end----------\n";
}

void mem(){
    //Pipeline WB control signals. Simply pass them to ex_mem pipeline register.
    mem_wb.setRegWrite(ex_mem.getRegWrite());
    mem_wb.setMemToReg(ex_mem.getMemToReg());

    //Discard relevant M control signals

    // Not sure what this does? Branch related/Jump (maybe jump)
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
    if(ex_mem.getMemWrite()){
        d_mem[memoryAddress / 4] = ex_mem.getRTVal();
        std::cout << "memory "; printf("0x%x", memoryAddress); std::cout << " modified to "; printf("0x%x\n", ex_mem.getRTVal());
    }
    // total_clock_cycles++;

    //Passing from ex_mem pipeline register to mem_wb pipeline register.
    mem_wb.setRegDst(ex_mem.getRegDst());

    // mem_wb.setReadMemData(lwValue);
    // mem_wb.setRegDst();
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
    //std::cout << "@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#" << std::endl;
    //std::cout << "@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#" << std::endl;
    //std::cout << "@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#@#" << std::endl;
    // total_clock_cycles++;
}

void controlUnit(){
    //std::cout<< "----------Control Unit Start----------\n";
    if(opcode == "000000"){ //Considering R type instructions.
        //std::cout << "opcode =" << opcode << std::endl;
        id_exe.setRegWrite(true); //True - R does write to register.
        //std::cout << "id_exe.setRegWrite("<<id_exe.getRegWrite()<<");" << std::endl;
        id_exe.setRegDst(1); //RegDst MUX selector bit set to 1 since bin 15-11 is destination register.
        //std::cout << "id_exe.setRegDst("<<id_exe.getRegDst()<<");" << std::endl;
        id_exe.setBranch(false); //No branch required for R type.
        //std::cout << "id_exe.setBranch("<<id_exe.getBranch()<<");" << std::endl;
        id_exe.setALUSrc(0); //ALUSrc MUX selector bit to 0 since no sign extension is necessary for R type.
        //std::cout << "id_exe.setALUSrc("<<id_exe.getALUSrc()<<");" << std::endl;
        id_exe.setInstType("10"); //R Type Instructions: ADD, SUB, AND, OR, SLT, NOR
        //std::cout << "id_exe.setInstType("<<id_exe.getInstType()<<");" << std::endl;
        id_exe.setMemWrite(false); //R type instructions do NOT write to memory, leave as default 'false'.
        //std::cout << "id_exe.setMemWrite("<<id_exe.getMemWrite()<<");" << std::endl;
        id_exe.setMemToReg(0); //MemToReg MUX selector bit is left as default 0 since the input value comes from ALU result. 
        //std::cout << "id_exe.setMemToReg("<<id_exe.getMemToReg()<<");" << std::endl;
        id_exe.setMemRead(false); //MemRead is left as default value 'false' since no memory is read for R type.
        //std::cout << "id_exe.setMemRead("<<id_exe.getMemRead()<<");" << std::endl;
        id_exe.setJump(false); //No jump for R-type instructions.
        //std::cout << "id_exe.setJump("<<id_exe.getJump()<<");" << std::endl;
    }
    else if(opcode == "000010"){ //Considering J type
        //std::cout << "opcode =" << opcode << std::endl;
        id_exe.setJump(true); //No jump for R-type instructions.
        //std::cout << "id_exe.setJump("<<id_exe.getJump()<<");" << std::endl;
        id_exe.setRegWrite(false);
        //std::cout << "id_exe.setRegWrite("<<id_exe.getRegWrite()<<");" << std::endl;
        id_exe.setBranch(false);
        //std::cout << "id_exe.setBranch("<<id_exe.getBranch()<<");" << std::endl;
        id_exe.setMemWrite(false);
        //std::cout << "id_exe.setMemWrite("<<id_exe.getMemWrite()<<");" << std::endl;
        id_exe.setMemRead(false);
        //std::cout << "id_exe.setMemRead("<<id_exe.getMemRead()<<");" << std::endl;
    }
    else{ //Considering I type instructions
        if(opcode == "100011") { //Considering LW instruction.
            //std::cout << "opcode =" << opcode << std::endl;
            id_exe.setRegWrite(true); //True - R does write to register.
            //std::cout << "id_exe.setRegWrite("<<id_exe.getRegWrite()<<");" << std::endl;
            id_exe.setRegDst(0); //RegDst MUX selector bit set to 0 since bin 20-16 is destination register for LW.
            //std::cout << "id_exe.setRegDst("<<id_exe.getRegDst()<<");" << std::endl;
            id_exe.setBranch(false); //No branch required for R type.
            //std::cout << "id_exe.setBranch("<<id_exe.getBranch()<<");" << std::endl;
            id_exe.setALUSrc(1); //ALUSrc MUX selector bit to 1 since sign extension is necessary for LW type.
            //std::cout << "id_exe.setALUSrc("<<id_exe.getALUSrc()<<");" << std::endl;
            id_exe.setInstType("00"); 
            //std::cout << "id_exe.setInstType("<<id_exe.getInstType()<<");" << std::endl;
            id_exe.setMemWrite(false); //LW instruction does NOT write to memory, leave as default 'false'.
            //std::cout << "id_exe.setMemWrite("<<id_exe.getMemWrite()<<");" << std::endl;
            id_exe.setMemToReg(1); //MemToReg MUX selector bit is set to 1 since the input value comes from the memory. 
            //std::cout << "id_exe.setMemToReg("<<id_exe.getMemToReg()<<");" << std::endl;
            id_exe.setMemRead(true); //MemRead is set to true since memory is read for LW.
            //std::cout << "id_exe.setMemRead("<<id_exe.getMemRead()<<");" << std::endl;
            id_exe.setJump(false); //No jump for LW instructions.
            //std::cout << "id_exe.setJump("<<id_exe.getJump()<<");" << std::endl;
        }
        else if (opcode == "000100") { //Considering beq instruction.
            //std::cout << "opcode =" << opcode << std::endl;
            id_exe.setRegWrite(false); //False - beq does not write to register.
            //std::cout << "id_exe.setRegWrite("<<id_exe.getRegWrite()<<");" << std::endl;
            id_exe.setBranch(true); //Brancing required for beq.
            //std::cout << "id_exe.setBranch("<<id_exe.getBranch()<<");" << std::endl;
            id_exe.setALUSrc(0); //ALUSrc MUX selector bit to 0 since no sign extension is necessary for beq.
            //std::cout << "id_exe.setALUSrc("<<id_exe.getALUSrc()<<");" << std::endl;
            id_exe.setInstType("01"); 
            //std::cout << "id_exe.setInstType("<<id_exe.getInstType()<<");" << std::endl;
            id_exe.setMemWrite (false); //Beq instruction does NOT write to memory, leave as default 'false'.
            //std::cout << "id_exe.setMemWrite("<<id_exe.getMemWrite()<<");" << std::endl;
            id_exe.setMemRead (false); //MemRead is set to true since memory is not read for beq.
            //std::cout << "id_exe.setMemRead("<<id_exe.getMemRead()<<");" << std::endl;
            id_exe.setJump(false); //No jump for beq instructions.
            //std::cout << "id_exe.setJump("<<id_exe.getJump()<<");" << std::endl;
        }
        else if (opcode == "101011"){ //Considering SW instruction
            //std::cout << "opcode =" << opcode << std::endl;
            id_exe.setRegWrite(false); //False - SW does write to register.
            //std::cout << "id_exe.setRegWrite("<<id_exe.getRegWrite()<<");" << std::endl;
            id_exe.setRegDst(0);
            //std::cout << "id_exe.setRegDst("<<id_exe.getRegDst()<<");" << std::endl;          
            id_exe.setBranch(false); //No branch required for SW.
            //std::cout << "id_exe.setBranch("<<id_exe.getBranch()<<");" << std::endl;
            id_exe.setALUSrc(1); //ALUSrc MUX selector bit to 1 since sign extension is necessary for SW.
            //std::cout << "id_exe.setALUSrc("<<id_exe.getALUSrc()<<");" << std::endl;
            id_exe.setInstType("00");
            //std::cout << "id_exe.setInstType("<<id_exe.getInstType()<<");" << std::endl;
            id_exe.setMemWrite(true); //SW instructions do write to memory.
            //std::cout << "id_exe.setMemWrite("<<id_exe.getMemWrite()<<");" << std::endl;
            // MemtoReg = X?
            id_exe.setMemRead(false); //MemRead is left as default value 'false' since no memory is read for SW.
            ////std::cout << "id_exe.setMemRead("<<id_exe.getMemRead()<<");" << std::endl;
            id_exe.setJump(false); //No jump for SW instruction.
            //std::cout << "id_exe.setJump("<<id_exe.getJump()<<");" << std::endl;
        }
    }
    //std::cout<< "----------Control Unit End----------\n";
}

//Opcode and funct field together determine ALU Op
void aluControl(){
    //std::cout<< "----------aluControl start----------\n";
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
    //std::cout<< "ALU_Op: " << ALU_Op << "\n";
    //std::cout<< "----------aluControl end----------\n";
}


