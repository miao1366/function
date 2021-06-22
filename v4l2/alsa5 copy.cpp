#include <sys/types.h>  //open()
#include <sys/stat.h>   //open()
#include <fcntl.h>      //open()
#include <errno.h>      //errno
#include <string.h>     //strerror(), strncmp(), memcpy(), memset()
#include <stdio.h>      //printf()
#include <stdlib.h>     //malloc(), free()
#include <alsa/asoundlib.h>  //snd_pcm_*()


#define LOG(fmt, args...) printf(fmt ". file:%s line:%d.\n",  ##args, __FILE__, __LINE__)


// https://www.sohu.com/a/421421974_100109711
#define WAVE_DEVICE_NAME_MAX_LEN  32
#define WAVE_FILE_NAME_MAX_LEN    64

typedef struct  {
    int16_t             iNumChannels;
    unsigned int        uiSampleRate;
    int16_t             iBitsPerSample;
    int                 iWafeFd;
    int                 iCapturePeriod;
    int                 iPeriodFrame;
    snd_pcm_access_t    eSndPcmAccessType;
    snd_pcm_stream_t    eSndPcmStreamType;
    char                pcDeviceName[WAVE_DEVICE_NAME_MAX_LEN + 1];
    char                pcFileName[WAVE_FILE_NAME_MAX_LEN + 1];
    snd_pcm_t           *pstSndPcmHandle;
    snd_pcm_hw_params_t *pHwParams;
}WaveHead;


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

// #define WAVE_LITTLE_ENDIAN(data,size) do {       \
//         char *pcData = (char*)(data);            \
//         int iLoopNums = size / 2;                \
//         char cTemp = 0;                          \
//         for (int i = 0; i < iLoopNums; i++) {    \
//             cTemp = pcData[i];                   \
//             pcData[i] = pcData[size-i-1];        \
//             pcData[size-i-1] = cTemp;            \
//         }                                        \
//     } while(0);

#define WAVE_LITTLE_ENDIAN(data,size)

static inline void wave_little_endian(char *pcData, size_t size) {
    int iLoopNums = size / 2;
    char cTemp = 0;
    for (int i = 0; i < iLoopNums; i++) {
        cTemp = pcData[i];
        pcData[i] = pcData[size-i-1];
        pcData[size-i-1] = cTemp;
    }
}

