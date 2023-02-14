#include "Register.h"
#include <iostream>

Register::Register()
{
    for (int i = 0; i < 28; i++)
    {
        registers.push_back(0);
    }
}
// ===========================================================================

void Register::clear_registers()
{
    for(int i = 0; i < 28; i++)
    {
        registers[i] = 0;
    }
}
// ===========================================================================
void Register::add(std::vector<std::string>& parts, const std::string set[])
{
    unsigned int i, j, k; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(set[m]) == 0)
        {
            i = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(set[m]) == 0)
        {
            j = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[2].compare(set[m]) == 0)
        {
            k = m;
        }
    }
    if (i != 0)
        registers[i] = registers[j] + registers[k];
}
// ===========================================================================
// ===========================================================================
void Register::addi_2r(std::vector<std::string>& parts, const std::string set[])
{
    unsigned int i, j, k; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(set[m]) == 0)
        {
            i = m;
            break;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(set[m]) == 0)
        {
            j = m;
            break;
        }
    }
    
    k = stoi(parts[2]);
    if (i != 0)
        registers[i] = registers[j] + k;
}
// ===========================================================================
// ===========================================================================
void Register::addi_1r(std::vector<std::string>& parts, const std::string set[])
{
    unsigned int i, j; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(set[m]) == 0)
        {
            i = m;
            break;
        }
    }
    j = stoi(parts[1]);
    if (i != 0)
        registers[i] += j;
}
// ===========================================================================
// ===========================================================================
void Register::load_i(std::vector<std::string>& parts, const std::string set[])
{
    unsigned int i; // index holders of registers
    int j;        

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(set[m]) == 0)
        {
            i = m;
            break;
        }
    }
    j = stoi(parts[1]);
    if (i != 0)
        registers[i] = j;
    
}
// ===========================================================================
// ===========================================================================
void Register::sub(std::vector<std::string>& parts, const std::string reg_set[])
{
    unsigned int i, j, k; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(reg_set[m]) == 0)
        {
            i = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(reg_set[m]) == 0)
        {
            j = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[2].compare(reg_set[m]) == 0)
        {
            k = m;
        }
    }
    if (i != 0)
        registers[i] = registers[j] - registers[k];
}
// ===========================================================================
// ===========================================================================
void Register::mul(std::vector<std::string>& parts, const std::string reg_set[])
{
    unsigned int i, j, k; // index holders of registers

    for (int m = 0; m < 26; m++)
    {
        if (parts[0].compare(reg_set[m]) == 0)
        {
            i = m;
        }
    }

    for (int m = 0; m < 26; m++)
    {
        if (parts[1].compare(reg_set[m]) == 0)
        {
            j = m;
        }
    }

    for (int m = 0; m < 26; m++)
    {
        if (parts[2].compare(reg_set[m]) == 0)
        {
            k = m;
        }
    }

    registers[27] = registers[j] * registers[k]; // LO
    if (i != 0)
        registers[i] = registers[j] * registers[k];
}
// ===========================================================================
// ===========================================================================
void Register::mult(std::vector<std::string>& parts, const std::string reg_set[])
{
    unsigned int i, j; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(reg_set[m]) == 0)
        {
            i = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(reg_set[m]) == 0)
        {
            j = m;
        }
    }


    registers[27] = registers[i] * registers[j];
}
// ===========================================================================

// ===========================================================================
void Register::div_3r(std::vector<std::string>& parts, const std::string reg_set[])
{
    unsigned int i, j, k; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(reg_set[m]) == 0)
        {
            i = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(reg_set[m]) == 0)
        {
            j = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[2].compare(reg_set[m]) == 0)
        {
            k = m;
        }
    }
    
    registers[26] = registers[j] % registers[k]; // HI
    registers[27] = registers[j] / registers[k]; // LO
    if (i != 0)
        registers[i] = registers[j] / registers[k];
}
// ===========================================================================
// ===========================================================================
void Register::div_2r(std::vector<std::string>& parts, const std::string reg_set[])
{
    unsigned int i, j; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(reg_set[m]) == 0)
        {
            i = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(reg_set[m]) == 0)
        {
            j = m;
        }
    }

    registers[26] = registers[i] % registers[j]; // HI
    registers[27] = registers[i] / registers[j]; // LO
}
// ===========================================================================
// ===========================================================================
void Register::move(std::vector<std::string>& parts, const std::string reg_set[])
{
    unsigned int i, j; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(reg_set[m]) == 0)
        {
            i = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(reg_set[m]) == 0)
        {
            j = m;
        }
    }

    if (i != 0)
        registers[i] = registers[j];
}
// ===========================================================================

// ===========================================================================
void Register::mfhi(std::vector<std::string>& parts, const std::string reg_set[])
{
    unsigned int i; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(reg_set[m]) == 0)
        {
            i = m;
        }
    }
    if (i != 0)
        registers[i] = registers[26];
}
// ===========================================================================

