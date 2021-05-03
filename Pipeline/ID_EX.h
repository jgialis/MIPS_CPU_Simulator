#ifndef ID_EX_H
#define ID_EX_H

class ID_EX {
    
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
    ID_EX();
    //Getters
    int getNext_pc() const;
    int getRSVal() const;
    int getRTVal() const;
    std::string getFunct() const;
    std::string getRSID() const;
    std::string getRTID() const;
    std::string getRDID() const;
    std::string getSignExtendedVal() const;

    //EX getters
    int getRegDst() const;
    std::string getInstType() const;
    int getALUSrc() const;
    //MEM getters
    bool getBranch() const;
    bool getMemRead() const;
    bool getMemWrite() const;
    //WB getters
    bool getRegWrite() const;
    int getMemToReg() const;
    bool getJump() const;

    //Setters
    void setNext_pc(int next_pc);
    void setRSVal(int rsVal);
    void setRTVal(int rtVal);
    void setFunct(std::string funct);
    void setRSID(std::string rsID);
    void setRTID(std::string rtID);
    void setRDID(std::string rdID);
    void setSignExtendedVal(std::string signExtendedVal);

    //EXE setters
    void setRegDst(int RegDst);
    void setInstType(std::string InstType);
    void setALUSrc(int ALUSrc);
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
    
ID_EX::ID_EX(){};
//Getters
int ID_EX::getNext_pc() const{
    return next_pc;
}
int ID_EX::getRSVal() const{
    return rsVal;
}
int ID_EX::getRTVal() const{
    return rtVal;
}
std::string ID_EX::getFunct() const{
    return funct;
}
std::string ID_EX::getRSID() const{
    return rsID;
}
std::string ID_EX::getRTID() const{
    return rtID;
}
std::string ID_EX::getRDID() const{
    return rdID;
}
std::string ID_EX::getSignExtendedVal() const{
    return signExtendedVal;
}

//EX getters
int ID_EX::getRegDst() const{
    return RegDst;
}
std::string ID_EX::getInstType() const{
    return InstType;
}
int ID_EX::getALUSrc() const{
    return ALUSrc;
}
//MEM getters
bool ID_EX::getBranch() const{
    return branch;
}
bool ID_EX::getMemRead() const{
    return MemRead;
}
bool ID_EX::getMemWrite() const{
    return MemWrite;
}
//WB getters
bool ID_EX::getRegWrite() const{
    return RegWrite;
}
int ID_EX::getMemToReg() const{
    return MemtoReg;
}
bool ID_EX::getJump() const{
    return jump;
}

//Setters
void ID_EX::setNext_pc(int next_pc){
    this->next_pc= next_pc;
}
void ID_EX::setRSVal(int rsVal){
    this->rsVal = rsVal;
}
void ID_EX::setRTVal(int rtVal){
    this-> rtVal = rtVal;
}
void ID_EX::setFunct(std::string funct){
    this-> funct = funct;
}
void ID_EX::setRSID(std::string rsID){
    this-> rsID = rsID;
}
void ID_EX::setRTID(std::string rtID){
    this->rtID = rtID;
}
void ID_EX::setRDID(std::string rdID){
    this->rdID = rdID;
}
void ID_EX::setSignExtendedVal(std::string signExtendedVal){
    this->signExtendedVal = signExtendedVal;
}

//EXE setters
void ID_EX::setRegDst(int RegDst){
    this->RegDst = RegDst;
}
void ID_EX::setInstType(std::string InstType){
    this->InstType = InstType;
}
void ID_EX::setALUSrc(int ALUSrc){
    this->ALUSrc = ALUSrc;
}
//MEM setters
void ID_EX::setBranch(bool branch){
    this->branch = branch;
}
void ID_EX::setMemRead(bool MemRead){
    this->MemRead = MemRead;
}
void ID_EX::setMemWrite(bool MemWrite){
    this->MemWrite = MemWrite;
}
//WB setters
void ID_EX::setRegWrite(bool RegWrite){
    this->RegWrite = RegWrite;
}
void ID_EX::setMemToReg(int MemtoReg){
    this->MemtoReg = MemtoReg;
}
void ID_EX::setJump(bool jump){
    this->jump = jump;
}