#include "logging.h"

#include "stm32f1xx.h"

// static int _printf_assist(const char *format, va_list args);
// static int (*print_case(const char *format))(va_list);
// static int __io_putchar(const char c);

// static int print_int(va_list list);
// static int print_char(va_list list);
// static int print_str(va_list list);
// static int print_perc(va_list list);

// /*
//  * can print %d, %c, %s
//  */
// int logging(const char *format, ...)
// {
//     va_list args;
//     int len;

//     if (format == NULL)
//         return (-1);

//     va_start(args, format);
//     len = _printf_assist(format, args);
//     va_end(args);

//     return (len);
// }

// static int _printf_assist(const char *format, va_list args)
// {
//     int (*func_ptr)(va_list);
//     int i, len;

//     i = 0;
//     len = 0;
//     for (; format && format[i]; i++)
//     {
//         if (format[i] == '%')
//         {
//             if (format[i + 1] == '\0')
//                 return (-1);

//             while (format[i + 1] == ' ')
//             {
//                 if (format[i + 2] == '\0')
//                     return (-1);
//                 i++;
//             }

//             func_ptr = print_case(&format[++i]); /* for invalid formats */

//             if (func_ptr != NULL)

//                 len += func_ptr(args);

//             else

//                 len += __io_putchar('%') + __io_putchar(format[i]);
//         }
//         else
//             len += __io_putchar(format[i]);
//     }

//     return (len);
// }

// static int __io_putchar(const char c)
// {
//     USART_SendData(USART1, c);
//     while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//     {
//     };
//     return 1;
//     // return (write(1, &c, 1));
// }
// static int (*print_case(const char *format))(va_list)
// {
//     int i = 0;

//     spec_handler fmt_types[] = {
//         {"c", print_char},
//         {"s", print_str},
//         {"%", print_perc},
//         {"d", print_int},
//         {"i", print_int},
//         {NULL, NULL}};

//     for (; fmt_types[i].specifier; i++)
//     {
//         if (*format == *(fmt_types[i].specifier))
//         {
//             return (fmt_types[i].conversion_func);
//         }
//     }
//     return (NULL);
// }
// /**
//  * print_int - prints integers from va_list
//  * @list: list of integers to be printed
//  * Return: number of characters printed
//  */
// int print_int(va_list list)
// {
//     int num = va_arg(list, long);
//     char buffer[32];

//     itoa(num, buffer, 10);
//     char *ptr = &buffer[0];
//     int len = 0;
//     while (*ptr)
//     {
//         __io_putchar(*ptr);
//         ptr++;
//         len++;
//     }
//     return len;
// }

// /**
//  * print_char - prints a character from va_list
//  * @list: list of args
//  * Return: number of characters printed
//  */

// int print_char(va_list list)
// {
//     __io_putchar(va_arg(list, int));
//     return (1);
// }

// /**
//  * print_str - prints a string from va_list
//  * @list: containins string to be printed
//  * Return: number of characters printed
//  */

// int print_str(va_list list)
// {
//     int i = 0;
//     char *str;

//     str = va_arg(list, char *);
//     if (!str)
//         str = "(null)";

//     for (; str[i]; i++)
//         __io_putchar(str[i]);

//     return (i);
// }

// /**
//  * print_perc - prints percent sign
//  * @list: list of args
//  * Return: number of characters printed
//  */

// static int print_perc(va_list list)
// {
//     (void)(list);
//     __io_putchar('%');
//     return (1);
// }

// https://gist.github.com/elevendroids/d5c3a6cef840f55f7258

/*
	Copyright 2001, 2002 Georges Menie (www.menie.org)
	stdarg version contributed by Christian Ettinger

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
	putchar is the only external dependency for this file,
	if you have a working putchar, leave it commented out.
	If not, uncomment the define below and
	replace outbyte(c) by your own function call.

#define putchar(c) outbyte(c)
*/

#include <stdarg.h>

