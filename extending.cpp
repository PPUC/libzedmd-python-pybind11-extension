#include <stdarg.h>
#include <stdlib.h>
#include <chrono>
#include <cstring>
#include <cstdint>

#include <ZeDMD.h>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

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
      ZeDMD_ext(uint16_t x = 128, uint16_t y = 32) { 
        pZeDMD = new ZeDMD();
        
        pZeDMD->SetLogCallback(LogCallback, nullptr);
        
        if (pZeDMD->Open()) {
          pZeDMD->DisableDebug();
          width = x;
          height = y;
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
      void RenderRgb888(py::array_t<uint8_t>& img){
        
        pZeDMD->RenderRgb888((uint8_t*)img.data());
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
      /** @brief Set the brightness for the next start
       *
       *  @param brightness a value between 0 and 15
       */
      void SetBrightness(uint8_t brightness){
        pZeDMD->SetBrightness(brightness);
      }

      uint8_t GetBrightness(){
        return pZeDMD->GetBrightness();
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

      /** @brief Save settings previously set 
       * 
       * Save Brightness, RGBOrder, UsbPackageSize, PanelMinRefreshRate, ...
       * New settings will be apllied after next ZeDMD start or reset
      */     
      void SaveSettings() {
        pZeDMD->SaveSettings();
      }
    
      void Reset() { 
        pZeDMD->Reset();
        std::this_thread::sleep_for(std::chrono::seconds(2));        
      }

      void LedTest() { 
        pZeDMD->LedTest();
      }

      void Close() {
        pZeDMD->Close();
      }

      void WhiteTest() {
        uint8_t* pImage = (uint8_t*)malloc(width * height * 3 * sizeof(uint8_t));
        int index;
        for (int y = 0; y < height; ++y)
        {
          for (int x = 0; x < width; ++x)
          {
            index = (y * width + x) * 3;
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
        char filename[50];
        uint16_t size = width * height * 3;
        uint8_t* rgb888 = (uint8_t*)malloc(size * sizeof(uint8_t));
        FILE* fileptr;

        for (int i = 1; i <= 100; i++)
        {
          snprintf(filename, 49, "extern/libzedmd/test/rgb888_%dx%d/%04d.raw", width, height, i);
          printf("Render raw: %s\n", filename);
          fileptr = fopen(filename, "rb");
          std::ignore = fread(rgb888, size, 1, fileptr);
          fclose(fileptr);

          pZeDMD->RenderRgb888(rgb888);
          std::this_thread::sleep_for(std::chrono::milliseconds(8));
        }

        free(rgb888);
      }

      const uint16_t &GetWidth() const { return width; }
      const uint16_t &GetHeight() const { return height; }

    private:
      ZeDMD* pZeDMD;
      uint16_t width;
      uint16_t height;

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
      .def("Reset", &ZeDMD_ext::Reset)
      .def("WhiteTest", &ZeDMD_ext::WhiteTest)
      .def("RenderTest", &ZeDMD_ext::RenderTest)
      .def("RenderRgb888", &ZeDMD_ext::RenderRgb888)
      .def("SetUsbPackageSize", &ZeDMD_ext::SetUsbPackageSize)
      .def("SetPanelMinRefreshRate", &ZeDMD_ext::SetPanelMinRefreshRate)
      .def("SaveSettings", &ZeDMD_ext::SaveSettings) 
      .def("GetWidth", &ZeDMD_ext::GetWidth)   
      .def("GetHeight", &ZeDMD_ext::GetHeight)
      .def("GetBrightness", &ZeDMD_ext::GetBrightness)
  ;  
}
