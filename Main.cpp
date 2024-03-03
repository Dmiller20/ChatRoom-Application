#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

int main()
{
	WSADATA dat;
	DWORD asize = 20000;
	PIP_ADAPTER_ADDRESSES adapters;
	PIP_ADAPTER_ADDRESSES adapter = adapters;
	PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;

	//winsock initialization
	if(WSAStartup(MAKEWORD(2, 2), &dat))
	{
		std::cout << "Failed to initialize. \n";
		return -1;
	}

	do
	{
		adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);
	
		if(!adapters)
		{
			std::cout << "Couldnt allocate %ld bytes for adapters \n" << asize;
			WSACleanup();
			return -1;
		}

		int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &asize);

		if(r == ERROR_BUFFER_OVERFLOW)
		{
			std::cout << "GetAdaptersAddresses wants %ld bytes. \n" << asize;
			free(adapters);
		}else if(r == ERROR_SUCCESS)
		{
			break;
		}else
		{
			std::cout << "Error from GetAdaptersAddresses: %d\n" << r;
			free(adapters);
			WSACleanup();
			return -1;
		}
	} while (!adapters);

	while (adapter)
	{
		std::cout << "\n Adapter name: %s\n" << adapter->FriendlyName;

		while (address)
		{
			printf("\t%s", address->Address.lpSockaddr->sa_family == AF_INET ? "IPv4" : "IPv6");


			char ap[100];

			getnameinfo(address->Address.lpSockaddr, address->Address.iSockaddrLength, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);

			std::cout << "\t%s\n" << ap;

			address = address->Next;
		}
		address = address->Next;
	}
	free(adapters);
	WSACleanup();

	std::cout << "Hello world" << std::endl;

	return 0;
}