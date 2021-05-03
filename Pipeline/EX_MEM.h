#ifndef EX_MEM_H
#define EX_MEM_H


class EX_MEM {
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
    EX_MEM();
    //Getters
    int getSum() const;
    int getZero() const;
    std::string getALUResult() const;
    int getRTVal() const;
    int getRegDst() const;//Gets register ID pipelined from execute stage.
    //MEM getters
    bool getBranch() const;
    bool getMemRead() const;
    bool getMemWrite() const;
    //WB getters
    bool getRegWrite() const;
    int getMemToReg() const;
    bool getJump() const;

    //Setters
    void setSum(int sum);
    void setZero(int zero);
    void setALUResult(std::string ALUresult);
    void setRTVal(int rtVal);
    void setRegDst(int regDST); //Sets register ID to be pipelined according to RegDst mux.
    //MEM setters
    void setBranch(bool branch);
    void setMemRead(bool MemRead);
    void setMemWrite(bool MemWrite);
    //WB setters
    void setRegWrite(bool RegWrite);
    void setMemToReg(int MemtoReg);
    void setJump(bool jump);
};

#endif

EX_MEM::EX_MEM(){};
//Getters
int EX_MEM::getSum() const{
    return sum;
}
int EX_MEM::getZero() const{
    return zero;
}
std::string EX_MEM::getALUResult() const{
    return ALUresult;
}
int EX_MEM::getRTVal() const{
    return rtVal;
}
int EX_MEM::getRegDst() const{
    return regDST;
} //Gets register ID pipelined from execute stage.
//MEM getters
bool EX_MEM::getBranch() const{
    return branch;
}
bool EX_MEM::getMemRead() const{
    return MemRead;
}
bool EX_MEM::getMemWrite() const{
    return MemWrite;
}
//WB getters
bool EX_MEM::getRegWrite() const{
    return RegWrite;
}
int EX_MEM::getMemToReg() const{
    return MemtoReg;
}
bool EX_MEM::getJump() const{
    return jump;
}

//Setters
void EX_MEM::setSum(int sum){ 
    this->sum= sum;
}
void EX_MEM::setZero(int zero){
    this->zero = zero;
}
void EX_MEM::setALUResult(std::string ALUresult){
    this-> ALUresult = ALUresult;
}
void EX_MEM::setRTVal(int rtVal){
    this->rtVal = rtVal;
}
void EX_MEM::setRegDst(int regDST){
    this-> regDST = regDST;
} //Sets register ID to be pipelined according to RegDst mux.
//MEM setters
void EX_MEM::setBranch(bool branch){
    this->branch = branch;
}
void EX_MEM::setMemRead(bool MemRead){
    this->MemRead = MemRead;
}
void EX_MEM::setMemWrite(bool MemWrite){
    this->MemWrite = MemWrite;
}
//WB setters
void EX_MEM::setRegWrite(bool RegWrite){
    this->RegWrite = RegWrite;
}
void EX_MEM::setMemToReg(int MemtoReg){
    this->MemtoReg = MemtoReg;
}
void EX_MEM::setJump(bool jump){
    this->jump = jump;
}