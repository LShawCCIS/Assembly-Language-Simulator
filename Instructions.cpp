#include "Instructions.h"

const static int COMMAND_SIZE_ = 31;
// ==========================================================================================================
// ==========================================================================================================
// constructor not really doing anything 
Instructions::Instructions()
{
    for (int i = 0; i < 28; i++)
    {
        registers.push_back(0);
    }
}
// ==========================================================================================================
// ==========================================================================================================


// ==========================================================================================================
// ==========================================================================================================
// this is going to find the command entered from our string so
// we know what type of syntax we need to check for it
bool Instructions::find_command(std::string input, Register & reg, std::map< int, int > & data_memory,
                                std::map< int, std::string >& label_memory, bool batch)const
{
    //std::cout << "Entered into find_command... good luck soldier" << std::endl;
    std::string command = ""; // this string is used to store our command
    bool real_command; // this is to see if the command entered is real or not
    int i = 0; // this is going to our iterator through the string
    
    int index; // if the command is valid, this will hold the index of what command was
               // entered from our instruction_set array
    
    int text = 0, globl = 0, main = 0; // these will be used to make sure we only write each of these once
    // to our file
    std::string g_m = ".globl main";
    std::string t = ".text";
    std::string m = "main:";

    std::vector< std::string > parts; // this will hold our substrings for passing to register class
    
    while (i < input.size())
    {
        // get past any whitespaces in the string
        if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n' || input[i] == '\0')
        {
            i++;
        }
        // as soon as we find a character, we need to store it in the string
        // whenever we find another whitespace, command is found so we need to
        // exit the loop
        else
        {
            while ((input[i] != ' ' && input[i] != '\t' && input[i] != '\n' && input[i] != '\0') && i < input.size())
            {
                command.push_back(input[i]);
                i++;
            }
            break;
        }
    }
    
    std::string temp = command + " main"; // so we get the next part
    // now we need to see if the user typed globl main, text, or main:
    if (command.compare(t) == 0 || temp.compare(g_m) == 0 || command.compare(m) == 0 )
    {
      return false; // Prevents errors from being thrown if entered in multiple times
    }

    if (command == "")
    {
        return true;
    }
    // we need to make sure that we aren't looking at a label, if we are we need to go past
    // and find our command
    int x = 0;
    if (command.size() > 0)
    {
        x = command.size();
        x -= 1;
    }
     // this gets us the highest value out of range
     // subtract one so we get the very last char of our string
    if (command[x] == ':' && command.compare(temp) != 0) // we found a label so we must get rid of it
    {
        i += 1;
        command = "";
        while (i < input.size())
        {
            // get past any whitespaces in the string
            if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n' || input[i] == '\0')
            {
                i++;
            }
            // as soon as we find a character, we need to store it in the string
            // whenever we find another whitespace, command is found so we need to
            // exit the loop
            else
            {
                while ((input[i] != ' ' && input[i] != '\t' && input[i] != '\n' && input[i] != '\0') && i < input.size())
                {
                    command.push_back(input[i]);
                    i++;
                }
                break;
            }
        }
    }
    // we have found our command, now we need to see if it matches any of our
    // preset commands in our string array

    for (int j = 0; j < COMMAND_SIZE_; j++)
    {
        if (command.compare(instruction_set[j]) == 0)
        {
            real_command = true;
            index = j;
        }           
    }

    // now we see if our boolean is true, if so then we move onto checking our
    // arguments. Otherwise we throw an error
//------------------------------------------------------------------------------------------------------------
//  Our Command was Valid
    
    if (real_command == true)
    {
        bool is_unsigned = false;
        // the command exisits so now we do a switch case in order find out where to go
        // to check the rest of the syntax
        
        // based off of our index, we will find where to go to check what syntax we need for
        // the entered command
        switch(index)
        {
            //add
            case 0:
            {
                // add needs 3 registers, so goto validate_3reg
                // need to send our input string and where we stopped
                bool cake = validate_3reg(input, i, parts);
                if (cake)
                {
                    reg.add(parts, register_set);
                    return true;
                }
                return false;
                break;
            }
            //addi
            case 1:
            {
                // addi needs either 2 regs and and integer, or 1 reg and an integer
                // need to send our input string and where we stopped
                bool cake = validate_2regI(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.addi_2r(parts, register_set);
                    return true;
                }
                else if (!cake)
                {
                    while (!parts.empty())
                    {
                        parts.pop_back();
                    }
                    cake = validate_1regI(input, i, parts);
                }
                if (cake)
                {
                    if (!batch)
                        reg.addi_1r(parts, register_set);
                    return true;
                }
                return false;
            }
            //addiu
            case 2:
            {
                bool cake = validate_2regI(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.addi_2r(parts, register_set);
                    return true;
                }
                else if (!cake)
                {
                    parts.pop_back();
                    cake = validate_1regI(input, i, parts);
                }
                if (cake)
                {
                    if (!batch)
                        reg.addi_1r(parts, register_set);
                    return true;
                }
                return false;
            }
            //sub
            case 3:
            {
                bool cake = validate_3reg(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.sub(parts, register_set);
                    return true;
                }
                return false;
            }
            //subu
            case 4:
            {
                bool cake = validate_3reg(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.sub(parts, register_set);
                    return true;
                }
                return false;
            }
            // mul
            case 5:
            {
                bool cake = validate_3reg(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.mul(parts, register_set);
                    return true;
                }
                return false;
            }
            // mult
            case 6:
            {
                bool cake = validate_2reg(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.mult(parts, register_set);
                }
                return false;
            }
            // multu
            case 7:
            {
                bool cake = validate_2reg(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.mult(parts, register_set);
                    return true;
                }
                return false;
            }
            // div
            case 8:
            {
                bool cake = validate_3reg_no_print(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.div_3r(parts, register_set);
                    return true;
                }
                else if (!cake)
                {
                    parts.pop_back();
                    parts.pop_back();
                    cake = validate_2reg(input, i, parts);
                }
                if (cake)
                {
                    if (!batch)
                        reg.div_2r(parts, register_set);
                    return true;
                }
                return false;
            }
            // divu
            case 9:
            {
                 bool cake = validate_3reg_no_print(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.div_3r(parts, register_set);
                    return true;
                }
                else if (!cake)
                {
                    parts.pop_back();
                    parts.pop_back();
                    cake = validate_2reg(input, i, parts);
                }
                if (cake)
                {
                    if (!batch)
                        reg.div_2r(parts, register_set);
                    return true;
                }
                return false;
            }
            // move
            case 10:
            {
                bool cake = validate_2reg(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.move(parts, register_set);
                    return true;
                }
                return false; 
            }
            //mfhi
            case 11:
            {
                bool cake = validate_1reg(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.mfhi(parts, register_set);
                    return true;
                }
                return false;
            }
            // mflo
            case 12:
            {
                bool cake = validate_1reg(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.mflo(parts, register_set);
                }
                return false;
            }
            // li
            case 13:
            {
                bool cake = validate_1regI(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.load_i(parts, register_set);
                    return true;
                }
                return false;
            }
            // syscall
            case 14:
            {
                bool cake = validate_syscall(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.syscall(parts, register_set, data_memory, label_memory);
                    return true;
                }
                return false;
            }
            // la
            case 15:
            {
                bool cake = validate_load_addr(input, i, parts, label_memory);
                if (cake)
                {
                    if (!batch)
                        reg.la(parts, register_set, label_memory);              
                    return true;
                }
                else if (!cake)
                {
                    while (!parts.empty())
                    {
                        parts.pop_back();
                    }
                    cake = validate_load_addrH(input, i, parts, data_memory);
                }
                if (cake)
                {
                    if (!batch)
                        reg.la_H(parts, register_set, label_memory);
                    return true;
                }
                return false;
            }
            // sltu
            case 21:
            {
                is_unsigned = true;
            }
            //slt
            case 17:
            {
                bool cake = validate_2regI(input, i, parts);
                if (cake)
                {
                    std::cout << is_unsigned << std::endl;
                    if (!batch)
                        reg.slt_2(parts, register_set, is_unsigned);
                    return true;
                }
                else if (!cake)
                {
                    while (!parts.empty())
                    {
                        parts.pop_back();
                    }
                    cake = validate_3reg(input, i, parts);
                }
                if (cake)
                {
                    if (!batch)
                        reg.slt_3(parts, register_set, is_unsigned);
                    return true;
                }
                return false;
            }
            // sgtu
            case 22:
            {
                is_unsigned = true;
            }
            // sgt
            case 18:
            {               
                bool cake = validate_2regI(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.sgt_2(parts, register_set, is_unsigned);
                    return true;
                }
                else if (!cake)
                {
                    while (!parts.empty())
                    {
                        parts.pop_back();
                    }
                    cake = validate_3reg(input, i, parts);
                }
                if (cake)
                {
                    if (!batch)
                        reg.sgt_3(parts, register_set, is_unsigned);
                    return true;
                }
                return false;
            }
            // sleu
            case 24:
            {
                is_unsigned = true;
            }
            // sle
            case 19:
            {
                bool cake = validate_2regI(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.sle_2(parts, register_set, is_unsigned);
                    return true;
                }
                else if (!cake)
                {
                    while (!parts.empty())
                    {
                        parts.pop_back();
                    }
                    cake = validate_3reg(input, i, parts);
                }
                if (cake)
                {
                    if (!batch)
                        reg.sle_3(parts, register_set, is_unsigned);
                    return true;
                }
                return false;
            }
            // sgeu
            case 23:
            {
                is_unsigned = true;
            }
            // sge
            case 20:
            {
                bool cake = validate_2regI(input, i, parts);
                if (cake)
                {
                    if (!batch)
                        reg.sge_2(parts, register_set, is_unsigned);
                    return true;
                }
                else if (!cake)
                {
                    while (!parts.empty())
                    {
                        parts.pop_back();
                    }
                    cake = validate_3reg(input, i, parts);
                }
                if (cake)
                {
                    if (!batch)
                        reg.sge_3(parts, register_set, is_unsigned);
                    return true;
                }
                return false;
            }
            // lw
            case 29:
            {
                bool cake = validate_wordL(input, i, parts, label_memory);
                if (cake)
                {
                    if (!batch)
                        reg.lw_H(parts, register_set, label_memory, data_memory);
                    return true;
                }
                else if (!cake)
                {
                    while (!parts.empty())
                    {
                        parts.pop_back();
                    }
                    cake = validate_word(input, i, parts);
                }
                if (cake)
                {
                    if (!batch)
                        reg.lw(parts, register_set, data_memory);
                    return true;
                }
                return false;
            }
            // sw
            case 30:
            {
                bool cake = validate_wordL(input, i, parts, label_memory);
                if (cake)
                {
                    if (!batch)
                        reg.sw_H(parts, register_set, label_memory, data_memory);
                    return true;
                }
                else if (!cake)
                {
                    while (!parts.empty())
                    {
                        parts.pop_back();
                    }
                    cake = validate_word(input, i, parts);
                }
                if (cake)
                {
                    if (!batch)
                        reg.sw(parts, register_set, data_memory);
                    return true;
                }
                return false;
            }
        }
        return false;   
    }
//------------------------------------------------------------------------------------------------------------
//  Our Command was not valid
    
    else
    {
        std::cout << "ERROR:" << command << std::endl;
        // the command does not exist, so we must print out an error and
        // point to where the error occurred
        std::cout << "Error occured, invalid command!" << std::endl;
        std::cout << input << std::endl;
        i--;
        for (int j = 0; j < i; j++)
        {
            if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
            {
                std::cout << input[j];
            }
            else
            {
                std::cout << ' ';
            } 
        }
        i++;
        std::cout << '^' << std::endl;
        return false;       
    }
    
}
// ==========================================================================================================
// ==========================================================================================================


