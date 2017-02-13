#include "PCH.hpp"
#include "Names.hpp"

void Names::TickInit(uint32_t delta)
{
    LoadDictionary();

    srand(clock());

    FinishInit();
}

void Names::TickClose(uint32_t delta)
{
    DestroyDictionary();

    FinishClose();
}

void LoadWords(std::fstream& file, char*& data, size_t count)
{
    data = new char[Names::MAX_WORD_SIZE * count];
    memset((void*) data, '\0', Names::MAX_WORD_SIZE * count * sizeof(char));
    for (size_t idx = 0; idx < count; ++idx)
    {
        std::string word;
        if (!(file >> word))
        {
            LOG(WARNING) << "Unexpected EOF";
            return;
        }
        if (word.size() >= Names::MAX_WORD_SIZE)
        {
            LOG(WARNING) << "Name in dictionary is too long. Truncating: " << word;
            word = word.substr(0, Names::MAX_WORD_SIZE - 1);
        }
        memcpy((void*) &data[idx * Names::MAX_WORD_SIZE], word.c_str(), word.size());
    }
}

void Names::LoadDictionary()
{
    /*
        TODO: resave the dictionary in binary form
    */

    std::string dictfname = "Assets/names.dict";
    std::fstream dictf(dictfname, std::ios_base::in);

    std::string tag, count;
    while (dictf >> tag >> count)
    {
        if (tag == "MALE")
        {
            mMaleNamesCnt = stoi(count);
            LoadWords(dictf, mMaleNames, mMaleNamesCnt);
        }
        else if (tag == "FEMALE")
        {
            mFemaleNamesCnt = stoi(count);
            LoadWords(dictf, mFemaleNames, mFemaleNamesCnt);
        }
        else if (tag == "LAST")
        {
            mLastNamesCnt = stoi(count);
            LoadWords(dictf, mLastNames, mLastNamesCnt);
        }
    }

    mDictionaryLoaded = true;

    LOG(INFO) << "Dictionary loaded. " << mMaleNamesCnt << " male names, "
        << mFemaleNamesCnt << " female names, " << mLastNamesCnt << " last names.";

    dictf.close();
}

void Names::DestroyDictionary()
{
    if (mMaleNames != nullptr)
    {
        delete[] mMaleNames;
        mMaleNames = nullptr;
        mMaleNamesCnt = 0;
    }
    if (mMaleNames != nullptr)
    {
        delete[] mMaleNames;
        mMaleNames = nullptr;
        mMaleNamesCnt = 0;
    }
    if (mLastNames != nullptr)
    {
        delete[] mLastNames;
        mLastNames = nullptr;
        mLastNamesCnt = 0;
    }
    mDictionaryLoaded = false;
}

std::string Names::GetRandomMaleName()
{
    std::string name = "";
    if (mMaleNamesCnt > 0)
    {
        size_t idx = rand() % mMaleNamesCnt;
        name.append(&mMaleNames[idx * MAX_WORD_SIZE]);
    }
    if (mLastNamesCnt > 0)
    {
        name.append(1, ' ');
        size_t idx = rand() % mLastNamesCnt;
        name.append(&mLastNames[idx * MAX_WORD_SIZE]);
    }
    return name;
}
