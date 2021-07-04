// The functions contained in this file are pretty dummy
// and are included only as a placeholder. Nevertheless,
// they *will* get included in the shared library if you
// don't remove them :)
//
// Obviously, you 'll have to write yourself the super-duper
// functions to include in the resulting library...
// Also, it's not necessary to write every function in this file.
// Feel free to add more files in this project. They will be
// included in the resulting library.
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <SDL2/SDL.h>
#include <sys/ioctl.h>
#include <fstream>
#define device_name "/dev/video0"
#define W1 640
#define H1 480
class V2l{
public:
    int W, H;
    SDL_Window* wnd;
    SDL_Renderer* render;
    int8_t* pointer;
    V2l(int W, int H, char* Title){
        this->H=H;
        this->W=W;
        init();
        wnd = SDL_CreateWindow(Title,  800,  0, W, H, SDL_WINDOW_SHOWN);
        render = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
    };
    void dumptofile2(char* filename, int8_t* p, int X, int Y){
       std::cout<<"duping...\n";
       FILE* file = fopen( filename, "wb" );
       fwrite( p, 1, X*Y, file );
       fclose(file);
    };

    const char* init(){
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            return SDL_GetError();
        return 0;
    }
    void render3(SDL_Window* wnd, SDL_Renderer* render){
        int  file_device = open(device_name, O_RDWR, 0);
        if (file_device == -1){
            printf ("%s error %d, %s\n",device_name, errno, strerror(errno));
            exit(EXIT_FAILURE);
        };

        struct v4l2_buffer LOADBUFF;
        struct v4l2_requestbuffers   RQBUFF;
        while (1)
        {
//setting by default 640 X 480 X2 bytes YUV2  30 fps;;
/* prepare buffer on mmap*/
            RQBUFF.count = 1;
            RQBUFF.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            RQBUFF.memory = V4L2_MEMORY_MMAP;
            ioctl(file_device,VIDIOC_REQBUFS, &RQBUFF);
            printf("/* start capturing */");
            memset(&LOADBUFF, 0, sizeof(LOADBUFF));
            LOADBUFF.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            LOADBUFF.memory      = V4L2_MEMORY_MMAP;
            LOADBUFF.index       = 0;
/* loop of capturing each frame */
            ioctl(file_device, VIDIOC_QUERYBUF, &LOADBUFF);
            printf("out buffer ready");
            printf("buff length %d \n", LOADBUFF.length);
            void* MYBUFF=mmap(NULL,LOADBUFF.length,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                file_device,
                LOADBUFF.m.offset);
//extract und render
            int8_t RAWBUFF[W*H];
            int8_t * o=RAWBUFF;
            int8_t * q;
            memset(&LOADBUFF, 0, sizeof(LOADBUFF));
            LOADBUFF.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            LOADBUFF.memory = V4L2_MEMORY_MMAP;
            LOADBUFF.index = 0;
            ioctl(file_device, VIDIOC_QBUF, &LOADBUFF);
            enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            ioctl(file_device, VIDIOC_STREAMON, &type);
            /**************STREAM********/
            LOADBUFF.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            LOADBUFF.memory = V4L2_MEMORY_MMAP;
            while (ioctl(file_device, VIDIOC_DQBUF, &LOADBUFF) == EAGAIN){   }
            q = MYBUFF;
            for (int x=0;x<640;x++){
                for (int y=0;y<480;y++){
                    *o=*q;
                    o++;q++;q++;
                }
            }
            pointer=RAWBUFF;
            this->dumptofile2("out.x", pointer, 640,480);
            for (int j = 0; j<H; j++)
            {
                for (int i = 0; i<W; i++)
                {
                    printf("value %d\n",*(pointer));
                    SDL_SetRenderDrawColor(render, *(pointer), *(pointer), *(pointer++), 255);
                    SDL_RenderDrawPoint(render, i, j);
                }
            }
            while (1)
            {
                SDL_Event e;
                if (SDL_PollEvent(&e))
                if (e.type == SDL_QUIT)
                    break;
                SDL_RenderPresent(render);
                SDL_Delay(5);
                ioctl(file_device, VIDIOC_STREAMOFF, &type);
                ioctl(file_device,VIDIOC_REQBUFS, &RQBUFF);
                munmap(NULL,LOADBUFF.length);
                break;
            }

        };
    };


