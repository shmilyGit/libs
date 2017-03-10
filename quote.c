/**
 *功能: 转义单引号,主要是针对某些数据库的执行命令
 *
 *编译: gcc quote.c `pkg-config --cflags --libs glib2.0`
*/

#include "puiblic.h"

char *quote (char* string, int length)
{
  char *new, *new_start;
  const char *start, *end;
  int count = 0;

  start = string;
  while ((start = strchr (start, '\''))) start++, count++;

  new = new_start = g_malloc0 (length + count + 1);

  start = string;
  end = string + length;
  for (; start < end; start++, new++)
    {
      char ch = *start;
      if (ch == '\'')
        {
          *new = '\'';
          new++;
          *new = '\'';
        }
      else
        *new = ch;
    }

  return new_start;
}

int main()
{
	char *str = "select name from stu where name=Lucy's";
	char * p = quote (str, strlen(str));
	printf ("%s\n", p);
	g_free(p);
}
