#pragma once

namespace vh
{
    /*
          //        \\
        RRRR SkyBox RRRR
          \\        //
    */
    class RSkyBox : public Resource
    {
    private:
        std::string mPosX;
        std::string mNegX;
        std::string pos_y;
        std::string neg_y;
        std::string pos_z;
        std::string neg_z;
    };

} // namespace vh
