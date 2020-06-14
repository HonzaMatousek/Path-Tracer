#include <fstream>
#include "ImageTGA.h"

ImageTGA::ImageTGA(int width, int height) : Image(width, height) {}

void ImageTGA::Save(const std::string &fileName, double exposition) const {
    std::ofstream out(fileName, std::ios_base::binary);
    unsigned char TGAheader[18] = {0,0,2, 0,0, 0,0, 16, 0,0, 0,0, (unsigned char)(width & 0xFFu), (unsigned char)(width >> 8u), (unsigned char)(height & 0xFFu), (unsigned char)(height >> 8u), 24, 0x20};
    out.write((char *)TGAheader, sizeof(TGAheader));
    unsigned char buf[3];
    for(auto & line : pixels) {
        for(auto & pixel : line) {
            pixel.WriteIntoByteBufferBGR(buf, exposition);
            out.write((char *)buf, sizeof(buf));
        }
    }
}
