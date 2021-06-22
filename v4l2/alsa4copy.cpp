/**
@file         TestAlsaPlayWave.cpp
@brief       This is a short example to play the audio wave, please define the path in the main func
@par          author: jlm
@par          pre env: alsa
@todo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sched.h>
#include <errno.h>
#include <getopt.h>
#include <iostream>
#include <alsa/asoundlib.h>
#include <sys/time.h>
#include <math.h>
using namespace std;

/*********Type definition***********************/
typedef unsigned char  uint8;
typedef unsigned short uint16;

typedef enum EBitsPerSample
{
    BITS_UNKNOWN = 0,
    BITS_PER_SAMPLE_8 = 8,
    BITS_PER_SAMPLE_16 = 16,
    BITS_PER_SAMPLE_32 = 32
}EBitsPerSample_t;

typedef enum ENumOfChannels
{
    NUM_OF_CHANNELS_1 = 1,
    NUM_OF_CHANNELS_2 = 2
}ENumOfChannels_t;

#if 0
/** PCM state */
typedef enum _snd_pcm_state {
    /** Open */
    SND_PCM_STATE_OPEN = 0,
    /** Setup installed */ 
    SND_PCM_STATE_SETUP,
    /** Ready to start */
    SND_PCM_STATE_PREPARED,
    /** Running */
    SND_PCM_STATE_RUNNING,
    /** Stopped: underrun (playback) or overrun (capture) detected */
    SND_PCM_STATE_XRUN,
    /** Draining: running (playback) or stopped (capture) */
    SND_PCM_STATE_DRAINING,
    /** Paused */
    SND_PCM_STATE_PAUSED,
    /** Hardware is suspended */
    SND_PCM_STATE_SUSPENDED,
    /** Hardware is disconnected */
    SND_PCM_STATE_DISCONNECTED,
    SND_PCM_STATE_LAST = SND_PCM_STATE_DISCONNECTED
} snd_pcm_state_t;
#endif

typedef struct ALSA_CONFIGURATION
{

   std::string alsaDevice;

   std::string friendlyName;

   /// Read: Buffer size should be large enough to prevent overrun (read / write buffer full)
   unsigned int alsaBufferSize;

   /// Chunk size should be smaller to prevent underrun (write buffer empty)
   unsigned int alsaPeriodFrame;

   unsigned int samplingFrequency;//48kHz

   EBitsPerSample bitsPerSample;//16

   ENumOfChannels numOfChannels;

   bool block; // false means nonblock

   snd_pcm_access_t accessType;

   snd_pcm_stream_t streamType; // Playback or capture

   unsigned int alsaCapturePeriod; // Length of each capture period
}Alsa_Conf;

typedef struct Wave_Header
{
    uint8 ChunkID[4];
    uint8 ChunkSize[4];
    uint8 Format[4];
    uint8 SubChunk1ID[4];
    uint8 SubChunk1Size[4];
    uint8 AudioFormat[2];
    uint8 NumChannels[2];
    uint8 SampleRate[4];
    uint8 ByteRate[4];
    uint8 BlockAlign[2];
    uint8 BitsPerSample[2];
    uint8 CombineWaveFileExtra2Bytes[2];
    uint8 SubChunk2ID[4];
    uint8 SubChunk2Size[4];
}Wave_Header_t;


typedef struct Wave_Header_Size_Info
{
    uint8 ChunkID[4];
    uint8 ChunkSize[4];
    uint8 Format[4];
    uint8 SubChunk1ID[4];
    uint8 SubChunk1Size[4];
}Wave_Header_Size_Info_t;

typedef struct Wave_Header_Audio_Info
{
    uint8 AudioFormat[2];
    uint8 NumChannels[2];
    uint8 SampleRate[4];
    uint8 ByteRate[4];
    uint8 BlockAlign[2];
    uint8 BitsPerSample[2];
}Wave_Header_Audio_Info_t;

typedef struct Wave_Header_Additional_Info
{
    uint8 AdditionalInfo_2Bytes[2]; //this depends on the SubChunk1Size,normal if SubChunk1Size=16 then match the normal wave format, if SubChunk1Size=18 then 2 more additional info bytes
}Wave_Header_Additional_Info_t;

typedef struct Wave_Header_Data_Info
{
    uint8 SubChunk2ID[4];
    uint8 SubChunk2Size[4];
}Wave_Header_Data_Info_t;



