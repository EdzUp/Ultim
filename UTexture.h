/*
    ETexture.h - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#ifndef ETEXTURESYSTEM
    #define ETEXTURESYSTEM

    #include <string>
    #include <iostream>
    #include <fstream>
    #include <vector>
    #include <GL/gl.h>
    #include "Vec3.h"
/*
Even devices that don't support NPOT texture should support NPOT load.

Create the texture as an exact power of 2 and NO CONTENT using glTexImage2D, passing a null pointer for data.

data may be a null pointer. In this case, texture memory is allocated to accommodate a texture of width width
and height height. You can then download subtextures to initialize this texture memory. The image is undefined
if the user tries to apply an uninitialized portion of the texture image to a primitive.
Then use glTexSubImage2D to upload a NPOT image, which occupies only a portion of the total texture. This can
be done without any CPU-side image rearrangement.
*/
    class AnimUV {
        public:
        VecUV UV[ 4 ];
    };

    class UTexture {
        private:
        float IWidth;
        float IHeight;

    public:
        GLuint Texture;
        bool AnimatedTexture;
        double u3;
        double v3;
        std::vector<unsigned char> Buffer;
        std::vector<AnimUV> AnimList;

        //image functions
        bool SetFrames( float width, float height, float amount );
        bool IsAnimated( void );
        float Width( void );
        float Height( void );
        void Free( void );

        //loaders
        int LoadEdzUp( std::string filename );
        int LoadBMP( std::string filename );
        int LoadPNG( std::string filename );

    private:
        void Flip( void );
        void loadFile( std::vector<unsigned char>& buffer, const std::string& filename );
    };   //end of class ETexture
#endif // ETEXTURESYSTEM
