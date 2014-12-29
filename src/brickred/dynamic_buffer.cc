#include <brickred/dynamic_buffer.h>

#include <arpa/inet.h>
#include <cstring>
#include <algorithm>

namespace brickred {

DynamicBuffer::DynamicBuffer(size_t init_size, size_t expand_size) :
    buffer_(init_size), expand_size_(expand_size),
    read_index_(0), write_index_(0)
{
}

DynamicBuffer::~DynamicBuffer()
{
}

void DynamicBuffer::swap(DynamicBuffer &other)
{
    buffer_.swap(other.buffer_);
    std::swap(expand_size_, other.expand_size_);
    std::swap(read_index_, other.read_index_);
    std::swap(write_index_, other.write_index_);
}

size_t DynamicBuffer::capacity() const
{
    return buffer_.size();
}

size_t DynamicBuffer::discardableBytes() const
{
    return read_index_;
}

size_t DynamicBuffer::readableBytes() const
{
    return write_index_ - read_index_;
}

size_t DynamicBuffer::writableBytes() const
{
    return buffer_.size() - write_index_;
}

const char *DynamicBuffer::readBegin() const
{
    return &buffer_[read_index_];
}

char *DynamicBuffer::writeBegin()
{
    return &buffer_[write_index_];
}

void DynamicBuffer::read(size_t size)
{
    read_index_ += std::min(size, readableBytes());

    if (read_index_ == write_index_) {
        clear();
    }
}

void DynamicBuffer::write(size_t size)
{
    write_index_ += std::min(size, writableBytes());
}

void DynamicBuffer::reserveWritableBytes(size_t size)
{
    if (writableBytes() >= size) {
        return;
    }

    size_t free_bytes = discardableBytes() + writableBytes();
    size_t new_size = buffer_.size();

    if (free_bytes >= size && free_bytes - size < expand_size_) {
        new_size += expand_size_;
    } else {
        new_size += expand_size_ * ((size - free_bytes) / expand_size_ + 1);
    }

    size_t readable_bytes = readableBytes();
    std::vector<char> new_buffer(new_size);
    ::memcpy(&new_buffer[0], readBegin(), readable_bytes);

    buffer_.swap(new_buffer);
    read_index_ = 0;
    write_index_ = readable_bytes;
}

void DynamicBuffer::clear()
{
    read_index_ = 0;
    write_index_ = 0;
}

bool DynamicBuffer::peekInt8(uint8_t &v, size_t offset)
{
    if (readableBytes() < offset + 1) {
        return false;
    }
    const char *p = readBegin() + offset;

    v = *(const uint8_t *)p;

    return true;
}

bool DynamicBuffer::peekInt8(uint16_t &v, size_t offset)
{
    uint8_t v8;
    if (peekInt8(v8, offset) == false) {
        return false;
    }

    v = v8;

    return true;
}

bool DynamicBuffer::peekInt8(uint32_t &v, size_t offset)
{
    uint8_t v8;
    if (peekInt8(v8, offset) == false) {
        return false;
    }

    v = v8;

    return true;
}

bool DynamicBuffer::peekInt8(uint64_t &v, size_t offset)
{
    uint8_t v8;
    if (peekInt8(v8, offset) == false) {
        return false;
    }

    v = v8;

    return true;
}

bool DynamicBuffer::peekInt16(uint16_t &v, size_t offset)
{
    if (readableBytes() < offset + 2) {
        return false;
    }
    const char *p = readBegin() + offset;

    v = ntohs(*(const uint16_t *)p);

    return true;
}

bool DynamicBuffer::peekInt16(uint32_t &v, size_t offset)
{
    uint16_t v16;
    if (peekInt16(v16, offset) == false) {
        return false;
    }

    v = v16;

    return true;
}

bool DynamicBuffer::peekInt16(uint64_t &v, size_t offset)
{
    uint16_t v16;
    if (peekInt16(v16, offset) == false) {
        return false;
    }

    v = v16;

    return true;
}

bool DynamicBuffer::peekInt32(uint32_t &v, size_t offset)
{
    if (readableBytes() < offset + 4) {
        return false;
    }
    const char *p = readBegin() + offset;

    v = ntohl(*(const uint32_t *)p);

    return true;
}

bool DynamicBuffer::peekInt32(uint64_t &v, size_t offset)
{
    uint32_t v32;
    if (peekInt32(v32, offset) == false) {
        return false;
    }

    v = v32;

    return true;
}

bool DynamicBuffer::peekInt64(uint64_t &v, size_t offset)
{
    if (readableBytes() < offset + 8) {
        return false;
    }
    const char *p = readBegin() + offset;

    v = (uint64_t)ntohl(*(const uint32_t *)(p + 4)) +
        ((uint64_t)ntohl(*(const uint32_t *)p) << 32);

    return true;
}

bool DynamicBuffer::readInt8(uint8_t &v)
{
    if (peekInt8(v) == false) {
        return false;
    }
    read(1);

    return true;
}

bool DynamicBuffer::readInt8(uint16_t &v)
{
    if (peekInt8(v) == false) {
        return false;
    }
    read(1);

    return true;
}

bool DynamicBuffer::readInt8(uint32_t &v)
{
    if (peekInt8(v) == false) {
        return false;
    }
    read(1);

    return true;
}

bool DynamicBuffer::readInt8(uint64_t &v)
{
    if (peekInt8(v) == false) {
        return false;
    }
    read(1);

    return true;
}

bool DynamicBuffer::readInt16(uint16_t &v)
{
    if (peekInt16(v) == false) {
        return false;
    }
    read(2);

    return true;
}

bool DynamicBuffer::readInt16(uint32_t &v)
{
    if (peekInt16(v) == false) {
        return false;
    }
    read(2);

    return true;
}

bool DynamicBuffer::readInt16(uint64_t &v)
{
    if (peekInt16(v) == false) {
        return false;
    }
    read(2);

    return true;
}

bool DynamicBuffer::readInt32(uint32_t &v)
{
    if (peekInt32(v) == false) {
        return false;
    }
    read(4);

    return true;
}

bool DynamicBuffer::readInt32(uint64_t &v)
{
    if (peekInt32(v) == false) {
        return false;
    }
    read(4);

    return true;
}

bool DynamicBuffer::readInt64(uint64_t &v)
{
    if (peekInt64(v) == false) {
        return false;
    }
    read(8);

    return true;
}

void DynamicBuffer::writeInt8(uint8_t v)
{
    reserveWritableBytes(1);
    char *p = writeBegin();
    *(uint8_t *)p = v;
    write(1);
}

void DynamicBuffer::writeInt16(uint16_t v)
{
    reserveWritableBytes(2);
    char *p = writeBegin();
    *(uint16_t *)p = htons((uint16_t)v);
    write(2);
}

void DynamicBuffer::writeInt32(uint32_t v)
{
    reserveWritableBytes(4);
    char *p = writeBegin();
    *(uint32_t *)p = htonl((uint32_t)v);
    write(4);
}

void DynamicBuffer::writeInt64(uint64_t v)
{
    reserveWritableBytes(8);
    char *p = writeBegin();
    *(uint32_t *)p = htonl((uint32_t)(v >> 32));
    *(uint32_t *)(p + 4) = htonl((uint32_t)(v & 0xffffffff));
    write(8);
}

} // namespace brickred
