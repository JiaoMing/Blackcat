//
//  GraySprite.cpp
//  Blackcat
//
//  Created by haojunhua on 13-12-11.
//
//

#include "GraySprite.h"

GraySprite::GraySprite(){
    
}

GraySprite::~GraySprite(){
    
}

GraySprite* GraySprite::create( const char* pszFileName ){
    GraySprite* graySprite = new GraySprite();
    if (graySprite && graySprite->initWithFile(pszFileName)){
        graySprite->autorelease();
        return graySprite;
    }else{
        CC_SAFE_RELEASE(graySprite);
        return NULL;
    }
}

GraySprite* GraySprite::createWithSpriteFrameName(const char *pszSpriteFrameName){
    GraySprite* graySprite = new GraySprite();
    if (graySprite && graySprite->initWithSpriteFrameName(pszSpriteFrameName)){
        graySprite->autorelease();
        return graySprite;
    }else{
        CC_SAFE_RELEASE(graySprite);
        return NULL;
    }
}


bool GraySprite::initWithTexture(CCTexture2D* pTexture, const CCRect& tRect ){
    do{
        CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, tRect));
        
        GLchar* pszFragSource =
        "#ifdef GL_ES \n \
        precision mediump float; \n \
        #endif \n \
        uniform sampler2D u_texture; \n \
        varying vec2 v_texCoord; \n \
        varying vec4 v_fragmentColor; \n \
        void main(void) \n \
        { \n \
        // Convert to greyscale using NTSC weightings \n \
        vec4 col = texture2D(u_texture, v_texCoord); \n \
        float grey = dot(col.rgb, vec3(0.299, 0.587, 0.114)); \n \
        gl_FragColor = vec4(grey, grey, grey, col.a); \n \
        }";
        
        CCGLProgram* pProgram = new CCGLProgram();
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
        this->setShaderProgram(pProgram);
        pProgram->release();
        CHECK_GL_ERROR_DEBUG();
        
        this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        CHECK_GL_ERROR_DEBUG();
        
        this->getShaderProgram()->link();
        CHECK_GL_ERROR_DEBUG();
        
        this->getShaderProgram()->updateUniforms();
        CHECK_GL_ERROR_DEBUG();
        
        return true;
    } while (0);
    return false;
}