/*********Global Variable***********************/
snd_pcm_uframes_t g_frames; //just test purpose

/*********Func Declaration***********************/
void TestAlsaDevice(snd_pcm_t** phandler);
bool PrepareAlsaDevice(Alsa_Conf* palsaCfg,snd_pcm_t** phandler);
bool closeAlsaDevice(snd_pcm_t** phandler);
bool ParseWaveFile(const string wavepath,Alsa_Conf* palsaCfg);
uint64_t HandleLittleEndian(uint8* arr,int size);
bool PlayWave(FILE** fp,snd_pcm_t** phandler,Alsa_Conf* palsaCfg);

uint64_t HandleLittleEndian(uint8* arr,int size)
{
    uint64_t value=0;
    for(int i=0;i<size;i++)
    {
        //std::cout << arr[i] <<"  "<< (arr[i]<<(8*i))<<std::endl;
        value=value+(arr[i]<<(8*i));
    }
    return value;
}

#if 0
//this is the return value 
ChunkID = "RIFF"
ChunkSize = 54310
Format = "WAVE"
fmt = "fmt "
SubChunk1Size = 18
AudioFormat = 1
NumChannels = 2
SampleRate = 16000
ByteRate = 64000
BlockAlign = 4
BitsPerSample = 16
SubChunk2ID = "data"
SubChunk2Size = 54272
#endif 


//parse the wave file
bool ParseWaveFile(const string wavepath,Alsa_Conf* palsaCfg,FILE** fp)
{
    int ret=0;
    //FILE* fp=NULL;
    *fp=fopen(wavepath.c_str(),"rb");
    if(*fp==NULL)
    {
        cout<<"Can parse the wave file-->need check the file name"<<endl;
    }
    
    /*********************size info parse begin*************************/
    //read size info
    Wave_Header_Size_Info_t wav_size_info;    
    memset(&wav_size_info,0,sizeof(Wave_Header_Size_Info_t));
    ret=fread(&wav_size_info,sizeof(Wave_Header_Size_Info_t),1,*fp); 
    
    if(ret<1)
    {
        cout<<"read error"<<endl;
        return false;
    }
    string ChunkID="";
    for(int i=0;i<4;i++)
    {
        ChunkID+=wav_size_info.ChunkID[i];
    }
    string riff="RIFF";
    if(0!=strcmp(ChunkID.c_str(),riff.c_str()))
    {
        cout<<"Invalid the fist Chunk id"<<endl;
        return false;
    }
    
    uint64_t ChunkSize=HandleLittleEndian(wav_size_info.ChunkSize,4);
    cout<<"The ChunSize is "<<ChunkSize<<endl;
    
    string Format="";
    for(int i=0;i<4;i++)
    {
        Format+=wav_size_info.Format[i];
    }
        
    if(0!=strcmp(Format.c_str(),"WAVE"))
    {
        cout<<"Invalid the format"<<endl;
        return false;
    }

    string SubChunk1ID="";
    for(int i=0;i<4;i++)
    {
        SubChunk1ID+=wav_size_info.SubChunk1ID[i];
    }
    string fmt="fmt ";
    if(0!=strcmp(SubChunk1ID.c_str(),fmt.c_str()))
    {
        cout<<"Invalid the SubChunk1ID "<<endl;
        return false;
    }
    uint16 SubChunk1Size=HandleLittleEndian(wav_size_info.SubChunk1Size,4);
    
    if(SubChunk1Size!=16 && SubChunk1Size!=18)
    {
        cout<<"Invalid the SubChunk1Size"<<endl;
        return false;
    }
    /*********************Audio info parse begin*************************/
    Wave_Header_Audio_Info_t wav_audio_info;    
    memset(&wav_audio_info,0,sizeof(Wave_Header_Audio_Info_t));
    ret=fread(&wav_audio_info,sizeof(Wave_Header_Audio_Info_t),1,*fp); 
    if(ret<1)
    {
        cout<<"read error"<<endl;
        return false;
    }
    //fseek(fp,sizeof(Wave_Header_Size_Info_t),0);//文件指针偏移3个字节到'2' because fread will shift the pointer
    uint16 AudioFormat    =HandleLittleEndian(wav_audio_info.AudioFormat,2);
    
    uint16 NumChannels    =HandleLittleEndian(wav_audio_info.NumChannels,2);
    
    uint16 SampleRate    =HandleLittleEndian(wav_audio_info.SampleRate,4);
    
    uint16 ByteRate        =HandleLittleEndian(wav_audio_info.ByteRate,4);
    
    uint16 BlockAlign    =HandleLittleEndian(wav_audio_info.BlockAlign,2);
    
    uint16 BitsPerSample=HandleLittleEndian(wav_audio_info.BitsPerSample,2);


    palsaCfg->numOfChannels=(ENumOfChannels)NumChannels;
    palsaCfg->samplingFrequency=SampleRate;
    palsaCfg->bitsPerSample=(EBitsPerSample)BitsPerSample;
    
    /*********************Additional info parse begin if needed*************************/
    if(SubChunk1Size==18)
    {
        Wave_Header_Additional_Info_t wav_additional_info;    
        memset(&wav_additional_info,0,sizeof(Wave_Header_Additional_Info_t));
        fread(&wav_additional_info,sizeof(Wave_Header_Additional_Info_t),1,*fp);
        
        cout<<"read wav_additional_info"<<endl;
        if(ret<1)
        {
            cout<<"read error"<<endl;
            return false;
        }
        uint16 AdditionalInfo=HandleLittleEndian(wav_additional_info.AdditionalInfo_2Bytes,2);
        cout<<"read AdditionalInfo value="<<AdditionalInfo<<endl;

    }

    /*********************Data info parse begin *************************/
    Wave_Header_Data_Info_t    wave_data_info;
    memset(&wave_data_info,0,sizeof(Wave_Header_Data_Info_t));
    fread(&wave_data_info,sizeof(Wave_Header_Data_Info_t),1,*fp); 
    
    if(ret<1)
    {
        cout<<"read error"<<endl;
        return false;
    }
    string SubChunk2ID="";
    for(int i=0;i<4;i++)
    {
        SubChunk2ID+=wave_data_info.SubChunk2ID[i];
    }
    string fact="fact";
    string data="data";
    if(0==strcmp(SubChunk2ID.c_str(),fact.c_str()))
    {
        cout<<"SubChunk2ID fact"<<endl;
    }
    else if(0==strcmp(SubChunk2ID.c_str(),data.c_str()))
    {
        cout<<"SubChunk2ID data"<<endl;
    }
    else
    {
        cout<<"Invalid SubChunk2ID "<<endl;
        return false;
    }
    uint16 SubChunk2Size=HandleLittleEndian(wave_data_info.SubChunk2Size,4);

    
    cout<<"End Parse"<<endl;
    return true;
}

