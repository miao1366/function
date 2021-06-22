#include <sys/types.h>        //open()
#include <sys/stat.h>         //open()
#include <fcntl.h>            //open()
#include <errno.h>            //errno
#include <string.h>           //strerror(), strncmp(), memcpy(), memset(), strlen()
#include <stdio.h>            //printf()
#include <stdlib.h>           //malloc(), free()

#include <alsa/asoundlib.h>   //snd_pcm_*()


#define LOG(fmt, args...) printf(fmt ". file:%s line:%d.\n",  ##args, __FILE__, __LINE__)


#define WAVE_DEVICE_NAME_MAX_LEN  32
#define WAVE_FILE_NAME_MAX_LEN    64



typedef struct  {
    int16_t             iNumChannels;
    unsigned int        uiSampleRate;
    int16_t             iBitsPerSample;
    int                 iFd;
    int                 iCapturePeriod;
    int                 iPeriodFrame;
    snd_pcm_access_t    eSndPcmAccessType;
    snd_pcm_stream_t    eSndPcmStreamType;
    char                pcDeviceName[WAVE_DEVICE_NAME_MAX_LEN + 1];
    char                pcFileName[WAVE_FILE_NAME_MAX_LEN + 1];
    snd_pcm_t           *pstSndPcmHandle;
    snd_pcm_hw_params_t *pHwParams;
}WaveHead;

// https://www.sohu.com/a/421421974_100109711

#define WAVE_HEAD_BIT_SIZE               44
#define WAVE_CHUNK_ID_BIT_SIZE           4
#define WAVE_CHUNK_SIZE_BIT_SIZE         4
#define WAVE_FORMAT_BIT_SIZE             4
#define WAVE_SUB_CHUNK1_ID_BIT_SIZE      4
#define WAVE_SUB_CHUNK1_SIZE_BIT_SIZE    4
#define WAVE_AUDIO_FORMAT_BIT_SIZE       2
#define WAVE_NUM_CHANNELS_BIT_SIZE       2
#define WAVE_SAMPLE_RATE_BIT_SIZE        4
#define WAVE_BYTE_RATE_BIT_SIZE          4
#define WAVE_BLOCK_ALIGN_BIT_SIZE        2
#define WAVE_BITS_PER_SAMPLE_BIT_SIZE    2
#define WAVE_SUB_CHUNK2_ID_BIT_SIZE      4
#define WAVE_SUB_CHUNK2_SIZE_BIT_SIZE    4

#define WAVE_SUB_CHUNK1_SIZE_16          16
#define WAVE_SUB_CHUNK1_SIZE_18          18

#define WAVE_ADDITION_INFO_SIZE          2

#define WAVE_PCM_FORMAT_8                8
#define WAVE_PCM_FORMAT_16               16
#define WAVE_PCM_FORMAT_32               32

#define WAVE_STR_RIFF    "RIFF"
#define WAVE_STR_WAVE    "WAVE"
#define WAVE_STR_FMT     "fmt "
#define WAVE_STR_FACT    "fact"
#define WAVE_STR_DATA    "data"

#define WAVE_DEFAULT_DEVICE "default"

#define MIN(a, b)  (a) < (b) ? (a):(b)


WaveHead* wave_new_inst(const char*pcWaveFile) {
    if (!pcWaveFile || !strlen(pcWaveFile)) {
        return NULL;
    }
    WaveHead *pstWaveHead = (WaveHead*)malloc(sizeof(WaveHead));
    memset(pstWaveHead, 0, sizeof(WaveHead));
    pstWaveHead->iFd = -1;
    pstWaveHead->iCapturePeriod = 50;
    pstWaveHead->eSndPcmAccessType = SND_PCM_ACCESS_RW_INTERLEAVED;
    pstWaveHead->eSndPcmStreamType = SND_PCM_STREAM_PLAYBACK;
    strncpy(pstWaveHead->pcDeviceName, WAVE_DEFAULT_DEVICE, MIN(strlen(WAVE_DEFAULT_DEVICE), WAVE_DEVICE_NAME_MAX_LEN));
    strncpy(pstWaveHead->pcFileName, pcWaveFile, MIN(strlen(pcWaveFile), WAVE_FILE_NAME_MAX_LEN));
    return pstWaveHead;
}

