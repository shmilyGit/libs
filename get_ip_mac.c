#include <sys/socket.h>   
#include <sys/types.h>
#include <stdlib.h>   
#include <stdio.h>   
#include <string.h>   
#include <arpa/inet.h>   
#include <unistd.h>   
#include <sys/ioctl.h>   
#include <linux/if_arp.h>   

unsigned long get_local_ip(char *dev)   
{   
    int s;    
    struct ifreq req;    
    int err;    
    strcpy(req.ifr_name, dev);    
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)   
    {   
        perror("socket()");   
        return -1;   
    }   
    err = ioctl(s, SIOCGIFADDR, &req);    
    close(s);    
    if (err == -1)    
        return err;    
    return ((struct sockaddr_in*)&req.ifr_addr)->sin_addr.s_addr;   
}   

//目前还有点问题
int get_local_mac(char *dev, unsigned char *mac)   
{   
    int s = socket(AF_INET, SOCK_DGRAM, 0);    
    struct ifreq req;    
    int err;    
    strcpy(req.ifr_name, dev);    
    err = ioctl(s, SIOCGIFHWADDR, &req);    
    close(s);    
    if (err == -1)    
        return err;    
    memcpy(mac, req.ifr_hwaddr.sa_data,6);    
    return 0;    
}   

int main ()
{
	char ip[16];
	unsigned char mac[10];
	unsigned long ret = get_local_ip("eth0");

	printf ("%s\n", inet_ntop(AF_INET, (void *)&ret, ip, 16));
}
