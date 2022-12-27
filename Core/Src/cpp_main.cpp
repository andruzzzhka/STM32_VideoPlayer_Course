
#include "main.h"
#include "stdlib.h"
#include "stdio.h"

#include "usb_device.h"
#include "fatfs.h"
#include <string.h>
#include "st7735.h"
#include "fonts.h"

#include <vector>
#include "images.h"

#include "mpeg/player.h"

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"

const uint8_t _pin[256+256+256] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
    0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
    0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
    0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
    0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
};
const uint8_t* _pin_ = _pin + 256;
#define PIN(_x) _pin_[(_x)]

#define MAX_BUFSIZE (3 * MAX_L3_FRAME_PAYLOAD_BYTES)
#define FRAMEBUFFER_SIZE 160*128*2

static const float videoPictureRates[] = {
    0.000, 23.976, 24.000, 25.000, 29.970, 30.000, 50.000, 59.940,
    60.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000
};

static const char* ioErrorsText[] = { "OK", "Disk error", "Internal error", "Not ready", "No file", "No path", "Invalid name",
        "Access denied", "Name already exist", "Invalid object", "Write protected", "Invalid drive", "Not mounted",
        "No file system", "MKFS aborted", "Timeout", "Locked", "Out of memory", "Too many open files", "Invalid parameters"};

uint8_t framebuffer[FRAMEBUFFER_SIZE];
uint8_t audiobuffer[MAX_BUFSIZE];
int audiobufferSize, audiobufferPos;
int eof, desync, rate = -1;

uint32_t decoderLoopsCompleted = 0;
int samplesCounter = 0;
int targetFrameTime = -1;
int frameTimeOffset = 0;

MpegDecoder* dec;
mp3dec_t mp3;

FATFS fileSystem = {0};

int filesIsVideo(const char *str);
void fileMenuLoop(char* path);
void showVideo(char* path);
void renderFrame(Frame* frame);
void drawScreen();
void audioCallback(const uint8_t* payload, int size, int64_t pts, int isExpected);
void ioErrorCallback(FRESULT errorCode);

extern "C" {

    void cpp_main()
    {
      CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
      DWT->CYCCNT = 0;
      DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

      printf("Initializing display...\n");
      ST7735_Init();

      memset(framebuffer, 0xFF, 160*128*2);

      printf("Display initialized\n");

      ST7735_DrawImage_FB(framebuffer, 48, 24, 64, 64, img_micro_sd);
      ST7735_DrawImage_FB(framebuffer, 72, 92, 16, 16, img_circle);

      drawScreen();

      HAL_Delay(250);

      int status = -1;

      status = HAL_SD_Init(&hsd);
      printf("SD Card init status: %d\n", status);

      if(status != 0)
      {
          char* errorCode = (char*)ioErrorsText[status];
          ST7735_WriteString_FB(framebuffer, 80 - (strlen(errorCode)*7)/2, 114, errorCode, Font_7x10, ST7735_BLACK, ST7735_WHITE);
      }

      if(status == 0)
          ST7735_DrawImage_FB(framebuffer, 72, 92, 16, 16, img_check);
      else
      {
          ST7735_DrawImage_FB(framebuffer, 72, 92, 16, 16, img_close);
          drawScreen();
          return;
      }

      drawScreen();
      HAL_Delay(250);

      MX_FATFS_Init();
      FRESULT res = f_mount(&fileSystem, SDPath, 1);

      fileMenuLoop("/");
    }

}

int filesIsVideo(const char *str)
{
    char *dot = strrchr(str, '.');

    if (NULL == dot) return 0;
    return strcmp(dot, ".TS") == 0;
}

