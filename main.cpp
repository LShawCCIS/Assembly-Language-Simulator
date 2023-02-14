#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "Instructions.h"
#include "Register.h"
#include "Memory.h"
#include <fstream>
#include <sstream>
#include <cstring>


void intro();
void help();
void registers(Instructions&);
void print_current_(std::vector<std::string>&, std::vector<std::string>&);
void print_text_record(std::vector<std::string>&);
void print_data_record(std::vector<std::string>&);
void save_file(std::vector< std::string >, std::vector<std::string>);
void clear_registers(Register &);
void check_start_(std::string, int &, int &, int &, std::vector<std::string>&);
void check_data_start(std::string, int &, std::vector<std::string>&);
bool load_text_segment(std::string);
bool load_data_segment(std::string);
void re_run(std::vector<std::string>&, std::vector<std::string>&,
            std::map<int, int>&, std::map<int, std::string>&, Register &);
void clear_memory(std::map<int, int> &, std::map<int, std::string> &);

static unsigned int spaces = 4;

int main()
{
    intro();
    int pass = 0;
    int text = 0;
    int globl = 0;
    int main = 0;
    int data = 0;
    bool in_text = true;
    bool in_data = false;
    bool in_batch = false;
    std::string input;
    std::string myfile;
    Instructions instruc;
    Memory memory;
    Register reg;

    std::vector< std::string > text_record;
    std::vector< std::string > data_record;
    std::map< int, int > data_memory;
    std::map< int, std::string > label_memory;
    

    //std::ofstream f(myfile, std::ios::out);
    //f << "\t .text\n" << "\t .globl main\n\n" << "main: \t";
    

    while (1)
    {
        if (in_batch)
        {
            std::cout << "BATCH ";
        }
        if (in_text)
        {
            std::cout << "TEXT > ";
        }
        if (in_data)
        {
            std::cout << "DATA > ";
        }
        
  
        std::getline(std::cin, input);

        if (input.compare("help") == 0)
        {
            help();
        }
        else if (input.compare("r") == 0)
        {
            reg.print_register_list();
        }
        else if (input.compare("quit") == 0)
        {
            break;
        }
        
        else if (input.compare(".text") == 0 && text < 1)
        {
            text_record.push_back("\t.text\n");
            text++;
        }
        
        else if (input.compare(".globl main") == 0 && globl < 1)
        {
            text_record.push_back("\t.globl main\n\n");
            globl++;
        }
        else if (input.compare("main:") == 0 && main < 1)
        {
            if (text < 1 && globl < 1)
            {
                text_record.push_back("\t.text\n");
                text++;
                text_record.push_back("\t.globl main\n\n");
                globl++;
                text_record.push_back("main:\n");
                main++;
            }
            else if (text < 1 && globl >= 1)
            {
                text_record.push_back("\t.text\n");
                text++;
                text_record.push_back("main:\n");
                main++;
            }
            else if (globl < 1 && text >= 1)
            {
                text_record.push_back("\t.globl main\n\n");
                globl++;
                text_record.push_back("main:\n");
                main++;
            }
            else
            {
                text_record.push_back("main:\n");
                main++;
            }
            
        }

        else if (input.compare(".data") == 0 && data < 1)
        {
             data_record.push_back("\t.data\n");
             data++;
        }       
        
        else if (input.compare("print") == 0)
        {
            std::cout << std::endl;
            print_current_(text_record, data_record);
            std::cout << std::endl;
        }

        else if (input.compare("ptext") == 0)
        {
            std::cout << std::endl;
            print_text_record(text_record);
            std::cout << std::endl;
        }
        
        else if (input.compare("pdata") == 0)
        {
            std::cout << std::endl;
            print_data_record(data_record);
            std::cout << std::endl;
        }

        else if (input.compare("pdata_m") == 0)
        {
            std::cout << std::endl;
            memory.print_data(data_memory);
            std::cout << std::endl;
        }

        else if (input.compare("plabel") == 0)
        {
            std::cout << std::endl;
            memory.print_labels(label_memory);
            std::cout << std::endl;
        }
        
        else if (input.compare("save") == 0)
        {
            std::cout << "Saving file..." << std::endl;
            save_file(text_record,  data_record);
            std::cout << "File Saved" << std::endl;
        }
        
        else if (input.compare("clear") == 0)
        {
            clear_registers(reg);
            clear_memory(data_memory, label_memory);
        }

        else if (input.compare("load") == 0)
        {
            std::cout << "Please enter your file name." << std::endl;
            std::cout << "Example: myfile.txt" << std::endl;
            std::getline(std::cin, myfile);
            std::string line;
            bool in_text = true;
            bool in_data = false;
            int counter1 = 0; // counting lines until we find data
            int counter2 = 0; // counting until we reach data
            
            std::ifstream file(myfile);
            if (file.is_open())
            {
                while (std::getline(file, line))
                {
                    std::cout << line << std::endl;
                    bool temp = load_data_segment(line);
                    counter1++;
                    if (temp)
                    {
                        in_data = true;
                    }
                    if(in_data)
                    {
                        bool valid = true;
                        if (!temp)
                        {
                            //std::cout << "yo" << '\n';
                            valid = memory.find_command(line, data_memory,
                                                        label_memory, false);
                        }
                        if (!valid)
                        {
                            //std::cout << "y" << '\n';
                            file.close();
                            break;
                        }
                    }
                }
            }
            file.close();
            std::ifstream file2(myfile);
            if (file2.is_open())
            {
                while (std::getline(file2, line))
                {
                    counter2++;
                    bool temp = load_text_segment(line);
                    if (counter2 < counter1 - 1)
                    {
                        if (!temp)
                        {
                            bool valid = instruc.find_command(line, reg,
                                                              data_memory, label_memory, false);
                            if (!valid)
                            {
                                file.close();
                                break;
                            }
                        }
                    }
                }
            }
            file2.close();         
        }
        else if (input.compare("data") == 0)
        {
            in_text = false;
            in_data = true;
        }
        else if (input.compare("text") == 0)
        {
            in_text = true;
            in_data = false;
        }
        else if (input.compare("b") == 0)
        {
            in_batch = true;    
        }
        else if (input.compare("i") == 0)
        {
            in_batch = false;
            re_run(text_record, data_record, data_memory, label_memory, reg);
        }
        else if (input.compare("run") == 0)
        {
            re_run(text_record, data_record, data_memory, label_memory, reg);
        }
// BATCH ======================================================================
        else if (in_batch)
        {
            if (in_text)
            {
                if (text == 0 || globl == 0 || main == 0)
                {
                    check_start_(input, text, globl, main, text_record);    
                } 
                pass++; // make sure we put text, globl main, and main in once
                bool valid = instruc.find_command(input, reg,
                                                  data_memory, label_memory, true);        
                if (valid)
                {
                    if (text < 1)
                    {
                        text_record.push_back("\t.text\n");
                        text++;
                    }
                    if (globl < 1)
                    {
                        text_record.push_back("\t.globl main\n");
                        globl++;
                    }
                    if (main < 1)
                    {
                        text_record.push_back("main:\n");
                        main++;
                    }
                    text_record.push_back(input + "\n");
                }
            }
            else if (in_data)
            {
                if (data == 0)
                {
                    check_data_start(input, data, data_record);    
                }
                
                bool valid = memory.find_command(input, data_memory,
                                                 label_memory, true);
                if (valid)
                {
                    if (data < 1)
                    {
                        data_record.push_back("\t.data\n");
                        data++;
                    }
                    
                    data_record.push_back(input + '\n');
                }
            }
        }
//=============================================================================
        else
        {
            if (in_text)
            {
                if (text == 0 || globl == 0 || main == 0)
                {
                    check_start_(input, text, globl, main, text_record);    
                } 
                pass++; // make sure we put text, globl main, and main in once
                bool valid = instruc.find_command(input, reg,
                                                  data_memory, label_memory, false);
                
                if (valid)
                {
                    if (text < 1)
                    {
                        text_record.push_back("\t.text\n");
                        text++;
                    }
                    if (globl < 1)
                    {
                        text_record.push_back("\t.globl main\n");
                        globl++;
                    }
                    if (main < 1)
                    {
                        text_record.push_back("main:\n");
                        main++;
                    }
                    text_record.push_back(input + "\n");
                }
            }
            else if (in_data)
            { 
                if (data == 0)
                {
                    check_data_start(input, data, data_record);    
                }
                
                bool valid = memory.find_command(input, data_memory,
                                                 label_memory, false);
                if (valid)
                {
                    if (data < 1)
                    {
                        data_record.push_back("\t.data\n");
                        data++;
                    }
                    
                    data_record.push_back(input + '\n');
                }
            }
            
        }
        
        
    }
  
    return 0;
    
}

