#include "define.h"
#if PSS_PLATFORM == PLATFORM_WIN
#include<WinSock2.h>
#pragma comment(lib, "wsock32.lib")
#else
#include <arpa/inet.h>
#endif

inline uint64 htonll_uint64(uint64 val)
{
    return (((uint64)htonl((int)((val << 32) >> 32))) << 32) | (unsigned int)htonl((int)(val >> 32));
}

inline int64 htonll_int64(int64 val)
{
    return (((int64)htonl((int)((val << 32) >> 32))) << 32) | (unsigned int)htonl((int)(val >> 32));
}
class CWriteBuffer
{
public:
    CWriteBuffer(std::string* buffer) : buffer_(buffer)
    {
        write_ptr_ = (uint32)buffer->size();
    }

    CWriteBuffer& operator << (uint8 u1Data)
    {
        buffer_->append((char*)&u1Data, sizeof(u1Data));
        write_ptr_ += sizeof(u1Data);
        return *this;
    };

    CWriteBuffer& operator << (uint16 u2Data)
    {
        if (true == is_net_sort_)
        {
            //��������ת��Ϊ����
            u2Data = htons(u2Data);
        }

        buffer_->append((char*)&u2Data, sizeof(u2Data));
        write_ptr_ += sizeof(u2Data);
        return *this;
    };

    CWriteBuffer& operator << (uint32 u4Data)
    {
        if (true == is_net_sort_)
        {
            //��������ת��Ϊ����
            u4Data = htonl(u4Data);
        }

        buffer_->append((char*)&u4Data, sizeof(u4Data));
        write_ptr_ += sizeof(u4Data);
        return *this;
    };

    CWriteBuffer& operator << (uint64 u8Data)
    {
        if (true == is_net_sort_)
        {
            //��������ת��Ϊ����
            u8Data = htonll_uint64(u8Data);
        }

        buffer_->append((char*)&u8Data, sizeof(u8Data));
        write_ptr_ += sizeof(u8Data);
        return *this;
    };

    CWriteBuffer& operator << (int8 n1Data)
    {
        buffer_->append((char*)&n1Data, sizeof(n1Data));
        write_ptr_ += sizeof(n1Data);
        return *this;
    };

    CWriteBuffer& operator << (int16 n2Data)
    {
        if (true == is_net_sort_)
        {
            //��������ת��Ϊ����
            n2Data = htons(n2Data);
        }

        buffer_->append((char*)&n2Data, sizeof(n2Data));
        write_ptr_ += sizeof(n2Data);
        return *this;
    };

    CWriteBuffer& operator << (int32 n4Data)
    {
        if (true == is_net_sort_)
        {
            //��������ת��Ϊ����
            n4Data = htonl(n4Data);
        }

        buffer_->append((char*)&n4Data, sizeof(n4Data));
        write_ptr_ += sizeof(n4Data);
        return *this;
    };

    CWriteBuffer& operator << (int64 n8Data)
    {
        if (true == is_net_sort_)
        {
            //��������ת��Ϊ����
            n8Data = htonll_int64(n8Data);
        }

        buffer_->append((char*)&n8Data, sizeof(n8Data));
        write_ptr_ += sizeof(n8Data);
        return *this;
    };

    CWriteBuffer& operator << (float32 f4Data)
    {
        buffer_->append((char*)&f4Data, sizeof(f4Data));
        write_ptr_ += sizeof(f4Data);
        return *this;
    };

    CWriteBuffer& operator << (float64 f8Data)
    {
        buffer_->append((char*)&f8Data, sizeof(f8Data));
        write_ptr_ += sizeof(f8Data);
        return *this;
    };

    CWriteBuffer& operator << (std::string& str)
    {
        *this << (uint32)str.length();

        buffer_->append(str.c_str(), str.length());
        write_ptr_ += sizeof(str.length());
        return *this;
    };

    void write_data(const char* data, uint32 size)
    {
        buffer_->append(data, size);
        write_ptr_ += size;
    };

    void write_data_from_string(std::string data)
    {
        buffer_->append(data.c_str(), data.size());
        write_ptr_ += (uint32)data.size();
    };

    void set_net_sort(bool net_sort)
    {
        is_net_sort_ = net_sort;
    }

private:
    std::string* buffer_ = nullptr;
    uint32 write_ptr_ = 0;
    bool is_net_sort_ = false;
};