int wave_file_check(WaveHead* pstWaveHead) {
    int iHeadPos = 0;
    
    if(!pstWaveHead) {
        return -1;
    }
    if (-1 == (pstWaveHead ->iFd = open(pstWaveHead->pcFileName, O_RDONLY))) {
        LOG( "open file failed. %s",strerror(errno));
        return -1;
    }

    char pcFileHead[WAVE_HEAD_BIT_SIZE + WAVE_ADDITION_INFO_SIZE + 1] = {0};
    if ((WAVE_HEAD_BIT_SIZE) != read(pstWaveHead->iFd, pcFileHead, WAVE_HEAD_BIT_SIZE)) {
        LOG("read data failed");
        return -1;
    }
    
    if (strncmp(pcFileHead + iHeadPos, WAVE_STR_RIFF, WAVE_CHUNK_ID_BIT_SIZE)) {
        LOG("illegal wave file head. head:%s", pcFileHead);
        return -1;
    }
    iHeadPos += WAVE_CHUNK_ID_BIT_SIZE;
    iHeadPos += WAVE_CHUNK_SIZE_BIT_SIZE;

    if (strncmp(pcFileHead + iHeadPos, WAVE_STR_WAVE, WAVE_FORMAT_BIT_SIZE)) {
        LOG("illegal wave file head. head:%s", pcFileHead);
        return -1;
    }

    iHeadPos += WAVE_FORMAT_BIT_SIZE;
    if (strncmp(pcFileHead + iHeadPos, WAVE_STR_FMT, WAVE_SUB_CHUNK1_ID_BIT_SIZE)) {
        LOG("illegal wave file head. head:%s", pcFileHead);
        return -1;
    }
    iHeadPos += WAVE_SUB_CHUNK1_ID_BIT_SIZE;
    int iSubChunk1Size = 0;
    memcpy(&iSubChunk1Size, pcFileHead + iHeadPos, WAVE_SUB_CHUNK1_SIZE_BIT_SIZE);
    if (!(WAVE_SUB_CHUNK1_SIZE_16 == iSubChunk1Size || WAVE_SUB_CHUNK1_SIZE_18 == iSubChunk1Size)) {
        LOG("illegal sub chunk size. size:%d", iSubChunk1Size);
        return -1;
    }

    iHeadPos += WAVE_SUB_CHUNK1_SIZE_BIT_SIZE;
    iHeadPos += WAVE_AUDIO_FORMAT_BIT_SIZE;
    int16_t iNumChannels = 0;
    memcpy(&iNumChannels, pcFileHead + iHeadPos, WAVE_NUM_CHANNELS_BIT_SIZE);

    iHeadPos += WAVE_NUM_CHANNELS_BIT_SIZE;
    unsigned int uiSampleRate = 0;
    memcpy(&uiSampleRate, pcFileHead + iHeadPos, WAVE_SAMPLE_RATE_BIT_SIZE);

    iHeadPos += WAVE_SAMPLE_RATE_BIT_SIZE;
    iHeadPos += WAVE_BYTE_RATE_BIT_SIZE;
    iHeadPos += WAVE_BLOCK_ALIGN_BIT_SIZE;
    int16_t iBitsPerSample = 0;
    memcpy(&iBitsPerSample, pcFileHead + iHeadPos, WAVE_BITS_PER_SAMPLE_BIT_SIZE);
    if(!(WAVE_PCM_FORMAT_8 == iBitsPerSample || WAVE_PCM_FORMAT_16 == iBitsPerSample 
            || WAVE_PCM_FORMAT_32 ==  iBitsPerSample)) {
        LOG("illegal bit per sample. value:%d", iBitsPerSample);
        return -1;
    }

    iHeadPos += WAVE_BITS_PER_SAMPLE_BIT_SIZE;

    if (WAVE_SUB_CHUNK1_SIZE_18 == iSubChunk1Size) {
        if ((WAVE_ADDITION_INFO_SIZE) != read(pstWaveHead->iFd, pcFileHead + WAVE_HEAD_BIT_SIZE, WAVE_ADDITION_INFO_SIZE)) {
        LOG("read data failed");
        return -1;
    }
        iHeadPos += WAVE_ADDITION_INFO_SIZE;
        
    }

    if (!(strncmp(pcFileHead + iHeadPos, WAVE_STR_FACT, WAVE_SUB_CHUNK2_ID_BIT_SIZE) == 0 ||
                strncmp(pcFileHead + iHeadPos, WAVE_STR_DATA, WAVE_SUB_CHUNK2_ID_BIT_SIZE) == 0)) {
        LOG("illegal wave file head. head:%s", pcFileHead);
        //return -1;
    }
    
    pstWaveHead->iNumChannels = iNumChannels;
    pstWaveHead->uiSampleRate = uiSampleRate;
    pstWaveHead->iBitsPerSample = iBitsPerSample;
    pstWaveHead->iPeriodFrame = pstWaveHead->uiSampleRate * pstWaveHead-> iCapturePeriod / 1000;
    LOG("iNumChannels:%d :uiSampleRate:%u:  iBitsPerSample%d:  iPeriodFrame%d", iNumChannels, uiSampleRate, iBitsPerSample, pstWaveHead->iPeriodFrame);
    return 0;
}

