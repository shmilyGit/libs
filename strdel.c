//功能： 从一个字符串中删除指定的字符串

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * strdel(char *str, char *obj)
{
	//使用的时候要判断一下返回值,不为NULL的时候,使用完一定要释放
	int num_obj = 0, ret_size = 0, offsize = 0, end_str = 0;
	char *start = str;
	char *p = NULL;
	char *ret_buf = NULL, *ret = NULL;

	if (!obj)
		return NULL;

	while ((p = strstr (start, obj)))
	{
		num_obj ++;

		p++;
		if (*p != '\0')
		{
			start = p;
		}
	}
	
	ret_size = strlen(str) - num_obj * strlen(obj) + 1;

	ret_buf = (char *)malloc(ret_size);
	memset (ret_buf, 0, ret_size);


	ret = ret_buf;
	start = str;

	while (num_obj --)
	{
		p = strstr(start, obj);
		ret_buf += offsize;
		memcpy(ret_buf, start, p - start);
		offsize = p - start;
		start = p + strlen(obj);
	}

	//拷贝str中最后一个obj到结尾的字符
	memcpy(ret_buf + offsize, start, strlen(start));

	return ret;
}

int main()
{
	char buff[100] =  "&nbsp;&nbsp;&nbsp;&nbsp;你好我在做一个test测试.";

	char *p = strdel (buff, "&nbsp;");
	printf ("p : %s\n", p);
	free(p);
	return 0;
}
