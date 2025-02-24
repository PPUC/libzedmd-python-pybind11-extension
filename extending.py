#! /usr/bin/env python

import ctypes
import pathlib
import time

from PIL import Image, ImageSequence


# Load ZeDMD library using ctypes
libzedmd = ctypes.CDLL(str(pathlib.Path(__file__).parent.resolve()) + '/extern/libzedmd/build/libzedmd.so')


from build.extending import ZeDMD_ext

ZeDMD = ZeDMD_ext()
ZeDMD.SetBrightness(6)
# ZeDMD.SetRGBOrder(3) 
# ZeDMD.SetUsbPackageSize(1024)
# ZeDMD.SetPanelMinRefreshRate(60)
ZeDMD.SaveSettings()
ZeDMD.Reset()

image = Image.open("test.jpg")
image = image.convert('RGB')
ZeDMD.RenderRgb888(image.getdata())
time.sleep(3)

imageGif = Image.open("animation.gif")
for frame in ImageSequence.Iterator(imageGif):
  frame = frame.convert('RGB')
  ZeDMD.RenderRgb888(frame.getdata())
  time.sleep(0.008)

# ZeDMD.WhiteTest()
# ZeDMD.RenderTest()
# ZeDMD.LedTest() 

ZeDMD.Close()
