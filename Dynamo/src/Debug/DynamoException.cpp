#include "dynamopch.h"
#include "DynamoException.h"

DynamoException::DynamoException(const char* file, UINT line)
	:m_File(file), m_Line(line)
{
}

DynamoException::DynamoException(const char* file, UINT line, const std::string& msg)
	:DynamoException(file, line)
{
	std::stringstream oss;
	oss << m_File << "(" << m_Line << ")\n"
		<< msg << std::endl;
	m_What = oss.str();
}

DynamoException::DynamoException(const char* file, UINT line, DWORD w)
	:DynamoException(file, line)
{
	_com_error err(w);
	std::stringstream oss;
	oss << m_File << "(" << m_Line << ")\n"
		<< err.ErrorMessage() << std::endl;
	m_What = oss.str();
}

const char* DynamoException::GetType() const
{
	return "Dynamo Exception";
}

const char* DynamoException::what() const
{
	return m_What.c_str();
}