// ===========================================================================
void Register::mflo(std::vector<std::string>& parts, const std::string reg_set[])
{
    unsigned int i; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(reg_set[m]) == 0)
        {
            i = m;
        }
    }
    if (i != 0)
        registers[i] = registers[27];
}

// ===========================================================================
// ===========================================================================
void Register::slt_2(std::vector<std::string>& parts, const std::string set[],
    bool unsigned_)
{
    unsigned int i, j, k; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(set[m]) == 0)
        {
            i = m;
            break;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(set[m]) == 0)
        {
            j = m;
            break;
        }
    }
    
    k = stoi(parts[2]);
    if (unsigned_ && i != 0)
        registers[i] = (registers[j] < k);
    else if (!unsigned_ && i != 0)
        registers[i] = ((signed int)registers[j] < (signed int) k); 
}
// ===========================================================================
// ===========================================================================


// ===========================================================================
// ===========================================================================
void Register::slt_3(std::vector<std::string>& parts, const std::string reg_set[], bool unsigned_)
{
    unsigned int i, j, k; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(reg_set[m]) == 0)
        {
            i = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(reg_set[m]) == 0)
        {
            j = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[2].compare(reg_set[m]) == 0)
        {
            k = m;
        }
    }
    if (unsigned_ && i != 0)
        registers[i] = (registers[j] < registers[k]);
    else if (!unsigned_ && i != 0)
        registers[i] = ((signed int)registers[j] < (signed int)registers[k]);
}
// ===========================================================================
// ===========================================================================

// ===========================================================================
// ===========================================================================
void Register::sgt_2(std::vector<std::string>& parts, const std::string set[],
    bool unsigned_)
{
    unsigned int i, j, k; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(set[m]) == 0)
        {
            i = m;
            break;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(set[m]) == 0)
        {
            j = m;
            break;
        }
    }
    
    k = stoi(parts[2]);
    if (unsigned_ && i != 0)
        registers[i] = (registers[j] > k);
    else if (!unsigned_ && i != 0)
        registers[i] = ((signed int)registers[j] > (signed int)k);
}
// ===========================================================================
// ===========================================================================

// ===========================================================================
// ===========================================================================
void Register::sgt_3(std::vector<std::string>& parts, const std::string reg_set[], bool unsigned_)
{
    unsigned int i, j, k; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(reg_set[m]) == 0)
        {
            i = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(reg_set[m]) == 0)
        {
            j = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[2].compare(reg_set[m]) == 0)
        {
            k = m;
        }
    }
    if (unsigned_ && i != 0)
        registers[i] = (registers[j] > registers[k]);
    else if (!unsigned_ && i != 0)
        registers[i] = ((signed int)registers[j] > (signed int)registers[k]);
}
// ===========================================================================
// ===========================================================================

// ===========================================================================
// ===========================================================================
void Register::sle_2(std::vector<std::string>& parts, const std::string set[],
    bool unsigned_)
{
    unsigned int i, j, k; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(set[m]) == 0)
        {
            i = m;
            break;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(set[m]) == 0)
        {
            j = m;
            break;
        }
    }
    
    k = stoi(parts[2]);
    
    if (unsigned_ && i != 0)
        registers[i] = (registers[j] <= k);
    else if (!unsigned_ && i != 0)
        registers[i] = ((signed int)registers[j] <= (signed int)k);
}
// ===========================================================================
// ===========================================================================

// ===========================================================================
// ===========================================================================
void Register::sle_3(std::vector<std::string>& parts, const std::string reg_set[], bool unsigned_)
{
    unsigned int i, j, k; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(reg_set[m]) == 0)
        {
            i = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(reg_set[m]) == 0)
        {
            j = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[2].compare(reg_set[m]) == 0)
        {
            k = m;
        }
    }

    if (unsigned_ && i != 0)
        registers[i] = (registers[j] <= registers[k]);
    else if (!unsigned_ && i != 0)
        registers[i] = ((signed int)registers[j] <= (signed int)registers[k]);
    
}
// ===========================================================================
// ===========================================================================

// ===========================================================================
// ===========================================================================
void Register::sge_2(std::vector<std::string>& parts, const std::string set[],
                     bool unsigned_)
{
    std::cout << "here" << std::endl;
    unsigned int i, j, k; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(set[m]) == 0)
        {
            i = m;
            break;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(set[m]) == 0)
        {
            j = m;
            break;
        }
    }
    
    k = stoi(parts[2]);
    
    if (unsigned_ && i != 0)
        registers[i] = (registers[j] >= k);
    else if (!unsigned_ && i != 0)
        registers[i] = ((signed int)registers[j] >= (signed int)k);
}
// ===========================================================================
// ===========================================================================

