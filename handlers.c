#include "main.h"

/**
 * handle_write_char - Prints a string
 * @c: char types
 * @buffer: Buffer array to handle print
 * @flags: Calculates active flags
 * @width: get width
 * @precision: Precision specification
 * @size: Size specifier
 * Return: Number of chars printed
 */
int handle_write_char(char c, char buffer[],
			int flags, int width, int precision, int size)
{
	/* Char is stored at left and padding at buffer's right */
	int f = 0;
	char padd = ' ';

	UNUSED(precision);
	UNUSED(size);

	if (flags & F_ZERO)
		padd = '0';

	buffer[f++] = c;
	buffer[f] = '\0';
	if (width > 1)
	{
		buffer[BUFF_SIZE - 1] = '\0';
		for (f = 0; f < width - 1; f++)
			buffer[BUFF_SIZE - f - 2] = padd;

		if (flags & F_MINUS)
			return (write(1, &buffer[BUFF_SIZE - f - 1], width - 1));
		else
			return (write(1, &buffer[BUFF_SIZE - f - 1],
				 width - 1) + write(1, &buffer[0], 1));
	}

	return (write(1, &buffer[0], 1));
}

/* Write Number */
/**
 * write_number - Prints a string
 * @is_negative: List of arguments
 * @ind: Char types
 * @buffer: Buffer array to handle print
 * @flags: Calculates active flags
 * @width: get width
 * @precision: Precision specification
 * @size: Size specifier
 * Return: Number of chars to print
 */
int write_number(int is_negative, int ind, char buffer[],
		 int flags, int width, int precision, int size)
{
	int length = BUFF_SIZE - ind - 1;
	char padd = ' ', extra_ch = 0;

	UNUSED(size);

	if ((flags & F_ZERO) && !(flags & F_MINUS))
		padd = '0';
	if (is_negative)
		extra_ch = '-';
	else if (flags & F_PLUS)
		extra_ch = '+';
	else if (flags & F_SPACE)
		extra_ch = ' ';

	return (write_num(ind, buffer, flags, width,
		 precision, length, padd, extra_ch));
}

/**
 * write_num - Writes a number using a buffer
 * @ind: index at which the number starts on buffer
 * @buffer: Buffer array to handle print
 * @flags: Calculates active flags
 * @width: get width
 * @precision: Precision specification
 * @length: Number length
 * @padd: Padding char
 * @extra_c: Extra char
 * Return: Number of chars to print
 */
int write_num(int ind, char buffer[], int flags,
		 int width, int precision, int length,
		 char padd, char extra_c)
{
	int f, padd_start = 1;

	if (precision == 0 && ind == BUFF_SIZE - 2 &&
		 buffer[ind] == '0' && width == 0)
		return (0); /*printf(".0d", 0) no char is printed*/
	if (precision == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0')
		buffer[ind] = padd = ' '; /*width displayed with padding ' '*/
	if (precision > 0 && precision < length)
		padd = ' ';
	while (precision > length)
		buffer[--ind] = '0', length++;
	if (extra_c != 0)
		length++;
	if (width > length)
	{
		for (f = 1; f < width - length + 1; f++)
			buffer[f] = padd;
		buffer[f] = '\0';
		if (flags & F_MINUS && padd == ' ') /*Assigns extra char to left of buffer*/
		{
			if (extra_c)
				buffer[--ind] = extra_c;
			return (write(1, &buffer[ind], length) + write(1, &buffer[1], f - 1));
		}
		else if (!(flags & F_MINUS) && padd == ' ') /*Extra char to left of buff*/
		{
			if (extra_c)
				buffer[--ind] = extra_c;
			return (write(1, &buffer[1], f - 1) + write(1, &buffer[ind], length));
		}
		else if (!(flags & F_MINUS) && padd == '0') /*Extra char to left of padd*/
		{
			if (extra_c)
				buffer[--padd_start] = extra_c;
			return (write(1, &buffer[padd_start], f - padd_start)
				 + write(1, &buffer[ind], length - (1 - padd_start)));
		}
	}
	if (extra_c)
		buffer[--ind] = extra_c;
	return (write(1, &buffer[ind], length));
}

/**
 * write_num - Writes an unsigned number
 * @is_negative: Number indicates if num is negative
 * @ind: index at which the number starts on buffer
 * @buffer: Buffer array to handle print
 * @flags: Calculates active flags
 * @width: get width
 * @precision: Precision specification
 * @size: Size specifier
 * Return: Number of written chars to print
 */
int write_unsgnd(int is_negative, int ind, char buffer[],
		 int flags, int width, int precision, int size)
{
/*Number is stored at buffer's right and starts at position f*/
	int length = BUFF_SIZE - ind - 1, f = 0;
	char padd = ' ';

	UNUSED(is_negative);
	UNUSED(size);

	if (precision == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0')
		return (0); /*printf(".0d", 0) no char is print*/

	while (precision > length)
	{
		buffer[--ind] = '0';
		length++;
	}

	if ((flags & F_ZERO) && !(flags & F_MINUS))
		padd = '0';

	if (width > length)
	{
		for (f = 0; f < width - length; f++)
			buffer[f] = padd;

		buffer[f] = '\0';

	if (flags & F_MINUS) /*Assigns extra char to left of buffer[buffer>padd]*/
		return (write(1, &buffer[ind], length) + write(1, &buffer[0], f));
	else /*Assigns extra char to left of padding [padd>buffer]*/
		return (write(1, &buffer[0], f) + write(1, &buffer[ind], length));
	}

	return (write(1, &buffer[ind], length));
}

/**
 * write_pointer - Writes a memory address
 * @ind: index at which the number starts on buffer
 * @buffer: Buffer array to handle print
 * @flags: Calculates active flags
 * @width: get width
 * @length: Number length
 * @padd: Padding char
 * @extra_c: Extra char
 * @padd_start: Index at which padding is to start
 * Return: Number of chars to print
 */
int write_pointer(char buffer[], int ind, int length,
		 int width, int flags, char padd,
		 char extra_c, int padd_start)
{
	int f;

	if (width > length)
	{
		for (f = 3; f < width - length + 3; f++)
			buffer[f] = padd;
		buffer[f] = '\0';
		if (flags & F_MINUS && padd == ' ') /*Assigns extra char to left of buffer*/
		{
			buffer[--ind] = 'x';
			buffer[--ind] = '0';
			if (extra_c)
				buffer[--ind] = extra_c;
			return (write(1, &buffer[ind], length) + write(1, &buffer[3], f - 3));
		}
		else if (!(flags & F_MINUS) && padd == ' ') /*Extra char to left of buffer*/
		{
			buffer[--ind] = 'x';
			buffer[--ind] = '0';
			if (extra_c)
				buffer[--ind] = extra_c;
			return (write(1, &buffer[3], f - 3) + write(1, &buffer[ind], length));
		}
		else if (!(flags & F_MINUS) && padd == '0') /*Extra char to left of padd*/
		{
			if (extra_c)
				buffer[--padd_start] = extra_c;
			buffer[1] = '0';
			buffer[2] = 'x';
			return (write(1, &buffer[padd_start], f - padd_start)
				 + write(1, &buffer[ind], length - (1 - padd_start) - 2));
		}
	}
	buffer[--ind] = 'x';
	buffer[--ind] = '0';
	if (extra_c)
		buffer[--ind] = extra_c;
	return (write(1, &buffer[ind], BUFF_SIZE - ind - 1));
}
