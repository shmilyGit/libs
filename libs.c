/**
	*功能:在xml中取某一个标签的值
	*
	*param[in]  char *		p_xml 	传入的xml
	*param[in]	char *		p_mar 	取该标签的值
	*
	*/
	
char *get_markvalue(char * p_xml, char *p_mark)
{
  char *xml = p_xml;
  char *mark = p_mark;
  char *res = NULL;
  char *p = NULL;
	char *q = NULL;
  int n = 0; 

  res = strstr(xml, mark);
  if (NULL == res) 
    return NULL;
	
	n = strlen(p_mark);

  res += n;
  p = res; 

	q = strstr(res, "<");	
	n = q - res;

  char *buff = (char *)malloc(n + 1);
  memset(buff, 0, n + 1);
  strncpy(buff, p, n);

  return buff;

}


/**
 * 功能：在xml中指定标签后添加一个标签
 *
 *pamra[in]		char *		p_xml				传入的p_xml
 *param[in]		char *		p_mark  			指定的标签,在其后插入
 *param[in]		char *		p_insert  			要插入的标签
 *
 */

char * insert_mark(char *p_xml, char *p_mark, char *p_insert)
{
	char *ret_xml = NULL;
	char *xml = p_xml;
	char *mark = p_mark;
	char *res = NULL;
	int n = 0;
	int m_count = 0;
	
	ret_xml = (char *)malloc(strlen(p_xml) + strlen(p_insert) + 1);	
	memset(ret_xml, 0, strlen(p_xml) + strlen(p_insert) + 1);

	n = strlen(mark);

	res = strstr(xml, mark);
	if (res == NULL)
		return xml;
	
	res += n;

	while (xml != res)
	{
		xml ++;
		m_count ++;
	}

	strncpy(ret_xml, p_xml, m_count);	
	strcat(ret_xml, p_insert);	
	strcat(ret_xml, res);

	return ret_xml;
}


/**
 * 简单的字符替换函数
 *
 * param[in]:	char *		string			The operation chars.
 * param[in]:	char *		src				The replaced  char.
 * param[in]:	char *		dest			The replace  	char.
 *
 * 当返回值不为NULL的时候记得释放.
 */

char * replace_str(char *string, char *src, char *dest)
{
	if (strstr(string, src))
	{
		char *p = NULL;
		char *buff = (char *)malloc(strlen(string));
		memset(buff, 0, strlen(string));
		
		p = strtok(string,src);
		while (p)
		{
			if (strlen(buff) == 0)
				strcpy(buff,p);
			else
			{
				strcat(buff,dest);
				strcat(buff,p);
			}
			p = strtok(NULL,src);
		}
		return buff;
	}
	else
		return NULL;
}


/**
 * 功能:去除字符串前后的空格
 * 
 * param[in]:		char *		oper		要操作的字符串
 * param[in]:		char 		mode		模式：前后(a),左(l),右(r)
 *
 * 注意:使用该函数之后，记得要释放内存
 */

char * trim(char *oper, char mode)
{
	char *ret = NULL;
	char *p = oper;
	int n = 0;

	if (mode == 'l')
	{
		while (*p == ' ')
			p ++;

		ret = (char *)malloc(strlen(p) + 1);
		memset(ret, 0, strlen(p) + 1);
		strcpy(ret, p);
	}

	if (mode == 'r')
	{
		char *q = strrchr(p, ' ');
		
		if (q != NULL)
		{
			if (*(++q) != '\0')
			{
				ret = (char *)malloc(strlen(p) + 1);
				memset(ret, 0, strlen(p) + 1);
				strcpy(ret, p);
				printf("ret = %s\n",ret);
			}
			else 
			{
				q -= 2;
				while (*q == ' ')
					q --;
				n = q - p;
				ret = (char *)malloc(n + 2);
				memset(ret, 0, n + 2);
				strncpy(ret, p, n + 1);
			}
		}
	}

	if (mode == 'a')
	{
		while (*p == ' ')
			p ++;

		char *q = strrchr(p, ' ');
		
		if (q != NULL)
		{
			if (*(++q) != '\0')
			{
				ret = (char *)malloc(strlen(p) + 1);
				memset(ret, 0, strlen(p) + 1);
				strcpy(ret, p);
				printf("ret = %s\n",ret);
			}
			else 
			{
				q -= 2;
				while (*q == ' ')
					q --;
				n = q - p;
				ret = (char *)malloc(n + 2);
				memset(ret, 0, n + 2);
				strncpy(ret, p, n + 1);
			}
		}else
		{
				ret = (char *)malloc(strlen(p) + 1);
				memset(ret, 0, strlen(p) + 1);
				strcpy(ret, p);
		}
	}
	
	return ret;
}


