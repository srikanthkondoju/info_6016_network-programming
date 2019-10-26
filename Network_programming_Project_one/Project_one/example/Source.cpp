#include<iostream>
#include<string>
#include<buffer.h>
#include<vector>
int main()
{
	bufferClass* mybuff = new bufferClass(20);

	std::string mystring= "//////////////lkugfcvbnm,.000./////";

	mystring = mybuff->Serializestring(mystring);

	std::cout << mystring << std::endl;

	mystring = mybuff->deserealizestring(mystring);

	std::cout << mystring << std::endl;
	system("pause");
	//std::vector<int> myvec;

	//myvec.push_back(20);

	//myvec.push_back(45);

	//myvec.push_back(47);

	//myvec.push_back(5);

	//myvec.push_back(22);

	//myvec.push_back(36);

	//myvec.push_back(100);

	//myvec.push_back(-2);

	//for (int i = 0; i < myvec.size(); i++)
	//{
	//	std::cout<<myvec[i]<<std::endl;
	//}

	//
	//std::vector<int>::iterator it = std::find(myvec.begin(), myvec.end(), 45);
	//if (it != myvec.end())
	//{
	//	int index = std::distance(myvec.begin(), it);

	//	std::cout << "found at " << index << std::endl;

	//	myvec.erase(myvec.begin() + index);
	//	//std::string mapIndex = rooms[index];
	//	//std::vector<SOCKET> temp = roomsandclients[mapIndex];
	//	////printf(" size before erasing %d", temp.size());
	//	//temp.erase(temp.begin() + index);
	//	//roomsandclients[mapIndex] = temp;
	//	////printf(" size after erasing %d", temp.size());
	//	//broadcast(msgroom, msgstring);
	//}

	//for (int i = 0; i < myvec.size(); i++)
	//{
	//	std::cout << myvec[i] << std::endl;
	//}


}