// ===========================================================================
// ===========================================================================
void Register::sge_3(std::vector<std::string>& parts, const std::string reg_set[], bool unsigned_)
{
    unsigned int i, j, k; // index holders of registers

    for (int m = 0; m < 28; m++)
    {
        if (parts[0].compare(reg_set[m]) == 0)
        {
            i = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[1].compare(reg_set[m]) == 0)
        {
            j = m;
        }
    }

    for (int m = 0; m < 28; m++)
    {
        if (parts[2].compare(reg_set[m]) == 0)
        {
            k = m;
        }
    }

    if (unsigned_ && i != 0)
        registers[i] = (registers[j] >= registers[k]);
    else if (!unsigned_ && i != 0)
        registers[i] = ((signed int)registers[j] >= (signed int)registers[k]);
}
// ===========================================================================
// ===========================================================================

// ===========================================================================
// ===========================================================================
void Register::syscall(std::vector<std::string>& parts, const std::string reg_set[],
                       std::map<int, int> &data_memory,
                       std::map<int,std::string> & label_memory)
{
    switch(registers[1])
    {
        // print int
        case 1:
        {
            std::cout << registers[3] << std::endl;
            break;
        }
        // print string
        case 4:
        {
            int address = registers[3];
            std::map< int, int >::iterator it = data_memory.begin();
            while (it != data_memory.end())
            {
                if (it->first == address)
                {
                    break;
                }
                it++;
            }
            
            while (it->second != '\0' && it != data_memory.end())
            {
                std::cout << char(it->second);
                address++;
                it++;
            }
            std::cout << std::endl;
            int i = 0;
            break;
        }
        // read int
        case 5:
        {
            int x;
            std::cin >> x;
            registers[1] = x;
            std::cout << std::endl;
            break;
        }
        // read string
        case 8:
        {
            bool exists = false;
            std::string line;
            int address = registers[3];
            std::getline(std::cin, line);
            if (line.size() > registers[4] - 1)
            {
                line[registers[4]] = '\0';
            }
            std::map< int, int >::iterator it = data_memory.begin();
            std::map< int, std::string >::iterator it2 = label_memory.begin();
            

            while(it != data_memory.end())
            {
                if (it->first == address)
                {
                    exists = true;
                    break;
                }
                it++;
            }
            while(it2 != label_memory.end())
            {
                if (it2->first == address)
                {
                    exists = true;
                    break;
                }
                it2++;
            }
           
            int i = 0;
            if (exists)
            {
                while (i != registers[4])
                {
                    data_memory.insert(std::pair< int, int >(address, int(line[i])));
                    i++;
                    address++;
                }  
            }
                                   
            
            break;
        }
        // print char
        case 11:
        {
            char c = registers[3];
            std::cout << c << std::endl;
            break;
        }
        
    }
}
// ===========================================================================
// ===========================================================================

// ===========================================================================
// ===========================================================================
void Register::la(std::vector<std::string>& parts, const std::string reg_set[], std::map< int, std::string > & label_memory)
{
    int reg = 0;
    int address = 0;
    std::map< int, std::string >::iterator it = label_memory.begin();
    for (int i = 0; i < 28; i++)
    {
        if (parts[0].compare(register_set[i]) == 0)
        {
            reg = i;
            break;
        }
    }

    if (reg != 0)
    {
        while (it != label_memory.end())
        {
            address = it->first;
            if (std::stoi(parts[1]) == address)
            {
                registers[reg] = it->first;
                break;
            }
            it++;
        }
    }
    

    
}
// ===========================================================================
// ===========================================================================


// ===========================================================================
// ===========================================================================
void Register::la_H(std::vector<std::string>& parts, const std::string reg_set[], std::map< int, std::string > & label_memory)
{
    std::stringstream ss;
    
    int reg = 0;
    int num;
    std::map< int, std::string >::iterator it = label_memory.begin();
    for (int i = 0; i < 28; i++)
    {
        if (parts[0].compare(register_set[i]) == 0)
        {
            reg = i;
            break;
        }
    }

    if (reg != 0)
    {
        ss << std::dec << it->first;
        ss >> num;
        registers[reg] = num;
    }
    
}
// ===========================================================================
// ===========================================================================

// ===========================================================================
// ===========================================================================
void Register::lw(std::vector<std::string>& parts, const std::string reg_set[], std::map< int, int > & data_memory)
{
    int r1, r2;
    int num_;
    int address = 0;
    for (int i = 0; i < 28; i++)
    {
        if (parts[0].compare(register_set[i]) == 0)
        {
            r1 = i;
            break;
        }
    }
    
    if (r1 != 0)
    {
        num_ = std::stoi(parts[1]);
    
        for (int i = 0; i < 28; i++)
        {
            if (parts[2].compare(register_set[i]) == 0)
            {
                r2 = i;
                break;
            }
        }
        std::map< int, int >::iterator it = data_memory.begin();
    
        while (it != data_memory.end())
        {
            if (registers[r2] == it->first)
            {
                address = it->first + num_;
                break;
            }
            it++;
        }

        it = data_memory.begin();
        while (it != data_memory.end())
        {
            if (address == it->first)
            {
                registers[r1] = it->second;
                break;
            }
            it++;
        }
    }
    
}
// ===========================================================================
// ===========================================================================

