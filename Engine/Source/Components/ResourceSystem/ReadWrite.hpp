#pragma once

namespace vh
{

template<typename T>
bool OpenFile(T& stream, const std::string& filename, std::ios_base::openmode mode)
{
    stream.open(filename, mode);

    if (!stream.is_open())
    {
        LOG(WARNING) << "Could not open file: " << filename;
    }

    return stream.is_open();
}

template<typename T>
void Write(std::ostream& stream, const T* obj, size_t count = 1)
{
    stream.write(reinterpret_cast<const char*>(obj), sizeof(T) * count);
}

template<typename T>
void Read(std::istream& stream, T* obj, size_t count = 1)
{
    stream.read(reinterpret_cast<char*>(obj), sizeof(T) * count);
}

} // namespace vh
