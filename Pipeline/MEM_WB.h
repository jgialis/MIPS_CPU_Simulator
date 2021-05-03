#ifndef MEM_WB_H
#define MEM_WB_H


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
    MEM_WB();
    std::string getALUResult() const;
    int getReadMemData() const;
    int getRegDst() const;
    //WB getters
    bool getRegWrite() const;
    int getMemToReg() const;
    bool getJump() const;

    void setALUResult(std::string ALUresult);
    void setReadMemData(int readMemData);
    void setRegDst(int regDST);
    //WB setters
    void setRegWrite(bool RegWrite);
    void setMemToReg(int MemtoReg);
    void setJump(bool jump);
};

#endif

MEM_WB::MEM_WB(){}

std::string MEM_WB::getALUResult() const{
    return ALUresult;
}
int MEM_WB::getReadMemData() const{
    return readMemData;
}
int MEM_WB::getRegDst() const{
    return regDST;
}
//WB getters
bool MEM_WB::getRegWrite() const{
    return RegWrite;
}
int MEM_WB::getMemToReg() const{
    return MemtoReg;
}
bool MEM_WB::getJump() const{
    return jump;
}

void MEM_WB::setALUResult(std::string ALUresult){
    this-> ALUresult = ALUresult;
}
void MEM_WB::setReadMemData(int readMemData){
    this-> readMemData = readMemData;
}
void MEM_WB::setRegDst(int regDST){
    this-> regDST = regDST;
}
//WB setters
void MEM_WB::setRegWrite(bool RegWrite){
    this->RegWrite = RegWrite;
}
void MEM_WB::setMemToReg(int MemtoReg){
    this->MemtoReg = MemtoReg;
}
void MEM_WB::setJump(bool jump){
    this->jump = jump;
}