bool PlayWave(FILE** fp,snd_pcm_t** phandler,Alsa_Conf* palsaCfg)
{

    int err=0;
    bool ret=false;
    snd_pcm_uframes_t frames=palsaCfg->alsaPeriodFrame;
    int bytesPerFrame=(int)palsaCfg->numOfChannels*palsaCfg->bitsPerSample/8; //4bytes
    uint16 audio_data_size=frames*bytesPerFrame;//one period 10ms ,1600*10/1000*(2*16/8)=640bytes one period 
    uint8* buffer=new uint8[audio_data_size]; 
    cout<<"Start play wave"<<endl;

    if(*fp==NULL || *phandler==NULL || palsaCfg==NULL)
    {
        cout<<"End play wave because something is NULL"<<endl;
        return false;
    }
    //fseek(*fp,46,SEEK_SET);  //no need to do fseek because already shifted 
    cout<<"available frame "<<snd_pcm_avail(*phandler)<<"my frames is "<<frames<<endl;

    while(true)
    {
        if(feof(*fp))
        {
            cout<<"Reach end of the file"<<endl;
            break;
        }
        else
        {    
            if(snd_pcm_avail(*phandler)<frames)
            {
                continue;
            }
            else
            {
                memset(reinterpret_cast<void*>(buffer),0,sizeof(uint8)*audio_data_size);
                err=fread(buffer,sizeof(uint8)*audio_data_size,1,*fp);
                if(err=0)
                {
                    cout<<"read error"<<endl;
                }
                if ( NULL != buffer ) 
                {
                    err = snd_pcm_writei(*phandler, buffer, frames);
                    if (err < 0)
                    {
                        cout<<"Fail to write the audio data to ALSA. Reason: "<<(snd_strerror(err));
                        // recover ALSA device
                        err = snd_pcm_recover(*phandler, err, 0);
                        if (err < 0)
                        {
                            cout<<"Fail to recover ALSA device. Reason: "<<(snd_strerror(err));
                            ret = false;
                        }
                        else
                        {
                            cout<<"ALSA device is recovered from error state"<<endl;
                        }
                    }
                }
                else 
                {
                    cout<<"Write buffer is NULL!"<<endl;
                }
            }
        }
        //usleep(palsaCfg->alsaCapturePeriod / (2 * 1000));
    }
    delete[] buffer;
    buffer=NULL;
    return ret;
}
bool PrepareAlsaDevice(Alsa_Conf* palsaCfg,snd_pcm_t** phandler)
{
    bool ret=false;
    bool success=true;
    int error=0;    
    snd_pcm_format_t format;
    snd_pcm_hw_params_t *hw_params = NULL;
    int dir=0;
    if(palsaCfg!=NULL)
    {
        // open ALSA device
        error=snd_pcm_open(phandler,palsaCfg->alsaDevice.c_str(),palsaCfg->streamType,palsaCfg->block? 0:SND_PCM_NONBLOCK);
        if(error<0) //0 on success otherwise a negative error code
        {
            success=false;
            cout<<"Open Alsadevice error error code="<<snd_strerror(error)<<endl;
        }
        
        if(success)
        {
            //allocate hardware parameter structur
            error=snd_pcm_hw_params_malloc(&hw_params);//alsao can use  snd_pcm_hw_params_alloca(&hwparams)
            if(error<0)
            {
                success=false;
                hw_params=NULL;
                cout<<"Set hw params error error code="<<snd_strerror(error)<<endl;
            }
        }
        
        if(success)
        {
            //Fill params with a full configuration space for a PCM.  initialize the hardware parameter
            error=snd_pcm_hw_params_any(*phandler,hw_params);        
            if (error < 0) 
            {                
                success=false;
                cout<<"Broken configuration for PCM: no configurations available: "<<snd_strerror(error)<<endl;
            }
        }
        
        if(success)
        {
            // set the access type
            error = snd_pcm_hw_params_set_access(*phandler, hw_params, palsaCfg->accessType);
            if (error < 0)
            {
                cout<<"Fail to set access type. Reason: "<<snd_strerror(error)<<endl;
                success = false;
            }
        }
        
        if(success)
        {
            switch (palsaCfg->bitsPerSample)
            {
                case BITS_PER_SAMPLE_8:
                {
                    format = SND_PCM_FORMAT_U8;
                    break;
                }
                case BITS_PER_SAMPLE_16:
                {
                    format = SND_PCM_FORMAT_S16_LE; //indicate this was little endian
                    break;
                }
                case BITS_PER_SAMPLE_32:
                {
                    format = SND_PCM_FORMAT_S32_LE;
                    break;
                }
                default:
                {
                    format = SND_PCM_FORMAT_S16_LE;
                    cout<<"Invalid format"<<endl;
                    success=false;
                }
            }
            
            if(success)
            {
                error=snd_pcm_hw_params_set_format(*phandler,hw_params,format);
                if(error<0)
                {
                    cout<<"set format not available for "<<snd_strerror(error)<<endl;
                    success=false;
                }
            }

        }
        
        if(success)
        {
            error=snd_pcm_hw_params_set_rate_near(*phandler,hw_params,&palsaCfg->samplingFrequency,0);
            if(error<0)
            {
                cout<<"set rate not available for "<<snd_strerror(error)<<endl;
                success=false;
            }
        }
        
        if(success)
        {
            error=snd_pcm_hw_params_set_channels(*phandler,hw_params,palsaCfg->numOfChannels);
            if(error<0)
            {
                cout<<"set_channels not available for "<<snd_strerror(error)<<endl;
                success=false;
            }
        }
        if (success)
        {    
            // set period size (period size is also a chunk size for reading from ALSA)
            snd_pcm_uframes_t alsaPeriodFrame = static_cast<snd_pcm_uframes_t>(palsaCfg->alsaPeriodFrame); // One frame could be 4 bytes at most

            // set period size
            error = snd_pcm_hw_params_set_period_size_near(*phandler, hw_params, &alsaPeriodFrame, &dir);
            if (error < 0)
            {
                cout<<"Fail to set period size. Reason: "<<snd_strerror(error)<<endl;
                success = false;
            }
        }

        if (success)
        {
            // set hardware parameters
            error = snd_pcm_hw_params(*phandler, hw_params);
            if (error < 0)
            {
                cout<<"Fail to set hardware parameter. Reason: "<<snd_strerror(error)<<endl;
                success = false;
            }
        }

        if (success)
        {            
            error=snd_pcm_hw_params_get_period_size(hw_params, &g_frames, &dir); //get frame
            cout<<"Frame is "<<g_frames<<endl;

            // free the memory for hardware parameter structure
            snd_pcm_hw_params_free(hw_params);
            hw_params = NULL;
            // Prepare ALSA device
            error = snd_pcm_prepare(*phandler);
            if (error < 0)
            {
                cout<<"Fail to prepare ALSA device. Reason: "<<(snd_strerror(error))<<endl;
                success = false;
            }
        }

        if (success)
        {
            cout<<"ALSA device is ready to use"<<endl;
        }
        else
        {
            // fail to prepare ALSA device ==> un-initialize ALSA device
            if (hw_params != NULL)
            {
                snd_pcm_hw_params_free(hw_params);
                hw_params = NULL;
            }
            closeAlsaDevice(phandler);
        }

    }
    return success;
}





