/*
 使用方法：
 
 CMD5 md5;
 CString csMd5Buffer=md5.MD5("a string");
 执行完成之后csMd5Buffer中即存储了由"a string"计算得到的MD5值:
 3a315533c0f34762e0c45e3d4e9d525c
 
 */

#ifndef drawFree_MD5_h
#define drawFree_MD5_h

#include <string>
#include <iostream>

class ZQMD5
{
public:
    typedef unsigned int size_type; // must be 32bit
    
    ZQMD5();
    ZQMD5(const std::string& text);
    void update(const unsigned char *buf, size_type length);
    void update(const char *buf, size_type length);
    ZQMD5& finalize();
    std::string hexdigest() const;
    friend std::ostream& operator<<(std::ostream&, ZQMD5 md5);
    
private:
    void init();
    typedef unsigned char uint1; //  8bit
    typedef unsigned int uint4;  // 32bit
    enum {blocksize = 64}; // VC6 won't eat a const static int here
    
    void transform(const uint1 block[blocksize]);
    static void decode(uint4 output[], const uint1 input[], size_type len);
    static void encode(uint1 output[], const uint4 input[], size_type len);
    
    bool finalized;
    uint1 buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
    uint4 count[2];   // 64bit counter for number of bits (lo, hi)
    uint4 state[4];   // digest so far
    uint1 digest[16]; // the result
    
    // low level logic operations
    static inline uint4 F(uint4 x, uint4 y, uint4 z);
    static inline uint4 G(uint4 x, uint4 y, uint4 z);
    static inline uint4 H(uint4 x, uint4 y, uint4 z);
    static inline uint4 I(uint4 x, uint4 y, uint4 z);
    static inline uint4 rotate_left(uint4 x, int n);
    static inline void FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
    static inline void GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
    static inline void HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
    static inline void II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
};
std::string md5(const std::string str);


#endif