WaveHead* wave_new_inst(const char*pcWaveFile) {
    if (!pcWaveFile) {
        return NULL;
    }
    WaveHead *pstWaveHead = (WaveHead*)malloc(sizeof(WaveHead));
    memset(pstWaveHead, 0, sizeof(WaveHead));
    pstWaveHead->iWafeFd = -1;
    pstWaveHead->iCapturePeriod = 50;
    pstWaveHead->eSndPcmAccessType = SND_PCM_ACCESS_RW_INTERLEAVED;
    pstWaveHead->eSndPcmStreamType = SND_PCM_STREAM_PLAYBACK;
    strcpy(pstWaveHead->pcDeviceName, WAVE_DEFAULT_DEVICE);
    strcpy(pstWaveHead->pcFileName, pcWaveFile);
    return pstWaveHead;
}
int wave_file_check(WaveHead* pstWaveHead) {
    if(!pstWaveHead) {
        return -1;
    }
    int iFd = -1;
    if (-1 == (iFd =open(pstWaveHead->pcFileName, O_RDONLY))) {
        LOG( "open file failed. %s",strerror(errno));
        return -1;
    }
    pstWaveHead->iWafeFd = iFd;
    char pcFileHead[WAVE_HEAD_BIT_SIZE + 1] = {0};
    if ((WAVE_HEAD_BIT_SIZE) != read(iFd, pcFileHead, WAVE_HEAD_BIT_SIZE)) {
        LOG("read too small data");
        return -1;
    }
    int iHeadPos = 0;
    
    if (strncmp(pcFileHead + iHeadPos, WAVE_STR_RIFF, WAVE_CHUNK_ID_BIT_SIZE)) {
        LOG("illegal wave file head. head:%s", pcFileHead);
        return -1;
    }
    iHeadPos += WAVE_CHUNK_ID_BIT_SIZE;
    int iChunSize = 0;
    memcpy(&iChunSize, pcFileHead + iHeadPos, WAVE_CHUNK_SIZE_BIT_SIZE);
    LOG("chun size:%d",iChunSize);

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
    WAVE_LITTLE_ENDIAN(&iSubChunk1Size, WAVE_SUB_CHUNK1_SIZE_BIT_SIZE)
    if (!(WAVE_SUB_CHUNK1_SIZE_16 == iSubChunk1Size || iSubChunk1Size == WAVE_SUB_CHUNK1_SIZE_18)) {
        LOG("illegal sub chunk size. size:%d", iSubChunk1Size);
        return -1;
    }

    iHeadPos += WAVE_SUB_CHUNK1_SIZE_BIT_SIZE;
    iHeadPos += WAVE_AUDIO_FORMAT_BIT_SIZE;
    int16_t iNumChannels = 0;
    memcpy(&iNumChannels, pcFileHead + iHeadPos, WAVE_NUM_CHANNELS_BIT_SIZE);
    WAVE_LITTLE_ENDIAN(&iNumChannels, WAVE_NUM_CHANNELS_BIT_SIZE);

    iHeadPos += WAVE_NUM_CHANNELS_BIT_SIZE;
    unsigned int uiSampleRate = 0;
    memcpy(&uiSampleRate, pcFileHead + iHeadPos, WAVE_SAMPLE_RATE_BIT_SIZE);
    WAVE_LITTLE_ENDIAN(&iNumChannels, WAVE_SAMPLE_RATE_BIT_SIZE);

    iHeadPos += WAVE_SAMPLE_RATE_BIT_SIZE;
    iHeadPos += WAVE_BYTE_RATE_BIT_SIZE;
    iHeadPos += WAVE_BLOCK_ALIGN_BIT_SIZE;
    int16_t iBitsPerSample = 0;
    memcpy(&iBitsPerSample, pcFileHead + iHeadPos, WAVE_BITS_PER_SAMPLE_BIT_SIZE);
    WAVE_LITTLE_ENDIAN(&iBitsPerSample, WAVE_BITS_PER_SAMPLE_BIT_SIZE);
    if(!(iBitsPerSample == WAVE_PCM_FORMAT_8 || iBitsPerSample == WAVE_PCM_FORMAT_16 
            || iBitsPerSample == WAVE_PCM_FORMAT_32)) {
        LOG("illegal bit per sample. value:%d", iBitsPerSample);
        return -1;
    }

    iHeadPos += WAVE_BITS_PER_SAMPLE_BIT_SIZE;
    if (WAVE_SUB_CHUNK1_SIZE_18 == iSubChunk1Size) {
        iHeadPos += WAVE_ADDITION_INFO_SIZE;
    }

    if (!(strncmp(pcFileHead + iHeadPos, WAVE_STR_FACT, WAVE_SUB_CHUNK2_ID_BIT_SIZE) == 0 ||
                strncmp(pcFileHead + iHeadPos, WAVE_STR_DATA, WAVE_SUB_CHUNK2_ID_BIT_SIZE) == 0)) {
        LOG("illegal wave file head. head:%s", pcFileHead);
        return -1;
    }
    
    
    pstWaveHead->iNumChannels = iNumChannels;
    pstWaveHead->uiSampleRate = uiSampleRate;
    pstWaveHead->iBitsPerSample = iBitsPerSample;
    pstWaveHead->iPeriodFrame = pstWaveHead->uiSampleRate * pstWaveHead-> iCapturePeriod / 1000;
    LOG("%d:%u:%d:%d", iNumChannels, uiSampleRate, iBitsPerSample, pstWaveHead->iPeriodFrame);
    return 0;
}

