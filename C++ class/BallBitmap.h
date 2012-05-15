//
//  BallBitmap.h
//  Graph
//
//  Created by 武 孙 on 12-5-7.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

class CBallBitmap
{
public:
    CBallBitmap();
    virtual ~CBallBitmap();
    
public:
    // 创建号码球
    void Create(BYTE btRadius, UIColor *fillColor, LPCSTR lpszText, UIFont *font, UIColor *textColor, UIColor *borderColor);
    
    // 销毁号码球
    void Destroy();
    
    // 在矩形区域内绘制该号码球
    void DrawInRect(CGContextRef context, CGRect rect);
    
private:
     CGImageRef  m_imageBall;                // 号码球图像指针
};
