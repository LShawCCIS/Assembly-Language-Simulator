#ifndef REGISTER_H
#define REGISTER_H

#include <vector>
#include <iomanip>
#include <string>
#include <map>


class Register
{
  public:

    Register();
    void clear_registers();
    void add(std::vector<std::string>& parts, const std::string reg_set[]);
    void addi_2r(std::vector<std::string>& parts, const std::string reg_set[]);
    void addi_1r(std::vector<std::string>& parts, const std::string reg_set[]);
    void addiu(std::vector<std::string>& parts, const std::string reg_set[]);
    void sub(std::vector<std::string>& parts, const std::string reg_set[]);
    
    void mul(std::vector<std::string>& parts, const std::string reg_set[]);
    void mult(std::vector<std::string>& parts, const std::string reg_set[]);
    void div_3r(std::vector<std::string>& parts, const std::string reg_set[]);
    void div_2r(std::vector<std::string>& parts, const std::string reg_set[]);
    void move(std::vector<std::string>& parts, const std::string reg_set[]);
    void mfhi(std::vector<std::string>& parts, const std::string reg_set[]);
    void mflo(std::vector<std::string>& parts, const std::string reg_set[]);

    void load_i(std::vector<std::string>& parts, const std::string set[]);
    
    void slt_2(std::vector<std::string>& parts, const std::string set[], bool);
    void slt_3(std::vector<std::string>& parts, const std::string set[], bool);
    void sgt_2(std::vector<std::string>& parts, const std::string set[], bool);
    void sgt_3(std::vector<std::string>& parts, const std::string set[], bool);
    void sle_2(std::vector<std::string>& parts, const std::string set[], bool);
    void sle_3(std::vector<std::string>& parts, const std::string set[], bool);
    void sge_2(std::vector<std::string>& parts, const std::string set[], bool);
    void sge_3(std::vector<std::string>& parts, const std::string set[], bool);

    void syscall(std::vector<std::string>& parts, const std::string set[],
                 std::map< int, int >&, std::map< int, std::string >&);

    void la(std::vector<std::string>&, const std::string set[],
            std::map<int, std::string> & label_memory);

    void la_H(std::vector<std::string>&, const std::string set[],
            std::map<int, std::string> & label_memory);

    void lw(std::vector<std::string>&, const std::string set[],
            std::map<int, int> & data_memory);

    void lw_H(std::vector<std::string>&, const std::string set[],
              std::map<int, std::string> & label_memory, std::map<int,int> &);

    void sw(std::vector<std::string>&, const std::string set[],
            std::map<int, int> & data_memory);

    void sw_H(std::vector<std::string>&, const std::string set[],
              std::map<int, std::string> & label_memory, std::map<int,int> &);
 
    void print_register_list();
        
  private:
    std::vector < unsigned int > registers;
    std::string register_set[28] = {"$0",  "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
                                    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
                                    "$t8", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6",
                                    "$s7", "$t8", "$t9", "$HI", "$LO"};

};

#endif
