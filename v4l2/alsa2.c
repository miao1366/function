#include <alsa/asoundlib.h>
#include <math.h>
#include <inttypes.h>

int main(int argc, char **argv)
{
    long loops;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;
    unsigned int val;
    int rc;
    int size;
    int dir;
    char *buffer;

    /* Open PCM device for playback. */
    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
        exit(1);
    }

    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(handle, params);

    /* Interleaved mode */
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    /* Signed 16-bit format */
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16);
    /* Two channels (stereo) */
    snd_pcm_hw_params_set_channels(handle, params, 2);
    /* 16000 samples/second sampling rate */
    val = 16000;
    snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);
    /* Set period size to 32 frames. */
    frames = 32;
    snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

    /* Write the parameters to the driver */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
        exit(1);
    }

    /* Use a buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);

    size = frames * 2 * 2; /* 2 bytes/sample, 2 channels */

    buffer = (char *) malloc(size);
    if(buffer == NULL) {
        fprintf(stderr, "Not enough Memory!\n");
        exit(1);
    }

    /* We want to loop for 15 seconds */
    snd_pcm_hw_params_get_period_time(params, &val, &dir);

    /* 15 seconds in microseconds divided by period time */
    loops = 15000000 / val;

    for (size_t i = 0; i < size; i += 4) {
        // Generate a 500Hz tone
        *((int16_t *)(buffer + i)) = (uint16_t)(16384.0*sin(i*2.0*M_PI/size));
        *((int16_t *)(buffer + i + 2)) = (uint16_t)(16384.0*sin(i*2.0*M_PI/size));
        // Generate a 2kHz tone
        *((int16_t *)(buffer + i)) += (uint16_t)(16384.0*sin(i*8.0*M_PI/size));
        *((int16_t *)(buffer + i + 2)) += (uint16_t)(16384.0*sin(i*8.0*M_PI/size));
    }

    while (loops > 0) {
        loops--;
        //rc = read(0, buffer, size);
        //if (rc == 0) {
        //    fprintf(stderr, "end of file on input\n");
        //    break;
        //} else if (rc != size) {
        //    fprintf(stderr, "short read: read %d bytes\n", rc);
        //}

        rc = snd_pcm_writei(handle, buffer, frames);
        if (rc == -EPIPE) {
            /* EPIPE means underrun */
            fprintf(stderr, "underrun occurred\n");
            snd_pcm_prepare(handle);
        } else if (rc < 0) {
            fprintf(stderr,"error from writei: %s\n", snd_strerror(rc));
        } else if (rc != (int)frames) {
            fprintf(stderr,"short write, write %d frames\n", rc);
        }
    }
    snd_pcm_drop(handle);
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(buffer);
    return 0;
}