#include "Memory.h"
#include <map>
#include <iomanip>
#include <vector>
#include <queue>



Memory::Memory()
{}

//==========================================================================================
//==========================================================================================
bool Memory::find_command(std::string input, std::map< int, int > & data_memory,
                          std::map < int , std::string > & label_memory,
                          bool batch)
{
    // the same as our instruction class method, find command then get args
    std::string command = "";
    std::string d = ".data";
    bool real_command;
    int i = 0;
    int index = 0;

    std::vector< std::string > parts; // this is our vector to hold parts of our
                                      // string that are not the command
    while (i < input.size())
    {
        // get past any whitespaces in the string
        if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n' || input[i] == '\0')
        {
            i++;
        }
        // as soon as we find a non whitespace character, we need to store it in the string
        // After this, if we find a whitespace character again our command is over
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

    if (command.compare(d) == 0)
    {
        return false;
    }
    if (command == "")
    {
        return true;
    }
    // now we need to see if there was a label in the string
    int x;
    if (command.size() > 0)
    {
        x = command.size() - 1;
    }

    if (command[x] == ':') // we found a label
    {
        //std::cout << "wat" << std::endl;
        std::string holder; // grab our label to put into our vector of labels
        for (int k = 0; k < command.size() - 1; k++)
        {
            holder.push_back(command[k]);
        }
        //std::cout << "trash" << std::endl;
        bool exists = false;
        //std::cout << label_holder.size() << '\n';
        std::map< int, std::string >::iterator it = label_memory.begin();
        for (int k = 0; k < label_memory.size(); k++)
        {
            //std::cout << "for" << std::endl;
            if (it == label_memory.end())
                break;
            if (label_holder[k].compare(it->second) == 0)
            {
                //exists = true;
                break;
            }
            it++;
        }
        //std::cout << "done" << std::endl;
        if (!exists)
            label_holder.push_back(holder);
        else
        {
            label_holder.clear();
            std::cout << "Error occured, label already exists!" << std::endl;
            std::cout << input << std::endl;
            for (int j = 0; j < i - 1; j++)
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
        
        i++;
        command = "";
        // repeat the same process again
        while (i < input.size())
        {
            if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n' || input[i] == '\0')
            {
                i++;
            }
          
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
    // now we have our command, we must make sure it is .word, .byte, .asciiz, or .space
    for (int j = 0; j < 5; j++)
    {
        if (command.compare(data_set[j]) == 0)
        {
            real_command = true;
            index = j;
        }           
    }

    // if the command exists
    if (real_command)
    {
        switch(index)
        {
            // word
            case 0:
            {
                bool cake = validate_word_byte(input, i, parts, label_memory);
                if (cake)
                {
                    add_word(data_memory, parts, label_memory);
                    return true;
                }
                else
                {
                    return false;
                }
                break;
            }
            // asciiz
            case 1:
            {
                bool cake = validate_asciiz(input, i, parts);
                if (cake)
                {
                    add_string(data_memory, parts, label_memory);
                    return true;
                }
                return false;;
            }

            // ascii
            case 2:
            {
                bool cake = validate_asciiz(input, i, parts);
                if (cake)
                {
                    add_string(data_memory, parts, label_memory);
                    return true;
                }
                return false;
            }
            // space
            case 3:
            {
                bool cake = validate_space(input, i, parts);
                if (cake)
                {
                    add_space(data_memory, parts, label_memory);
                    return true;
                }
                return false;
            }
            // byte
            case 4:
            {
                bool cake = validate_word_byte(input, i, parts, label_memory);
                if (cake)
                {
                    add_byte(data_memory, parts, label_memory);
                    return true;
                }
                return false;
            }
        }
    }
    return false;
}
//==========================================================================================
//==========================================================================================



//==========================================================================================
//==========================================================================================
bool Memory::validate_asciiz(std::string input, int i, std::vector< std::string > & parts)const
{
    // make sure that the string is not out of bounds
    if (i >= input.size())
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

    else
    {
        // now we need to make sure that we only find strings, anything else besides that and
        // whitespace should throw us an error

        std::string temp; // this string will get everything past our command
        for (int j = i; j < input.size(); j++)
        {
            temp.push_back(input[j]);
        }

        int hold_i = i; // holding i so we can use it on input 
        i = 0; // reset i for our new string
        int counter = 0; // makes sure we have atleast 1 integer

        while(i < temp.size())
        {
            while(temp[i] == ' ' || temp[i] == '\n' || temp[i] == '\t')
            // for as long as we don't find a non-white space char
            {
                i++;
            }
            if (i >= temp.size())
            {
                break;
            }
            // now we are at our first non-whitespace char
            if (temp[i] != '\"' && temp[i] != '#' )
            // if the char is not a number or a comment
            {
                std::cout << "Error occured, invalid arguments!" << std::endl;
                std::cout << input << std::endl;
                for (int j = 0; j < hold_i+i; j++)
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
                // we found the start of a string, we need to go forward 1 to find our char
                i++;
                std::string found_string = "";
                if (temp[i] == '\"')
                {
                    parts.push_back(found_string);
                }
                else
                {
                    while (temp[i] != '\"')
                    {
                        if (i >= temp.size())
                        {
                            break;
                        }
                        found_string.push_back(temp[i]);
                        i++;
                    }
                    if (i >= temp.size())
                    {
                        break;
                    }
                    i++;
                    counter++;
                    parts.push_back(found_string);
                }     
            }
        }
        if (counter < 1)
        {
                            std::cout << "Messing up here" << std::endl;
            std::cout << "Error occured, invalid arguments!" << std::endl;
            std::cout << input << std::endl;
            for (int j = 0; j < hold_i+i; j++)
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
//==========================================================================================
//==========================================================================================



//==========================================================================================
//==========================================================================================
bool Memory::validate_space(std::string input, int i, std::vector<std::string> & parts)const
{
     // make sure that the string is not out of bounds
    if (i >= input.size())
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

    else
    {
        // now we need to make sure that we only find numbers, anything else besides 
        // whitespace should throw us an error

        std::string temp; // this string will everything past our command
        for (int j = i; j < input.size(); j++)
        {
            temp.push_back(input[j]);
        }

        int hold_i = i; // holding i so we can use it on input 
        i = 0; // reset i for our new string
        int counter = 0; // makes sure we have atleast 1 integer

        while(i < temp.size())
        {
            while(temp[i] == ' ' || temp[i] == '\n' || temp[i] == '\t')
            // for as long as we don't find a non-white space char
            {
                i++;
            }
            if (i >= temp.size())
            {
                break;
            }
            // now we are at our first non-whitespace char
            if ((temp[i] > 57 || temp[i] < 48) && temp[i] != '#')
            // if the char is not a number or a comment
            {
                std::cout << "Error occured, invalid arguments!" << std::endl;
                std::cout << input << std::endl;
                for (int j = 0; j < hold_i+i; j++)
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
                std::string num; // make a string to hold the number
                while ((temp[i] >= 48 && temp[i] <= 57 )&& i < temp.size())
                {
                    num.push_back(temp[i]);
                    i++;
                }
                counter++;
                parts.push_back(num);
                
            }
        }
        
        if (counter < 1 || counter > 1)
        {
            std::cout << "Error occured, invalid arguments!" << std::endl;
            std::cout << input << std::endl;
            for (int j = 0; j < hold_i+i; j++)
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
//==========================================================================================
//==========================================================================================

//==========================================================================================
//==========================================================================================
bool Memory::validate_word_byte(std::string input, int i, std::vector< std::string > & parts, std::map< int, std::string >& label_memory)const
{
    // make sure that the string is not out of bounds
    if (i >= input.size())
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

    else
    {
        // now we need to make sure that we only find numbers, anything else besides 
        // whitespace should throw us an error

        std::string temp; // this string will everything past our command
        for (int j = i; j < input.size(); j++)
        {
            temp.push_back(input[j]);
        }

        int hold_i = i; // holding i so we can use it on input 
        i = 0; // reset i for our new string
        int counter = 0; // makes sure we have atleast 1 integer

        while(i < temp.size())
        {
            while(temp[i] == ' ' || temp[i] == '\n' || temp[i] == '\t')
            // for as long as we don't find a non-white space char
            {
                i++;
            }
            if (i >= temp.size())
            {
                break;
            }
            // now we are at our first non-whitespace char
            if ((temp[i] > 57 || temp[i] < 48) && temp[i] != '#' && (temp[i] != '-' && counter < 1))
            // if the char is not a number or a comment
            {
                int z = i;
                std::string t2;
                while (temp[z] != ' ' && temp[z] != '\t' && temp[z] != '\n' && z < temp.size())
                {
                    t2.push_back(temp[z]);
                    z++;
                }
                std::map< int, std::string >::const_iterator it =
                                                            label_memory.begin();
                while (it != label_memory.end())
                {
                    if (t2.compare(it->second) == 0)
                    {
                        std::string t2 = std::to_string(it->first);
                        parts.push_back(t2);
                        return true;
                    }
                    it++;
                }
                std::cout << "Error occured, invalid arguments!" << std::endl;
                std::cout << input << std::endl;
                for (int j = 0; j < hold_i+i; j++)
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
                std::string num; // make a string to hold the number
                if (temp[i] == '-')
                {
                    num.push_back(temp[i]);
                    i++;
                }
                while ((temp[i] >= 48 && temp[i] <= 57 )&& i < temp.size())
                {
                    num.push_back(temp[i]);
                    i++;
                }
                counter++;
                parts.push_back(num);
                
            }
        }
        if (counter < 1)
        {
            std::cout << "Error occured, invalid arguments!" << std::endl;
            std::cout << input << std::endl;
            for (int j = 0; j < hold_i+i; j++)
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
//==========================================================================================
//==========================================================================================
void Memory::add_word(std::map< int, int >& data_segment, std::vector< std::string > & parts, std::map< int, std::string >& label_memory)
{
    int first_pass = 0;
    for (int i = 0; i < parts.size(); i++)
    {
        int x = stoi(parts[i]);
        if (address % 4 == 0)
        {
            data_segment.insert(std::pair< int, int >(address, x));
            if (label_holder.size() != 0 && first_pass == 0)
            {
                for (int j = 0; j < label_holder.size(); j++)
                {
                    label_memory.insert(std::pair< int, std::string >(address,
                                                             label_holder[j]));
                    
                }
                first_pass++;
                while (!label_holder.empty())
                {
                    label_holder.pop_back();
                }
            }
            address += 4;
        }
        else
        {
            while (address % 4 != 0)
            {
                address++;
            }
            if (label_holder.size() != 0 && first_pass == 0)
            {
                for (int j = 0; j < label_holder.size(); j++)
                {
                    label_memory.insert(std::pair< int, std::string >(address,
                                                             label_holder[j]));
                }
                first_pass++;
                while (!label_holder.empty())
                {
                    label_holder.pop_back();
                }
            }
            data_segment.insert(std::pair< int, int >(address, x));
            address += 4;
        }      
    }
}

void Memory::add_byte(std::map< int, int >& data_segment, std::vector< std::string > & parts, std::map< int, std::string >& label_memory)
{
    int first_pass = 0;
    for (int i = 0; i < parts.size(); i++)
    {
        int x = stoi(parts[i]);
        data_segment.insert(std::pair< int, int >(address, x));
        if (label_holder.size() != 0 && first_pass == 0)
            {
                for (int j = 0; j < label_holder.size(); j++)
                {
                    label_memory.insert(std::pair< int, std::string >(address,
                                                             label_holder[j]));
                }
                first_pass++;
                while (!label_holder.empty())
                {
                    label_holder.pop_back();
                }
            }
        address += 1;
             
    }
}

void Memory::add_string(std::map< int, int >& data_segment, std::vector< std::string > & parts, std::map< int, std::string >& label_memory)
{
    int first_pass = 0;
    std::string temp;
    for (int i = 0; i < parts.size(); i++)
    {
        temp = parts[i];
        for (int j = 0; j < temp.size(); j++)
        {
            int x = temp[j];
            data_segment.insert(std::pair< int, int >(address, x));
            if (label_holder.size() != 0 && first_pass == 0)
            {
                for (int j = 0; j < label_holder.size(); j++)
                {
                    label_memory.insert(std::pair< int, std::string >(address,
                                                              label_holder[j]));
                }
                first_pass++;
                while (!label_holder.empty())
                {
                    label_holder.pop_back();
                }
            }
            address += 1;
        }            
    }
}

void Memory::add_space(std::map< int, int >& data_segment,
                       std::vector< std::string > & parts,
                       std::map< int, std::string >& label_memory)
{
    int first_pass = 0;
    for (int i = 0; i < parts.size(); i++)
    {
        int x = stoi(parts[i]);
        if (label_holder.size() != 0 && first_pass == 0)
        {
            for (int j = 0; j < label_holder.size(); j++)
            {
                label_memory.insert(std::pair< int, std::string >(address,
                                                         label_holder[j]));
            }
            first_pass++;
            while (!label_holder.empty())
            {
                label_holder.pop_back();
            }
        }
    address += x;
             
    }
}

void Memory::print_data(std::map< int, int > & myMap)const
{
    std::queue< int > hold_bytes;
    std::queue< int > hold_bytes2;
    std::queue< int > hold_bytes3;
    std::string input; // this will print our hex values neatly
    std::string hex_val; // this will hold hex values to turn into an int to print
    std::cout << "===================================================================================="
              << std::endl;
    std::cout << "DATA SEGMENT" << std::endl;
    std::cout << "===================================================================================="
              << std::endl;

    std::cout << std::setw(4) << "" << "addr  (hex)|" << std::setw(4) << "" << "addr  (int)|"
              << std::setw(4) << "" << "value  (hex)|" << std::setw(4) << ""
              << "value  (int)|" << std::setw(4) << "" << "value  (char)|" << std::endl;
    std::cout << "---------------+---------------+----------------+----------------+"
              << "-----------------+"
              << std::endl;
    std::map< int, int >::iterator it = myMap.begin();
    while(it != myMap.end())
    {
        bool mod4 = true;
        std::cout << std::setw(15) << std::hex << it->first << "|" << std::setw(15) 
                  << std::dec << it->first << "|";
        
        std::map< int, int>::iterator it2 = it; // get the same pointer location
        std::map< int, int>::iterator check; // for later making sure we don't print an extra space
        it2++; // increase it by one
        if (it2->first % 4 != 0 && it2 != myMap.end())
        {
            std::stringstream ss;
            int x = it->second;
            ss << std::hex << x;
            input += ss.str();
            hex_val += ss.str();
            hold_bytes.push(it->second);
            input += ' ';
            mod4 = false;
        }
        while (it2->first % 4 != 0 && it2 != myMap.end())
        {
            check = it2;
            check++;
            std::stringstream ss;
            int x = it2->second;
            ss << std::hex << x;
            hold_bytes.push(it2->second);
            input += ss.str();
            hex_val += ss.str();
            if (it2->first % 4 != 3 && check != myMap.end())
            {
                 input += ' ';
            }
            it2++;
        }
        
        if (mod4)
        {
            char c = it->second;
            
            
            std::cout << std::setw(16) << std::hex << it->second << "|"
                      << std::setw(16) << std::dec << it->second << "|";
                      
            if (it->second < 32 || it->second >= 177)
            {
                std::cout << std::setw(18) << "|" << std::endl;
            }
            else
            {
                std::cout << std::setw(17) << c << "|" <<  std::endl;
            }
            it = it2;
        }
        else
        {
            int num;
            std::stringstream ss;
            ss << std::hex << hex_val;
            ss >> num;
            std::cout << std::setw(16) << input << "|" << std::setw(16) << num << "|";

            std::string temp;
            char c;
            while (!hold_bytes.empty())
            {
                c = hold_bytes.front();
                if (c < 32 || c >= 177)
                {
                    temp += "";
                }
                else
                {
                    temp += c;
                }
                if (hold_bytes.size() != 1)
                {
                    temp += ' ';
                } 
                hold_bytes.pop();
            }
                     
            std::cout << std::setw(17) << temp << "|" << std::endl;
            it = it2;
            input = "";
        }
        
    }
}

void Memory::print_labels(std::map< int, std::string >& label_memory)const
{
    std::cout << "===================================================================================="
              << std::endl;
    std::cout << "LABELS" << std::endl;
    std::cout << "===================================================================================="
              << std::endl;
    
    std::map< int, std::string >::const_iterator it = label_memory.begin();
    while (it != label_memory.end())
    {
        std::cout << std::setw(10) << it->first << "|"
                  << std::setw(10) << it->second
                  << std::endl;
        it++;
    }
}
