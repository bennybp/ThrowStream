/*
   An example of using the ThrowStream class and macros.
   Copyright 2013 Benjamin Pritchard
   Relased under the MIT License
*/

#include <iostream>
#include <string>
#include <sstream>
#include "ThrowStream.h"

using std::string;
using std::cout;
using std::cin;
using std::stringstream;
using std::exception;

double Inverse(int i)
{
    if(i == 0)
        THROWSTREAM << "Error: I can't take the inverse of 0!";

    return 1.0/double(i);
}

double MultiplyInverse(double a, int b)
{
    try
    {
        return Inverse(a)*Inverse(b);
    }
    catch(exception & ex)
    {
        THROWSTREAMAPPEND(ex) << "Called from MultiplyInverse: a = "
                              << a << " b = " << b;
    }
}


int main(int argc, char ** argv)
{
    try
    {
        cout << "\n\nEnter two integers and I will calculate (1/a)*(1/b)\n";
        int a,b;
        string stra, strb;
        cout << "\nEnter an integer (a) :> ";
        getline(cin,stra);
        cout << "\nEnter an integer (b) :> ";
        getline(cin,strb);

        bool error = false;
        THROWSTREAMOBJ(ts) << "Error parsing your numbers!";
        stringstream ssa(stra), ssb(strb);

        ssa >> a;
        ssb >> b;

        if(ssa.bad() || ssa.fail() || !ssa.eof())
        {
            error = true;
            THROWSTREAMOBJAPPEND(ts) << "Error parsing integer 'a': bad, fail, eof = "
                                     << ssa.bad() << ssa.fail() << ssa.eof();
        }
        if(ssb.bad() || ssb.fail() || !ssb.eof())
        {
            error = true;
            THROWSTREAMOBJAPPEND(ts) << "Error parsing integer 'b': bad, fail, eof = "
                                     << ssb.bad() << ssb.fail() << ssb.eof();
        }
        if(error)
            throw ts;

        cout << "\n\n(1/a)*(1/b) = " << MultiplyInverse(a,b) << "\n\n";
    }
    catch(const exception & ex)
    {
        cout << "\n\nException! what() = " << ex.what() << "\n\n";
    }

    return 0;
}

