/************************************************************************* 
  > File Name  : local_player.c 
  > Desc.      : Read from file and playback, or capture and write to file.
  > Author     : horistudio@163.com
  > Date       : 2020/05/26
 ************************************************************************/ 
/* Use the newer ALSA API */

#define ALSA_PCM_NEW_HW_PARAMS_API 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <alsa/asoundlib.h>

int playback();

int main(int argc, char *argv[]) {

playback();


// #if 1
//   int rc;
//   int size;
//   snd_pcm_t *handle;
//   snd_pcm_hw_params_t *params;
//   unsigned int val;
//   int dir;
//   snd_pcm_uframes_t frames;
//   char *buffer;
 
// #if 0
//   /* Open PCM device for playback. */
//   rc = snd_pcm_open(&handle, "default",
//                     SND_PCM_STREAM_PLAYBACK, 0);
// #endif
 
//   /* Open PCM device for capture. */
//   rc = snd_pcm_open(&handle, "default",
//                     SND_PCM_STREAM_CAPTURE, 0);
//   if (rc < 0) {
//     fprintf(stderr,
//             "unable to open pcm device: %s/n",
//             snd_strerror(rc));
//     exit(1);
//   }
 
//   /* Allocate a hardware parameters object. */
//   snd_pcm_hw_params_alloca(&params);
 
//   /* Fill it in with default values. */
//   snd_pcm_hw_params_any(handle, params);
 
//   /* Set the desired hardware parameters. */
 
//   /* Interleaved mode */
//   snd_pcm_hw_params_set_access(handle, params,
//                       SND_PCM_ACCESS_RW_INTERLEAVED);
 
//   /* Signed 16-bit little-endian format */
//   snd_pcm_hw_params_set_format(handle, params,
//                               SND_PCM_FORMAT_S16_LE);
 
//   /* Two channels (stereo) */
//   snd_pcm_hw_params_set_channels(handle, params, 8);
 
//   /* 44100 bits/second sampling rate (CD quality) */
//   val = 48000;
//   snd_pcm_hw_params_set_rate_near(handle, params,
//                                   &val, &dir);
 
//   /* Set period size to 32 frames. */
//   frames = 32;
//   snd_pcm_hw_params_set_period_size_near(handle,
//                               params, &frames, &dir);
 
//   /* Write the parameters to the driver */
//   rc = snd_pcm_hw_params(handle, params);
//   if (rc < 0) {
//     fprintf(stderr,
//             "unable to set hw parameters: %s/n",
//             snd_strerror(rc));
//     exit(1);
//   }
 
//   /* Use a buffer large enough to hold one period */
//   snd_pcm_hw_params_get_period_size(params, &frames,
//                                     &dir);
//   size = frames * 4; /* 2 bytes/sample, 2 channels */
//   buffer = (char *) malloc(size);
//   printf("------>frames:%ld, size:%d\n",frames,size);
// #endif
 
// #if 1
//   	//************************[3.文件操作]*******************************
// 	#if 0
//   /*通过主函数传参，传入.wav格式文件名*/
// 	FILE *fp_r 	= NULL;
// 	int ret 	= 0;
// 	fp_r = fopen(argv[1], "rb");
// 	if(fp_r == NULL) {
// 		printf("open file fail!\n");
// 		exit(1);
// 	}
// 	#endif
 
// 	/* 打开一个文件，将录音音频流写入文件 */ 
// 	FILE *fp = NULL;
// 	int file_len = 0;
 
// 	fp = fopen("record_dump.wav", "a+");
// 	if(fp == NULL) {
// 		printf("open file fail!\n");
// 		exit(1);
// 	}
 
// 	printf("Configurations are finished......\n");
	
//     while (1) {
// 	  	/* 1.read from file and playback */
// 		 #if 0
// 	    //read audio file 
// 	    memset(buffer,0,size);
// 		  rc = fread(buffer, 1, size, fp_r);
// 	    if (rc == 0) {
// 	      fprintf(stderr, "end of file on input/n");
// 	      break;
// 	    } else if (rc != size) {
// 	      fprintf(stderr,
// 	              "short read: read %d bytes/n", rc);
// 	    }
		
// 	    rc = snd_pcm_writei(handle, buffer, frames);
// 	    if (rc == -EPIPE) {
// 	      /* EPIPE means underrun */
// 	      fprintf(stderr, "underrun occurred/n");
// 	      snd_pcm_prepare(handle);
// 	    } else if (rc < 0) {
// 	      fprintf(stderr,
// 	              "error from writei: %s/n",
// 	              snd_strerror(rc));
// 	    }  else if (rc != (int)frames) {
// 	      fprintf(stderr,
// 	              "short write, write %d frames/n", rc);
// 	    }
// 		#endif
 
// 		/* 2.capture and write to file */
// 		#if 1
// 		/* capture */
// 		memset(buffer, 0, size);
// 		rc = snd_pcm_readi(handle, buffer, frames);
// 	    printf("snd_pcm_readi,frames:%ld, buffer:%s\n",frames, buffer);
// 	    if (rc == -EPIPE)
// 	    {
// 	        /* EPIPE means overrun */
// 	        fprintf(stderr, "overrun occurred\n");
// 	        snd_pcm_prepare(handle);
// 	    } else if (rc < 0) {
// 	        fprintf(stderr,
// 	                "error from read: %s\n",
// 	                snd_strerror(rc));
// 	    } else if (rc != (int)frames) {
//           fprintf(stderr, 
// 	        	"short read, read %d frames\n", rc);
// 	    }
		
// 		if (fp) {
// 	        file_len = fwrite(buffer, 1, size, fp);
// 			printf("fwrite:%d bytes \n", file_len);
		 	
// 		} else {
// 		  printf("%s[%d] file open fail\n",__func__,__LINE__);
// 		}
// 		#endif
//   }
// #endif
 
//   fclose(fp);
//   //close(fd_f);
//   snd_pcm_drain(handle);
//   snd_pcm_close(handle);
//   free(buffer);
 
//   return 0;
}

