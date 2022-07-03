#include <unistd.h>
#include "utils_io.h"

int recvn(int fd, void* ptrBuffer, size_t n)
{
	size_t bytesToRead = n;
	size_t bytesRead = 0;

	while (bytesToRead > bytesRead)
	{
		ssize_t result = read(fd, ptrBuffer + bytesRead, bytesToRead);

		if (result == -1)
		{
			break;
		}

		bytesToRead -= result;
		bytesRead += result;
	}

	return (bytesRead < bytesToRead) ? -1 : 0;
}

int sendn(int fd, const void* ptrBuffer, size_t n)
{
	size_t bytesToWrite = n;

	while (bytesToWrite > 0)
	{
		ssize_t result = write(fd, ptrBuffer, bytesToWrite);

		if (result == -1)
		{
			break;
		}

		bytesToWrite -= result;
		ptrBuffer += result;
	}

	return (bytesToWrite > 0) ? -1 : 0;
}