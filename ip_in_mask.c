#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// gcc test.c -lm

typedef struct _ip {
    int family;
    int bits;

    /* see sfip_size(): these address bytes
     * must be the last field in this struct */
    union
    {
        u_int8_t  u6_addr8[16];
        u_int16_t u6_addr16[8];
        u_int32_t u6_addr32[4];
//        UINT64    u6_addr64[2];
    } ip;
    #define ip8  ip.u6_addr8
    #define ip16 ip.u6_addr16
    #define ip32 ip.u6_addr32
//    #define ip64 ip.u6_addr64
} sfip_t;


typedef enum _return_values {
    SFIP_SUCCESS=0,
    SFIP_FAILURE,
    SFIP_LESSER,
    SFIP_GREATER,
    SFIP_EQUAL,
    SFIP_ARG_ERR,
    SFIP_CIDR_ERR,
    SFIP_INET_PARSE_ERR,
    SFIP_INVALID_MASK,
    SFIP_ALLOC_ERR,
    SFIP_CONTAINS,
    SFIP_NOT_CONTAINS,
    SFIP_DUPLICATE,         /* Tried to add a duplicate variable name to table */
    SFIP_LOOKUP_FAILURE,    /* Failed to lookup a variable from the table */
    SFIP_UNMATCHED_BRACKET, /* IP lists that are missing a closing bracket */
    SFIP_NOT_ANY,           /* For !any */
    SFIP_CONFLICT           /* For IP conflicts in IP lists */
} SFIP_RET;

#define ARG_CHECK1(a, z) 
#define ARG_CHECK2(a, b, z) 
#define ARG_CHECK3(a, b, c, z)

#define sfip_family(ip) ip->family

static inline int sfip_str_to_fam(const char *str) {
    const char* s;
    ARG_CHECK1(str, 0);
    s = strchr(str, (int)':');
    if ( s && strchr(s+1, (int)':') ) return AF_INET6;
    if ( strchr(str, (int)'.') ) return AF_INET;
    return AF_UNSPEC;
}

static inline int _count_bits(unsigned int val) {
    unsigned int count; 

    for (count = 0; val; count++) {
        val &= val - 1;
    }

    return count;
}

int sfip_ismapped(sfip_t *ip) {
    unsigned int *p;

    ARG_CHECK1(ip, 0);

    if(sfip_family(ip) == AF_INET) 
        return 0;
       
    p = ip->ip32;

    if(p[0] || p[1] || (ntohl(p[2]) != 0xffff && p[2] != 0)) return 0;

    return 1;
}

static inline unsigned char sfip_bits(sfip_t *ip) {
    ARG_CHECK1(ip, 0);
    return (unsigned char)ip->bits;
}

static inline int sfip_cidr_mask(sfip_t *ip, int val) {
    int i;
    unsigned int mask = 0; 
    unsigned int *p;
    int index = (int)ceil(val / 32.0) - 1;
   
    ARG_CHECK1(ip, SFIP_ARG_ERR);

    p = ip->ip32;

    if( val < 0 ||
        ((sfip_family(ip) == AF_INET6) && val > 128) ||
        ((sfip_family(ip) == AF_INET) && val > 32) ) {
        return SFIP_ARG_ERR;
    }
    
    /* Build the netmask by converting "val" into 
     * the corresponding number of bits that are set */
    for(i = 0; i < 32- (val - (index * 32)); i++)
        mask = (mask<<1) + 1;

    p[index] = htonl((ntohl(p[index]) & ~mask));

    index++;

    /* 0 off the rest of the IP */
    for( ; index<4; index++) p[index] = 0;

    return SFIP_SUCCESS;
}

static inline int _netmask_str_to_bit_count(char *mask, int family) {
    u_int32_t buf[4];
    int bits, i, nBits, nBytes;
    uint8_t* bytes = (uint8_t*)buf;

    /* XXX 
     * Mask not validated.  
     * Only sfip_pton should be using this function, and using it safely. 
     * XXX */

    if(inet_pton(family, mask, buf) < 1)
        return -1;

    bits =  _count_bits(buf[0]);

    if(family == AF_INET6) {
        bits += _count_bits(buf[1]);
        bits += _count_bits(buf[2]);
        bits += _count_bits(buf[3]);
        nBytes = 16;
    } else {
        nBytes = 4;
    }

    // now make sure that only the most significant bits are set
    nBits = bits;
    for ( i = 0; i < nBytes; i++ ) {
        if ( nBits >= 8 ) {
            if ( bytes[i] != 0xff ) return -1;
            nBits -= 8;

        } else if ( nBits == 0 ) {
            if ( bytes[i] != 0x00 ) return -1;

        } else {
            if ( bytes[i] != ((0xff00 >> nBits) & 0xff) ) return -1;
            nBits = 0;
        }
    }
    return bits;
}

