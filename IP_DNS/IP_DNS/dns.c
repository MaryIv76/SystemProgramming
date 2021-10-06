
/*************************************************************************
   LAB 1

	Edit this file ONLY!

*************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#define HASH_TABLE_SIZE 13003

#include "dns.h"
#include "stdlib.h"
#include <stdio.h>

typedef struct _DNS_INFO
{
	char* domainName;
	IPADDRESS ip;
	struct _DNS_INFO* nextDNSInfo;

} DNS_INFO, * PDNS_INFO;

DNSHandle InitDNS()
{
	DNSHandle hDNS = (unsigned int)(PDNS_INFO)calloc(HASH_TABLE_SIZE, sizeof(DNS_INFO));

	if ((PDNS_INFO*)hDNS != NULL)
	{
		for (int i = 0; i < HASH_TABLE_SIZE; i++)
		{
			((PDNS_INFO)hDNS)[i].domainName = NULL;
			((PDNS_INFO)hDNS)[i].nextDNSInfo = NULL;
			((PDNS_INFO)hDNS)[i].ip = 0;
		}

		return hDNS;
	}

	return (DNSHandle)INVALID_DNS_HANDLE;
}

unsigned int HashLy(const char* str)
{
	unsigned int hash = 0;
	const int const1 = 1664525;
	const int const2 = 1013904223;

	for (; *str; str++)
		hash = (hash * const1) + (unsigned char)(*str) + const2;

	return hash % HASH_TABLE_SIZE;
}

void CreateCurrentDNSInfo(PDNS_INFO currentDNSInfo, IPADDRESS ip, char* domain)
{
	currentDNSInfo->domainName = (char*)malloc(strlen(domain) + 1);
	strcpy(currentDNSInfo->domainName, domain);
	currentDNSInfo->ip = ip;
	currentDNSInfo->nextDNSInfo = NULL;
}

void AddToHashTable(DNSHandle hDNS, IPADDRESS ip, char* domain)
{
	unsigned int index = HashLy(domain);

	PDNS_INFO currentDNSInfo = &((PDNS_INFO)hDNS)[index];

	if (currentDNSInfo->domainName == NULL)
	{
		CreateCurrentDNSInfo(currentDNSInfo, ip, domain);
	}
	else
	{
		while (currentDNSInfo->nextDNSInfo != NULL)
		{
			currentDNSInfo = currentDNSInfo->nextDNSInfo;
		}

		currentDNSInfo->nextDNSInfo = (PDNS_INFO)malloc(sizeof(DNS_INFO));
		CreateCurrentDNSInfo(currentDNSInfo->nextDNSInfo, ip, domain);
	}
}

void LoadHostsFile(DNSHandle hDNS, const char* hostsFilePath)
{
	FILE* fInput = NULL;
	fInput = fopen(hostsFilePath, "r");

	if (NULL == fInput)
		return;

	unsigned int ipPart1 = 0, ipPart2 = 0, ipPart3 = 0, ipPart4 = 0;
	const int domainLength = 300;
	char* domain = (char*)malloc(domainLength + 1);

	while (fscanf_s(fInput, "%d.%d.%d.%d %s", &ipPart1, &ipPart2, &ipPart3, &ipPart4, domain, domainLength) != EOF)
	{
		// Fowler "Refactoring" Extract Method?
		IPADDRESS ip = (ipPart1 & 0xFF) << 24 |
			(ipPart2 & 0xFF) << 16 |
			(ipPart3 & 0xFF) << 8 |
			(ipPart4 & 0xFF);

		AddToHashTable(hDNS, ip, domain);
	}

	free(domain);
	fclose(fInput);
}

IPADDRESS DnsLookUp(DNSHandle hDNS, const char* hostName)
{
	unsigned int index = HashLy(hostName);
	PDNS_INFO currentDNSInfo = &((PDNS_INFO)hDNS)[index];

	while (currentDNSInfo != NULL && strcmp(currentDNSInfo->domainName, hostName) != 0)
	{
		currentDNSInfo = currentDNSInfo->nextDNSInfo;
	}

	if (currentDNSInfo)
	{
		return currentDNSInfo->ip;
	}

	return INVALID_IP_ADDRESS;
}

void ShutdownDNS(DNSHandle hDNS)
{
	PDNS_INFO currentDNSInfo;
	PDNS_INFO nextDNSInfo;

	for (int i = 0; i < HASH_TABLE_SIZE; i++)
	{
		int indexInList = 0;
		nextDNSInfo = &((PDNS_INFO)hDNS)[i];
		while (nextDNSInfo != NULL)
		{
			currentDNSInfo = nextDNSInfo;
			nextDNSInfo = currentDNSInfo->nextDNSInfo;

			if (currentDNSInfo->domainName)
			{
				free(currentDNSInfo->domainName);
			}

			indexInList++;
			if (indexInList == 1)
			{
				continue;
			}

			free(currentDNSInfo);
		}
	}

	if ((PDNS_INFO)hDNS != NULL)
	{
		free((PDNS_INFO)hDNS);
	}
}
