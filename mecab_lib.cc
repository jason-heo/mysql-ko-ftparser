#include "mecab_lib.h"


namespace MeCab
{

Parser* initialize_parser()
{
    MeCab::Parser *parser = MeCab::createTagger("");

    if (parser == NULL)
    {
        throw std::runtime_error("init failed:"
                                    + std::string(MeCab::getTaggerError()));
    }

    return parser;
} // end of initialize_parser()


void deinitialize_parser(Parser*& parser)
{
    delete parser;
    parser = NULL;
} // end of deinitialize_parser()


bool is_indexable_node(const MeCab::Node* node)
{
    switch (node->posid)
    {
        case MeCab::POSID::COMPOUND:
        case MeCab::POSID::NN_NOUN:
        case MeCab::POSID::SL_FOREIGN:
        case MeCab::POSID::SN_NUMERIC:

             return true;
             break;

        default:
            return false;
            break;
    }
} // end of is_indexable_node()


} // end of namespace MeCab