int wave_prepare(WaveHead* pstWaveHead) {
    snd_pcm_format_t format;
    snd_pcm_hw_params_t *hw_params = NULL;
    int iSndRet = 0;
    if ((iSndRet = snd_pcm_open(&pstWaveHead->pstSndPcmHandle, pstWaveHead->pcDeviceName, 
            pstWaveHead->eSndPcmStreamType, 0)) < 0) {
        LOG("snd_pcm_open() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }

    if ((iSndRet = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        LOG("snd_pcm_hw_params_malloc() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }
    pHwParams
    if ((iSndRet = snd_pcm_hw_params_any(pstWaveHead->pstSndPcmHandle,hw_params)) < 0) {
        LOG("snd_pcm_hw_params_any() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }

    if ((iSndRet = snd_pcm_hw_params_set_access(pstWaveHead->pstSndPcmHandle, hw_params, pstWaveHead->eSndPcmAccessType)) < 0) {
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

    if ((iSndRet = snd_pcm_hw_params_set_format(pstWaveHead->pstSndPcmHandle, hw_params, format)) < 0) {
        LOG("snd_pcm_hw_params_set_format() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }

    if ((iSndRet = snd_pcm_hw_params_set_rate_near(pstWaveHead->pstSndPcmHandle, hw_params, &(pstWaveHead->uiSampleRate), 0)) < 0) {
        LOG("snd_pcm_hw_params_set_rate_near() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }

    if ((iSndRet = snd_pcm_hw_params_set_channels(pstWaveHead->pstSndPcmHandle, hw_params, pstWaveHead->iNumChannels)) < 0) {
        LOG("snd_pcm_hw_params_set_channels() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }

    snd_pcm_uframes_t periodFrame = static_cast<snd_pcm_uframes_t>(pstWaveHead->iPeriodFrame);
    int iDir=0;
    if ((iSndRet = snd_pcm_hw_params_set_period_size_near(pstWaveHead->pstSndPcmHandle, hw_params, &periodFrame, &iDir)) < 0) {
        LOG("snd_pcm_hw_params_set_period_size_near() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }

    if ((iSndRet = snd_pcm_hw_params(pstWaveHead->pstSndPcmHandle, hw_params)) < 0) {
        LOG("snd_pcm_hw_params() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }
    snd_pcm_uframes_t tt;
    snd_pcm_hw_params_get_period_size(hw_params, &tt,&iDir);
    LOG("tt::%ld", tt);
    snd_pcm_hw_params_free(hw_params);

    if ((iSndRet = snd_pcm_prepare(pstWaveHead->pstSndPcmHandle)) < 0) {
        LOG("snd_pcm_prepare() failed. reason:%s.", snd_strerror(iSndRet));
        return -1;
    }
    return 0;
}

int wave_play(const WaveHead *pstWaveHead) {
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
    LOG("frame:%ld", snd_pcm_avail(pstWaveHead->pstSndPcmHandle));
    while(1) {
        // if (snd_pcm_avail(pstWaveHead->pstSndPcmHandle) < frams) {
        //     continue;
        // }
        memset(pcBuff, 0, iAudioDataSize);
        iReadSize = read(pstWaveHead->iWafeFd, pcBuff, iAudioDataSize);
        if (iReadSize <= 0) {
            break;
        }
        if ((iResult = snd_pcm_writei(pstWaveHead->pstSndPcmHandle, pcBuff, iReadSize/iBytePerFrame)) < 0) {
            LOG("snd_pcm_writei() failed. reason:%s.", snd_strerror(iResult));
        }
        //usleep(1000);
    }
    // while(snd_pcm_avail(pstWaveHead->pstSndPcmHandle)) {
    //     usleep(1000);
    // }
    while (SND_PCM_STATE_XRUN == snd_pcm_state(pstWaveHead->pstSndPcmHandle)) {
        usleep(1000);
    }
    
    //sleep(20);
    return 0;
}

void wave_close(const WaveHead *pstWaveHead) {
    if (-1 != pstWaveHead->iWafeFd) {
        close(pstWaveHead->iWafeFd);
    }
    if (pstWaveHead->pstSndPcmHandle) {
        snd_pcm_close(pstWaveHead->pstSndPcmHandle);
    }
}

int main(int argc, char ** argv) {        
    WaveHead *pstWaveHead = wave_new_inst(argv[1]);
    wave_file_check(pstWaveHead);
    wave_prepare(pstWaveHead);
    wave_play(pstWaveHead);
    return 0;
}