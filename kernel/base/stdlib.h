#ifndef STDLIB_H
#define STDLIB_H

#include <base/stddef.h>
#include <base/stdint.h>
#include <base/panic.h>

#include <stdarg.h>

/**
 * @brief Converts string to double
 *
 * @remark Who needs floating point numbers in kernel mode anyway?
 * @todo Implement #strtod
 */
double strtod (const char* str, char** endptr);

/**
 * @brief Converts string to long integer.
 */
long int strtol (const char* str, char** endptr, int base);


/**
 * @brief Works like #snprintf, but accepts an explicit argument list.
 */
int vsnprintf(char* str, size_t strn, const char* format, va_list arglist);

/**
 * @brief Writes formatted text to the supplied buffer.
 * @param str the output buffer
 * @param strn size of the output buffer
 * @param format the format string which supports the usual format specifiers except for floating point values.
 * @return
 *   * if the return value is negative, an error occured
 *   * if the return value greater than or equal to zero,
 *     it denotes the number of bytes that would have been written to \c str.
 *     When this number is greater than \c strn, the string returned in \c str is incomplete.
 *
 *  @todo implement floating point formatting
 */
int snprintf(char* str, size_t strn, const char* format, ...);


#endif
