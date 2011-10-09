#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include <boost/mpl/vector.hpp>

#include <boost/spirit/include/qi_parse.hpp>

#include <lp/textsplit.hpp>

struct lp_data_printer
{
    void
    operator () ( std::pair< lp::text_type, std::string > const &text ) const
    {
        std::cout << "Block ("
                  << ( text.first == lp::NORMAL ? "Normal" : "Lp" )
                  << ")"
                  << std::endl;
        std::cout << text.second << std::endl;
    }
};

int main ( int argc, char *argv[] )
{
    char const* filename;
    if ( argc > 1 )
    {
        filename = argv[1];
    }
    else
    {
        std::cerr << "Error: No input file provided." << std::endl;
        return 1;
    }

    std::ifstream in ( filename, std::ios_base::in );

    if ( !in )
    {
        std::cerr << "Error: Could not open input file: "
                  << filename << std::endl;
        return 1;
    }

    std::string storage; // We will read the contents here.
    in.unsetf ( std::ios::skipws ); // No white space skipping!
    std::copy (
        std::istream_iterator< char > ( in ),
        std::istream_iterator< char > (),
        std::back_inserter ( storage ) );

    typedef boost::spirit::lex::lexertl::token<
        const char *, boost::mpl::vector< std::string >
    > token_type;

    typedef boost::spirit::lex::lexertl::lexer< token_type > lexer_type;

    typedef lp::lp_tokens< lexer_type >::iterator_type iterator_type;
    lp::lp_tokens< lexer_type > lexer;

    lp::lp_text_grammar< iterator_type > parser ( lexer );

    char const * iter = storage.c_str ();
    char const * end  = &iter[ storage.size () ];

    bool r =
        boost::spirit::lex::tokenize_and_parse ( iter, end, lexer, parser );

    if ( r && iter == end )
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n";
        std::cout << "-------------------------\n";

        std::for_each ( parser.data.begin (),
                        parser.data.end (),
                        lp_data_printer () );
        return 0;
    }
    else
    {
        char const * some = iter + 30;
        std::string context ( iter, ( some > end ) ? end : some );
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \": " << context << "...\"\n";
        std::cout << "-------------------------\n";
        return 1;
    }

    return 0;
}
