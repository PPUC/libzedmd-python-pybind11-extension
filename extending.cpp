// Copyright Ralf W. Grosse-Kunstleve 2002-2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <stdarg.h>
#include <stdlib.h>
#include <chrono>
#include <cstring>
#include <thread>
#include <cstdint>

#include <ZeDMD.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace { // Avoid cluttering the global namespace.


  void ZEDMDCALLBACK LogCallback(const char* format, va_list args, const void* pUserData)
  {
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);

    printf("%s\n", buffer);
  }

  // A friendly class.
  class ZeDMD_ext
  {
    public:
      ZeDMD_ext() { 
        pZeDMD = new ZeDMD();
        
        pZeDMD->SetLogCallback(LogCallback, nullptr);
        
        if (pZeDMD->Open()) {
          pZeDMD->DisableDebug();
          uint16_t width = pZeDMD->GetWidth();
          uint16_t height = pZeDMD->GetHeight();
          pZeDMD->SetFrameSize(width, height);
          pZeDMD->DisableUpscaling();
        } else {
          printf("Error : ZeDMD->Open() is false\n");
        }
      }

      void ClearScreen() {
        pZeDMD->ClearScreen();
      }

      /** @brief Render a RGB24 frame
       *
       *  Renders a true color RGB frame. By default the zone streaming mode is
       *  used. The encoding is RGB888.
       *  @see DisableRGB24Streaming()
       *
       *  @param frame the RGB frame
       */
      void RenderRgb888(std::vector<std::vector<uint8_t>>& image_data){
  
        uint8_t* pImage = (uint8_t*)malloc(128 * 32 * 3 * sizeof(uint8_t));
        int index;
        int i = 0;
        for (int y = 0; y < 32; ++y)
        {
          for (int x = 0; x < 128; ++x)
          {
            i = (y * 128 + x);
            index = (y * 128 + x) * 3;
            pImage[index++] = (image_data[i])[0];
            pImage[index++] = (image_data[i])[1];
            pImage[index] = (image_data[i])[2];
          }
        }
        pZeDMD->RenderRgb888(pImage);
        free(pImage);   
      }
     
      /** @brief Set the RGB order
       *
       *  ZeDMD supports different LED panels.
       *  Depending on the panel, the RGB order needs to be adjusted.
       *  @see https://github.com/PPUC/ZeDMD
       *
       *  @param rgbOrder a value between 0 and 5
       */
      void SetRGBOrder(uint8_t rgbOrder){
        pZeDMD->SetRGBOrder(rgbOrder);
      }
      /** @brief Set the brightness
       *
       *  Set the brightness of the LED panels.
       *  @see https://github.com/PPUC/ZeDMD
       *
       *  @param brightness a value between 0 and 15
       */
      void SetBrightness(uint8_t brightness){
        pZeDMD->SetBrightness(brightness);
      }
      
      void SetPanelMinRefreshRate(uint8_t minRefreshRate){
        pZeDMD->SetPanelMinRefreshRate(minRefreshRate);
      }

      /** @brief Set the USB package size
       * 
       * @param usbPackageSize a value between 32 and 1920, but only multiple of 32
      */
      void SetUsbPackageSize(uint16_t usbPackageSize) {
        pZeDMD->SetUsbPackageSize(usbPackageSize);
      }

      void SaveSettings() {
        pZeDMD->SaveSettings();
      }
    
      void LedTest() { 
        pZeDMD->LedTest();
      }

      void Close() {
        pZeDMD->Close();
      }

      void WhiteTest() {
        uint8_t* pImage = (uint8_t*)malloc(128 * 32 * 3 * sizeof(uint8_t));
        int index;
        for (int y = 0; y < 32; ++y)
        {
          for (int x = 0; x < 128; ++x)
          {
            index = (y * 128 + x) * 3;
            pImage[index++] = 255;
            pImage[index++] = 255;
            pImage[index] = 255;
          }
        }
        pZeDMD->RenderRgb888(pImage);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        free(pImage);   
      }

      void RenderTest() {
        char filename[45];
        uint16_t size = 128 * 32 * 3;
        uint8_t* rgb888 = (uint8_t*)malloc(size * sizeof(uint8_t));
        FILE* fileptr;

        for (int i = 1; i <= 100; i++)
        {
          snprintf(filename, 44, "extern/libzedmd/test/rgb888_%dx%d/%04d.raw", 128, 32, i);
          printf("Render raw: %s\n", filename);
          fileptr = fopen(filename, "rb");
          std::ignore = fread(rgb888, size, 1, fileptr);
          fclose(fileptr);

          pZeDMD->RenderRgb888(rgb888);
          std::this_thread::sleep_for(std::chrono::milliseconds(8));
        }

        free(rgb888);
      }

    private:
      ZeDMD* pZeDMD;

  };
  
}

PYBIND11_MODULE(extending, m)
{
    py::class_<ZeDMD_ext>(m, "ZeDMD_ext")
        .def(py::init())
        .def("ClearScreen", &ZeDMD_ext::ClearScreen)
        .def("SetRGBOrder", &ZeDMD_ext::SetRGBOrder)
        .def("SetBrightness", &ZeDMD_ext::SetBrightness)
        .def("Close", &ZeDMD_ext::Close)
        .def("LedTest", &ZeDMD_ext::LedTest)
        .def("WhiteTest", &ZeDMD_ext::WhiteTest)
        .def("RenderTest", &ZeDMD_ext::RenderTest)
        .def("RenderRgb888", &ZeDMD_ext::RenderRgb888)
        .def("SetUsbPackageSize", &ZeDMD_ext::SetUsbPackageSize)
        .def("SetPanelMinRefreshRate", &ZeDMD_ext::SetPanelMinRefreshRate)
        .def("SaveSettings", &ZeDMD_ext::SaveSettings)       
    ;
    
}