int wave_prepare(WaveHead* pstWaveHead) {
    snd_pcm_format_t format;
    snd_pcm_hw_params_t *pHwParams = NULL;
    int iSndRet = 0;
    if ((iSndRet = snd_pcm_open(&pstWaveHead->pstSndPcmHandle, pstWaveHead->pcDeviceName, 
            pstWaveHead->eSndPcmStreamType, 0)) < 0) {
        LOG("snd_pcm_open() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }

    if ((iSndRet = snd_pcm_hw_params_malloc(&pHwParams)) < 0) {
        LOG("snd_pcm_hw_params_malloc() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }
    pstWaveHead -> pHwParams = pHwParams;
    if ((iSndRet = snd_pcm_hw_params_any(pstWaveHead->pstSndPcmHandle,pHwParams)) < 0) {
        LOG("snd_pcm_hw_params_any() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }

    if ((iSndRet = snd_pcm_hw_params_set_access(pstWaveHead->pstSndPcmHandle, pHwParams, pstWaveHead->eSndPcmAccessType)) < 0) {
        LOG("snd_pcm_hw_params_set_access() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }

    switch(pstWaveHead->iBitsPerSample) {
        case WAVE_PCM_FORMAT_8 : {format = SND_PCM_FORMAT_U8; break;}
        case WAVE_PCM_FORMAT_16: {format = SND_PCM_FORMAT_S16_LE; break;}
        case WAVE_PCM_FORMAT_32: {format = SND_PCM_FORMAT_S32_LE; break;}
        default: {
            LOG("illegal iBitsPerSample. iBitsPerSample:%d.", pstWaveHead->iBitsPerSample);
            return -1;
        }
    }

    if ((iSndRet = snd_pcm_hw_params_set_format(pstWaveHead->pstSndPcmHandle, pHwParams, format)) < 0) {
        LOG("snd_pcm_hw_params_set_format() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }

    if ((iSndRet = snd_pcm_hw_params_set_rate_near(pstWaveHead->pstSndPcmHandle, pHwParams, &(pstWaveHead->uiSampleRate), 0)) < 0) {
        LOG("snd_pcm_hw_params_set_rate_near() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }

    if ((iSndRet = snd_pcm_hw_params_set_channels(pstWaveHead->pstSndPcmHandle, pHwParams, pstWaveHead->iNumChannels)) < 0) {
        LOG("snd_pcm_hw_params_set_channels() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }

    snd_pcm_uframes_t periodFrame = static_cast<snd_pcm_uframes_t>(pstWaveHead->iPeriodFrame);
    int iDir=0;
    if ((iSndRet = snd_pcm_hw_params_set_period_size_near(pstWaveHead->pstSndPcmHandle, pHwParams, &periodFrame, &iDir)) < 0) {
        LOG("snd_pcm_hw_params_set_period_size_near() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }

    if ((iSndRet = snd_pcm_hw_params(pstWaveHead->pstSndPcmHandle, pHwParams)) < 0) {
        LOG("snd_pcm_hw_params() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }
    if ((iSndRet = snd_pcm_prepare(pstWaveHead->pstSndPcmHandle)) < 0) {
        LOG("snd_pcm_prepare() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }
    return 0;
}

int wave_play(const WaveHead *pstWaveHead) {
    if (!pstWaveHead) {
        return -1;
    }
    int iResult;
    snd_pcm_uframes_t frams = pstWaveHead->iPeriodFrame;
    int iBytePerFrame = pstWaveHead->iNumChannels * pstWaveHead->iBitsPerSample / 8;
    int iAudioDataSize = frams * iBytePerFrame;
    char *pcBuff = (char*)(malloc(iAudioDataSize));
    if (NULL == pcBuff) {
        LOG("malloc() failed.");
        return -1;
    }
    
    int iReadSize = 0;
    int i = 0;
    while(1) {
        memset(pcBuff, 0, iAudioDataSize);
        iReadSize = read(pstWaveHead->iFd, pcBuff, iAudioDataSize);
        if (iReadSize != iAudioDataSize) {
            LOG("malloc() failed.");
            break;
        }
        if ((iResult = snd_pcm_writei(pstWaveHead->pstSndPcmHandle, pcBuff, iReadSize/iBytePerFrame)) < 0) {
            LOG("snd_pcm_writei() failed. reason:%s.", snd_strerror(iResult));
            if(iResult == -EPIPE || iResult == -EINTR) {
                snd_pcm_prepare(pstWaveHead->pstSndPcmHandle);
            }
        }
       // LOG("snd_pcm_writei() while(i). i:%d.", ++i);

    }
    free(pcBuff);
    return 0;
}

void wave_close(const WaveHead *pstWaveHead) {
    if (!pstWaveHead) {
        return;
    }
    if (-1 != pstWaveHead->iFd) {
        close(pstWaveHead->iFd);
    }

    if (pstWaveHead->pHwParams) {
        snd_pcm_hw_params_free(pstWaveHead->pHwParams);
    }

    if (pstWaveHead->pstSndPcmHandle) {
        snd_pcm_drain(pstWaveHead->pstSndPcmHandle);
        snd_pcm_drop(pstWaveHead->pstSndPcmHandle);
        snd_pcm_close(pstWaveHead->pstSndPcmHandle);
    }
    free((void*)pstWaveHead);
}

int main(int argc, char ** argv) {        
    WaveHead *pstWaveHead = wave_new_inst(argv[1]);
    wave_file_check(pstWaveHead);
    wave_prepare(pstWaveHead);
    wave_play(pstWaveHead);
    wave_close(pstWaveHead);
    //sleep(20);
    return 1;
}