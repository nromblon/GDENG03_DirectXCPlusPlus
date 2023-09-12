#include "Utilities.h"

#include <stdexcept>
#include <Windows.h>

std::string Utilities::GetErrorStr()
{
	const DWORD error_message_id_dword = ::GetLastError();

	const LPSTR message_buffer_lpstr = nullptr;

	//Ask Win32 to give us the string version of that message ID.
	//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
	const size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, error_message_id_dword, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&message_buffer_lpstr, 0, nullptr);

	//Copy the error message into a std::string.
	std::string message(message_buffer_lpstr, size);

	//Free the Win32's string's buffer.
	LocalFree(message_buffer_lpstr);

	return message;
}
