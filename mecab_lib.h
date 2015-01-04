#include <iostream>
#include <stdexcept>

#include "mecab.h"

namespace MeCab
{
    typedef Tagger Parser;

    Parser* initialize_parser();
    void deinitialize_parser(Parser*& Parser);

    bool is_indexable_node(const MeCab::Node* node);
    
    namespace POSID
    {
        enum POSID_ENUM {
            COMPOUND = 1,
            NN_NOUN = 150,
            SL_FOREIGN = 162,
            SN_NUMERIC = 163
        };
    }
}
