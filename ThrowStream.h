/*! \file
 *  \brief     An easy way to throw descriptive exceptions
 *  \author    Benjamin Pritchard (ben@bennyp.org)
 *  \copyright 2013 Benjamin Pritchard. Released under the MIT License
 */

#ifndef BPLIB_THROWSTREAM_H
#define BPLIB_THROWSTREAM_H

#include <exception>
#include <string>
#include <sstream>
#include <iostream>


using std::string;
using std::exception;
using std::ostream;
using std::stringstream;


// Forward declaration
class ThrowStream;
ostream & operator<<(ostream & os, const ThrowStream & ts);


//! Main ThrowStream class
/*!
    This class allows appending of exception information, creating a backtrace-like
    output.
 */
class ThrowStream : public exception
{
private:
    string _desc; //!< The current, full backtrace

public:
    friend ostream & operator<<(ostream & os, const ThrowStream & ts);

    // Constructors
    //! Construct using the line, file, and function
    /*!
     *  \param[in] line The line on which the exception occurred
     *  \param[in] file The file in which the exception occurred
     *  \param[in] function function The function in which the exception occurred
     */
    ThrowStream(const unsigned long line, const string & file, const string & function)
    {
        Append(line, file, function);
    }


    //! Construct by copying an exception and adding a new line, file, and function
    /*!
     *  \param[in] ex An exception to copy
     *  \param[in] line The line on which the exception occurred
     *  \param[in] file The file in which the exception occurred
     *  \param[in] function function The function in which the exception occurred
     */
    ThrowStream(const exception & ex, unsigned long line, const string & file, const string & function)
    {
        Append(ex, line, file, function);
    }

    //! Destructor definition needed to declare it throw()
    ~ThrowStream() throw() { }


    //! Add a new line to the backtrace
    /*!
     *  This returns ThrowStream & to allow using operator<<
     *
     *  \param[in] line The line on which the exception occurred
     *  \param[in] file The file in which the exception occurred
     *  \param[in] function function The function in which the exception occurred
     */
    ThrowStream & Append(const unsigned long line, const string & file, const string & function)
    {
        stringstream ss;

        ss << '\n';

#ifdef THROWSTREAM_EXCEPTIONSOURCE
        ss << "( " << file << ":" << line << " , in " << function << "() )    ->  ";
#endif

        _desc.append(ss.str());

        return *this;
    }


    //! Add a new line to the backtrace, copying an existing exception
    /*!
     *  This returns ThrowStream & to allow using operator<<
     *
     *  \param[in] ex An exception to copy
     *  \param[in] line The line on which the exception occurred
     *  \param[in] file The file in which the exception occurred
     *  \param[in] function function The function in which the exception occurred
     */
    ThrowStream & Append(const exception & ex, const unsigned long line,
                         const string & file, const string & function)
    {
        //depends on if this is actually a throwstream
        const ThrowStream * pts;
        if((pts = dynamic_cast<const ThrowStream *>(&ex)))
        {
            _desc.append(pts->_desc);
        }
        else
        {
            //will end up a double append, but oh well, it's the
            //best I can do with only an exception
            Append(line,file,function) << ex.what();
        }

        Append(line,file,function);

        return *this;
    }


    //! Get the description as a character array
    /*!
     *  This will output a (hopefully) nice backtrace
     *
     *  \return An array of characters representing the backtrace
     */
    char const* what() const throw()
    {
        return _desc.c_str();
    }


    //! Add information to the current entry in the backtrace
    /*!
        \param[in] rhs Data to add. This must be able to be inserted into
                       a stringstream object.
        \return The modified ThrowStream object
     */
    template<typename T>
    ThrowStream & operator<<(const T & rhs)
    {
        stringstream ss;
        ss << rhs;
        _desc.append(ss.str());
        return *this;
    }
};


//! Create a THROWSTREAM object representing an exception at this location, and throw it
/*!
 *  This is used primarily to throw the first exception. To add a description:
 *
 *  \code{.cpp}
 *    THROWSTREAM << "Some description: " << somevar;
 *  \endcode
 */
#define THROWSTREAM throw ThrowStream(__LINE__, __FILE__, __FUNCTION__)


//! Copy information from another exception, and then throw the exception
/*!
 *  The exception to copied can be an std::exception or any derivative of std::exception
 *
 *  \code{.cpp}
 *    THROWSTREAMAPPEND(somex) << "Called from here: somevar = " << somevar;
 *  \endcode
 */
#define THROWSTREAMAPPEND(ex) throw ThrowStream( (ex), __LINE__, __FILE__, __FUNCTION__)


//! Creates a ThrowStream object with a specified name with the current location added
/*!
 *  This is to allow appending to it later (see THROWSTREAMOBJAPPEND). This does not
 *  actually throw the exception
 *
 *  \code{.cpp}
 *    THROWSTREAMOBJ(myex) << "Called from here: somevar = " << somevar;
 *  \endcode
 *
 *  \param ex The name of the object to create
 */
#define THROWSTREAMOBJ(ex) ThrowStream (ex)(__LINE__, __FILE__, __FUNCTION__); (ex)


//! Add information to an already-named ThrowStream object
/*!
 *  Adds location information. This does not throw the exception.
 *
 *  \code{.cpp}
 *    THROWSTREAMOBJAPPEND(myex) << "Called from here: somevar = " << somevar;
 *  \endcode
 *
 *  \param ex The name of the object to append to
 */
#define THROWSTREAMOBJAPPEND(ex) (ex).Append(__LINE__, __FILE__, __FUNCTION__)


//! Copy a ThrowStream object into an already-created object, and add the current location.
/*!
 *  Adds location information as well. This does not throw the exception.
 *  The exception to copy can be an std::exception or any derivative of std::exception
 *
 *  \param ex The object to append to
 *  \param ey The exception to be appended
 */
#define THROWSTREAMOBJAPPENDCOPY(ex,ey) ex.Append((ey), __LINE__, __FILE__, __FUNCTION__)


//! Creates a ThrowStream object based on another exception.
/*!
 *  Adds location information as well. This does not throw the exception.
 *  The exception to copy can be an std::exception or any derivative of std::exception
 *
 *  \param ex The name of the object to create
 *  \param ey The exception object to copy
 */
#define THROWSTREAMOBJCOPY(ex,ey) ThrowStream (ex)((ey),__LINE__, __FILE__, __FUNCTION__ ); (ex)


//! Allow output to an ostream using the stream operator
/*!
    \param[in,out] os An ostream object to output to
    \param[in] ts A throwstream object to output
    \return The ostream object again
 */
inline ostream & operator<<(ostream & os, const ThrowStream & ts)
{
    os << ts._desc;
    return os;
}

#endif //BPLIB_THROWSTREAM_H

