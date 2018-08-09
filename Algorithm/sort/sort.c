#include <stdio.h>

//冒泡排序
/*
冒泡排序就是把小的元素往前调或者把大的元素往后调。
比较是相邻的两个元素比较，
交换也发生在这两个元素之间。
所以，如果两个元素相等，是不会再交换的；
如果两个相等的元素没有相邻，
那么即使通过前面的两两交换把两个相邻起来
，这时候也不会交换，所以相同元素的前后顺序并没有改变
所以冒泡排序是一种稳定排序算法
 * */
void 
BubbleSort(unsigned int a[], unsigned int n)
{
    unsigned int  i, j, tmp;

    if (n > 1) {
        for (i = 0; i < n-1; ++ i) {
            for (j = 0; j < n-i-1; ++ j) {
                if (a[j] > a[j+1]) {
                tmp = a[j]; a[j] = a[j+1]; a[j+1] = tmp;
                }
            }
        }
    }
}
int main()
{
    unsigned int rule_id[99]= {1,99,87,66,12,34,22};
    unsigned int rule_cnt = 7;
    BubbleSort(rule_id,rule_cnt);
    int i = 0;
    for(i = 0;i<rule_cnt;i++){
        printf("%d ",rule_id[i]);
    }
    printf("\n");
    return 0;
}