void fileMenuLoop(char* path)
{
    f_chdir(path);

    while (true)
    {
        FRESULT res;
        DIR dir;
        FILINFO fno;

        uint8_t fileOffset = 0;
        uint8_t drawnFileOffset = -1;

        char* cwd = new char[22];
        f_getcwd(cwd, 21);

        memset(framebuffer, 0xFF, 160*128*2);
        ST7735_WriteString_FB(framebuffer, 1, 3, cwd, Font_7x10, ST7735_BLACK, ST7735_WHITE);

        res = f_opendir(&dir, "");
        if(res != FR_OK)
        {
            ioErrorCallback(res);

            return;
        }

        std::vector<std::pair<uint8_t, char*>> files;
        if (strcmp(cwd, "/") != 0)
            files.push_back({1, ".."});

        while (true)
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;

            uint8_t isDir = (fno.fattrib & AM_DIR) != 0;

            if(!isDir && !filesIsVideo(fno.fname)) continue;

            char* fileName = new char[13];
            strncpy(fileName, fno.fname, 12);

            files.push_back({isDir, fileName});
        }

        res = f_closedir(&dir);

        while (true)
        {
            if(drawnFileOffset != fileOffset)
            {
                ST7735_FillRectangle_FB(framebuffer, 0, 16, 160, 112, ST7735_WHITE);

                for (int i = fileOffset; i < std::min((int)files.size(), fileOffset + 7); i++)
                {
                    if(i == fileOffset)
                    {
                        ST7735_FillRectangle_FB(framebuffer, 18, (i - fileOffset) * 16 + 16, 160-18, 16, ST7735_BLUE);
                    }

                    auto file = files[i];

                    ST7735_WriteString_FB(framebuffer, 18, (i - fileOffset) * 16 + 16 + 4, file.second, Font_7x10, i == fileOffset ? ST7735_WHITE : ST7735_BLACK, i == fileOffset ? ST7735_BLUE : ST7735_WHITE);
                    ST7735_DrawImage_FB(framebuffer, 1, (i - fileOffset) * 16 + 16, 16, 16, file.first ? img_folder : img_video_player);
                }

                drawScreen();
                drawnFileOffset = fileOffset;
                HAL_Delay(250);
            }

            if (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_11) == GPIO_PIN_SET)
            {
                fileOffset++;

                if(fileOffset >= files.size())
                    fileOffset = 0;
            }

            if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
            {
                HAL_Delay(100);
                while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) { }
                HAL_Delay(100);

                if(files[fileOffset].first)
                {
                    f_chdir(files[fileOffset].second);
                    break;
                }
                else
                    showVideo(files[fileOffset].second);
            }
        }
    }

}

void showVideo(char* path)
{
    FIL testFile;
    UINT testBytes;
    FRESULT res;

    Frame _frame_buffers[2];

    printf("Opening file...\n");

    res = f_open(&testFile, path, FA_READ);

    printf("Opened file: %d\n", res);

    if(res != FR_OK)
    {
        ioErrorCallback(res);
        return;
    }

    _frame_buffers[0].init();
    _frame_buffers[1].init();

    dec = new MpegDecoder(&testFile, &_frame_buffers[0], &_frame_buffers[1]);
    mp3dec_init(&mp3);

    dec->renderCallback = renderFrame;
    dec->audioDecodeCallback = audioCallback;
    dec->ioErrorCallback = ioErrorCallback;

    memset(framebuffer, 0x00, 160*128*2);
    DWT->CYCCNT = 0;
    dec->run();

    res = f_close(&testFile);
    printf("Closed file: %d\n", res);

    NVIC_SystemReset();
}

void renderFrame(Frame* frame)
{
    if(targetFrameTime == -1)
    {
        targetFrameTime = (100000.0 / videoPictureRates[dec->picture_rate]) + 5;
    }

    int yOffset = (FB_HEIGHT - dec->vertical_size) / 2;
    int xOffset = (FB_WIDTH - dec->horizontal_size) / 2;

    for(int slice = 0; slice < FB_SLICES; slice++)
    {
        uint8_t* buf = frame->_slices[slice];

        for (int y = 0; y < FB_SLICE_HEIGHT && (FB_SLICE_HEIGHT * slice + y) < dec->vertical_size; y+=2)
        {
            for (int x = 0; x < FB_WIDTH && x < dec->horizontal_size; x+=2)
            {
                int32_t y0 = ((buf[y * FB_STRIDE + x] - 16) * 76309) >> 16;
                int32_t y1 = ((buf[y * FB_STRIDE + x + 1] - 16) * 76309) >> 16;
                int32_t y2 = ((buf[y * FB_STRIDE + x + FB_STRIDE] - 16) * 76309) >> 16;
                int32_t y3 = ((buf[y * FB_STRIDE + x + FB_STRIDE + 1] - 16) * 76309) >> 16;

                int32_t cb = buf[(y >> 1) * FB_STRIDE + FB_WIDTH + (x >> 1)] - 128;
                int32_t cr = buf[(y >> 1) * FB_STRIDE + FB_STRIDE * (FB_SLICE_HEIGHT / 2) + FB_WIDTH + (x >> 1)] - 128;

                int r = (cr * 104597) >> 16;
                int g = (cb * 25674 + cr * 53278) >> 16;
                int b = (cb * 132201) >> 16;

                uint8_t y0g = PIN(y0 - g);
                int idx = (y + FB_SLICE_HEIGHT * slice + yOffset) * FB_WIDTH * 2 + x * 2 + xOffset * 2;
                framebuffer[idx] = (PIN(y0 + r) & 0b11111000) | (y0g >> 5);
                framebuffer[idx + 1] = ((PIN(y0 + b) >> 3) & 0b00011111) | ((y0g << 3) & 0b11100000);


                uint8_t y1g = PIN(y1 - g);
                idx = (y + FB_SLICE_HEIGHT * slice + yOffset) * FB_WIDTH * 2 + x * 2 + xOffset * 2;
                framebuffer[idx + 2] = (PIN(y1 + r) & 0b11111000) | (y1g >> 5);
                framebuffer[idx + 3] = ((PIN(y1 + b) >> 3) & 0b00011111) | ((y1g << 3) & 0b11100000);


                uint8_t y2g = PIN(y2 - g);
                idx = (y + FB_SLICE_HEIGHT * slice + yOffset) * FB_WIDTH * 2 + x * 2 + xOffset * 2;
                framebuffer[idx + FB_WIDTH * 2] = (PIN(y2 + r) & 0b11111000) | (y2g >> 5);
                framebuffer[idx + FB_WIDTH * 2 + 1] = ((PIN(y2 + b) >> 3) & 0b00011111) | ((y2g << 3) & 0b11100000);


                uint8_t y3g = PIN(y3 - g);
                idx = (y + FB_SLICE_HEIGHT * slice + yOffset) * FB_WIDTH * 2 + x * 2 + xOffset * 2;
                framebuffer[idx + FB_WIDTH * 2 + 2] = (PIN(y3 + r) & 0b11111000) | (y3g >> 5);
                framebuffer[idx + FB_WIDTH * 2 + 3] = ((PIN(y3 + b) >> 3) & 0b00011111) | ((y3g << 3) & 0b11100000);
            }
        }
    }

    while(!spiTransmitComplete) { };

    drawScreen();

    while (((DWT->CYCCNT) / 1680) < (targetFrameTime + frameTimeOffset)) { };

    DWT->CYCCNT = 0;
}

