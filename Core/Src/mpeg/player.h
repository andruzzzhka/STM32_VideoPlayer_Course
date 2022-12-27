//https://github.com/rossumur/espflix

#ifndef player_hpp
#define player_hpp

#include "stdint.h"
#include <queue>
#include "fatfs.h"

#define FB_WIDTH 160
#define FB_HEIGHT 128
#define FB_STRIDE (FB_WIDTH*3/2)

#define FB_SLICE_HEIGHT 16
#define FB_SLICES (FB_HEIGHT/FB_SLICE_HEIGHT)


uint8_t clamp(int n);

//========================================================================================
//========================================================================================
// MPEG buffers
// 1) we only have 2 so no b frames
// 2) They are in 16 pixel high strips
// 3) The are only accessed in 32 bit wide chunks
// Frame Buffer is for a 160x128 display
// FB_SLICE_HEIGHTx(FB_WIDTH*3/2) byte chunks
// YYYYUU
// YYYYVV

class Buffer {
public:
    uint32_t len;
    uint8_t data[16*188];
};

class Frame {
public:
    uint8_t* _slices[FB_SLICES];
    void init();
    uint8_t* get_y(int y);
    uint8_t* get_cr(int y);
    uint8_t* get_cb(int y);
    void erase();

protected:
    static uint32_t _memCounter;
};

// integrated transport demux/MPEG decoder
class MpegDecoder
{
public:
    Frame* _fb[2];
    int _fb_index;
    Frame* _reference;  // last frame we decoded, currently being displayed
    Frame* _current;    // frame we are currently drawing into
    int64_t _pts;
    int64_t _last_pts;
    int64_t _audio_pts;
    int _audio_expected;
    int _audio_mark;

    // bitstream
    uint32_t _b = 0;
    int _b_count = 0;
    const uint8_t* _data;
    const uint8_t* _end;
    int _mark = 0;
    Buffer* _buffer = 0;

    // sequence
    int horizontal_size;
    int vertical_size;
    int pel_aspect_ratio;
    int picture_rate;
    int bit_rate;
    int quantizer_scale;

    FIL* _videoFile;

    void (*renderCallback)(Frame* frame);
    void (*audioDecodeCallback)(const uint8_t* payload, int size, int64_t pts, int isExpected);
    void (*ioErrorCallback)(FRESULT errorCode);

    void flush_picture(int mode = 0);

    enum {
        PICTURE = 0x00,
        SLICE_FIRST = 0x01,
        SLICE_LAST = 0xAF,
        USER_DATA = 0xB2,
        SEQUENCE_START = 0xB3,
        EXTENSION = 0xB5,
        SEQUENCE_END = 0xB7,
        GROUP = 0xB8
    };

    enum {
        I_FRAME = 1,
        P_FRAME = 2,
        B_FRAME = 3,
        D_FRAME = 4
    };

    MpegDecoder(FIL* videoFile, Frame* fb0, Frame* fb1);
    void    reset();
    void    run();
    int64_t get_pts();

protected:
    int     demux(int pid, const uint8_t* d, const uint8_t* end, int payload_unit_start);

    uint8_t more();
    inline int get_bits(int n);
    inline int peek_bits(int n);
    inline int get_bit();
    inline int get_vlc(const uint32_t* vlc);
    inline int get_vlc_dct();

    // gop timecode
    int drop_frame;
    int hours;
    int minutes;
    int seconds;
    int pictures;

    // picture
    int picture_coding_type;
    int full_pel_forward;
    int forward_r_size;

    // macroblocks
    int mb_width;
    int mb_height;
    int mb_size;
    int mb_x;
    int mb_y;

    uint8_t* y_addr;
    uint8_t* cr_addr;
    uint8_t* cb_addr;

    int y_dc;
    int cr_dc;
    int cb_dc;

    int forward_motion_h;
    int forward_motion_v;

    uint8_t intra_q[64];
    uint8_t non_intra_q[64];

    const uint8_t* read_matrix(uint8_t* dst);
    void sequence();
    void gop();
    void picture();

    void reset_predictors();

    // mb
    void mocomp(uint8_t* dst, int pos_x, int pos_y, int size, int c = 0);
    void inc_mb(int n = 1);
    void blit(uint8_t* dst, uint8_t* src, int size = 16);
    void predict_zero();
    void predict();
    int motion_vector(int m, int r_size);
    void motion_vectors(bool fw);

    // 8x8
    void idct(int* b, int n);
    int block(int block, bool intra);
    void copy_block(uint8_t* dst, int* b);
    void copy_block_dc(uint8_t* dst, int dc);
    void add_block(uint8_t* dst, int* b);
    void add_block_dc(uint8_t* dst, int dc);

    bool slice_done();
    int slice(int s);
    int marker(int m);
    void pause();
};

#endif /* player_hpp */
