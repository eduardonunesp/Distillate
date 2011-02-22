#ifndef __BITMAPDATA_HPP__
#define __BITMAPDATA_HPP__ 

namespace Distillate 
{
    namespace SDL
    {
        class BitmapData 
        {
        public:
            int width;
            int height;

            BitmapData();
            ~BitmapData();
        };
    }
}
#endif
