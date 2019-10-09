#include "Utils/VhUtils_PCH.hpp"
#include "FileSystem.hpp"

void vh::Utils::ListFiles(const char* folder, std::vector<std::string>& outFiles)
{
#ifdef CMAKE_PLATFORM_WINDOWS
    std::vector<std::string> names;
    char search_path[200];
    sprintf_s(search_path, "%s/*.*", folder);
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path, &fd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            // read all (real) files in current folder, delete '!' read other 2 default folder . and ..
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                std::string path(folder);
                path.append("/");
                path.append(fd.cFileName);
                outFiles.push_back(path);
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
#endif
}
