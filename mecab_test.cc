#include <iostream>
#include <stdexcept>

#include "mecab_lib.h"

void read_parse_print(MeCab::Parser* parser);
void parse_print(MeCab::Parser* parser, const std::string& input);

int main (int argc, char **argv) 
{
    MeCab::Parser* parser = MeCab::initialize_parser();

    read_parse_print(parser);

    MeCab::deinitialize_parser(parser);

    return 0;
} // end of main()


void read_parse_print(MeCab::Parser* parser)
{
    while (1)
    {
        std::string input;

        std::getline(std::cin, input);

        if (input == "") break;

        parse_print(parser, input);
    }
    
} // end of read_parse_print()


void parse_print(MeCab::Parser* parser, const std::string& input)
{
    for (const MeCab::Node* node = parser->parseToNode(input.c_str())
         ; node != NULL
         ; node = node->next)
    {
        if (node == NULL)
        {
            throw std::runtime_error(parser->what());
        }
        
        if (MeCab::is_indexable_node(node))
        {
            std::cout << "- ";
            std::cout.write (node->surface, node->length);
            std::cout << std::endl;
        }
    }

    std::cout << "---- end of result ----" << std::endl;
} // end of parse_print()
