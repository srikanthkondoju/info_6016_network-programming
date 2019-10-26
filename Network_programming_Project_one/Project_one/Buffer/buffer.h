#pragma once
#ifndef Buffer_H
#define Buffer_H

#include <vector>
#include<bitset>

class bufferClass {

public:

	bufferClass(size_t size);

	size_t size;

	void writeInt32LE(std::size_t index, int32_t value);

	void writeInt32LE(int32_t value);

	uint32_t readUInt32LE(std::size_t index);

	uint32_t readUInt32LE();

	void writeInt16LE(std::size_t index, int16_t value);

	void writeInt16LE(int16_t value);

	uint16_t readUInt16LE(std::size_t index);

	uint16_t readUInt16LE();

	int32_t SerializeInt(int32_t value);

	int32_t deSerializeInt(int32_t value);

	int16_t SerializeShort(int16_t value);

	int16_t deSerializeShort(int16_t value);

	int8_t SerializeChar(int8_t value);

	int8_t deSerializeChar(int8_t value);

	std::string Serializestring(std::string mystring);

	std::string deserealizestring(std::string mystring);
};

#endif