// ==========================================================================================================
// ==========================================================================================================
bool Instructions::validate_3reg(std::string input, int i, std::vector< std::string > & parts)const
{
    // i is the space after the command that was found, so before we do anything, we should make sure
    // that i is still within our string bounds
    if (i >= input.size())
    {
        std::cout << "Error occured, invalid arguments!" << std::endl;
        std::cout << input << std::endl;
        for (int j = 0; j < i; j++)
        {
            if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
            {
                std::cout << input[j];
            }
            else
            {
                std::cout << ' ';
            } 
        }
        std::cout << '^' << std::endl;
        return false;
    }

    else
    {
        int counter = 0;  // we must have 3 regs, which means we need to count them
        std::string temp; // get a string that will start from where we left off on input
        for (int j = i; j < input.size(); j++)
        {
            temp.push_back(input[j]);
        }

        int i_hold = i; // holds i's value for printing purposes
        i = 0; // set i to 0 for our temp string
        while (i < temp.size())
        {
            while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
            {
                i++;
            }
            // we have now arrived at our non-space character, which must be $ or # otherwise it's an error
            if (temp[i] != '$' && temp[i] != '#')
            {
                std::cout << "Error occured, invalid arguments!" << std::endl;
                std::cout << input << std::endl;
                for (int j = 0; j < i_hold+i; j++)
                {
                    if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                    {
                        std::cout << input[j];
                    }
                    else
                    {
                        std::cout << ' ';
                    } 
                }
                std::cout << '^' << std::endl;
                return false;
            }
            else if (temp[i] == '#')
            {
                break;
            }
            else
            {
                // now we check to see if the register is an actual register
                std::string pos_reg;
                while (temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n' && temp[i] != '#' && i < temp.size()
                       && temp[i] != ',')
                {
                    pos_reg.push_back(temp[i]);
                    i++;
                }
                
                bool found = false;
                for (int j = 0; j < 28; j++)
                {
                    
                    if (pos_reg.compare(register_set[j]) == 0)
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    int double_back = i; // we may pass by bad values, so we need this to go back just incase
                    parts.push_back(pos_reg);
                    while (temp[i] != '$' && i < temp.size())
                    {
                        i++;
                    }
                    counter++;
                    // we must make sure that there weren't any values that we passed over that are bad
                    if (i >= temp.size())
                    {
                        while ((temp[double_back] == ',' || temp[double_back] == ' ' || temp[double_back] == '\t' ||
                                temp[double_back] == '\n') && double_back < i)
                        {
                            double_back++;
                        }

                        if (double_back < i)
                        {
                            if (temp[double_back] != '#')
                            {
                                std::cout << "Error occured, invalid arguments!" << std::endl;
                                std::cout << input << std::endl;
                                for (int j = 0; j < double_back + i_hold; j++)
                                {
                                    if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                                    {
                                        std::cout << input[j];
                                    }
                                    else
                                    {
                                        std::cout << ' ';
                                    } 
                                }
                                std::cout << '^' << std::endl;
                                return false;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    std::cout << "Error occured, invalid register!" << std::endl;
                    std::cout << input << std::endl;
                    i--;
                    for (int j = 0; j < i_hold+i; j++)
                    {
                        if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                        {
                            std::cout << input[j];
                        }
                        else
                        {
                            std::cout << ' ';
                        } 
                    }
                    std::cout << '^' << std::endl;
                    return false;
                }
            }
        }
//---------------------------------------------------------------------------------------------------------------------------
        // end of while loop
        if (counter < 3 || counter > 3)
        {
            std::cout << "Error occured, register amount error!" << std::endl;
            std::cout << input << std::endl;
            i--;
            for (int j = 0; j < i_hold+i; j++)
            {
                if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                {
                    std::cout << input[j];
                }
                else
                {
                    std::cout << ' ';
                }
            }
            std::cout << '^' << std::endl;
            return false;
        }
        else
        {
            return true;
        }
    }

}

// ==========================================================================================================
// ==========================================================================================================


// ==========================================================================================================
// ==========================================================================================================
bool Instructions::validate_2regI(std::string input, int i, std::vector< std::string > & parts)const
{
    // first thing we must do is make sure that i is not out of bounds of our string
    if (i >= input.size())
    {
        return false;
    }
    
    else
    {
        int neg_count = 0;
        int counter = 0;  // we must have 2 regs, which means we need to count them
        int int_count = 0;
        std::string temp; // get a string that will start from where we left off on input
        for (int j = i; j < input.size(); j++)
        {
            temp.push_back(input[j]);
        }
        int i_hold = i; // holds i's value for printing purposes
        i = 0; // set i to 0 for our temp string
        while (i < temp.size())
        {
            while ((temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n') && i < temp.size())
            {
                if (i >= temp.size())
                {
                    break;
                }
                i++;
            }
            if (i >= temp.size())
            {
                break;
            }
            // we have now arrived at our non-space character, which must be $ or # otherwise it's an error
            if (temp[i] != '$' && temp[i] != '#' && (temp[i] < 48 || temp[i] > 57) && (temp[i] != '-' && int_count >= 1))
            {
                return false;
            }
            else if (temp[i] == '#')
            {
                break;
            }
            else if ((temp[i] >= 48 && temp[i] <= 57) || (temp[i] == '-' && int_count < 1))
            {
                if ((counter < 2 || counter > 2) || int_count >= 1)
                {
                    return false;
                }
                else
                {
                    int_count++;
                    // now we grab the whole number
                    std::string pos_reg;
                    if (temp[i] == '-')
                    {
                        pos_reg.push_back(temp[i]);
                        i++;
                    }
                    
                    while ((temp[i] >= 48 && temp[i] <= 57) && i < temp.size())
                    {
                        pos_reg.push_back(temp[i]);
                        i++;
                    }
                    parts.push_back(pos_reg);
                    
                }
            }
            else
            {
                // now we check to see if the register is an actual register
                std::string pos_reg;
                
                while (temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n' && temp[i] != '#' && i < temp.size()
                       && temp[i] != ',')
                {
                    pos_reg.push_back(temp[i]);
                    i++;
                }
                bool found = false;
                for (int j = 0; j < 28; j++)
                {
                    if (pos_reg.compare(register_set[j]) == 0)
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    parts.push_back(pos_reg);
                    counter++; // increase our counter count
                    i++; // increase our i because we are on a comma or a space
                }
                else
                {
                    break;
                }
            }
        }

//---------------------------------------------------------------------------------------------------------------------------
        // end of while loop
        if ((counter < 2 || counter > 2) || (int_count < 1 || int_count > 1))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}
// ==========================================================================================================
// ==========================================================================================================


// ==========================================================================================================
// ==========================================================================================================

bool Instructions::validate_2reg(std::string input, int i, std::vector< std::string >& parts)const
{
    // i is the space after the command that was found, so before we do anything, we should make sure
    // that i is still within our string bounds
    if (i >= input.size())
    {
        std::cout << "Error occured, invalid arguments!" << std::endl;
        std::cout << input << std::endl;
        for (int j = 0; j < i; j++)
        {
            if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
            {
                std::cout << input[j];
            }
            else
            {
                std::cout << ' ';
            } 
        }
        std::cout << '^' << std::endl;
        return false;
    }

    else
    {
        int counter = 0;  // we must have 3 regs, which means we need to count them
        std::string temp; // get a string that will start from where we left off on input
        for (int j = i; j < input.size(); j++)
        {
            temp.push_back(input[j]);
        }

        int i_hold = i; // holds i's value for printing purposes
        i = 0; // set i to 0 for our temp string
        while (i < temp.size())
        {
            while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
            {
                i++;
            }
            // we have now arrived at our non-space character, which must be $ or # otherwise it's an error
            if (temp[i] != '$' && temp[i] != '#')
            {
                std::cout << "Error occured, invalid arguments!" << std::endl;
                std::cout << input << std::endl;
                for (int j = 0; j < i_hold+i; j++)
                {
                    if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                    {
                        std::cout << input[j];
                    }
                    else
                    {
                        std::cout << ' ';
                    } 
                }
                std::cout << '^' << std::endl;
                return false;
            }
            else if (temp[i] == '#')
            {
                break;
            }
            else
            {
                // now we check to see if the register is an actual register
                std::string pos_reg;
                while (temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n' && temp[i] != '#' && i < temp.size()
                       && temp[i] != ',')
                {
                    pos_reg.push_back(temp[i]);
                    i++;
                }

                bool found = false;
                for (int j = 0; j < 28; j++)
                {
                    if (pos_reg.compare(register_set[j]) == 0)
                    {
                        found = true;
                    }
                }

                if (found)
                {
                    parts.push_back(pos_reg);
                    int double_back = i; // we may pass by bad values, so we need this to go back just incase
                    
                    while (temp[i] != '$' && i < temp.size())
                    {
                        i++;
                    }
                    counter++;
                    // we must make sure that there weren't any values that we passed over that are bad
                    if (i >= temp.size())
                    {
                        while ((temp[double_back] == ',' || temp[double_back] == ' ' || temp[double_back] == '\t' ||
                                temp[double_back] == '\n') && double_back < i)
                        {
                            double_back++;
                        }

                        if (double_back < i)
                        {
                            if (temp[double_back] != '#')
                            {
                                std::cout << "Error occured, invalid arguments!" << std::endl;
                                std::cout << input << std::endl;
                                for (int j = 0; j < double_back + i_hold; j++)
                                {
                                    if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                                    {
                                        std::cout << input[j];
                                    }
                                    else
                                    {
                                        std::cout << ' ';
                                    } 
                                }
                                std::cout << '^' << std::endl;
                                return false;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    std::cout << "Error occured, invalid register!" << std::endl;
                    std::cout << input << std::endl;
                    i--;
                    for (int j = 0; j < i_hold+i; j++)
                    {
                        if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                        {
                            std::cout << input[j];
                        }
                        else
                        {
                            std::cout << ' ';
                        } 
                    }
                    std::cout << '^' << std::endl;
                    return false;
                }
            }
        }
//---------------------------------------------------------------------------------------------------------------------------
        // end of while loop
        if (counter < 2 || counter > 2)
        {
            std::cout << "Error occured, register amount error!" << std::endl;
            std::cout << input << std::endl;
            i--;
            for (int j = 0; j < i_hold+i; j++)
            {
                if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                {
                    std::cout << input[j];
                }
                else
                {
                    std::cout << ' ';
                }
            }
            std::cout << '^' << std::endl;
            return false;
        }
        else
        {
            return true;
        }
    }
}


// ==========================================================================================================
// ==========================================================================================================


bool Instructions::validate_1reg(std::string input, int i, std::vector< std::string >& parts)const
{
    // i is the space after the command that was found, so before we do anything, we should make sure
    // that i is still within our string bounds
    if (i >= input.size())
    {
        std::cout << "Error occured, invalid arguments!" << std::endl;
        std::cout << input << std::endl;
        for (int j = 0; j < i; j++)
        {
            if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
            {
                std::cout << input[j];
            }
            else
            {
                std::cout << ' ';
            } 
        }
        std::cout << '^' << std::endl;
        return false;
    }

    else
    {
        int counter = 0;  // we must have 3 regs, which means we need to count them
        std::string temp; // get a string that will start from where we left off on input
        for (int j = i; j < input.size(); j++)
        {
            temp.push_back(input[j]);
        }

        int i_hold = i; // holds i's value for printing purposes
        i = 0; // set i to 0 for our temp string
        while (i < temp.size())
        {
            while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
            {
                i++;
            }
            // we have now arrived at our non-space character, which must be $ or # otherwise it's an error
            if (temp[i] != '$' && temp[i] != '#')
            {
                std::cout << "Error occured, invalid arguments!" << std::endl;
                std::cout << input << std::endl;
                for (int j = 0; j < i_hold+i; j++)
                {
                    if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                    {
                        std::cout << input[j];
                    }
                    else
                    {
                        std::cout << ' ';
                    } 
                }
                std::cout << '^' << std::endl;
                return false;
            }
            else if (temp[i] == '#')
            {
                break;
            }
            else
            {
                // now we check to see if the register is an actual register
                std::string pos_reg;
                while (temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n' && temp[i] != '#' && i < temp.size()
                       && temp[i] != ',')
                {
                    pos_reg.push_back(temp[i]);
                    i++;
                }

                bool found = false;
                for (int j = 0; j < 28; j++)
                {
                    if (pos_reg.compare(register_set[j]) == 0)
                    {
                        found = true;
                    }
                }

                if (found)
                {
                    parts.push_back(pos_reg);
                    int double_back = i; // we may pass by bad values, so we need this to go back just incase
                    
                    while (temp[i] != '$' && i < temp.size())
                    {
                        i++;
                    }
                    counter++;
                    // we must make sure that there weren't any values that we passed over that are bad
                    if (i >= temp.size())
                    {
                        while ((temp[double_back] == ',' || temp[double_back] == ' ' || temp[double_back] == '\t' ||
                                temp[double_back] == '\n') && double_back < i)
                        {
                            double_back++;
                        }

                        if (double_back < i)
                        {
                            if (temp[double_back] != '#')
                            {
                                std::cout << "Error occured, invalid arguments!" << std::endl;
                                std::cout << input << std::endl;
                                for (int j = 0; j < double_back + i_hold; j++)
                                {
                                    if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                                    {
                                        std::cout << input[j];
                                    }
                                    else
                                    {
                                        std::cout << ' ';
                                    } 
                                }
                                std::cout << '^' << std::endl;
                                return false;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    std::cout << "Error occured, invalid register!" << std::endl;
                    std::cout << input << std::endl;
                    i--;
                    for (int j = 0; j < i_hold+i; j++)
                    {
                        if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                        {
                            std::cout << input[j];
                        }
                        else
                        {
                            std::cout << ' ';
                        } 
                    }
                    std::cout << '^' << std::endl;
                    return false;
                }
            }
        }
//---------------------------------------------------------------------------------------------------------------------------
        // end of while loop
        if (counter < 1 || counter > 1)
        {
            std::cout << "Error occured, register amount error!" << std::endl;
            std::cout << input << std::endl;
            i--;
            for (int j = 0; j < i_hold+i; j++)
            {
                if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                {
                    std::cout << input[j];
                }
                else
                {
                    std::cout << ' ';
                }
            }
            std::cout << '^' << std::endl;
            return false;
        }
        else
        {
            return true;
        }
    }
}


// ==========================================================================================================
// ==========================================================================================================

bool Instructions::validate_1regI(std::string input, int i, std::vector < std::string > & parts)const
{
    // first thing we must do is make sure that i is not out of bounds of our string
    if (i >= input.size())
    {
        std::cout << "Error occured, invalid arguments!" << std::endl;
        std::cout << input << std::endl;
        for (int j = 0; j < i; j++)
        {
            if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
            {
                std::cout << input[j];
            }
            else
            {
                std::cout << ' ';
            } 
        }
        std::cout << '^' << std::endl;
        return false;
    }

    else
    {
        int int_count = 0;
        int neg_count = 0;
        int counter = 0;  // we must have 3 regs, which means we need to count them
        std::string temp; // get a string that will start from where we left off on input
        for (int j = i; j < input.size(); j++)
        {
            temp.push_back(input[j]);
        }

        int i_hold = i; // holds i's value for printing purposes
        i = 0; // set i to 0 for our temp string
        while (i < temp.size())
        {
            while ((temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n') && i < temp.size())
            {
                i++;
            }
            if (i >= temp.size())
            {
                break;
            }
            // we have now arrived at our non-space character, which must be $ or # otherwise it's an error
            if (temp[i] != '$' && temp[i] != '#' && (temp[i] != '-' && int_count >= 1)
                &&(temp[i] < 48 || temp[i] > 57))
            {
                std::cout << "Error occured, invalid arguments!" << std::endl;
                std::cout << input << std::endl;
                for (int j = 0; j < i_hold+i; j++)
                {
                    if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                    {
                        std::cout << input[j];
                    }
                    else
                    {
                        std::cout << ' ';
                    } 
                }
                std::cout << '^' << std::endl;
                return false;
            }
            else if (temp[i] == '#')
            {
                break;
            }
            else if ((temp[i] == '-' && int_count < 1) || (temp[i] >= 48 && temp[i] <= 57))
            {
                if ((counter < 1 || counter > 1) || int_count >= 1)
                {
                    std::cout << "Error occured, syntax error!" << std::endl;
                    std::cout << input << std::endl;
                    
                    for (int j = 0; j < i_hold+i; j++)
                    {
                        if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                        {
                            std::cout << input[j];
                        }
                        else
                        {
                            std::cout << ' ';
                        }
                    }
                    std::cout << '^' << std::endl;
                    return false;
                }
                else
                {
                    // now we grab the whole number
                    std::string pos_reg;
                    if (temp[i] == '-')
                    {
                        pos_reg.push_back(temp[i]);
                        i++;
                    }
                    while (temp[i] >= 48 && temp[i] <= 57)
                    {
                        pos_reg.push_back(temp[i]);
                        i++;
                    }
                    parts.push_back(pos_reg);
                    int_count++;
                }
            }
            else
            {
                // now we check to see if the register is an actual register
                std::string pos_reg;
                while (temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n' && temp[i] != '#' && i < temp.size()
                       && temp[i] != ',')
                {
                    pos_reg.push_back(temp[i]);
                    i++;
                }
                
                bool found = false;
                for (int j = 0; j < 28; j++)
                {
                    if (pos_reg.compare(register_set[j]) == 0)
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    parts.push_back(pos_reg);
                    counter++; // increase our counter count
                    i++; // increase our i because we are on a comma or a space                  
                }
                else
                {
                    std::cout << "Error occured, register invalid!" << std::endl;
                    std::cout << input << std::endl;
                    i --;
                    for (int j = 0; j < i + i_hold; j++)
                    {
                        if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                        {
                            std::cout << input[j];
                        }
                        else
                        {
                            std::cout << ' ';
                        }
                    }
                    std::cout << '^' << std::endl;
                    return false;   
                }
            }
        }

//---------------------------------------------------------------------------------------------------------------------------
        // end of while loop
        if (counter < 1 || counter > 1)
        {
            std::cout << "Error occured, register amount error!" << std::endl;
            std::cout << input << std::endl;
            i -= 2;
            for (int j = 0; j < i + i_hold; j++)
            {
                if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                {
                    std::cout << input[j];
                }
                else
                {
                    std::cout << ' ';
                }
            }
            std::cout << '^' << std::endl;
            return false;
        }
        else
        {
            return true;
        }
    }

}


// ==========================================================================================================
// ==========================================================================================================



// ==========================================================================================================
// ==========================================================================================================
bool Instructions::validate_3reg_no_print(std::string input, int i, std::vector< std::string >& parts)const
{
    // this function is the exact same as validate_3reg, but it has no error messages
    if (i >= input.size())
    {
        return false;
    }

    else
    {
        int counter = 0;  // we must have 3 regs, which means we need to count them
        std::string temp; // get a string that will start from where we left off on input
        for (int j = i; j < input.size(); j++)
        {
            temp.push_back(input[j]);
        }

        int i_hold = i; // holds i's value for printing purposes
        i = 0; // set i to 0 for our temp string
        while (i < temp.size())
        {
            while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
            {
                i++;
            }
            // we have now arrived at our non-space character, which must be $ or # otherwise it's an error
            if (temp[i] != '$' && temp[i] != '#')
            {
                return false;
            }
            else if (temp[i] == '#')
            {
                break;
            }
            else
            {
                // now we check to see if the register is an actual register
                std::string pos_reg;
                while (temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n' && temp[i] != '#' && i < temp.size()
                       && temp[i] != ',')
                {
                    pos_reg.push_back(temp[i]);
                    i++;
                }
                
                bool found = false;
                for (int j = 0; j < 28; j++)
                {
                    
                    if (pos_reg.compare(register_set[j]) == 0)
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    int double_back = i; // we may pass by bad values, so we need this to go back just incase
                    parts.push_back(pos_reg);
                    while (temp[i] != '$' && i < temp.size())
                    {
                        i++;
                    }
                    counter++;
                    // we must make sure that there weren't any values that we passed over that are bad
                    if (i >= temp.size())
                    {
                        while ((temp[double_back] == ',' || temp[double_back] == ' ' || temp[double_back] == '\t' ||
                                temp[double_back] == '\n') && double_back < i)
                        {
                            double_back++;
                        }

                        if (double_back < i)
                        {
                            if (temp[double_back] != '#')
                            {
                                return false;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    return false;
                }
            }
        }
//---------------------------------------------------------------------------------------------------------------------------
        // end of while loop
        if (counter < 3 || counter > 3)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

}
// ==========================================================================================================
// ==========================================================================================================

// ==========================================================================================================
// ==========================================================================================================

bool Instructions::validate_syscall(std::string input, int i, std::vector< std::string>&)const
{
    // i is the space after the command that was found, so if i out of bounds we
    // dont need to check anymore
    if (i >= input.size())
    {
        return true;
    }
    else
    {
        bool out_bounds = false;
        while (i < input.size())
        {
            while (input[i] == ' ' || input[i] ==  '\t' || input[i] == '\n')
            {
                if (i >= input.size())
                {
                    out_bounds = true;
                    break;
                }
                i++;
            }
            if (out_bounds == true)
            {
                return true;
            }
            else if (input[i] != '#')
            {
                std::cout << "Error occured, syntax error!" << std::endl;
                std::cout << input << std::endl;
                
                for (int j = 0; j < i; j++)
                {
                    if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                    {
                       std::cout << input[j];
                    }
                    else
                    {
                        std::cout << ' ';
                    }
                }
                std::cout << '^' << std::endl;
                 
                return false;
            }
            else if (input[i] == '#')
            {
                return true;
            }
        }
        return true;
    }
}

// ==========================================================================================================


// ==========================================================================================================
bool Instructions::validate_word(std::string input, int i, std::vector< std::string>& parts)const
{
    // i is the space after the command that was found, so before we do anything, we should make sure
    // that i is still within our string bounds
    if (i >= input.size())
    {
        std::cout << "Error occured, invalid arguments!" << std::endl;
        std::cout << input << std::endl;
        for (int j = 0; j < i; j++)
        {
            if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
            {
                std::cout << input[j];
            }
            else
            {
                std::cout << ' ';
            } 
        }
        std::cout << '^' << std::endl;
        return false;
    }

    else
    {
        int counter = 0;  // we must have 2 regs, which means we need to count them
        int num_count = 0; // we need to have an offset
        int paren_count = 0; // must have 2 parens
        std::string temp; // get a string that will start from where we left off on input
        for (int j = i; j < input.size(); j++)
        {
            temp.push_back(input[j]);
        }

        int i_hold = i; // holds i's value for printing purposes
        i = 0; // set i to 0 for our temp string
        while (i < temp.size())
        {
            while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
            {
                i++;
            }
            // we have now arrived at our non-space character, which must be $ or # otherwise it's an error
            if (temp[i] != '$' && temp[i] != '#')
            {
                std::cout << "Error occured, invalid arguments!" << std::endl;
                std::cout << input << std::endl;
                for (int j = 0; j < i_hold+i; j++)
                {
                    if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                    {
                        std::cout << input[j];
                    }
                    else
                    {
                        std::cout << ' ';
                    } 
                }
                std::cout << '^' << std::endl;
                return false;
            }
            else if (temp[i] == '#')
            {
                break;
            }
            else
            {
                // now we check to see if the register is an actual register
                std::string pos_reg;
                while (temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n' && temp[i] != '#' && i < temp.size()
                       && temp[i] != ',')
                {
                    pos_reg.push_back(temp[i]);
                    i++;
                }
                
                bool found = false;
                for (int j = 0; j < 28; j++)
                {
                    if (pos_reg.compare(register_set[j]) == 0)
                    {
                        found = true;
                    }
                }

                if (found)
                {
                    parts.push_back(pos_reg);
                    counter++;
                    
                    //int double_back = i; // we may pass by bad values, so we need this to go back just incase
                    
                    while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n'&& i < temp.size())
                    {
                        i++;
                    }

                    if ((temp[i] < 48 || temp[i] > 57) && temp[i] != 40)
                    {
                        std::cout << "Error occured, syntax error!" << std::endl;
                        std::cout << input << std::endl;
                        
                        for (int j = 0; j < i_hold+i; j++)
                        {
                            if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                            {
                                std::cout << input[j];
                            }
                            else
                            {
                                std::cout << ' ';
                            } 
                        }
                        std::cout << '^' << std::endl;
                        return false;
                    }
                    else
                    {
                        if (temp[i] > 47 && temp[i] <= 57)
                        {
                            std::string temp_;
                            while (temp[i] > 47 && temp[i] <= 57)
                            {
                                temp_.push_back(temp[i]);
                                i++;
                            }
                            parts.push_back(temp_);
                            num_count++;
                            // get to the parens
                            if (temp[i] != 40)
                            {
                                std::cout << temp[i] << std::endl;
                                break;
                            }
                            else
                            {
                                paren_count++;
                                i++;
                                while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
                                {
                                    i++;
                                }
                                if (temp[i] != '$')
                                {
                                    break;
                                }
                                else
                                {
                                    std::string reg_;
                                    while ((temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n'
                                            && temp[i] != ')') && i < temp.size())
                                    {
                                        reg_.push_back(temp[i]);
                                        i++;
                                    }
                                    // check validity
                                    bool found_ = false;
                                    for (int i = 0; i < 26; i++)
                                    {
                                        if (reg_.compare(register_set[i]) == 0)
                                        {
                                            found_ = true;
                                        }
                                    }

                                    if (found)
                                    {
                                        counter++;
                                        parts.push_back(reg_);
                                        while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n' && i < temp.size())
                                        {
                                            i++;
                                        }
                                        if (temp[i] != ')')
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            i++;
                                            paren_count++;
                                            while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n' && i < temp.size())
                                            {
                                                i++;
                                            }
                                            if (i >= temp.size())
                                            {
                                                return true;
                                            }
                                            else
                                            {
                                                if (temp[i] == '#')
                                                    break;
                                                else
                                                {
                                                    std::cout << "Error occured, syntax error!" << std::endl;
                                                    std::cout << input << std::endl;
                                                    
                                                    for (int j = 0; j < i_hold+i; j++)
                                                    {
                                                        if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                                                        {
                                                            std::cout << input[j];
                                                        }
                                                        else
                                                        {
                                                            std::cout << ' ';
                                                        }
                                                    }
                                                    std::cout << '^' << std::endl;
                                                    return false;
                                                }
                                            }
                                        }
                                    }
                                    else
                                        break;
                                }
                            }
                            
                        }
                        else if (temp[i] == 40)
                        {
                            parts.push_back("0"); // because no constant found
                            paren_count++;
                            i++;
                            while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
                            {
                                i++;
                            }
                            if (temp[i] != '$')
                            {
                                break;
                            }
                            else
                            {
                                std::string reg_;
                                while ((temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n'
                                       && temp[i] != ')') && i < temp.size())
                                {
                                    reg_.push_back(temp[i]);
                                    i++;
                                }
                                // check validity
                                bool found_ = false;
                                for (int i = 0; i < 26; i++)
                                {
                                    if (reg_.compare(register_set[i]) == 0)
                                    {
                                        found_ = true;
                                    }
                                }
                                
                                if (found)
                                {
                                    counter++;
                                    parts.push_back(reg_);
                                    while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n' && i < temp.size())
                                    {
                                        i++;
                                    }
                                    if (temp[i] != ')')
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        i++;
                                        paren_count++;
                                        while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n' && i < temp.size())
                                        {
                                            i++;
                                        }
                                        if (i >= temp.size())
                                        {
                                            return true;
                                        }
                                        else
                                        {
                                            if (temp[i] == '#')
                                                break;
                                            else
                                            {
                                                std::cout << "Error occured, syntax error!" << std::endl;
                                                std::cout << input << std::endl;
                                                  
                                                for (int j = 0; j < i_hold+i; j++)
                                                {
                                                    if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                                                    {
                                                        std::cout << input[j];
                                                    }
                                                    else
                                                    {
                                                        std::cout << ' ';
                                                    }
                                                }
                                                std::cout << '^' << std::endl;
                                                return false;
                                            }
                                        }
                                    }
                                }
                                else
                                    break;
                            }
                        }
                    }
                    
                }
                else
                {
                    std::cout << "Error occured, invalid register!" << std::endl;
                    std::cout << input << std::endl;
                    
                    for (int j = 0; j < i_hold+i; j++)
                    {
                        if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                        {
                            std::cout << input[j];
                        }
                        else
                        {
                            std::cout << ' ';
                        } 
                    }
                    std::cout << '^' << std::endl;
                    return false;
                }
            }
        }
//---------------------------------------------------------------------------------------------------------------------------
        // end of while loop
        if ((counter < 2 || counter > 2) || num_count > 1 || (paren_count > 2 || paren_count < 2))
        {
            
            std::cout << "Error occured, syntax error!" << std::endl;
            std::cout << input << std::endl;
            
            for (int j = 0; j < i_hold+i; j++)
            {
                if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                {
                    std::cout << input[j];
                }
                else
                {
                    std::cout << ' ';
                }
            }
            std::cout << '^' << std::endl;
            return false;
        }
        else
        {
            return true;
        }
    }
}
// ==========================================================================================================

bool Instructions::validate_wordL(std::string input, int i, std::vector< std::string>& parts,
                                  std::map< int, std::string > & label_memory)const
{
    // i is the space after the command that was found, so before we do anything, we should make sure
    // that i is still within our string bounds
    if (i >= input.size())
    {
        return false;
    }

    else
    {
        int counter = 0;  // we must have 2 regs, which means we need to count them
        std::string temp; // get a string that will start from where we left off on input
        for (int j = i; j < input.size(); j++)
        {
            temp.push_back(input[j]);
        }

        int i_hold = i; // holds i's value for printing purposes
        i = 0; // set i to 0 for our temp string
        while (i < temp.size())
        {
            while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
            {
                i++;
            }
            // we have now arrived at our non-space character, which must be $ or # otherwise it's an error
            if (temp[i] != '$' && temp[i] != '#')
            {
                return false;
            }
            else if (temp[i] == '#')
            {
                break;
            }
            else
            {
                // now we check to see if the register is an actual register
                std::string pos_reg;
                while (temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n' && temp[i] != '#' && i < temp.size()
                       && temp[i] != ',')
                {
                    pos_reg.push_back(temp[i]);
                    i++;
                }
                
                bool found = false;
                for (int j = 0; j < 28; j++)
                {
                    if (pos_reg.compare(register_set[j]) == 0)
                    {
                        found = true;
                    }
                }

                if (found)
                {
                    std::string addr;
                    parts.push_back(pos_reg);
                    counter++;

                    while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
                    {
                        i++;
                    }
                    
                    while (temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n' && temp[i] != '#' && i < temp.size()
                           && temp[i] != ',')
                    {
                        addr.push_back(temp[i]);
                        i++;
                    }

                    std::map< int, std::string >::iterator it = label_memory.begin();
                    bool in_map = false;
                    while (it != label_memory.end())
                    {
                        if (addr.compare(it->second) == 0)
                        {
                            in_map = true;
                            break;
                        }
                        it++;
                    }

                    if (in_map)
                    {
                        parts.push_back(addr);
                        while ((temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
                               && i < temp.size())
                        {
                            i++;
                        }
                        if (i >= temp.size())
                        {
                            return true;
                        }
                        else
                        {
                            if (temp[i] == '#')
                                break;
                            else
                            {
                                return false;
                            }
                        }
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
        }
//---------------------------------------------------------------------------------------------------------------------------
        // end of while loop
        if (counter > 1 || counter < 1)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}
// ==========================================================================================================

// ==========================================================================================================
bool Instructions::validate_load_addr(std::string input, int i, std::vector< std::string>& parts, std::map<int, std::string > & label_memory)const
{
    // make sure that we aren't out of bounds of our strin
    if (i >= input.size())
    {
        return false;
    }
    else
    {
        // we need to have only 1 register and a label that exists in our map
        std::string temp;
        int reg_count = 0;
        for (int j = i; j < input.size(); j++)
        {
            temp.push_back(input[j]);
        }

        int i_hold = i; // holds i's value for printing purposes
        i = 0; // set i to 0 for our temp string
        while (i < temp.size())
        {
            while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
            {
                i++;
            }
            // we have now arrived at our non-space character, which must be $ or # otherwise it's an error
            if (temp[i] != '$' && temp[i] != '#')
            {
                return false;
            }
            std::string reg;
            while ((temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n') && i < temp.size())
            {
                reg.push_back(temp[i]);
                i++;
            }
 
            // check to make sure register exists
            bool found = false;
            for (int j = 0; j < 28; j++)
            {
                if (reg.compare(register_set[j]) == 0)
                {
                    found = true;
                }
            }
            // we found our register
            if (found)
            {
                reg_count++;
                parts.push_back(reg);
                while ((temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n') && i < temp.size())
                {
                    i++;
                }
                if (i >= temp.size())
                {
                    return false;
                }
                else
                {
                    std::string get_label;
                    while ((temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n' && temp[i] != '#')
                           && i < temp.size())
                    {
                        get_label.push_back(temp[i]);
                        i++;
                    }
                    // now we need to check to see if the label exisits in the map
                    bool in_map = false;
                    std::map< int, std::string >::const_iterator it = label_memory.begin();
                    while (it != label_memory.end())
                    {
                        if (get_label.compare(it->second) == 0)
                        {
                            in_map = true;
                            break;
                        }
                        it++;
                    }
                    if (in_map)
                    {
                        parts.push_back(std::to_string(it->first));
                        while ((temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
                               && i < temp.size())
                        {
                            i++;
                        }
                        if (i >= temp.size())
                        {
                            return true;
                        }
                        else
                        {
                            if (temp[i] == '#')
                                break;
                            else
                            {
                                return false;
                            }
                        }
                    }
                    else
                    {
                        return false;
                    }
                    
                }
            }
            else
            {
                return false;
            }
        }
        if (reg_count < 1 || reg_count > 1)
        { 
            return false;
        }
        else
        {
            return true;
        }
    }

}
// ====================================================================================================
// ====================================================================================================
bool Instructions::validate_load_addrH(std::string input, int i, std::vector< std::string>& parts,
                                       std::map<int, int > & data_memory)const
{
    //std::cout << "in addrH" << std::endl;
    // make sure that we aren't out of bounds of our string
    if (i >= input.size())
    {
        std::cout << "Error occured, invalid arguments!" << std::endl;
        std::cout << input << std::endl;
        for (int j = 0; j < i; j++)
        {
            if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
            {
                std::cout << input[j];
            }
            else
            {
                std::cout << ' ';
            } 
        }
        std::cout << '^' << std::endl;
        return false;
    }
    else
    {
        // we need to have only 1 register and a label that exists in our map
        std::string temp;
        int reg_count = 0;
        for (int j = i; j < input.size(); j++)
        {
            temp.push_back(input[j]);
        }

        int i_hold = i; // holds i's value for printing purposes
        i = 0; // set i to 0 for our temp string
        while (i < temp.size())
        {
            while (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
            {
                i++;
            }
            // we have now arrived at our non-space character, which must be $ or # otherwise it's an error
            if (temp[i] != '$' && temp[i] != '#')
            {
                std::cout << "Error occured, invalid register!" << std::endl;
                std::cout << input << std::endl;
                for (int j = 0; j < i; j++)
                {
                    if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                    {
                        std::cout << input[j];
                    }
                    else
                    {
                        std::cout << ' ';
                    } 
                }
                std::cout << '^' << std::endl;
                return false;
            }
            std::string reg;
            while ((temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n') && i < temp.size())
            {
                reg.push_back(temp[i]);
                i++;
            }
 
            // check to make sure register exists
            bool found = false;
            for (int j = 0; j < 28; j++)
            {
                if (reg.compare(register_set[j]) == 0)
                {
                    found = true;
                }
            }
            // we found our register
            if (found)
            {
                reg_count++;
                parts.push_back(reg);
                while ((temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n') && i < temp.size())
                {
                    i++;
                }
                if (i >= temp.size())
                {
                    std::cout << "Error occured, invalid arguments!" << std::endl;
                    std::cout << input << std::endl;
                    for (int j = 0; j < i; j++)
                    {
                        if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                        {
                            std::cout << input[j];
                        }
                        else
                        {
                            std::cout << ' ';
                        } 
                    }
                    std::cout << '^' << std::endl;
                    return false;
                }
                else
                {
                    std::string get_addr;
                    if (temp[i] == '0')
                    {
                        i++;
                        if (temp[i] == 'x')
                        {
                            i++;
                        }
                        else
                        {
                            reg_count--;
                            break;
                        }
                    }
                    else
                    {
                        reg_count--;
                        break;
                    }
                    while ((temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n' && temp[i] != '#')
                           && i < temp.size())
                    {
                        get_addr.push_back(temp[i]);
                        i++;
                    }
                    // now we need to check to see if the address exists
                    bool in_map = false;
                    std::map< int, int >::const_iterator it = data_memory.begin();
                    std::stringstream ss;
                    std::stringstream ss2;
                    std::string temp1;
                    std::string temp2;
                    
                    ss << std::hex << get_addr;
                    ss >> temp1;
                    while (it != data_memory.end())
                    {
                        ss2 << std::hex << it->first;
                        ss2 >> temp2;
                        if (get_addr == std::to_string(it->first))
                        {
                            in_map = true;
                            break;
                        }
                        if (temp1.compare(temp2) == 0)
                        {
                            in_map = true;
                            break;
                        }
                        it++;
                    }
                    if (in_map)
                    {
                        parts.push_back(get_addr);
                        while ((temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
                               && i < temp.size())
                        {
                            i++;
                        }
                        if (i >= temp.size())
                        {
                            return true;
                        }
                        else
                        {
                            if (temp[i] == '#')
                                break;
                            else
                            {
                                std::cout << "Error occured, invalid arguments!" << std::endl;
                                std::cout << input << std::endl;
                                for (int j = 0; j < i; j++)
                                {
                                    if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                                    {
                                        std::cout << input[j];
                                    }
                                    else
                                    {
                                        std::cout << ' ';
                                    } 
                                }
                                std::cout << '^' << std::endl;
                                return false;
                            }
                        }
                    }
                    else
                    {
                        return false;
                    }
                    
                }
            }
            else
            {
                return false;
            }
        }
        if (reg_count < 1 || reg_count > 1)
        {
            std::cout << "Error occured, invalid arguments!" << std::endl;
            std::cout << input << std::endl;
            for (int j = 0; j < i + i_hold; j++)
            {
                if (input[j] == ' ' || input[j] == '\t' || input[j] == '\n')
                {
                    std::cout << input[j];
                }
                else
                {
                    std::cout << ' ';
                } 
            }
            std::cout << '^' << std::endl;
            return false;
        }
        else
        {
            return true;
        }
    }

}


// ==========================================================================================================

/*

 */
