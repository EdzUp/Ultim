/*
    ETexture.cpp - Copyright (C)EdzUp
    Programmed by Ed 'EdzUp' Upton
*/

#include "UTexture.h"
#include "picopng.h"                //include the lodepng system that is for PNG loading
#include <GL/gl.h>

int UTexture::LoadEdzUp( std::string filename ) {
    //this will load a encrypted texture for the game engine
    return( 0 );
}

int UTexture::LoadBMP( std::string filename ) {
    int width = 0;
    int height = 0;
    short BitsPerPixel = 0;

    std::fstream hFile(filename.c_str(), std::ios::in | std::ios::binary);
    if (!hFile.is_open()) return( -1 );

    hFile.seekg(0, std::ios::end);
    int Length = hFile.tellg();
    hFile.seekg(0, std::ios::beg);
    std::vector<std::uint8_t> FileInfo(Length);
    hFile.read(reinterpret_cast<char*>(FileInfo.data()), 54);

    if (FileInfo[0] != 'B' && FileInfo[1] != 'M') {
        hFile.close();
        return( -1 );
    }

    if (FileInfo[28] != 24 && FileInfo[28] != 32) {
        hFile.close();
        return( -1 );
    }

    BitsPerPixel = FileInfo[28];
    width = FileInfo[18] + (FileInfo[19] << 8);
    height = FileInfo[22] + (FileInfo[23] << 8);
    std::uint32_t PixelsOffset = FileInfo[10] + (FileInfo[11] << 8);
    std::uint32_t size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
    Buffer.resize(size);

    hFile.seekg (PixelsOffset, std::ios::beg);
    hFile.read(reinterpret_cast<char*>(Buffer.data()), size);
    hFile.close();

    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, Buffer.data());
    AnimatedTexture = false;
    IWidth = width;
    IHeight = height;

    return( 0 );
}

int UTexture::LoadPNG( std::string filename ) {
    //This is the main loader for the textures

    std::vector<unsigned char> image;

    loadFile( Buffer, filename );

    unsigned long w, h;
    int error = decodePNG(image, w, h, Buffer.empty() ? 0 : &Buffer[0], (unsigned long)Buffer.size());//if there's an error, display it

    if(error != 0) {
        return( -1 );
    }

    glGenTextures(1, &Texture );
    glBindTexture( GL_TEXTURE_2D, Texture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &( image.front() ) );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    AnimatedTexture = false;
    IWidth = w;
    IHeight = h;

    return( 0 );
}   //end of LoadPNGTexture

void UTexture::Free( void ){
    //This will release a texture from the OpenGL system
    glDeleteTextures( 1, &Texture );                //delete the entry
    Buffer.clear();
}

bool UTexture::SetFrames( float width, float height, float amount ) {
    //this will set the animation system for a frame set
    AnimUV TempUV;      //this will hold the frame UV's
    float CurrX = 0.0f;
    float CurrY = 0.0f;

    AnimatedTexture = true;

    for ( CurrY = 0.0f; CurrY<1.0f; CurrY += 1.0f /height ) {
        for ( CurrX =0.0f; CurrX<1.0f; CurrX+= 1.0f /width ) {
            //top left
            TempUV.UV[ 0 ].U = CurrX;
            TempUV.UV[ 0 ].V = CurrY;

            //top right
            TempUV.UV[ 1 ].U = CurrX+( 1.0f /width );
            TempUV.UV[ 1 ].V = CurrY;

            //bottom left
            TempUV.UV[ 2 ].U = CurrX;
            TempUV.UV[ 2 ].V = CurrY+( 1.0f /height );

            //bottom right
            TempUV.UV[ 3 ].U = CurrX+( 1.0f /width );
            TempUV.UV[ 3 ].V = CurrY+( 1.0f /height );

            AnimList.push_back( TempUV );       //add it to the list
        }
    }

    return( false );
}

bool UTexture::IsAnimated( void ) {
    return( AnimatedTexture );
}

float UTexture::Width( void ) {
    return( IWidth );
}

float UTexture::Height( void ) {
    return( IHeight );
}

void UTexture::Flip( void ){
    //This will flip the y buffer so we can actually use it
}

void UTexture::loadFile(std::vector<unsigned char>& buffer, const std::string& filename) //designed for loading files from hard disk in an std::vector
{
    std::ifstream file(filename.c_str(), std::ios::in|std::ios::binary|std::ios::ate);

    //get filesize
    std::streamsize size = 0;
    if(file.seekg(0, std::ios::end).good()) size = file.tellg();
    if(file.seekg(0, std::ios::beg).good()) size -= file.tellg();

    //read contents of the file into the vector
    if(size > 0) {
        buffer.resize((size_t)size);
        file.read((char*)(&buffer[0]), size);
    }
    else buffer.clear();
}