int playback() {
  int rc;
  int size;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int val;
  int dir;
  snd_pcm_uframes_t frames;
  char *buffer;
 
#if 0
  /* Open PCM device for playback. */
  rc = snd_pcm_open(&handle, "default",
                    SND_PCM_STREAM_PLAYBACK, 0);
#endif
 
  /* Open PCM device for capture. */
  rc = snd_pcm_open(&handle, "default",
                    SND_PCM_STREAM_CAPTURE, 0);
  if (rc < 0) {
    fprintf(stderr,
            "unable to open pcm device: %s/n",
            snd_strerror(rc));
    exit(1);
  }
 
  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);
 
  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);
 
  /* Set the desired hardware parameters. */
 
  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params,
                      SND_PCM_ACCESS_RW_INTERLEAVED);
 
  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params,
                              SND_PCM_FORMAT_S16_LE);
 
  /* Two channels (stereo) */
  snd_pcm_hw_params_set_channels(handle, params, 8);
 
  /* 44100 bits/second sampling rate (CD quality) */
  val = 48000;
  snd_pcm_hw_params_set_rate_near(handle, params,
                                  &val, &dir);
 
  /* Set period size to 32 frames. */
  frames = 32;
  snd_pcm_hw_params_set_period_size_near(handle,
                              params, &frames, &dir);
 
  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) {
    fprintf(stderr,
            "unable to set hw parameters: %s/n",
            snd_strerror(rc));
    exit(1);
  }
 
  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params, &frames,
                                    &dir);
  size = frames * 4; /* 2 bytes/sample, 2 channels */
  buffer = (char *) malloc(size);
  printf("------>frames:%ld, size:%d\n",frames,size);
 
#if 1
  	//************************[3.文件操作]*******************************
	#if 0
  /*通过主函数传参，传入.wav格式文件名*/
	FILE *fp_r 	= NULL;
	int ret 	= 0;
	fp_r = fopen(argv[1], "rb");
	if(fp_r == NULL) {
		printf("open file fail!\n");
		exit(1);
	}
	#endif
 
	/* 打开一个文件，将录音音频流写入文件 */ 
	FILE *fp = NULL;
	int file_len = 0;
 
	fp = fopen("record_dump.wav", "a+");
	if(fp == NULL) {
		printf("open file fail!\n");
		exit(1);
	}
 
	printf("Configurations are finished......\n");
	
    while (1) {
	  	/* 1.read from file and playback */
		 #if 0
	    //read audio file 
	    memset(buffer,0,size);
		  rc = fread(buffer, 1, size, fp_r);
	    if (rc == 0) {
	      fprintf(stderr, "end of file on input/n");
	      break;
	    } else if (rc != size) {
	      fprintf(stderr,
	              "short read: read %d bytes/n", rc);
	    }
		
	    rc = snd_pcm_writei(handle, buffer, frames);
	    if (rc == -EPIPE) {
	      /* EPIPE means underrun */
	      fprintf(stderr, "underrun occurred/n");
	      snd_pcm_prepare(handle);
	    } else if (rc < 0) {
	      fprintf(stderr,
	              "error from writei: %s/n",
	              snd_strerror(rc));
	    }  else if (rc != (int)frames) {
	      fprintf(stderr,
	              "short write, write %d frames/n", rc);
	    }
		#endif
 
		/* 2.capture and write to file */
		#if 1
		/* capture */
		memset(buffer, 0, size);
		rc = snd_pcm_readi(handle, buffer, frames);
	    printf("snd_pcm_readi,frames:%ld, buffer:%s\n",frames, buffer);
	    if (rc == -EPIPE)
	    {
	        /* EPIPE means overrun */
	        fprintf(stderr, "overrun occurred\n");
	        snd_pcm_prepare(handle);
	    } else if (rc < 0) {
	        fprintf(stderr,
	                "error from read: %s\n",
	                snd_strerror(rc));
	    } else if (rc != (int)frames) {
          fprintf(stderr, 
	        	"short read, read %d frames\n", rc);
	    }
		
		if (fp) {
	        file_len = fwrite(buffer, 1, size, fp);
			printf("fwrite:%d bytes \n", file_len);
		 	
		} else {
		  printf("%s[%d] file open fail\n",__func__,__LINE__);
		}
#endif
  }
