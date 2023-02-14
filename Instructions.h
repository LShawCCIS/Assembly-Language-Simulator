#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "Register.h"
#include <map>

class Instructions
{
  public:

    Instructions();
    bool find_command(std::string, Register &, std::map< int, int >&, std::map < int, std::string >&,
                      bool)const;
    bool validate_3reg(std::string, int, std::vector< std::string >&)const;
    bool validate_2regI(std::string, int, std::vector< std::string>&)const;
    bool validate_2reg(std::string, int, std::vector< std::string> &)const;
    bool validate_1regI(std::string, int, std::vector< std::string >&)const;
    bool validate_1reg(std::string, int, std::vector< std::string >&)const;
    bool validate_syscall(std::string, int, std::vector< std::string >&)const;
    bool validate_word(std::string, int, std::vector< std::string >&)const;
    bool validate_wordL(std::string, int, std::vector< std::string >&, std::map< int, std::string> &)const;
    bool validate_load_addr(std::string, int, std::vector< std::string >&, std::map< int, std::string> &)const;
    bool validate_load_addrH(std::string, int, std::vector< std::string >&, std::map< int, int> &)const;

    // in order to check some of our functions, we need to go through 2 different
    // syntax possibilites. no_print signifies that this is function copied from
    // above, but with no error messages
    bool validate_3reg_no_print(std::string, int, std::vector< std::string >&)const;

    
    
  private:
    std::string instruction_set[31] = {
                         "add",  "addi",  "addiu",   "sub",  "subu", "mul",
                         "mult", "multu", "div",     "divu", "move", "mfhi",
                         "mflo", "li",    "syscall", "la",   "j",    "slt",
                         "sgt",  "sle",   "sge",  "sltu", "sgtu", "sgeu", 
                         "sleu", "bgt",  "blt",  "bge", "ble",  "lw",
                         "sw"};
    
    std::string register_set[28] = {"$0",  "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
                                    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
                                    "$t8", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6",
                                    "$s7", "$t8", "$t9", "$HI", "$LO"};
    
    std::vector< unsigned int > registers;

};

#endif
