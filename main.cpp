#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <cstdint>

const uint32_t MEMORY_SIZE = std::pow(2,32);

class MainMemory {
    private:
        std::map<uint32_t,uint32_t> memory;
    public:
        uint32_t getData(uint32_t address){
            uint32_t data = address; // This is here if want to change the type of data in the memory
            if (memory.find(address) == memory.end()){
                memory.insert(std::make_pair(address, data));
            }
            return memory.at(address);
        }
};

class CacheBlock {
    private:
        int isValid;
        uint32_t tag;
        uint32_t data;
    public:
        CacheBlock(){
            this->isValid = 0;
            this->tag = 0;
            this->data = 0xDEADBEEF;
        }
        CacheBlock(int isValid, uint32_t tag, uint32_t data){
            this->isValid = isValid;
            this->tag = tag;
            this->data = data;
        }
        int getIsValid(){
            return isValid;
        }
        uint32_t getTag(){
            return tag;
        }
        uint32_t getData(){
            return data;
        }
        void setIsValid(int isValid){
            this->isValid = isValid;
        }
        void setTag(uint32_t tag){
            this->tag = tag;
        }
        void setData(uint32_t data){
            this->data = data;
        }
};

class Memory {
    private:
        MainMemory mainMemory;
        std::map<uint32_t,std::vector<CacheBlock>> cacheMemory;
        uint32_t nCacheBlocks;
        uint32_t nCacheWords;
        uint32_t nWays;
    public:
        Memory(uint32_t cacheSize, uint32_t m, uint32_t ways){
            // Initialize main memory
            mainMemory = MainMemory();

            // Check for valid input
            if (cacheSize % 2 != 0 || ways % 2 != 0){
                std::cout << "Failure: cache size and degree of asociativity must be powers of 2" << std::endl;
                return;
            }

            // Initialize cache memory
            nCacheBlocks = cacheSize / (4 * ways); // cache size in bytes / (4 bytes per word * ways of asociativity)
            nCacheWords = m;
            nWays = ways;
            std::vector<CacheBlock> cacheBlocks;

            for (uint32_t i = 0; i < nCacheBlocks; i++){
                for (uint32_t j = 0; j < ways; j++){
                    cacheBlocks.emplace_back();
                }
                cacheMemory.insert(std::make_pair(i,cacheBlocks));
            }
        }
        std::string getData(uint32_t address){
            if (address < 0 || address >= MEMORY_SIZE){
                return "Failure: Address out of bounds";
            }
            uint32_t index = address % nCacheBlocks;
            uint32_t tag = address / nCacheBlocks;
            for (uint32_t i = 0; i < nWays; i++){
                if (cacheMemory.at(index).at(i).getIsValid() == 1 && cacheMemory.at(index).at(i).getTag() == tag){
                    cacheMemory.at(index).push_back(cacheMemory.at(index).at(i));
                    cacheMemory.at(index).erase(cacheMemory.at(index).begin() + i);
                    return "data" + std::to_string(cacheMemory.at(index).back().getData());
                }
            }
            uint32_t data = mainMemory.getData(address);
            cacheMemory.at(index).erase(cacheMemory.at(index).begin());
            cacheMemory.at(index).emplace_back(1,tag,data);
            return "Failure: Cache miss";
        }
        void printCache(){
            for (uint32_t i = 0; i < nCacheBlocks; i++){
                std::cout << "Block " << i << ": ";
                for (uint32_t j = 0; j < nWays; j++){
                    std::cout << "Set " << j << ": " << cacheMemory.at(i).at(j).getIsValid() << " " << cacheMemory.at(i).at(j).getTag() << " " << cacheMemory.at(i).at(j).getData() << " | ";
                }
                std::cout << std::endl;
            }
        }
        uint32_t getNCacheBlocks(){
            return nCacheBlocks;
        }
        uint32_t getNCacheWords(){
            return nCacheWords;
        }
        uint32_t getNWays(){
            return nWays;
        }
};

/*  This program tries to simulate the memory hierchy of MIPS32
    The addresses are 32 bits long, so the memory size is 2^32
    Each word is 4 bytes long
    The only user input is the number of cache blocks and the words per block */

// TODO: add the words per block functionality
// TODO: improve modularity
// TODO: create an interface to the user

// TO TEST: ./run.out < addresses.in
int main (){
    Memory myMemory(64,1,4); // cache size in bytes, 2^m cache words, w ways
    int number;
    while (std::cin >> number){
        std::cout << myMemory.getData(number) << std::endl;
    }
    //myMemory.printCache();
    return 0;
}