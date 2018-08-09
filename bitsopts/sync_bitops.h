#ifndef _ASM_X86_SYNC_BITOPS_H
#define _ASM_X86_SYNC_BITOPS_H

/*
 * Copyright 2018.
 */


#define ADDR (*(volatile long *)addr)
//依据操作系统的不同而选择不同值
#define BITS_PER_LONG 64
/**
  线程安全的设置addr的第nr位置为1
 */
static inline void sync_set_bit(int nr, volatile unsigned long *addr)
{
	asm volatile("lock; btsl %1,%0"
		     : "+m" (ADDR)
		     : "Ir" (nr)
		     : "memory");
}

/**
 *  线程安全的清除addr的第nr位置为0
 */
static inline void sync_clear_bit(int nr, volatile unsigned long *addr)
{
	asm volatile("lock; btrl %1,%0"
		     : "+m" (ADDR)
		     : "Ir" (nr)
		     : "memory");
}

/**
 *  线程安全的改变addr的第nr位置取反操作
 */
static inline void sync_change_bit(int nr, volatile unsigned long *addr)
{
	asm volatile("lock; btcl %1,%0"
		     : "+m" (ADDR)
		     : "Ir" (nr)
		     : "memory");
}

/**
 * 测试第nr位的值:
    当该位为0时，返回0
	当指定的该位为1时,返回-1,并且把第nr位置置为1
 */
static inline int sync_test_and_set_bit(int nr, volatile unsigned long *addr)
{
	int oldbit;

	asm volatile("lock; btsl %2,%1\n\tsbbl %0,%0"
		     : "=r" (oldbit), "+m" (ADDR)
		     : "Ir" (nr) : "memory");
	return oldbit;
}

/**
 * 测试第nr位的值:
    当该位为0时，返回0
	当指定的该位为1时,返回-1,并且把第nr位置置为0
 */
static inline int sync_test_and_clear_bit(int nr, volatile unsigned long *addr)
{
	int oldbit;

	asm volatile("lock; btrl %2,%1\n\tsbbl %0,%0"
		     : "=r" (oldbit), "+m" (ADDR)
		     : "Ir" (nr) : "memory");
	return oldbit;
}

/**
 * 测试第nr位的值:
    当该位为0时，返回0,
	当指定的该位为1时,返回-1
	并且0->1   1->0 改变该位置的值
 */
static inline int sync_test_and_change_bit(int nr, volatile unsigned long *addr)
{
	int oldbit;

	asm volatile("lock; btcl %2,%1\n\tsbbl %0,%0"
		     : "=r" (oldbit), "+m" (ADDR)
		     : "Ir" (nr) : "memory");
	return oldbit;
}

/*test_bit宏定义的展开*/
/*nr 为常量时宏展开之后调用该函数*/
static inline int constant_test_bit(unsigned int nr, const volatile unsigned long *addr)
{
        return ((1UL << (nr % BITS_PER_LONG)) &
                (addr[nr / BITS_PER_LONG])) != 0;
}
/*nr 为变量时宏展开之后调用该函数*/
static inline int variable_test_bit(int nr, volatile const unsigned long *addr)
{
        int oldbit;

        asm volatile("bt %2,%1\n\t"
                     "sbb %0,%0"
                     : "=r" (oldbit)
                     : "m" (*(unsigned long *)addr), "Ir" (nr));

        return oldbit;
}
/*gcc编译器依据__builtin_constant_p(nr)函数判断nr为常量or变量*/
#define test_bit(nr, addr)                      \
        (__builtin_constant_p((nr))             \
         ? constant_test_bit((nr), (addr))      \
         : variable_test_bit((nr), (addr)))

		 
/**
 *  函数名:sync_test_bit
    测试第nr位的值:
    当该位为0时，返回0,
	当指定的该位为1时,返回1
 */
#define sync_test_bit(nr, addr) test_bit(nr, addr)

#undef ADDR

#endif /* _ASM_X86_SYNC_BITOPS_H */
