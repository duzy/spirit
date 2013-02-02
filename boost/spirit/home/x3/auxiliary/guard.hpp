/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_GUARD_FERBRUARY_2_2013_0649PM)
#define BOOST_SPIRIT_GUARD_FERBRUARY_2_2013_0649PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/support/context.hpp>
#include <boost/spirit/home/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/directive/expect.hpp>

namespace boost { namespace spirit { namespace x3
{
    enum error_handler_result
    {
        fail
      , retry
      , accept
      , rethrow
    };

    template <typename Subject, typename Handler>
    struct guard : unary_parser<Subject, guard<Subject, Handler>>
    {
        typedef unary_parser<Subject, guard<Subject, Handler>> base_type;
        static bool const is_pass_through_unary = true;

        guard(Subject const& subject, Handler handler)
          : base_type(subject), handler(handler) {}

        typedef typename
            traits::attribute_of<Subject>::type
        attribute_type;

        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, Attribute& attr) const
        {
            for (;;)
            {
                try
                {
                    Iterator i = first;
                    bool r = this->subject.parse(i, last, context, attr);
                    if (r)
                        first = i;
                    return r;
                }
                catch (expectation_failure<Iterator> const& x)
                {
                    switch (handler(first, x, context))
                    {
                        case fail:
                            return false;
                        case retry:
                            continue;
                        case accept:
                            return true;
                        case rethrow:
                            throw;
                    }
                }
            }
            return false;
        }

        Handler handler;
    };
}}}



#endif