void intro()
{
    std::cout << std::setw(spaces) << "";
    std::cout << "\n--------------------------------------\n"
              << "\n|   Welcome to Landon's MIPS32 C++   |\n"
              << "\n|            Interpreter!            |\n"
              << "\n|   In order to begin, just start    |\n"
              << "\n|              typing.               |\n"
              << "\n|   If you need the commands type    |\n"
              << "\n|        help to see the list!       |\n"
              << "\n--------------------------------------\n";
    std::cout << std::endl;
}

void help()
{
      std::cout << "-------------------------------------------\n"
                << "|                COMMANDS:                |\n"
                << "| help   - Shows you the list of commands |\n"
                << "| r      - Prints register list           |\n"
                << "| save   - Saves your file                |\n"
                << "| load   - Loads a specified file         |\n"
                << "| clear  - Clears registers               |\n"
                << "| quit   - Exits the program              |\n"
                << "| run    - runs the current program       |\n"
                << "| text   - switches to text segment       |\n"
                << "| ptext  - prints the text segement       |\n"
                << "| data   - Switches to data segment       |\n"
                << "| pdata  - prints the data segment        |\n"
                << "| plabel - Prints the label list          |\n"
                << "| print  - prints the current file        |\n"
                << "| b      - puts the user in batch mode    |\n"
                << "| i      - takes user out of batch        |\n"
                << "-------------------------------------------\n";

      std::cout << std::endl;
}