void Print_Buffer(uint8_t *buffer, uint16_t length)
{
	for (uint16_t i = 0; i < length; i++)
	{
		USART_SendData(USART1, buffer[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		{
		};
	}
}

static int ___io_putchar(const char c)
{
	USART_SendData(USART1, c);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
	};
	return 1;
	// return (write(1, &c, 1));
}

static void printchar(char **str, int c)
{
	// extern int putchar(int c);

	if (str)
	{
		**str = c;
		++(*str);
	}
	else
		___io_putchar(c); //(void)putchar(c);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad)
{
	register int pc = 0, padchar = ' ';

	if (width > 0)
	{
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr)
			++len;
		if (len >= width)
			width = 0;
		else
			width -= len;
		if (pad & PAD_ZERO)
			padchar = '0';
	}
	if (!(pad & PAD_RIGHT))
	{
		for (; width > 0; --width)
		{
			printchar(out, padchar);
			++pc;
		}
	}
	for (; *string; ++string)
	{
		printchar(out, *string);
		++pc;
	}
	for (; width > 0; --width)
	{
		printchar(out, padchar);
		++pc;
	}

	return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;

	if (i == 0)
	{
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints(out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0)
	{
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN - 1;
	*s = '\0';

	while (u)
	{
		t = u % b;
		if (t >= 10)
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg)
	{
		if (width && (pad & PAD_ZERO))
		{
			printchar(out, '-');
			++pc;
			--width;
		}
		else
		{
			*--s = '-';
		}
	}

	return pc + prints(out, s, width, pad);
}

static int print(char **out, const char *format, va_list args)
{
	register int width, pad;
	register int pc = 0;
	char scr[2];

	for (; *format != 0; ++format)
	{
		if (*format == '%')
		{
			++format;
			width = pad = 0;
			if (*format == '\0')
				break;
			if (*format == '%')
				goto out;
			if (*format == '-')
			{
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0')
			{
				++format;
				pad |= PAD_ZERO;
			}
			for (; *format >= '0' && *format <= '9'; ++format)
			{
				width *= 10;
				width += *format - '0';
			}
			if (*format == 's')
			{
				register char *s = (char *)va_arg(args, int);
				pc += prints(out, s ? s : "(null)", width, pad);
				continue;
			}
			if (*format == 'd')
			{
				pc += printi(out, va_arg(args, int), 10, 1, width, pad, 'a');
				continue;
			}
			if (*format == 'x')
			{
				pc += printi(out, va_arg(args, int), 16, 0, width, pad, 'a');
				continue;
			}
			if (*format == 'X')
			{
				pc += printi(out, va_arg(args, int), 16, 0, width, pad, 'A');
				continue;
			}
			if (*format == 'u')
			{
				pc += printi(out, va_arg(args, int), 10, 0, width, pad, 'a');
				continue;
			}
			if (*format == 'c')
			{
				/* char are converted to int then pushed on the stack */
				scr[0] = (char)va_arg(args, int);
				scr[1] = '\0';
				pc += prints(out, scr, width, pad);
				continue;
			}
		}
		else
		{
		out:
			printchar(out, *format);
			++pc;
		}
	}
	if (out)
		**out = '\0';
	va_end(args);
	return pc;
}

int Logging(logging_level_t level, const char *format, ...)
{
	if (level >= LOOGING_LEVEL)
	{
		va_list args;
		va_start(args, format);
		return print(0, format, args);
	}
	return -1;
}

// int slogging(char *out, const char *format, ...)
// {
//         va_list args;

//         va_start( args, format );
//         return print( &out, format, args );
// }

#ifdef TEST_PRINTF
int main(void)
{
	char *ptr = "Hello world!";
	char *np = 0;
	int i = 5;
	unsigned int bs = sizeof(int) * 8;
	int mi;
	char buf[80];

	mi = (1 << (bs - 1)) + 1;
	printf("%s\n", ptr);
	printf("printf test\n");
	printf("%s is null pointer\n", np);
	printf("%d = 5\n", i);
	printf("%d = - max int\n", mi);
	printf("char %c = 'a'\n", 'a');
	printf("hex %x = ff\n", 0xff);
	printf("hex %02x = 00\n", 0);
	printf("signed %d = unsigned %u = hex %x\n", -3, -3, -3);
	printf("%d %s(s)%", 0, "message");
	printf("\n");
	printf("%d %s(s) with %%\n", 0, "message");
	sprintf(buf, "justif: \"%-10s\"\n", "left");
	printf("%s", buf);
	sprintf(buf, "justif: \"%10s\"\n", "right");
	printf("%s", buf);
	sprintf(buf, " 3: %04d zero padded\n", 3);
	printf("%s", buf);
	sprintf(buf, " 3: %-4d left justif.\n", 3);
	printf("%s", buf);
	sprintf(buf, " 3: %4d right justif.\n", 3);
	printf("%s", buf);
	sprintf(buf, "-3: %04d zero padded\n", -3);
	printf("%s", buf);
	sprintf(buf, "-3: %-4d left justif.\n", -3);
	printf("%s", buf);
	sprintf(buf, "-3: %4d right justif.\n", -3);
	printf("%s", buf);

	return 0;
}

/*
 * if you compile this file with
 *   gcc -Wall $(YOUR_C_OPTIONS) -DTEST_PRINTF -c printf.c
 * you will get a normal warning:
 *   printf.c:214: warning: spurious trailing `%' in format
 * this line is testing an invalid % at the end of the format string.
 *
 * this should display (on 32bit int machine) :
 *
 * Hello world!
 * printf test
 * (null) is null pointer
 * 5 = 5
 * -2147483647 = - max int
 * char a = 'a'
 * hex ff = ff
 * hex 00 = 00
 * signed -3 = unsigned 4294967293 = hex fffffffd
 * 0 message(s)
 * 0 message(s) with %
 * justif: "left      "
 * justif: "     right"
 *  3: 0003 zero padded
 *  3: 3    left justif.
 *  3:    3 right justif.
 * -3: -003 zero padded
 * -3: -3   left justif.
 * -3:   -3 right justif.
 */

#endif