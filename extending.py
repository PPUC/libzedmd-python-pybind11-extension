#! /usr/bin/env python

import ctypes
import pathlib
import time

from PIL import Image, ImageSequence


# Load ZeDMD library using ctypes
libzedmd = ctypes.CDLL(str(pathlib.Path(__file__).parent.resolve()) + '/extern/libzedmd/build/libzedmd.so')


from build.extending import ZeDMD_ext

ZeDMD = ZeDMD_ext()
ZeDMD.SetBrightness(8)
# ZeDMD.SetRGBOrder(3)


img = Image.open("test.jpg") 
ZeDMD.RenderRgb888(img)
time.sleep(3)

imageGif = Image.open("animation.gif")
for frame in ImageSequence.Iterator(imageGif):
  ZeDMD.RenderRgb888(frame)
  time.sleep(0.016)

# ZeDMD.WhiteTest()
# ZeDMD.RenderTest()
# ZeDMD.LedTest() 

ZeDMD.Close()
