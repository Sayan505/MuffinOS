#include <sys/video/video.h>


void init_video(stiletto_video_t stiletto_video) {
    video.pFrame_buffer_base = stiletto_video.pFrame_buffer_base;
    video.frame_buffer_sz    = stiletto_video.frame_buffer_sz;

    video.horiz              = stiletto_video.horiz;
    video.vert               = stiletto_video.vert;
    
    video.ppsl               = stiletto_video.ppsl;

    color = 0xFFFFFFFF;
}
