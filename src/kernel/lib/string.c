#include <lib/string.h>

int strcmp(char *s1, char *s2)
{
	while (*s1 == *s2++)
		if (*s1++ == 0)
			return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}

int strncmp(char *s1, char *s2, int n)
{
	if (n == 0)
		return (0);
	do {
		if (*s1 != *s2++)
			return (*(unsigned char *)s1 - *(unsigned char *)--s2);
		if (*s1++ == 0)
			break;
	} while (--n != 0);
	return (0);
}

void memcpy(char* a, char* b, int n)
{
    int i = 0;
    while (i < n) {
        a[i] = b[i];
        i++;
    }
}

int strlen(char* s)
{
    int i = 0;
    while (s[i]) {
        i++;
    }
    return i;
}
