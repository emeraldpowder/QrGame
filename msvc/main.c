#include <windows.h>

extern "C" unsigned int strlen(const char *f)
{
   INT i=0;
   while(*f++) i++;
   return i;
}
void printf(char * fmtstr, ...)
{
   DWORD dwRet;
   CHAR buffer[256];
   va_list v1;
   va_start(v1,fmtstr);
   wvsprintf(buffer,fmtstr,v1);
   WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), buffer, strlen(buffer), &dwRet, 0);
   va_end(v1);
}
void main()
{
   printf("hello world!");
}