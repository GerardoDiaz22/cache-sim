#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cmath>

const int MEMORY_SIZE = std::pow(2,16);

class MainMemory {
    private:
        std::map<int,std::string> memory;
    public:
        MainMemory(){
            for (int i = 0; i < MEMORY_SIZE; i++) {
                std::string data = "data" + std::to_string(i);
                memory.insert(std::make_pair(i, data));
            }
        }
        std::string getData(int address){
            return memory[address];
        }
};

class CacheBlock {
    private:
        int isValid;
        int tag;
        std::string data;
    public:
        CacheBlock(){
            this->isValid = 0;
            this->tag = -1;
            this->data = "NaN";
        }
        CacheBlock(int isValid, int tag, std::string data){
            this->isValid = isValid;
            this->tag = tag;
            this->data = data;
        }
        int getIsValid(){
            return isValid;
        }
        int getTag(){
            return tag;
        }
        std::string getData(){
            return data;
        }
        void setIsValid(int isValid){
            this->isValid = isValid;
        }
        void setTag(int tag){
            this->tag = tag;
        }
        void setData(std::string data){
            this->data = data;
        }
};

class Memory {
    private:
        MainMemory mainMemory;
        std::map<int,std::vector<CacheBlock>> cacheMemory;
        int nCacheBlocks;
        int nCacheWords;
        int nWays;
    public:
        Memory(int n, int m, int w){
            // Initialize main memory
            mainMemory = MainMemory();

            // Check for valid input
            if (n % 2 != 0 || w % 2 != 0){
                std::cout << "Failure: number of cache blocks and number of ways of associativity must be powers of 2" << std::endl;
                return;
            }

            // Initialize cache memory
            nCacheBlocks = n / w;
            nCacheWords = m;
            nWays = w;
            std::vector<CacheBlock> cacheBlocks;

            for (int i = 0; i < nCacheBlocks; i++){
                for (int j = 0; j < w; j++){
                    cacheBlocks.emplace_back();
                }
                cacheMemory.insert(std::make_pair(i,cacheBlocks));
            }
        }
        std::string getData(int address){
            if (address < 0 || address >= MEMORY_SIZE){
                return "Failure: Address out of bounds";
            }
            int index = address % nCacheBlocks;
            int tag = address / nCacheBlocks;
            for (int i = 0; i < nWays; i++){
                if (cacheMemory.at(index).at(i).getIsValid() == 1 && cacheMemory.at(index).at(i).getTag() == tag){
                    cacheMemory.at(index).push_back(cacheMemory.at(index).at(i));
                    cacheMemory.at(index).erase(cacheMemory.at(index).begin() + i);
                    return cacheMemory.at(index).back().getData();
                }
            }
            std::string data = mainMemory.getData(address);
            cacheMemory.at(index).erase(cacheMemory.at(index).begin());
            cacheMemory.at(index).emplace_back(1,tag,data);
            return "Failure: Cache miss";
        }
        void printCache(){
            for (int i = 0; i < nCacheBlocks; i++){
                std::cout << "Block " << i << ": ";
                for (int j = 0; j < nWays; j++){
                    std::cout << cacheMemory.at(i).at(j).getIsValid() << " " << cacheMemory.at(i).at(j).getTag() << " " << cacheMemory.at(i).at(j).getData() << " ";
                }
                std::cout << std::endl;
            }
        }
        int getNCacheBlocks(){
            return nCacheBlocks * nWays;
        }
        int getNCacheWords(){
            return nCacheWords;
        }
        int getNWays(){
            return nWays;
        }
};

/*  This program tries to simulate the memory hierchy of MIPS32
    The addresses are 16 bits long, so the memory size is 2^16
    Each word is 4 bytes long
    The only user input is the number of cache blocks and the words per block */

// TODO: add the words per block functionality
// TODO: improve modularity
// TODO: try to scale to 32 bits
// TODO: changed the type of all address to 16 bits only, i think is with short int
// TODO: create an interface to the user

// TO TEST: ./run.out < addresses.in
int main (){
    Memory myMemory(4,1,4); // 2^n cache blocks, 2^m cache words, w ways
    int number;
    while (std::cin >> number){
        std::cout << myMemory.getData(number) << std::endl;
        //myMemory.printCache();
    }
    return 0;
}