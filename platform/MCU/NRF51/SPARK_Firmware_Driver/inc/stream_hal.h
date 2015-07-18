#ifndef _STREAM_HAL_H
#define	_STREAM_HAL_H

#ifdef __cplusplus

class Stream
{
    public:
        Stream();
        Stream(int bufferSize);
        int32_t socket_connect();
        int32_t send(const void* buffer, uint32_t len);
        int32_t receive(void* buffer, uint32_t len, unsigned long _timeout);
        int32_t close();

    
    private:
        uint8_t bleSocketBuffer[];
        uint32_t id;
};

#endif
#endif	/* STREAM_HAL_H */