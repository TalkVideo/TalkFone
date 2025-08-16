#include "Serializable.h"

Serializable::Serializable()
{
	objectId = 0;
	objectType = "abstract Serializable";
	columnsNames = 0;
	columnsNumber = 0;
}

Serializable::~Serializable()
{
	objectId = 0;
	delete columnsNames;
}

std::string Serializable::getObjectType() 
{
	return objectType;
}

string* Serializable::getColumnsNames()
{
	return columnsNames;
}

int Serializable::getColumnsNumber()
{
	return columnsNumber;
}

void Serializable::setObjectId(long int id)
{
	objectId = id;
}

long int Serializable::getObjectId()
{
	return objectId;
}
