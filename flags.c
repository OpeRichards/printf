#include "main.h"

/**
 * get_flags - Calculates active flags
 * @format: Formatted string in which to print arguments
 * @i: Take a parameter
 * Return: Flags
 */
int get_flags(const char *format, int *i)
{
	/* - + 0 # ' ' */
	/* 1 2 4 8  16 */
	int w, curr_i;
	int flags = 0;
	const char FLAGS_CH[] = {'-', '+', '0', '#', ' ', '\0'};
	const int FLAGS_ARR[] = {F_MINUS, F_PLUS, F_ZERO, F_SPACE, 0};

	for (curr_i = *i + 1; format[curr_i] != '\0'; curr_i++)
	{
		for (w = 0; FLAGS_CH[w] != '\0'; w++)
			if (format[curr_i] == FLAGS_CH[w])
			{
				flags |= FLAGS_ARR[w];
				break;
			}
		if (FLAGS_CH[w] == 0)
			break;
	}
	*i = curr_i - 1;
	return (flags);
}
