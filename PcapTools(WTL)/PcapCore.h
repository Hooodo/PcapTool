#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <pcap.h>

#pragma comment(lib, "wpcap.lib")
#define TCPDUMP_MAGIC       0xa1b2c3d4
#ifndef PCAP_VERSION_MAJOR
#define PCAP_VERSION_MAJOR 2
#define 
#define PCAP_VERSION_MINOR 
#define PCAP_VERSION_MINOR 4
#endif


#define LINKTYPE_NULL       DLT_NULL
#define LINKTYPE_ETHERNET   DLT_EN10MB  /* also for 100Mb and up */
#define LINKTYPE_EXP_ETHERNET   DLT_EN3MB   /* 3Mb experimental Ethernet */
#define LINKTYPE_AX25       DLT_AX25
#define LINKTYPE_PRONET     DLT_PRONET
#define LINKTYPE_CHAOS      DLT_CHAOS
#define LINKTYPE_TOKEN_RING DLT_IEEE802 /* DLT_IEEE802 is used for Token Ring */
#define LINKTYPE_ARCNET     DLT_ARCNET  /* BSD-style headers */
#define LINKTYPE_SLIP       DLT_SLIP
#define LINKTYPE_PPP        DLT_PPP
#define LINKTYPE_FDDI       DLT_FDDI

#define MAX_LEN 1600
#define ETHERNET_LEN 14
#define FILE_SAVE "pcap_write.pcap"

typedef struct _ip_hdr   
{   
#if LITTLE_ENDIAN   
	unsigned char ihl:4;     //首部长度   
	unsigned char version:4, //版本    
#else   
	unsigned char version:4; //版本   
	unsigned char ihl:4;     //首部长度   
#endif   
	unsigned char tos;       //服务类型   
	unsigned short tot_len;  //总长度   
	unsigned short id;       //标志   
	unsigned short frag_off; //分片偏移   
	unsigned char ttl;       //生存时间   
	unsigned char protocol;  //协议   
	unsigned short chk_sum;  //检验和   
	struct in_addr srcaddr;  //源IP地址   
	struct in_addr dstaddr;  //目的IP地址   
}ip_hdr;

extern FILE *fp_2;
extern char *buf;
extern pcap_t *p;	
extern pcap_dumper_t *fp;
extern struct pcap_pkthdr h;
extern int packet_count;

void pcap_write_init();

void pcap_write_body(uint8_t *data, int datalen);

void pcap_write_end();

void find_ip_head(FILE *fp3, int offset);

void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data);