/**
 * 功能: 检测是否是目录
 *
 * */

int check_is_dir (const char *name)
{
  struct stat sb;

  if (stat (name, &sb))
    {
      return -1;
    }
  else
    {
      return (S_ISDIR (sb.st_mode));
    }
}


/**
 * 功能: 统计指定目录有多少个文件.
 *
 * param[in]:  char *    dir_path  	<</ opened the directory name.
 *
 * return:	返回文件数.
 */

int count_file(char *dir_path)
{
	int num = 0;
	DIR *pdir = NULL;
	struct dirent *pdirent;

	if ((pdir = opendir(dir_path)) == NULL)
		printf("open dir_path %s failed!\n", dir_path);
	else
	{
		while ((pdirent = readdir(pdir)) != NULL)
		{
			if (!strcmp(pdirent->d_name, ".") || !strcmp(pdirent->d_name, ".."))
				continue;
			num ++;
		}
	}
	return num;
}


/**
 * 功能: 获得一个UUID，需要安装libuuid库
 *
 * */
char * make_uuid()
{
  char *id;
  uuid_t uuid;

  /* Generate an UUID. */
  uuid_generate (uuid);
  if (uuid_is_null (uuid) == 1)
    {
      g_warning ("%s: failed to generate UUID", __FUNCTION__);
      return NULL;
    }

  /* Allocate mem for string to hold UUID. */
  id = malloc (sizeof (char) * 37);
	memset(id, 0, sizeof(char) * 37);
  if (id == NULL)
    {
      g_warning ("%s: Cannot export UUID to text: out of memory", __FUNCTION__);
      return NULL;
    }

  /* Export the UUID to text. */
  uuid_unparse (uuid, id);

  return id;
}


/**
 * 功能: 取最大终端号(控制台终端)
 *
 *
 * num: 统计有多少个终端
 * temp: 返回的终端号
 */

int max (int a, int b)
{
	return a > b ? a : b;
}

int get_max_tty_num()
{
	int num = 0;
	int i = 0;
	int con_num[1024];
	int temp = 0;
	DIR *pdir = NULL;
	struct dirent *pdirent;

	if ((pdir = opendir("/dev/pts/")) == NULL)
		printf("open /dev/pts/ dir failed!\n");
	else
	{
		while ((pdirent = readdir(pdir)) != NULL)
		{
			if (!strcmp(pdirent->d_name, ".") || !strcmp(pdirent->d_name, ".."))
				continue;
			con_num[i] = atoi(pdirent->d_name);
			num ++;
			i ++;
		}
	}

	if (num == 1)
		return con_num[0];
	else if (num > 1)	
	{
		temp = con_num[0];
		for (i = 1; i < num; i++)
		{
			temp = max(temp, con_num[i]);	
		}
	}

	return temp;
}


//编码转换:从一种编码转为另一种编码
int code_convert(char *from_charset,char *to_charset,char *inbuf, size_t inlen,char *outbuf,size_t outlen)
{
    iconv_t cd;
    char **pin = &inbuf;
    char **pout = &outbuf;
    int ret = 0;

    cd = iconv_open(to_charset,from_charset);
    if (cd==0) 
    {
        return -1;
    }
    memset(outbuf,0x00,outlen);
    if (iconv(cd,pin,&inlen,pout,&outlen)==-1)
    {
        ret = -1;
    }
    iconv_close(cd);
    return ret;
}

//UNICODE码转为GB2312码
int utf8_to_gb2312(char *inbuf,int inlen,char *outbuf,int outlen)
{
    return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}
//GB2312码转为UNICODE码
int gb2312_to_utf8(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}

//GB18030码转为UNICODE码
int gb18030_to_utf8(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    return code_convert("gb18030","utf-8",inbuf,inlen,outbuf,outlen);
}
