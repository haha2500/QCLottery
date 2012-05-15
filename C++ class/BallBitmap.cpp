//
//  BallBitmap.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-7.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "BallBitmap.h"
#include "GraphGridPublic.h"

CBallBitmap::CBallBitmap()
{
    m_imageBall = NULL;
    Destroy();
}

CBallBitmap::~CBallBitmap()
{
    Destroy();
}

//////////////////////////////////////////////////////////////////////////
void CBallBitmap::Create(BYTE btRadius, UIColor *fillColor, LPCSTR lpszText, UIFont *font, UIColor *textColor, UIColor *borderColor)
{
    ASSERT(btRadius > 0 && m_imageBall == NULL && fillColor != nil && textColor != nil && borderColor != nil);
    ASSERT(lpszText != NULL && strlen(lpszText) <= 3);
    
    // 创建图像上下文
    CGRect rectBmp = CGRectMake(0, 0, btRadius * 2, btRadius * 2);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef ctxBmp = CGBitmapContextCreate(NULL, rectBmp.size.width, rectBmp.size.height, 8, rectBmp.size.width * 4, colorSpace, kCGImageAlphaPremultipliedLast);
    UIGraphicsPushContext(ctxBmp);
    
    // 绘制透明矩形
  //  [[UIColor clearColor] setFill];
  //  CGContextFillRect(ctxBmp, rectBmp);
    
    CGContextSetAllowsAntialiasing(ctxBmp, YES);
    // 绘制号码球
    [fillColor setFill];
    CGContextAddArc(ctxBmp, btRadius, btRadius, btRadius, 0, 2 * M_PI, 0);
    CGContextFillPath(ctxBmp);
 
    [borderColor setStroke];
    CGContextAddArc(ctxBmp, btRadius, btRadius, btRadius, 0, 2 * M_PI, 0);
    CGContextStrokePath(ctxBmp);
 
 /*   if(lpszText[0] == '-')	// 负数需要特殊处理
    {
        CGraphGridPublic::DrawText(ctxBmp, rectBmp, lpszText, font, textColor, nil, TRUE);
    }*/
    CGraphGridPublic::DrawText(ctxBmp, rectBmp, lpszText, font, textColor, nil);
        
    // 获取并保存图像指针
    m_imageBall = CGBitmapContextCreateImage(ctxBmp);
    
    // 释放图像上下文
    UIGraphicsPopContext();
    CGContextRelease(ctxBmp);
    CGColorSpaceRelease(colorSpace); 
}

void CBallBitmap::Destroy()
{
    if(m_imageBall != NULL)
    {
        CGImageRelease(m_imageBall);
        m_imageBall = NULL;
    }
}

void CBallBitmap::DrawInRect(CGContextRef context, CGRect rect)
{
    ASSERT(m_imageBall != NULL);
    
    // 居中显示
    int nImageWidth = CGImageGetWidth(m_imageBall);
    CGRect rectImage = CGRectMake(rect.origin.x + (rect.size.width - nImageWidth) / 2, rect.origin.y + (rect.size.height - nImageWidth) / 2, nImageWidth, nImageWidth);
    
    CGContextDrawImage(context, rectImage, m_imageBall);
}