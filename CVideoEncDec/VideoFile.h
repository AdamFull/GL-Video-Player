#ifndef AV_VIDEOFILE
#define AV_VIDEOFILE

#include "DataStream.h"

typedef struct CVideoFile
{

    AVFormatContext* av_format_ctx;
    AVPacket* av_packet;

    bool hwdecoding_video;
    bool hwdecoding_audio;

    CDataStream* vstream;
    CDataStream* astream;

} CVideoFile;

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
CVideoFile* video_file_alloc(void);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool video_file_open_decode(CVideoFile**, const char*);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool video_file_open_encode(CVideoFile**, const char*);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool video_file_read_frame(CVideoFile**);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool video_file_write_frame(CVideoFile**);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool video_file_allow_hwdecoding_video(CVideoFile**);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool video_file_allow_hwdecoding_audio(CVideoFile**);

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool video_find_hwdecoder();

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool video_activate_hwdecoder();

/**
 * Initialize an CHardwareAccelerator as encoder.
 *
 * @param hwdec_ptr
 *
 * @return Returns true if all initialization got well.
 */
bool video_file_close(CVideoFile**);

#endif