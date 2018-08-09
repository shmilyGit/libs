make all  编译程序
make test  测试程序
make clean 清除程序

1、增加模式
   ACSM_PATTERN  结构体中可以增加自己需要增加的变量成员
2、编译模式
   
3、搜索模式
   acsmSearch 函数 597行表示不区分大小写匹配成功
                   602行表示区分大小写匹配成功
                   Match函数是用户自定义函数,可以自定义  返回值为0,代表匹配多个
                                                         返回1     代表匹配成功一次即退出匹配
														 
														 
额外说明：
		一个key 有可能在字符串中被匹配两次
        例如：key为ni  字符串为woshinideni
        那么匹配结果时候Match函数被调用两次：index分别为5,9
        
		在特殊应用中,例如：
		统计 woshinideni中出现[ni]&&[wo]同时出现时候则为匹配成功,即为命中两次(去重ni和wo)
		则需要在ACSM_PATTERN 结构体中增加一个成员rematch=0
		         第一次命中时候置位1,第二命中时候检查rematch标记为,如果为1,则不调用Match函数