#pragma once

#include "Debug/DynamoException.h"

class Model {
public:
	Model(const std::string& path);
	~Model();

public:
	class ModelException : public DynamoException {
	public:
		ModelException(const char* file, unsigned int line, const char* msg);
		virtual const char* GetType() const override;
		virtual const char* what() const override;
	};
};

#define MODEL_EXCEP(msg) Model::ModelException(__FILE__, __LINE__, msg)