    void renderWOSDL(SDL_Window* wnd, SDL_Renderer* render){
        int  file_device = open(device_name, O_RDWR, 0);
        if (file_device == -1){
            printf ("%s error %d, %s\n",device_name, errno, strerror(errno));
            exit(EXIT_FAILURE);
        };

        struct v4l2_buffer LOADBUFF;
        struct v4l2_requestbuffers   RQBUFF;
        while (1)
        {
//setting by default 640 X 480 X2 bytes YUV2  30 fps;;
/* prepare buffer on mmap*/
            RQBUFF.count = 1;
            RQBUFF.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            RQBUFF.memory = V4L2_MEMORY_MMAP;
            ioctl(file_device,VIDIOC_REQBUFS, &RQBUFF);
            printf("/* start capturing */");
            memset(&LOADBUFF, 0, sizeof(LOADBUFF));
            LOADBUFF.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            LOADBUFF.memory      = V4L2_MEMORY_MMAP;
            LOADBUFF.index       = 0;
/* loop of capturing each frame */
            ioctl(file_device, VIDIOC_QUERYBUF, &LOADBUFF);
            printf("out buffer ready");
            printf("buff length %d \n", LOADBUFF.length);
            void* MYBUFF=mmap(NULL,LOADBUFF.length,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                file_device,
                LOADBUFF.m.offset);
//extract und render
            int8_t RAWBUFF[W*H];
            int8_t * o=RAWBUFF;
            int8_t * q;
            memset(&LOADBUFF, 0, sizeof(LOADBUFF));
            LOADBUFF.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            LOADBUFF.memory = V4L2_MEMORY_MMAP;
            LOADBUFF.index = 0;
            ioctl(file_device, VIDIOC_QBUF, &LOADBUFF);
            enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            ioctl(file_device, VIDIOC_STREAMON, &type);
            /**************STREAM********/
            LOADBUFF.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            LOADBUFF.memory = V4L2_MEMORY_MMAP;
            while (ioctl(file_device, VIDIOC_DQBUF, &LOADBUFF) == EAGAIN){   }
            q = MYBUFF;
            for (int x=0;x<640;x++){
                for (int y=0;y<480;y++){
                    *o=*q;
                    o++;q++;q++;
                }
            }
            pointer=RAWBUFF;
            this->dumptofile2("/tmp/out.x", pointer, 640,480);

        };
    };




};
using namespace std;
extern "C"
{

    void forJ(){
        V2l* v2l = new V2l(640, 480, "C++ rulezzzzz");
        v2l ->init();
        v2l ->render3(v2l->wnd, v2l->render);
        cout << "Hello world!" << endl;
    }


    void forJArr(int8_t* p){
        V2l* v2l = new V2l(640, 480, "C++ rulezzzzz");
        v2l ->init();
        p = v2l->pointer;
        v2l ->render3(v2l->wnd, v2l->render);
        cout << "Hello world!" << endl;
    }

    void dumptofile(char* filename, int8_t* p, int X, int Y){
       FILE* file = fopen( filename, "wb" );
       fwrite( p, 1, X*Y, file );
       fclose(file);
    }

    uint8_t* getframe(){
        int  file_device = open(device_name, O_RDWR, 0);
        if (file_device == -1){
            printf ("%s error %d, %s\n",device_name, errno, strerror(errno));
            exit(EXIT_FAILURE);
        };

        struct v4l2_buffer LOADBUFF;
        struct v4l2_requestbuffers   RQBUFF;

//setting by default 640 X 480 X2 bytes YUV2  30 fps;;
/* prepare buffer on mmap*/
        RQBUFF.count = 1;
        RQBUFF.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        RQBUFF.memory = V4L2_MEMORY_MMAP;
        ioctl(file_device,VIDIOC_REQBUFS, &RQBUFF);
        printf("/* start capturing */");
        memset(&LOADBUFF, 0, sizeof(LOADBUFF));
        LOADBUFF.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        LOADBUFF.memory      = V4L2_MEMORY_MMAP;
        LOADBUFF.index       = 0;
/* loop of capturing each frame */
        ioctl(file_device, VIDIOC_QUERYBUF, &LOADBUFF);
        printf("out buffer ready");
        printf("buff length %d \n", LOADBUFF.length);
        void* MYBUFF=mmap(NULL,LOADBUFF.length,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            file_device,
            LOADBUFF.m.offset);
//extract und render
        int8_t RAWBUFF[640*480];
        int8_t * o=RAWBUFF;
        int8_t * q;
        memset(&LOADBUFF, 0, sizeof(LOADBUFF));
        LOADBUFF.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        LOADBUFF.memory = V4L2_MEMORY_MMAP;
        LOADBUFF.index = 0;
        ioctl(file_device, VIDIOC_QBUF, &LOADBUFF);
        enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        ioctl(file_device, VIDIOC_STREAMON, &type);
            /**************STREAM********/
        LOADBUFF.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        LOADBUFF.memory = V4L2_MEMORY_MMAP;
        while (ioctl(file_device, VIDIOC_DQBUF, &LOADBUFF) == EAGAIN){   }
        q = MYBUFF;
        int x,y;
        for (x=0;x<640;x++){
            for (y=0;y<480;y++){
                *o=*q;
                o++;q++;q++;
            }
        }
        int8_t* pointer=RAWBUFF;
        return pointer;

    }

    void directstream(){
      V2l* v2l = new V2l(640, 480, "C++ rulezzzzz");
        v2l ->init();
        v2l ->renderWOSDL(v2l->wnd, v2l->render);
        cout << "Hello world!" << endl;
    }

    void update(int8_t* Arr, int size, int8_t value){
        for (int i=0; i<size; i++){
            Arr[i]=value;
        }
    }

    int* gen(int size, int value){
        int* Arr = (int*)malloc(size*sizeof(int));
        for (int i=0; i<size; i++){
            Arr[i]=value;
        }
        return Arr;
    }
}