void print_text_record(std::vector<std::string>& text_record)
{
    for (int i = 0; i < text_record.size(); i++)
    {
        std::cout << text_record[i];
    }
}

void print_data_record(std::vector<std::string>& data_record)
{
    for (int i = 0; i < data_record.size(); i++)
    {
        std::cout << data_record[i];
    }
}

void print_current_(std::vector<std::string>& text_record,
                       std::vector<std::string>& data_record)
{
    for (int i = 0; i < text_record.size(); i++)
    {
        std::cout << text_record[i];
    }

    for (int i = 0; i < data_record.size(); i++)
    {
        std::cout << data_record[i];
    }
}

void save_file(std::vector< std::string > text_record,
               std::vector< std::string > data_record)
{
    std::string myfile;
    std::cout << "Please enter in the name of the file you want to create.\n";
    std::getline(std::cin, myfile);
    
    std::ofstream f(myfile, std::ios::out);
    for (int i = 0; i < text_record.size(); i++)
    {
        f << text_record[i];
    }

    for (int i = 0; i < data_record.size(); i++)
    {
        f << data_record[i];
    }
    f.close();
}

void clear_registers(Register & reg)
{
    reg.clear_registers();
}
// clears our data section and labels from data
void clear_memory(std::map<int, int> & data_memory, std::map<int, std::string> & label_memory)
{
    data_memory.clear();
    label_memory.clear();
}

// this function checks to see whether or not we entered in
// globl main, .text, if we didn't it will put them in
// no matter what. Even if there are errors in the input
void check_start_(std::string input, int & text, int & gm, int & m,
                  std::vector<std::string>& text_record)
{
    std::string find_t = ".text";
    std::string find_gm = ".globl main";
    std::string find_m = "main:";

    size_t found = input.find(find_t);
    if (found != std::string::npos && text < 1)
    {
        text++;
        text_record.push_back("\t" + find_t + "\n");
    }

    found = input.find(find_gm);
    if (found != std::string::npos && gm < 1)
    {
        gm++;
        text_record.push_back("\t" + find_gm + "\n");
    }

    found = input.find(find_m);
    if (found != std::string::npos && m < 1)
    {
        if (text < 1 && gm < 1)
        {
            text++;
            text_record.push_back("\t" + find_t + "\n");
            gm++;
            text_record.push_back("\t" + find_gm + "\n");
            m++;
            text_record.push_back(find_m + "\n");
        }
        else if (text < 1 && gm >= 1)
        {
            text++;
            text_record.push_back("\t" + find_t + "\n");
            m++;
            text_record.push_back(find_m + "\n");
        }
        else if (gm < 1 && text >= 1)
        {
            gm++;
            text_record.push_back("\t" + find_gm + "\n");
            m++;
            text_record.push_back(find_m + "\n");
        }
        else
        {
            m++;
            text_record.push_back(find_m + "\n");
        }
        
    }
}


// this function checks to see whether or not we entered in
// .data if we didn't it will put them in
// no matter what. Even if there are errors in the input
void check_data_start(std::string input, int & data,
                  std::vector<std::string>& data_record)
{
    std::string find_d = ".data";

    size_t found = input.find(find_d);
    if (found != std::string::npos && data < 1)
    {
        data++;
        data_record.push_back("\t" + find_d + "\n");
    }
}

bool load_text_segment(std::string line)
{
    std::string text = ".text";
    std::string gm = ".globl";
    std::string main = "main:";
    std::string command;

    int i = 0;
    while (i < line.size())
    {
        if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\0')
        {
            i++;
        }
        else
        {
            while ((line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\0') && i < line.size())
            {
                command.push_back(line[i]);
                i++;
            }
            break;
        }
    }

    if (command.compare(text) == 0)
    {
        return true;
    }
    else if (command.compare(main) == 0)
    {
        return true;
    }
    else if (command.compare(gm) == 0)
    {
        while(i < line.size())
        {
            command.push_back(line[i]);
            i++;
        }
        if (command.compare(gm + " main") == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    
}

bool load_data_segment(std::string line)
{
    std::string d = ".data";
    std::string command;
    int i = 0;
    while (i < line.size())
    {
        if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\0')
        {
            i++;
        }
        else
        {
            while ((line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\0') && i < line.size())
            {
                command.push_back(line[i]);
                i++;
            }
            break;
        }
    }

    if (command.compare(d) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

void re_run(std::vector<std::string>& text_record,
            std::vector<std::string>& data_record,
            std::map<int, int>& data_memory,
            std::map<int, std::string>& label_memory, Register & reg)
{
    clear_registers(reg);
    clear_memory(data_memory, label_memory);
    Instructions instruc;
    Memory mem;
    bool valid;
    
    for (int i = 0; i < data_record.size(); i++)
    {
        mem.find_command(data_record[i], data_memory, label_memory, false);
    }

    for (int i = 0; i < text_record.size(); i++)
    {
        instruc.find_command(text_record[i], reg, data_memory, label_memory, false);
    }

}