#endif
 
  fclose(fp);
  //close(fd_f);
  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);
 
  return 0;

}

void capture () {
    #if 1
  int rc;
  int size;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int val;
  int dir;
  snd_pcm_uframes_t frames;
  char *buffer;
 
#if 0
  /* Open PCM device for playback. */
  rc = snd_pcm_open(&handle, "default",
                    SND_PCM_STREAM_PLAYBACK, 0);
#endif
 
  /* Open PCM device for capture. */
  rc = snd_pcm_open(&handle, "default",
                    SND_PCM_STREAM_CAPTURE, 0);
  if (rc < 0) {
    fprintf(stderr,
            "unable to open pcm device: %s/n",
            snd_strerror(rc));
    exit(1);
  }
 
  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);
 
  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);
 
  /* Set the desired hardware parameters. */
 
  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params,
                      SND_PCM_ACCESS_RW_INTERLEAVED);
 
  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params,
                              SND_PCM_FORMAT_S16_LE);
 
  /* Two channels (stereo) */
  snd_pcm_hw_params_set_channels(handle, params, 8);
 
  /* 44100 bits/second sampling rate (CD quality) */
  val = 48000;
  snd_pcm_hw_params_set_rate_near(handle, params,
                                  &val, &dir);
 
  /* Set period size to 32 frames. */
  frames = 32;
  snd_pcm_hw_params_set_period_size_near(handle,
                              params, &frames, &dir);
 
  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) {
    fprintf(stderr,
            "unable to set hw parameters: %s/n",
            snd_strerror(rc));
    exit(1);
  }
 
  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params, &frames,
                                    &dir);
  size = frames * 4; /* 2 bytes/sample, 2 channels */
  buffer = (char *) malloc(size);
  printf("------>frames:%ld, size:%d\n",frames,size);
#endif
 
#if 1
  	//************************[3.文件操作]*******************************
	#if 0
  /*通过主函数传参，传入.wav格式文件名*/
	FILE *fp_r 	= NULL;
	int ret 	= 0;
	fp_r = fopen(argv[1], "rb");
	if(fp_r == NULL) {
		printf("open file fail!\n");
		exit(1);
	}
	#endif
 
	/* 打开一个文件，将录音音频流写入文件 */ 
	FILE *fp = NULL;
	int file_len = 0;
 
	fp = fopen("record_dump.wav", "a+");
	if(fp == NULL) {
		printf("open file fail!\n");
		exit(1);
	}
 
	printf("Configurations are finished......\n");
	
    while (1) {
	  	/* 1.read from file and playback */
		 #if 0
	    //read audio file 
	    memset(buffer,0,size);
		  rc = fread(buffer, 1, size, fp_r);
	    if (rc == 0) {
	      fprintf(stderr, "end of file on input/n");
	      break;
	    } else if (rc != size) {
	      fprintf(stderr,
	              "short read: read %d bytes/n", rc);
	    }
		
	    rc = snd_pcm_writei(handle, buffer, frames);
	    if (rc == -EPIPE) {
	      /* EPIPE means underrun */
	      fprintf(stderr, "underrun occurred/n");
	      snd_pcm_prepare(handle);
	    } else if (rc < 0) {
	      fprintf(stderr,
	              "error from writei: %s/n",
	              snd_strerror(rc));
	    }  else if (rc != (int)frames) {
	      fprintf(stderr,
	              "short write, write %d frames/n", rc);
	    }
		#endif
 
		/* 2.capture and write to file */
		#if 1
		/* capture */
		memset(buffer, 0, size);
		rc = snd_pcm_readi(handle, buffer, frames);
	    printf("snd_pcm_readi,frames:%ld, buffer:%s\n",frames, buffer);
	    if (rc == -EPIPE)
	    {
	        /* EPIPE means overrun */
	        fprintf(stderr, "overrun occurred\n");
	        snd_pcm_prepare(handle);
	    } else if (rc < 0) {
	        fprintf(stderr,
	                "error from read: %s\n",
	                snd_strerror(rc));
	    } else if (rc != (int)frames) {
          fprintf(stderr, 
	        	"short read, read %d frames\n", rc);
	    }
		
		if (fp) {
	        file_len = fwrite(buffer, 1, size, fp);
			printf("fwrite:%d bytes \n", file_len);
		 	
		} else {
		  printf("%s[%d] file open fail\n",__func__,__LINE__);
		}
		#endif
  }
#endif
 
  fclose(fp);
  //close(fd_f);
  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);
 
  return 0;

}