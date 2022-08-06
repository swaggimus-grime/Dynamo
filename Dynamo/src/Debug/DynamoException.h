#pragma once

#include <exception>
#include <string>

class DynamoException : public std::exception {
public:
	DynamoException(const char* file, UINT line);
	DynamoException(const char* file, UINT line, const std::string& msg);
	DynamoException(const char* file, UINT line, DWORD w);
	virtual const char* GetType() const;
	virtual const char* what() const override;

protected:
	mutable std::string m_What;

private:
	std::string m_File;
	UINT m_Line;
};

#define DYNAMO_EXCEP(msg) DynamoException::DynamoException(__FILE__, __LINE__, msg)
#define DYNAMO_PREV_EXCEP DynamoException::DynamoException(__FILE__, __LINE__, GetLastError())

#define DYNAMO_ASSERT(p, failMsg) { if(!(p)) throw DYNAMO_EXCEP(failMsg); }
#define DX_ASSERT(r) { if(FAILED(r)) throw DYNAMO_EXCEP(r); }