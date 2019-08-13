#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dll.h"

/*================== 元素结构体 定义 ===============*/
typedef struct list_t 
{
    struct list_t *list_prev, *list_next;
    int id;
    int year;
    int month;
    int day;
}List_t;

/*================== 链表头结构 定义 ===============*/
typedef struct
{
    struct list_t *list_prev, *list_next;
    int list_count;
}ListHead_t;


/*================== 链表头 定义 ===============*/
ListHead_t  listHead;

int main()
{
    /*================== 链表头 初始化===============*/
    DLL_INIT(list_, &listHead);
    
    List_t list1, list2, list3; //此处为了方便,没有使用malloc分配内存,如在工程中使用,请灵活使用

    list1.id = 1;
    list1.year = 2019;
    list1.month = 8;
    list1.day = 10;
    
    list2.id = 2;
    list2.year = 2019;
    list2.month = 8;
    list2.day = 11;

    list3.id = 3;
    list3.year = 2019;
    list3.month = 8;
    list3.day = 12;


    /*================== 添加元素到链表中 ===============*/
    DLL_PUSH_TAIL(list_, &listHead, &list1);
    DLL_PUSH_TAIL(list_, &listHead, &list2);
    DLL_PUSH_TAIL(list_, &listHead, &list3);

    List_t * temp;

    /*==================  顺序遍历链表  ===============*/
    printf ("\n正序:\n");
    DLL_FOREACH(list_, &listHead, temp)
    {
        printf ("\tid = %d, year = %d, month = %d, day = %d \n", temp->id, temp->year, temp->month, temp->day);
    }

    /*==================  倒序遍历链表  ===============*/
    printf ("\n倒序:\n");
    DLL_FOREACH_REVERSE(list_, &listHead, temp)
    {
        printf ("\tid = %d, year = %d, month = %d, day = %d \n", temp->id, temp->year, temp->month, temp->day);
    }

    /*==================  统计元素个数 ===============*/
    printf ("\n统计:\n");
    printf ("\tlist elements num: %d\n", DLL_COUNT(list_, &listHead));

    printf ("\n\t更多使用方法,请自行参考dll.h\n\n" );
    return 0;
}
