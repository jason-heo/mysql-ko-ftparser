#include <iostream>

#include <cstdlib>
#include <cctype>

#include "mysql/plugin.h"
#include "mecab_lib.h"


#if !defined(__attribute__) && (defined(__cplusplus) || !defined(__GNUC__)    || __GNUC__ == 2 && __GNUC_MINOR__ < 8)
#define __attribute__(A)
#endif

/*
  한국어를 위한 MySQL FullText Parser입니다.
  형태소 분석기로 은전한닢 프로젝트(http://eunjeon.blogspot.kr/)를 이용합니다.
*/


static int mysql_ko_ftparser_plugin_init(void* arg __attribute__((unused)))
{
    return(0);
}


static int mysql_ko_ftparser_plugin_deinit(void* arg __attribute__((unused)))
{
    return(0);
}


static int mysql_ko_ftparser_init(
                    MYSQL_FTPARSER_PARAM* param __attribute__((unused)))
{
    param->ftparser_state = (void*) MeCab::initialize_parser();
    return(0);
}


static int mysql_ko_ftparser_deinit(
                    MYSQL_FTPARSER_PARAM* param __attribute__((unused)))
{
    MeCab::deinitialize_parser((MeCab::Parser*&) param->ftparser_state);
    return(0);
}


static void add_word(MYSQL_FTPARSER_PARAM* param, char* word, size_t len)
{
    MYSQL_FTPARSER_BOOLEAN_INFO bool_info =
        { FT_TOKEN_WORD, 0, 0, 0, 0, ' ', 0 };

    param->mysql_add_word(param, word, len, &bool_info);
}


static int mysql_ko_ftparser_parse(MYSQL_FTPARSER_PARAM* param)
{
    MeCab::Parser* parser = (MeCab::Parser*) param->ftparser_state;

    for (const MeCab::Node* node = parser->parseToNode(param->doc,param->length)
         ; node != NULL
         ; node = node->next)
    {
        if (node == NULL)
        {
            throw std::runtime_error(parser->what());
        }

        if (MeCab::is_indexable_node(node))
        {
            add_word(param, (char*) node->surface, node->length); 
        }
    }

    return(0);
}


static struct st_mysql_ftparser mysql_ko_ftparser_descriptor =
{
    MYSQL_FTPARSER_INTERFACE_VERSION,       /* interface version      */
    mysql_ko_ftparser_parse,                /* parsing function       */
    mysql_ko_ftparser_init,                 /* parser init functio    */
    mysql_ko_ftparser_deinit                /* parser deinit function */
};


/*
    Plugin library descriptor
*/

mysql_declare_plugin(ftexample)
{
    MYSQL_FTPARSER_PLUGIN,             /* type                            */
    &mysql_ko_ftparser_descriptor,     /* descriptor                      */
    "mysql_ko_ftparser",               /* name                            */
    "mysql Heo",                       /* author                          */
    "Korean Full-Text Parser",         /* description                     */
    PLUGIN_LICENSE_GPL,
    mysql_ko_ftparser_plugin_init,     /* init function (when loaded)     */
    mysql_ko_ftparser_plugin_deinit,   /* deinit function (when unloaded) */
    0x0001,                            /* version                         */
    NULL,                              /* status variables                */
    NULL,                              /* system variables                */
    NULL,
    0,
}

mysql_declare_plugin_end;
