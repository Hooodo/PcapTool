#include "stdafx.h"
#include "PcapCore.h"

FILE *fp_2 = NULL;
char *buf = NULL;
pcap_t *p = NULL;	
pcap_dumper_t *fp = NULL;
struct pcap_pkthdr h;
int packet_count = 0;

void pcap_write_init()
{
	p = pcap_open_dead(LINKTYPE_ETHERNET, 0x0000ffff);
	if (NULL == p){
		fprintf(stderr, "pcap_open_dead failed.\n");
		return;
	}
	fp = pcap_dump_open(p, FILE_SAVE);
	if (NULL == fp){
		fprintf(stderr, "pcap_dump_open failed.\n");
		return;
	}
}

void pcap_write_body(uint8_t *data, int datalen)
{
	h.ts.tv_sec = GetTickCount()/1000;
	h.ts.tv_usec = 0;
	h.caplen = datalen;
	h.len    = datalen;

	pcap_dump((uint8_t *)fp, &h, data);	
}

void pcap_write_end()
{
	pcap_dump_close(fp);
}

void find_ip_head(FILE *fp3, int offset)
{
	BYTE br;
	BYTE tr;
	int readcount;

	//printf("seeking %d\n", offset);
	//fseek(fp3, offset, SEEK_SET);
	readcount = fread(&br, 1, 1, fp3);
	while (readcount)
	{
		if (br == 0x45)
		{
			readcount = fread(&tr, 1, 1, fp3);
			if (tr == 0x00)
			{
				break;
			}
			else
			{
				br = tr;
			}
		}
		readcount = fread(&br, 1, 1, fp3);
	}
	fseek(fp3, -2, SEEK_CUR);
}

void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	buf = new char[header->caplen-14];
	memcpy(buf, pkt_data+14, header->caplen-14);
	fwrite(buf, header->caplen-14, 1, fp_2);
	packet_count++;
	delete buf;
}