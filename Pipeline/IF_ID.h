#ifndef IF_ID_H
#define IF_ID_H


class IF_ID {
    
   std::string instruction;
   int next_pc;

public:

    IF_ID();
    std::string getInstruction() const;
    int getNext_PC() const;
    void setInstruction(std::string instruction);
    void setNext_PC(int next_pc);
    ~IF_ID();
};

#endif

IF_ID::IF_ID(){}

std::string IF_ID::getInstruction() const{
    return this->instruction;
}

int IF_ID::getNext_PC() const{
    return this->next_pc;
}

void IF_ID::setNext_PC(int next_pc){
    this->next_pc = next_pc;
}

void IF_ID::setInstruction(std::string instruction){
    this->instruction = instruction;
}

IF_ID::~IF_ID(){}