bool closeAlsaDevice(snd_pcm_t** phandler)
{
    bool ret = true;
    snd_pcm_state_t state;
    int snd_ret;

    if (*phandler != NULL)
    {
        // drop the pending audio frame if needed
        state = snd_pcm_state(*phandler);                
        cout<<"Alsa handler sate: "<<state<<endl;

        if ((SND_PCM_STATE_RUNNING == state) || (SND_PCM_STATE_XRUN == state) || (SND_PCM_STATE_SUSPENDED == state))
        {
            snd_ret = snd_pcm_drop(*phandler);
            if (snd_ret < 0)
            {
                cout<<"Fail to drop ALSA device. Reason: "<<(snd_strerror(snd_ret))<<endl;
            }
        }
        // close ALSA handler
        snd_ret = snd_pcm_close(*phandler);
        if (snd_ret < 0)
        {
            cout<<"Fail to close ALSA device. Reason: "<<(snd_strerror(snd_ret))<<endl;
            ret = false;
        }
        *phandler = NULL;
        cout<<"CLOSE ALSA DEVICE"<<endl;
    }
    return ret;

}

int main(int argc, char ** argv)
{        
    bool ret=false;
    snd_pcm_t* m_phandler=NULL;
    Alsa_Conf* m_palsaCfg=new Alsa_Conf();
    m_palsaCfg->alsaDevice = string("default");
    //m_palsaCfg->samplingFrequency = 16000;
    m_palsaCfg->alsaCapturePeriod = 50;
    //m_palsaCfg->numOfChannels = NUM_OF_CHANNELS_1;
    m_palsaCfg->block = true; //block
    m_palsaCfg->friendlyName = "AlsaWave";
    //m_palsaCfg->bitsPerSample = BITS_PER_SAMPLE_16;
    m_palsaCfg->alsaPeriodFrame = m_palsaCfg->samplingFrequency * m_palsaCfg->alsaCapturePeriod / 1000; // calculate the number of frame in one period
    m_palsaCfg->alsaBufferSize = m_palsaCfg->alsaPeriodFrame * 8;  //means the whole buffer was perdion*8, e.g. 10ms means every 10ms we will get/send the data
    m_palsaCfg->accessType = SND_PCM_ACCESS_RW_INTERLEAVED;
    m_palsaCfg->streamType = SND_PCM_STREAM_PLAYBACK;

    FILE* fp=NULL;
    const string wavePath= argv[1];
    //parse the wave file 
    ret=ParseWaveFile(wavePath,m_palsaCfg,&fp);
    //update the value
    m_palsaCfg->alsaPeriodFrame = m_palsaCfg->samplingFrequency * m_palsaCfg->alsaCapturePeriod / 1000; // calculate the number of frame in one period
    
    if(ret)
    {
        //open alsa device
        ret=PrepareAlsaDevice(m_palsaCfg,&m_phandler);
    }

    if(ret)
    {
        PlayWave(&fp,&m_phandler,m_palsaCfg);
    }

    closeAlsaDevice(&m_phandler);
    if(fp!=NULL)
    {
        fclose(fp);
        fp=NULL;
    }
    delete m_palsaCfg;
    m_palsaCfg=NULL;

    
    return 0;
    
}