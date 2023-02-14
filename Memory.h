#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <vector>
#include <map>
#include <string>

class Memory
{

public:

    Memory();
    bool find_command(std::string, std::map< int, int > &,
                      std::map< int, std::string > &, bool);
    bool validate_word_byte(std::string, int, std::vector<std::string>&,
                            std::map< int, std::string >&)const;
    
    bool validate_asciiz(std::string, int, std::vector<std::string>&)const;
    bool validate_space(std::string, int, std::vector<std::string>&)const;
    void add_word(std::map< int, int >&, std::vector< std::string > &,
                  std::map< int, std::string >&);
    
    void add_byte(std::map< int, int >&, std::vector< std::string > &,
                  std::map< int, std::string >&);
    
    void add_string(std::map< int, int >&, std::vector< std::string > &,
                    std::map< int, std::string >&);
    
    void add_space(std::map< int, int >&, std::vector< std::string > &,
                   std::map< int, std::string >&);
    
    void print_data(std::map< int, int > &)const;
    void print_labels(std::map< int, std::string >&)const;
    
private:
    std::string data_set[5] = {".word", ".asciiz", ".ascii", ".space", ".byte"};
    int address = 268435456; // how we get the address of our data stuff
    std::vector< std::string > label_holder;
   //std::map< int, std::string> label_set;

};


#endif
