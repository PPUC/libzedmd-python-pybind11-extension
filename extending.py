#! /usr/bin/env python

import ctypes
import pathlib
import time
from sys import platform

from PIL import Image, ImageSequence


# Load ZeDMD library using ctypes
if platform in ("win32", "cygwin"):
  libzedmd = ctypes.CDLL(str(pathlib.Path(__file__).parent.resolve()) + '/extern/libzedmd/build/Release/zedmd64.dll')
  from build.Release.extending import ZeDMD_ext
elif platform == "darwin":
  libzedmd = ctypes.CDLL(str(pathlib.Path(__file__).parent.resolve()) + '/extern/libzedmd/build/libzedmd.dylib')
  from build.extending import ZeDMD_ext
else:
  libzedmd = ctypes.CDLL(str(pathlib.Path(__file__).parent.resolve()) + '/extern/libzedmd/build/libzedmd.so')
  from build.extending import ZeDMD_ext

ZeDMD = ZeDMD_ext()
# ZeDMD.SetBrightness(7)
# ZeDMD.SetRGBOrder(3)
# ZeDMD.SetUsbPackageSize(1024)
# ZeDMD.SetPanelMinRefreshRate(60)
# ZeDMD.SaveSettings()
# ZeDMD.Reset()

img = Image.open("test.jpg")
ZeDMD.RenderRgb888(img)
time.sleep(3)

imageGif = Image.open("animation.gif")
for frame in ImageSequence.Iterator(imageGif):
  ZeDMD.RenderRgb888(frame)
  time.sleep(0.008)

# ZeDMD.WhiteTest()
# ZeDMD.RenderTest()
# ZeDMD.LedTest()

ZeDMD.Close()
