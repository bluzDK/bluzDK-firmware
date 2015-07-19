#ifndef _STREAM_HAL_H
#define	_STREAM_HAL_H

typedef struct
{
    uint16_t   sa_family;
    uint8_t    sa_data[14];
} sockaddr_b;

#define MAX_STREAM_BUFFER_SIZE  1120

class Stream
{
    public:
        Stream();
        Stream(int bufferSize);
        int32_t connect(const sockaddr_b *addr, long addrlen);
        int32_t send(const void* buffer, uint32_t len);
        int32_t receive(void* buffer, uint32_t len, unsigned long _timeout);
        int32_t close();

    
    private:
        uint8_t bleSocketBuffer[MAX_STREAM_BUFFER_SIZE];
        uint32_t id;
};

#endif	/* STREAM_HAL_H */