void drawScreen()
{
    ST7735_SetAddressWindow(0, 0, 160, 128);
    HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit_DMA(&hspi1, framebuffer, sizeof(framebuffer));
}

void audioCallback(const uint8_t* payload, int size, int64_t pts, int isExpected)
{
    int bytes;
    mp3dec_frame_info_t info;

    {
        memcpy((void*)audiobuffer, (const void*) &audiobuffer[audiobufferPos], audiobufferSize);
        audiobufferPos = 0;

        bytes = std::min(MAX_BUFSIZE - audiobufferSize, size);

        memcpy((void*) &audiobuffer[audiobufferSize], payload, bytes);
        if (bytes > 0) {
            audiobufferSize += bytes;
        } else {
            eof = 1;
        }
    }

    if (eof || audiobufferSize >= MAX_L3_FRAME_PAYLOAD_BYTES * 2)
    {
        int readSamples = (int)mp3dec_decode_frame(&mp3, &audiobuffer[audiobufferPos], audiobufferSize, (mp3d_sample_t*)samples, &info);
        bytes = info.frame_bytes;

        if ((bytes < 4) || (bytes > MAX_L3_FRAME_PAYLOAD_BYTES) || (bytes > audiobufferSize))
        {
            desync = bytes = 1;
        }
        else
        {
            int samplesRange = readSamples * info.channels;

            for (int i = 0; i < samplesRange; i += info.channels)
                samples_DMA[samplesCounter++] = ((uint16_t)(((int32_t)samples[i] >> 1) + 32768));

            if(audioPlaying)
            {
                uint32_t dmaPos = dmaLoopsCompleted * DMA_BUFFER_SIZE + (DMA_BUFFER_SIZE - DMA1_Stream5->NDTR);
                uint32_t decPos = decoderLoopsCompleted * DMA_BUFFER_SIZE + samplesCounter;

                int diff = decPos - dmaPos - DMA_BUFFER_SIZE + MINIMP3_MAX_SAMPLES_PER_FRAME/2;

                if(diff > 200 && frameTimeOffset < 30)
                    frameTimeOffset += (diff / 200);
                else if(diff < -200 && frameTimeOffset > -30)
                    frameTimeOffset += (diff / 200);

                while (dmaLoopsCompleted > 0 && decoderLoopsCompleted > 0)
                {
                    dmaLoopsCompleted--;
                    decoderLoopsCompleted--;
                }
            }

            if(samplesCounter >= DMA_BUFFER_SIZE)
            {
                if(!audioPlaying)
                {
                    TIM6->ARR = (84000000 / 5) /  info.hz;
                    HAL_TIM_Base_Start(&htim6);
                    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)samples_DMA, samplesCounter, DAC_ALIGN_12B_L);
                    audioPlaying = 1;
                }

                samplesCounter = 0;
                decoderLoopsCompleted++;
            }


            if (desync) {
                desync = 0;
            }
        }
        audiobufferSize -= bytes;
        audiobufferPos += bytes;
    }
}

void ioErrorCallback(FRESULT errorCode)
{
    char* str = (char*)ioErrorsText[errorCode];

    memset(framebuffer, 0xFF, 160*128*2);
    ST7735_DrawImage_FB(framebuffer, 48, 24, 64, 64, img_micro_sd);
    ST7735_DrawImage_FB(framebuffer, 72, 92, 16, 16, img_close);
    ST7735_WriteString_FB(framebuffer, 80 - (strlen(str)*7)/2, 114, str, Font_7x10, ST7735_BLACK, ST7735_WHITE);
    drawScreen();

    HAL_TIM_Base_Stop(&htim6);

    while (true) {};
}