void Register::lw_H(std::vector<std::string>& parts, const std::string reg_set[], std::map< int, std::string > & label_memory, std::map< int, int > & data_memory)
{
    int r1;
    int address;
    for (int i = 0; i < 28; i++)
    {
        if (parts[0].compare(register_set[i]) == 0)
        {
            r1 = i;
            break;
        }
    }
    if (r1 != 0)
    {
        std::map< int, std::string >::iterator it = label_memory.begin();
    
        while (it != label_memory.end())
        {
            if (parts[1].compare(it->second) == 0)
            {
                address = it->first;
                break;
            }
            it++;
        }

        std::map< int, int >::iterator it2 = data_memory.begin();
        int hold;
        while (it2 != data_memory.end())
        {
            hold = it2->first;
            if (hold == address)
            {
                registers[r1] = it2->second;
                break;
            }
            it++;
        }
    }
   
}
// ===========================================================================
// ===========================================================================

// ===========================================================================
// ===========================================================================
void Register::sw(std::vector<std::string>& parts, const std::string reg_set[], std::map< int, int > & data_memory)
{
    int r1, r2;
    int num_;
    int address = 0;
    for (int i = 0; i < 28; i++)
    {
        if (parts[0].compare(register_set[i]) == 0)
        {
            r1 = i;
            break;
        }
    }

    if (r1 != 0)
    {
        num_ = std::stoi(parts[1]);
    
        for (int i = 0; i < 28; i++)
        {
            if (parts[2].compare(register_set[i]) == 0)
            {
                r2 = i;
                break;
            }
        }
        std::map< int, int >::iterator it = data_memory.begin();
    
        while (it != data_memory.end())
        {
            if (registers[r2] == it->first)
            {
                address = it->first + num_;
                break;
            }
            it++;
        }

        it = data_memory.begin();
        while (it != data_memory.end())
        {
            if (address == it->first)
            {
                it->second = registers[r1];
                break;
            }
            it++;
        }
    }
    
}
// ===========================================================================
// ===========================================================================

// ===========================================================================
// ===========================================================================

void Register::sw_H(std::vector<std::string>& parts, const std::string reg_set[], std::map< int, std::string > & label_memory, std::map< int, int > & data_memory)
{
    int r1;
    int address;
    for (int i = 0; i < 28; i++)
    {
        if (parts[0].compare(register_set[i]) == 0)
        {
            r1 = i;
            break;
        }
    }

    if (r1 != 0)
    {
        std::map< int, std::string >::iterator it = label_memory.begin();
    
        while (it != label_memory.end())
        {
            if (parts[1].compare(it->second) == 0)
            {
                address = it->first;
                break;
            }
            it++;
        }

        std::map< int, int >::iterator it2 = data_memory.begin();
        int hold;
        while (it2 != data_memory.end())
        {
            hold = it2->first;
            if (hold == address)
            {
                it2->second = registers[r1];
                break;
            }
            it++;
        }
    }
    
}
// ===========================================================================
// ===========================================================================
void Register::print_register_list()
{
    char c;
    std::cout << "REGISTER LIST:" << std::endl;
    std::cout << "------------------------------------------------------------------------------\n";
    std::cout << "NAME:" << std::setw(10) << "" << "HEX:" << std::setw(10) << ""
              << "OCTO:" << std::setw(10) << "" << "DEC:" << std::setw(10) << ""
              << "UNSIGNED:" << std::setw(10) << "" << "CHAR:"<< std::endl;
    for (int i = 0; i < 28; i++)
    {
        char c = registers[i];
        if (i == 0)
        {
            std::cout << register_set[i] << "  = " << std::setw(13)
                      << std::hex << (int) registers[i] << std::setw(15)
                      << std::oct << (int) registers[i] << std::setw(14)
                      << std::dec << (int) registers[i] << std::setw(19)
                      << registers[i] << std::setw(15) << c << std::endl;
        }
        else
        {
            std::cout << register_set[i] << " = " << std::setw(13)
                      << std::hex << (int) registers[i] << std::setw(15)
                      << std::oct << (int) registers[i] << std::setw(14)
                      << std::dec << (int) registers[i] << std::setw(19)
                      << registers[i] << std::setw(15) << c << std::endl;
        }
    }
    std::cout << "------------------------------------------------------------------------------\n" << std::endl;
}
