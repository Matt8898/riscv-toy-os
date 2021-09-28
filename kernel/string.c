#include "string.h"
#include <limits.h>

int strcmp(const char* s1, const char* s2) {
    while(*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

size_t strlen(const char *str) {
        const char *s;
        for (s = str; *s; ++s);
        return (s - str);
}

int strncmp( const char * s1, const char * s2, size_t n ) {
    while ( n && *s1 && ( *s1 == *s2 ) ){
        ++s1;
        ++s2;
        --n;
    }
    if ( n == 0 ) {
        return 0;
    }
    else {
        return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
    }
}

bool prefix(const char *pre, const char *str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

unsigned long strtoul(const char *__restrict nptr, int base) {
	const unsigned char *s = (const unsigned char *)nptr;
	unsigned long acc;
	int c;
	unsigned long cutoff;
	int neg = 0, any, cutlim;

	do {
		c = *s++;
	} while (c == ' ' || c == '\t');
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
	cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (c >= '0' && c <= '9')
			c -= '0';
		else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			c -= (c >= 'A' && c <= 'Z') ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
               if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = (unsigned long)ULONG_MAX;
	} else if (neg)
		acc = -acc;
	return (acc);
}
