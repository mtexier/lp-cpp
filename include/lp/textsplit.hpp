#ifndef LP_TEXTSPLIT_INCLUDED
#define LP_TEXTSPLIT_INCLUDED

#include <boost/config/warning_disable.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>

namespace lp {

    enum tokenids 
    {
        IDANY = boost::spirit::lex::min_token_id + 10
    };

    template < typename Lexer >
    struct lp_tokens
        : boost::spirit::lex::lexer< Lexer >
    {
        lp_tokens()
        {
            start_lp_comment = "\\/\\*>";
            end_lp_comment   = "\\*\\/";

            this->self.add
                ( start_lp_comment )
                ( end_lp_comment )
                ( ".", IDANY )
                ;
        }

        // the token 'word' exposes the matched string as its parser attribute
        boost::spirit::lex::token_def< std::string > start_lp_comment;
        boost::spirit::lex::token_def< std::string > end_lp_comment;
    };

    enum text_type { NORMAL, LP };

    template < typename Iterator >
    struct lp_text_grammar
        : boost::spirit::qi::grammar< Iterator >
    {
        template < typename Tokenizer >
        lp_text_grammar ( Tokenizer const &tokenizer)
            : lp_text_grammar::base_type ( main ),
              state ( NORMAL ),
              buffer ( "" )
        {
            using boost::spirit::qi::token;

            using namespace boost::spirit::qi::labels;

            namespace phoenix = boost::phoenix;
            using boost::phoenix::if_;
            using boost::phoenix::ref;

            main = *( tokenizer.start_lp_comment [
                          phoenix::bind ( &lp_text_grammar::save_buffer, this ),
                          ref ( state ) = LP ]

                    | tokenizer.end_lp_comment [
                        if_ ( ref ( state ) == LP ) [
                            phoenix::bind ( &lp_text_grammar::save_buffer,
                                            this ),
                            ref ( state ) = NORMAL ] ]

                    | boost::spirit::qi::token ( IDANY ) [
                        ref ( buffer ) << _1 ]
                    )
                 ;
        }

        void
        save_buffer ()
        {
            // TODO: to be improved.
            std::pair< text_type, std::string > text ( state, buffer.str () );
            data.push_back ( text );
            buffer.str ( "" );
        }

        text_type                                          state;
        std::stringstream                                  buffer;
        std::vector< std::pair< text_type, std::string > > data;

        boost::spirit::qi::rule< Iterator > main;
    };

}

#endif