SFIP_RET sfip_pton(const char *src, sfip_t *dst) {
    char *mask;
    char *sfip_buf;
    char *ip;
    int bits;

    if(!dst || !src) 
        return SFIP_ARG_ERR;
            
    if((sfip_buf = strdup(src)) == NULL) 
        return SFIP_ALLOC_ERR;

    ip = sfip_buf;
    dst->family = sfip_str_to_fam(src);

    /* skip whitespace or opening bracket */
    while(isspace((int)*ip) || (*ip == '[')) ip++;

    /* check for and extract a mask in CIDR form */
    if( (mask = strchr(ip, (int)'/')) != NULL ) {
        /* NULL out this character so inet_pton will see the 
         * correct ending to the IP string */
        char* end = mask++;
        while ( (end > ip) && isspace((int)end[-1]) ) end--;
        *end = 0;

        while(isspace((int)*mask)) mask++;

        /* verify a leading digit */
        if(((dst->family == AF_INET6) && !isxdigit((int)*mask)) ||
           ((dst->family == AF_INET) && !isdigit((int)*mask))) {
            free(sfip_buf);                          
            return SFIP_CIDR_ERR;
        }

        /* Check if there's a netmask here instead of the number of bits */
        if(strchr(mask, (int)'.') || strchr(mask, (int)':')) 
            bits = _netmask_str_to_bit_count(mask, sfip_str_to_fam(mask));
        else
            bits = atoi(mask);
    }
    else if(
            /* If this is IPv4, ia ':' may used specified to indicate a netmask */
            ((dst->family == AF_INET) && (mask = strchr(ip, (int)':')) != NULL) ||

            /* We've already skipped the leading whitespace, if there is more 
             * whitespace, then there's probably a netmask specified after it. */
             (mask = strchr(ip, (int)' ')) != NULL
    ) {
        char* end = mask++;
        while ( (end > ip) && isspace((int)end[-1]) ) end--;
        *end = 0;  /* Now the IP will end at this point */

        /* skip whitespace */
        while(isspace((int)*mask)) mask++;

        /* Make sure we're either looking at a valid digit, or a leading
         * colon, such as can be the case with IPv6 */
        if(((dst->family == AF_INET) && isdigit((int)*mask)) ||
           ((dst->family == AF_INET6) && (isxdigit((int)*mask) || *mask == ':'))) { 
            bits = _netmask_str_to_bit_count(mask, sfip_str_to_fam(mask));
        } 
        /* No netmask */
        else { 
            if(dst->family == AF_INET) bits = 32;
            else bits = 128;        
        }
    }
    /* No netmask */
    else {
        if(dst->family == AF_INET) bits = 32;
        else bits = 128;        
    }

    if(inet_pton(dst->family, ip, dst->ip8) < 1) {
        free(sfip_buf);                          
        return SFIP_INET_PARSE_ERR;
    }

    /* Store mask */
    dst->bits = bits;

    /* Apply mask */
    if(sfip_cidr_mask(dst, bits) != SFIP_SUCCESS) {
        free(sfip_buf);
        return SFIP_INVALID_MASK;
    }
    
    free(sfip_buf);
    return SFIP_SUCCESS;
}

SFIP_RET sfip_contains(sfip_t *net, sfip_t *ip) {
    unsigned int bits, mask, temp, i;
    int net_fam, ip_fam;
    unsigned int *p1, *p2;

    /* SFIP_CONTAINS is returned here due to how IpAddrSetContains 
     * handles zero'ed IPs" */
    ARG_CHECK2(net, ip, SFIP_CONTAINS);

    bits = sfip_bits(net);
    net_fam = sfip_family(net);
    ip_fam = sfip_family(ip);

    /* If the families are mismatched, check if we're really comparing
     * an IPv4 with a mapped IPv4 (in IPv6) address. */
    if(net_fam != ip_fam) {
        if((net_fam != AF_INET) || !sfip_ismapped(ip))
            return SFIP_ARG_ERR;

        /* Both are really IPv4.  Only compare last 4 bytes of 'ip'*/
        p1 = net->ip32;
        p2 = &ip->ip32[3];
        
        /* Mask off bits */
        bits = 32 - bits;
        temp = (ntohl(*p2) >> bits) << bits;

        if(ntohl(*p1) == temp) return SFIP_CONTAINS;

        return SFIP_NOT_CONTAINS;
    }

    p1 = net->ip32;
    p2 = ip->ip32;

    /* Iterate over each 32 bit segment */
    for(i=0; i < bits/32 && i < 3; i++, p1++, p2++) {
        if(*p1 != *p2) 
            return SFIP_NOT_CONTAINS;
    }

    mask = 32 - (bits - 32*i);
    if ( mask == 32 ) return SFIP_CONTAINS;

    /* At this point, there are some number of remaining bits to check.
     * Mask the bits we don't care about off of "ip" so we can compare
     * the ints directly */
    temp = ntohl(*p2);
    temp = (temp >> mask) << mask;

    /* If p1 was setup correctly through this library, there is no need to 
     * mask off any bits of its own. */
    if(ntohl(*p1) == temp) 
        return SFIP_CONTAINS;

    return SFIP_NOT_CONTAINS;

}


int main()
{
	int result = 0;
	sfip_t ip1, ip2;

	sfip_pton("1001:db8:85a3::/28", &ip1);
	sfip_pton("1001:db0::", &ip2);

	result = sfip_contains(&ip1, &ip2);

	printf("result: %d\n", result);
	
	return 0;
}
