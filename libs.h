/** 常用的宏实现 **/
#include "public.h"

#define FUNC_POSITION \
				do{printf("The %s function in the line %d is pass !\n", __FUNCTION__, __LINE__);}while(0)

#define FREE(p)	\
				do{ free(p); p = NULL;} while(0)
					

char *get_markvalue(char * , char *);

char * insert_mark(char *, char *, char *);

char * replace_str(char *, char *, char *);

char * trim(char *, char );

int check_is_dir (const char *);

int count_file(char *);

char * make_uuid();

int get_max_tty_num();

int code_convert(char *, char * , char * , size_t  , char * , size_t);
int utf8_to_gb2312(char * , int  , char * , int );
int gb2312_to_utf8(char * , int  , char * , int );
int gb18030_to_utf8(char * , int  , char * , int );

void custom_delay(int);





