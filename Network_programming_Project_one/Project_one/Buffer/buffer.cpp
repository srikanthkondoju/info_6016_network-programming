#include "buffer.h"

uint32_t bufferClass::readUInt32LE() {

	uint32_t value = 10;
	return value;

}

uint32_t bufferClass::readUInt32LE(std::size_t index) {

	uint32_t value = 10;
	return value;

}

void bufferClass::writeInt32LE(int32_t value) {

}

void bufferClass::writeInt32LE(std::size_t index, int32_t value) {

}

uint16_t bufferClass::readUInt16LE() {


	uint16_t value = 10;
	return value;
}

uint16_t bufferClass::readUInt16LE(std::size_t index) {

	uint16_t value = 10;
	return value;
}

void bufferClass::writeInt16LE(int16_t value) {

}

void bufferClass::writeInt16LE(std::size_t index, int16_t value) {

}

bufferClass::bufferClass(size_t size) {
	this->size = size;
}

int32_t bufferClass::SerializeInt(int32_t value) {

	int swapped = 0;
	// we need to grab the first byte and move ot to the last
	// Bytes in order as A B C D
	char A = value >> 24;
	char B = value >> 16;
	char C = value >> 8;
	char D = value >> 0;

	printf("A is: %s\n", std::bitset<8>(A).to_string().c_str());
	printf("B is: %s\n", std::bitset<8>(B).to_string().c_str());
	printf("C is: %s\n", std::bitset<8>(C).to_string().c_str());
	printf("D is: %s\n", std::bitset<8>(D).to_string().c_str());

	swapped |= D << 24;
	swapped |= C << 16;
	swapped |= B << 8;
	swapped |= A << 0;

	return swapped;
}

int32_t bufferClass::deSerializeInt(int32_t value) {
	int swapped = 0;
	// we need to grab the first byte and move ot to the last
	// Bytes in order as A B C D
	//char D = value >> 24;
	//char C = value >> 16;
	//char B = value >> 8;
	//char A = value >> 0;
	////printf("%d",sizeof(A));

	//printf("A is: %s\n", std::bitset<8>(A).to_string().c_str());
	//printf("B is: %s\n", std::bitset<8>(B).to_string().c_str());
	//printf("C is: %s\n", std::bitset<8>(C).to_string().c_str());
	//printf("D is: %s\n", std::bitset<8>(D).to_string().c_str());

	//swapped  = (swapped | A) << 24;
	//printf("swapped with A is %s\n", std::bitset<32>(swapped).to_string().c_str());
	//swapped  = (swapped | B) << 16;
	//printf("swapped with B is %s\n", std::bitset<32>(swapped).to_string().c_str());
	//swapped  = (swapped | C) << 8;
	//printf("swapped with C is %s\n", std::bitset<32>(swapped).to_string().c_str());
	//swapped  = (swapped | D) << 0;
	//printf("swapped with D is %s\n", std::bitset<32>(swapped).to_string().c_str());
	char A = value >> 24;
	char B = value >> 16;
	char C = value >> 8;
	char D = value >> 0;

	printf("A is: %s\n", std::bitset<8>(A).to_string().c_str());
	printf("B is: %s\n", std::bitset<8>(B).to_string().c_str());
	printf("C is: %s\n", std::bitset<8>(C).to_string().c_str());
	printf("D is: %s\n", std::bitset<8>(D).to_string().c_str());

	swapped |= D << 24;
	swapped |= C << 16;
	swapped |= B << 8;
	swapped |= A << 0;

	return swapped;
}

int16_t bufferClass::SerializeShort(int16_t value) {

	short swapped = 0;
	char A = value >> 8;
	char B = value >> 0;
	printf("A is: %s\n", std::bitset<8>(A).to_string().c_str());
	printf("B is: %s\n", std::bitset<8>(B).to_string().c_str());

	swapped |= B << 8;
	swapped |= A << 0;

	return swapped;

}

int16_t bufferClass::deSerializeShort(int16_t value) {
	short swapped = 0;
	char A = value >> 8;
	char B = value >> 0;
	printf("A is: %s\n", std::bitset<8>(A).to_string().c_str());
	printf("B is: %s\n", std::bitset<8>(B).to_string().c_str());

	swapped |= B << 8;
	swapped |= A << 0;

	return swapped;
}

int8_t bufferClass::SerializeChar(int8_t value) {
	return value;
}

int8_t bufferClass::deSerializeChar(int8_t value) {
	return value;
}

std::string bufferClass::Serializestring(std::string mybuffer)
{
	std::string serialised="";
	char *ptr1, *ptr2;
	ptr1 = new char[mybuffer.length()];
	ptr2 = new char[mybuffer.length()];
	for (int i = 0; i != mybuffer.length(); i++)
	{
		ptr1[i] = mybuffer.at(i);
	}

	try {
		std::memcpy(ptr2, ptr1, mybuffer.length());
	}
	catch (_exception e)
	{

	}
	//std::memcpy(ptr2, ptr1, sizeof(mybuffer));
	for (int i = 0; i < mybuffer.length(); i++)
	{
		serialised = serialised + (ptr2[i]);
	}
	return serialised;
}

std::string bufferClass::deserealizestring(std::string myserilisedbuffer)
{
	std::string deserialised = "";
	char* ptr1, * ptr2;
	ptr1 = new char[myserilisedbuffer.length()];
	ptr2 = new char[myserilisedbuffer.length()];
	for (int i = 0; i != myserilisedbuffer.length(); i++)
	{
		ptr1[i] = myserilisedbuffer.at(i);
	}
	try {
		std::memcpy(ptr2, ptr1, myserilisedbuffer.length());
	}
	catch (_exception e)
	{

	}
	for (int i = 0; i < myserilisedbuffer.length(); i++)
	{
		deserialised = deserialised + (ptr2[i]);
	}
	return deserialised;
}