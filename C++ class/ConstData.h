//
//  ConstData.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

// 号码000-999 的间距值集合
const BYTE SPACE_VALUE[1000] = { 9,  8,  7,  6,  5,  5,  6,  7,  8,  9,  9,  8,  7,  6,  5,  4,  5,  6,  7,  8, 
    9,  8,  7,  6,  5,  4,  4,  5,  6,  7,  9,  8,  7,  6,  5,  4,  3,  4,  5,  6, 
    9,  8,  7,  6,  5,  4,  4,  4,  4,  5,  9,  8,  7,  6,  5,  5,  5,  5,  5,  5, 
    9,  8,  7,  6,  6,  6,  6,  6,  6,  6,  9,  8,  7,  7,  7,  7,  7,  7,  7,  7, 
    9,  8,  8,  8,  8,  8,  8,  8,  8,  8,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 
    10,  9,  8,  7,  6,  5,  6,  7,  8,  9, 10,  9,  8,  7,  6,  5,  5,  6,  7,  8, 
    10,  9,  8,  7,  6,  5,  4,  5,  6,  7, 10,  9,  8,  7,  6,  5,  4,  4,  5,  6, 
    10,  9,  8,  7,  6,  5,  4,  3,  4,  5, 10,  9,  8,  7,  6,  5,  4,  4,  4,  4, 
    10,  9,  8,  7,  6,  5,  5,  5,  5,  5, 10,  9,  8,  7,  6,  6,  6,  6,  6,  6, 
    10,  9,  8,  7,  7,  7,  7,  7,  7,  7, 10,  9,  8,  8,  8,  8,  8,  8,  8,  8, 
    11, 10,  9,  8,  7,  6,  6,  7,  8,  9, 11, 10,  9,  8,  7,  6,  5,  6,  7,  8, 
    11, 10,  9,  8,  7,  6,  5,  5,  6,  7, 11, 10,  9,  8,  7,  6,  5,  4,  5,  6, 
    11, 10,  9,  8,  7,  6,  5,  4,  4,  5, 11, 10,  9,  8,  7,  6,  5,  4,  3,  4, 
    11, 10,  9,  8,  7,  6,  5,  4,  4,  4, 11, 10,  9,  8,  7,  6,  5,  5,  5,  5, 
    11, 10,  9,  8,  7,  6,  6,  6,  6,  6, 11, 10,  9,  8,  7,  7,  7,  7,  7,  7, 
    12, 11, 10,  9,  8,  7,  6,  7,  8,  9, 12, 11, 10,  9,  8,  7,  6,  6,  7,  8, 
    12, 11, 10,  9,  8,  7,  6,  5,  6,  7, 12, 11, 10,  9,  8,  7,  6,  5,  5,  6, 
    12, 11, 10,  9,  8,  7,  6,  5,  4,  5, 12, 11, 10,  9,  8,  7,  6,  5,  4,  4, 
    12, 11, 10,  9,  8,  7,  6,  5,  4,  3, 12, 11, 10,  9,  8,  7,  6,  5,  4,  4, 
    12, 11, 10,  9,  8,  7,  6,  5,  5,  5, 12, 11, 10,  9,  8,  7,  6,  6,  6,  6, 
    13, 12, 11, 10,  9,  8,  7,  7,  8,  9, 13, 12, 11, 10,  9,  8,  7,  6,  7,  8, 
    13, 12, 11, 10,  9,  8,  7,  6,  6,  7, 13, 12, 11, 10,  9,  8,  7,  6,  5,  6, 
    13, 12, 11, 10,  9,  8,  7,  6,  5,  5, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4, 
    13, 12, 11, 10,  9,  8,  7,  6,  5,  4, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4, 
    13, 12, 11, 10,  9,  8,  7,  6,  5,  4, 13, 12, 11, 10,  9,  8,  7,  6,  5,  5, 
    14, 13, 12, 11, 10,  9,  8,  7,  8,  9, 14, 13, 12, 11, 10,  9,  8,  7,  7,  8, 
    14, 13, 12, 11, 10,  9,  8,  7,  6,  7, 14, 13, 12, 11, 10,  9,  8,  7,  6,  6, 
    14, 13, 12, 11, 10,  9,  8,  7,  6,  5, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5, 
    14, 13, 12, 11, 10,  9,  8,  7,  6,  5, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5, 
    14, 13, 12, 11, 10,  9,  8,  7,  6,  5, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5, 
    15, 14, 13, 12, 11, 10,  9,  8,  8,  9, 15, 14, 13, 12, 11, 10,  9,  8,  7,  8, 
    15, 14, 13, 12, 11, 10,  9,  8,  7,  7, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6, 
    15, 14, 13, 12, 11, 10,  9,  8,  7,  6, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6, 
    15, 14, 13, 12, 11, 10,  9,  8,  7,  6, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6, 
    15, 14, 13, 12, 11, 10,  9,  8,  7,  6, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6, 
    16, 15, 14, 13, 12, 11, 10,  9,  8,  9, 16, 15, 14, 13, 12, 11, 10,  9,  8,  8, 
    16, 15, 14, 13, 12, 11, 10,  9,  8,  7, 16, 15, 14, 13, 12, 11, 10,  9,  8,  7, 
    16, 15, 14, 13, 12, 11, 10,  9,  8,  7, 16, 15, 14, 13, 12, 11, 10,  9,  8,  7, 
    16, 15, 14, 13, 12, 11, 10,  9,  8,  7, 16, 15, 14, 13, 12, 11, 10,  9,  8,  7, 
    16, 15, 14, 13, 12, 11, 10,  9,  8,  7, 16, 15, 14, 13, 12, 11, 10,  9,  8,  7, 
    17, 16, 15, 14, 13, 12, 11, 10,  9,  9, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8, 
    17, 16, 15, 14, 13, 12, 11, 10,  9,  8, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8, 
    17, 16, 15, 14, 13, 12, 11, 10,  9,  8, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8, 
    17, 16, 15, 14, 13, 12, 11, 10,  9,  8, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8, 
    17, 16, 15, 14, 13, 12, 11, 10,  9,  8, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8, 
    18, 17, 16, 15, 14, 13, 12, 11, 10,  9, 18, 17, 16, 15, 14, 13, 12, 11, 10,  9, 
    18, 17, 16, 15, 14, 13, 12, 11, 10,  9, 18, 17, 16, 15, 14, 13, 12, 11, 10,  9, 
    18, 17, 16, 15, 14, 13, 12, 11, 10,  9, 18, 17, 16, 15, 14, 13, 12, 11, 10,  9, 
    18, 17, 16, 15, 14, 13, 12, 11, 10,  9, 18, 17, 16, 15, 14, 13, 12, 11, 10,  9, 
    18, 17, 16, 15, 14, 13, 12, 11, 10,  9, 18, 17, 16, 15, 14, 13, 12, 11, 10,  9, };

// 号码000-999 的组选值集合
const WORD GROUP_CODE[1000]={ 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   1,  11,  12,  13,  14,  15,  16,  17,  18,  19, 
    2,  12,  22,  23,  24,  25,  26,  27,  28,  29,   3,  13,  23,  33,  34,  35,  36,  37,  38,  39, 
    4,  14,  24,  34,  44,  45,  46,  47,  48,  49,   5,  15,  25,  35,  45,  55,  56,  57,  58,  59, 
    6,  16,  26,  36,  46,  56,  66,  67,  68,  69,   7,  17,  27,  37,  47,  57,  67,  77,  78,  79, 
    8,  18,  28,  38,  48,  58,  68,  78,  88,  89,   9,  19,  29,  39,  49,  59,  69,  79,  89,  99, 
    1,  11,  12,  13,  14,  15,  16,  17,  18,  19,  11, 111, 112, 113, 114, 115, 116, 117, 118, 119, 
    12, 112, 122, 123, 124, 125, 126, 127, 128, 129,  13, 113, 123, 133, 134, 135, 136, 137, 138, 139, 
    14, 114, 124, 134, 144, 145, 146, 147, 148, 149,  15, 115, 125, 135, 145, 155, 156, 157, 158, 159, 
    16, 116, 126, 136, 146, 156, 166, 167, 168, 169,  17, 117, 127, 137, 147, 157, 167, 177, 178, 179, 
    18, 118, 128, 138, 148, 158, 168, 178, 188, 189,  19, 119, 129, 139, 149, 159, 169, 179, 189, 199, 
    2,  12,  22,  23,  24,  25,  26,  27,  28,  29,  12, 112, 122, 123, 124, 125, 126, 127, 128, 129, 
    22, 122, 222, 223, 224, 225, 226, 227, 228, 229,  23, 123, 223, 233, 234, 235, 236, 237, 238, 239, 
    24, 124, 224, 234, 244, 245, 246, 247, 248, 249,  25, 125, 225, 235, 245, 255, 256, 257, 258, 259, 
    26, 126, 226, 236, 246, 256, 266, 267, 268, 269,  27, 127, 227, 237, 247, 257, 267, 277, 278, 279, 
    28, 128, 228, 238, 248, 258, 268, 278, 288, 289,  29, 129, 229, 239, 249, 259, 269, 279, 289, 299, 
    3,  13,  23,  33,  34,  35,  36,  37,  38,  39,  13, 113, 123, 133, 134, 135, 136, 137, 138, 139, 
    23, 123, 223, 233, 234, 235, 236, 237, 238, 239,  33, 133, 233, 333, 334, 335, 336, 337, 338, 339, 
    34, 134, 234, 334, 344, 345, 346, 347, 348, 349,  35, 135, 235, 335, 345, 355, 356, 357, 358, 359, 
    36, 136, 236, 336, 346, 356, 366, 367, 368, 369,  37, 137, 237, 337, 347, 357, 367, 377, 378, 379, 
    38, 138, 238, 338, 348, 358, 368, 378, 388, 389,  39, 139, 239, 339, 349, 359, 369, 379, 389, 399, 
    4,  14,  24,  34,  44,  45,  46,  47,  48,  49,  14, 114, 124, 134, 144, 145, 146, 147, 148, 149, 
    24, 124, 224, 234, 244, 245, 246, 247, 248, 249,  34, 134, 234, 334, 344, 345, 346, 347, 348, 349, 
    44, 144, 244, 344, 444, 445, 446, 447, 448, 449,  45, 145, 245, 345, 445, 455, 456, 457, 458, 459, 
    46, 146, 246, 346, 446, 456, 466, 467, 468, 469,  47, 147, 247, 347, 447, 457, 467, 477, 478, 479, 
    48, 148, 248, 348, 448, 458, 468, 478, 488, 489,  49, 149, 249, 349, 449, 459, 469, 479, 489, 499, 
    5,  15,  25,  35,  45,  55,  56,  57,  58,  59,  15, 115, 125, 135, 145, 155, 156, 157, 158, 159, 
    25, 125, 225, 235, 245, 255, 256, 257, 258, 259,  35, 135, 235, 335, 345, 355, 356, 357, 358, 359, 
    45, 145, 245, 345, 445, 455, 456, 457, 458, 459,  55, 155, 255, 355, 455, 555, 556, 557, 558, 559, 
    56, 156, 256, 356, 456, 556, 566, 567, 568, 569,  57, 157, 257, 357, 457, 557, 567, 577, 578, 579, 
    58, 158, 258, 358, 458, 558, 568, 578, 588, 589,  59, 159, 259, 359, 459, 559, 569, 579, 589, 599, 
    6,  16,  26,  36,  46,  56,  66,  67,  68,  69,  16, 116, 126, 136, 146, 156, 166, 167, 168, 169, 
    26, 126, 226, 236, 246, 256, 266, 267, 268, 269,  36, 136, 236, 336, 346, 356, 366, 367, 368, 369, 
    46, 146, 246, 346, 446, 456, 466, 467, 468, 469,  56, 156, 256, 356, 456, 556, 566, 567, 568, 569, 
    66, 166, 266, 366, 466, 566, 666, 667, 668, 669,  67, 167, 267, 367, 467, 567, 667, 677, 678, 679, 
    68, 168, 268, 368, 468, 568, 668, 678, 688, 689,  69, 169, 269, 369, 469, 569, 669, 679, 689, 699, 
    7,  17,  27,  37,  47,  57,  67,  77,  78,  79,  17, 117, 127, 137, 147, 157, 167, 177, 178, 179, 
    27, 127, 227, 237, 247, 257, 267, 277, 278, 279,  37, 137, 237, 337, 347, 357, 367, 377, 378, 379, 
    47, 147, 247, 347, 447, 457, 467, 477, 478, 479,  57, 157, 257, 357, 457, 557, 567, 577, 578, 579, 
    67, 167, 267, 367, 467, 567, 667, 677, 678, 679,  77, 177, 277, 377, 477, 577, 677, 777, 778, 779, 
    78, 178, 278, 378, 478, 578, 678, 778, 788, 789,  79, 179, 279, 379, 479, 579, 679, 779, 789, 799, 
    8,  18,  28,  38,  48,  58,  68,  78,  88,  89,  18, 118, 128, 138, 148, 158, 168, 178, 188, 189, 
    28, 128, 228, 238, 248, 258, 268, 278, 288, 289,  38, 138, 238, 338, 348, 358, 368, 378, 388, 389, 
    48, 148, 248, 348, 448, 458, 468, 478, 488, 489,  58, 158, 258, 358, 458, 558, 568, 578, 588, 589, 
    68, 168, 268, 368, 468, 568, 668, 678, 688, 689,  78, 178, 278, 378, 478, 578, 678, 778, 788, 789, 
    88, 188, 288, 388, 488, 588, 688, 788, 888, 889,  89, 189, 289, 389, 489, 589, 689, 789, 889, 899, 
    9,  19,  29,  39,  49,  59,  69,  79,  89,  99,  19, 119, 129, 139, 149, 159, 169, 179, 189, 199, 
    29, 129, 229, 239, 249, 259, 269, 279, 289, 299,  39, 139, 239, 339, 349, 359, 369, 379, 389, 399, 
    49, 149, 249, 349, 449, 459, 469, 479, 489, 499,  59, 159, 259, 359, 459, 559, 569, 579, 589, 599, 
    69, 169, 269, 369, 469, 569, 669, 679, 689, 699,  79, 179, 279, 379, 479, 579, 679, 779, 789, 799, 
    89, 189, 289, 389, 489, 589, 689, 789, 889, 899,  99, 199, 299, 399, 499, 599, 699, 799, 899, 999, };

// 号码000-999 的类型值和变换类型值集合，高四位为类型值，低四位为变换类型值
const BYTE TYPE_VALUE[1000]={ 0, 18, 18, 18, 18, 17, 18, 18, 18, 18, 18, 18, 36, 54, 53, 51, 51, 53, 54, 36, 
    18, 36, 18, 54, 71, 67, 69, 67, 71, 54, 18, 54, 54, 18, 53, 67, 71, 71, 67, 53, 
    18, 53, 71, 53, 18, 51, 69, 71, 69, 51, 17, 51, 67, 67, 51, 17, 51, 67, 67, 51, 
    18, 51, 69, 71, 69, 51, 18, 53, 71, 53, 18, 53, 67, 71, 71, 67, 53, 18, 54, 54, 
    18, 54, 71, 67, 69, 67, 71, 54, 18, 36, 18, 36, 54, 53, 51, 51, 53, 54, 36, 18, 
    18, 18, 36, 54, 53, 51, 51, 53, 54, 36, 18,  0, 18, 18, 18, 18, 17, 18, 18, 18, 
    36, 18, 18, 36, 54, 53, 51, 51, 53, 54, 54, 18, 36, 18, 54, 71, 67, 69, 67, 71, 
    53, 18, 54, 54, 18, 53, 67, 71, 71, 67, 51, 18, 53, 71, 53, 18, 51, 69, 71, 69, 
    51, 17, 51, 67, 67, 51, 17, 51, 67, 67, 53, 18, 51, 69, 71, 69, 51, 18, 53, 71, 
    54, 18, 53, 67, 71, 71, 67, 53, 18, 54, 36, 18, 54, 71, 67, 69, 67, 71, 54, 18, 
    18, 36, 18, 54, 71, 67, 69, 67, 71, 54, 36, 18, 18, 36, 54, 53, 51, 51, 53, 54, 
    18, 18,  0, 18, 18, 18, 18, 17, 18, 18, 54, 36, 18, 18, 36, 54, 53, 51, 51, 53, 
    71, 54, 18, 36, 18, 54, 71, 67, 69, 67, 67, 53, 18, 54, 54, 18, 53, 67, 71, 71, 
    69, 51, 18, 53, 71, 53, 18, 51, 69, 71, 67, 51, 17, 51, 67, 67, 51, 17, 51, 67, 
    71, 53, 18, 51, 69, 71, 69, 51, 18, 53, 54, 54, 18, 53, 67, 71, 71, 67, 53, 18, 
    18, 54, 54, 18, 53, 67, 71, 71, 67, 53, 54, 18, 36, 18, 54, 71, 67, 69, 67, 71, 
    54, 36, 18, 18, 36, 54, 53, 51, 51, 53, 18, 18, 18,  0, 18, 18, 18, 18, 17, 18, 
    53, 54, 36, 18, 18, 36, 54, 53, 51, 51, 67, 71, 54, 18, 36, 18, 54, 71, 67, 69, 
    71, 67, 53, 18, 54, 54, 18, 53, 67, 71, 71, 69, 51, 18, 53, 71, 53, 18, 51, 69, 
    67, 67, 51, 17, 51, 67, 67, 51, 17, 51, 53, 71, 53, 18, 51, 69, 71, 69, 51, 18, 
    18, 53, 71, 53, 18, 51, 69, 71, 69, 51, 53, 18, 54, 54, 18, 53, 67, 71, 71, 67, 
    71, 54, 18, 36, 18, 54, 71, 67, 69, 67, 53, 54, 36, 18, 18, 36, 54, 53, 51, 51, 
    18, 18, 18, 18,  0, 18, 18, 18, 18, 17, 51, 53, 54, 36, 18, 18, 36, 54, 53, 51, 
    69, 67, 71, 54, 18, 36, 18, 54, 71, 67, 71, 71, 67, 53, 18, 54, 54, 18, 53, 67, 
    69, 71, 69, 51, 18, 53, 71, 53, 18, 51, 51, 67, 67, 51, 17, 51, 67, 67, 51, 17, 
    17, 51, 67, 67, 51, 17, 51, 67, 67, 51, 51, 18, 53, 71, 53, 18, 51, 69, 71, 69, 
    67, 53, 18, 54, 54, 18, 53, 67, 71, 71, 67, 71, 54, 18, 36, 18, 54, 71, 67, 69, 
    51, 53, 54, 36, 18, 18, 36, 54, 53, 51, 17, 18, 18, 18, 18,  0, 18, 18, 18, 18, 
    51, 51, 53, 54, 36, 18, 18, 36, 54, 53, 67, 69, 67, 71, 54, 18, 36, 18, 54, 71, 
    67, 71, 71, 67, 53, 18, 54, 54, 18, 53, 51, 69, 71, 69, 51, 18, 53, 71, 53, 18, 
    18, 51, 69, 71, 69, 51, 18, 53, 71, 53, 51, 17, 51, 67, 67, 51, 17, 51, 67, 67, 
    69, 51, 18, 53, 71, 53, 18, 51, 69, 71, 71, 67, 53, 18, 54, 54, 18, 53, 67, 71, 
    69, 67, 71, 54, 18, 36, 18, 54, 71, 67, 51, 51, 53, 54, 36, 18, 18, 36, 54, 53, 
    18, 17, 18, 18, 18, 18,  0, 18, 18, 18, 53, 51, 51, 53, 54, 36, 18, 18, 36, 54, 
    71, 67, 69, 67, 71, 54, 18, 36, 18, 54, 53, 67, 71, 71, 67, 53, 18, 54, 54, 18, 
    18, 53, 67, 71, 71, 67, 53, 18, 54, 54, 53, 18, 51, 69, 71, 69, 51, 18, 53, 71, 
    67, 51, 17, 51, 67, 67, 51, 17, 51, 67, 71, 69, 51, 18, 53, 71, 53, 18, 51, 69, 
    71, 71, 67, 53, 18, 54, 54, 18, 53, 67, 67, 69, 67, 71, 54, 18, 36, 18, 54, 71, 
    53, 51, 51, 53, 54, 36, 18, 18, 36, 54, 18, 18, 17, 18, 18, 18, 18,  0, 18, 18, 
    54, 53, 51, 51, 53, 54, 36, 18, 18, 36, 54, 71, 67, 69, 67, 71, 54, 18, 36, 18, 
    18, 54, 71, 67, 69, 67, 71, 54, 18, 36, 54, 18, 53, 67, 71, 71, 67, 53, 18, 54, 
    71, 53, 18, 51, 69, 71, 69, 51, 18, 53, 67, 67, 51, 17, 51, 67, 67, 51, 17, 51, 
    69, 71, 69, 51, 18, 53, 71, 53, 18, 51, 67, 71, 71, 67, 53, 18, 54, 54, 18, 53, 
    71, 67, 69, 67, 71, 54, 18, 36, 18, 54, 54, 53, 51, 51, 53, 54, 36, 18, 18, 36, 
    18, 18, 18, 17, 18, 18, 18, 18,  0, 18, 36, 54, 53, 51, 51, 53, 54, 36, 18, 18, 
    18, 36, 54, 53, 51, 51, 53, 54, 36, 18, 36, 18, 54, 71, 67, 69, 67, 71, 54, 18, 
    54, 54, 18, 53, 67, 71, 71, 67, 53, 18, 53, 71, 53, 18, 51, 69, 71, 69, 51, 18, 
    51, 67, 67, 51, 17, 51, 67, 67, 51, 17, 51, 69, 71, 69, 51, 18, 53, 71, 53, 18, 
    53, 67, 71, 71, 67, 53, 18, 54, 54, 18, 54, 71, 67, 69, 67, 71, 54, 18, 36, 18, 
    36, 54, 53, 51, 51, 53, 54, 36, 18, 18, 18, 18, 18, 18, 17, 18, 18, 18, 18,  0};

// 号码000-999 的形态值和细分形态值集合，高四位为形态值，低四位为细分形态值
const BYTE STATE_VALUE[1000]={ 0, 17, 17, 17, 17, 17, 17, 17, 17, 17, 56, 19, 18, 18, 18, 18, 18, 18, 18, 18, 
    56, 57, 19, 18, 18, 18, 18, 18, 18, 18, 56, 57, 57, 19, 18, 18, 18, 18, 18, 18, 
    56, 57, 57, 57, 19, 18, 18, 18, 18, 18, 56, 57, 57, 57, 57, 19, 18, 18, 18, 18, 
    56, 57, 57, 57, 57, 57, 19, 18, 18, 18, 56, 57, 57, 57, 57, 57, 57, 19, 18, 18, 
    56, 57, 57, 57, 57, 57, 57, 57, 19, 18, 56, 57, 57, 57, 57, 57, 57, 57, 57, 19, 
    38, 75, 76, 76, 76, 76, 76, 76, 76, 76, 36,  0, 17, 17, 17, 17, 17, 17, 17, 17, 
    55, 56, 19, 18, 18, 18, 18, 18, 18, 18, 55, 56, 57, 19, 18, 18, 18, 18, 18, 18, 
    55, 56, 57, 57, 19, 18, 18, 18, 18, 18, 55, 56, 57, 57, 57, 19, 18, 18, 18, 18, 
    55, 56, 57, 57, 57, 57, 19, 18, 18, 18, 55, 56, 57, 57, 57, 57, 57, 19, 18, 18, 
    55, 56, 57, 57, 57, 57, 57, 57, 19, 18, 55, 56, 57, 57, 57, 57, 57, 57, 57, 19, 
    38, 74, 75, 76, 76, 76, 76, 76, 76, 76, 37, 38, 75, 76, 76, 76, 76, 76, 76, 76, 
    36, 36,  0, 17, 17, 17, 17, 17, 17, 17, 55, 55, 56, 19, 18, 18, 18, 18, 18, 18, 
    55, 55, 56, 57, 19, 18, 18, 18, 18, 18, 55, 55, 56, 57, 57, 19, 18, 18, 18, 18, 
    55, 55, 56, 57, 57, 57, 19, 18, 18, 18, 55, 55, 56, 57, 57, 57, 57, 19, 18, 18, 
    55, 55, 56, 57, 57, 57, 57, 57, 19, 18, 55, 55, 56, 57, 57, 57, 57, 57, 57, 19, 
    38, 74, 74, 75, 76, 76, 76, 76, 76, 76, 37, 38, 74, 75, 76, 76, 76, 76, 76, 76, 
    37, 37, 38, 75, 76, 76, 76, 76, 76, 76, 36, 36, 36,  0, 17, 17, 17, 17, 17, 17, 
    55, 55, 55, 56, 19, 18, 18, 18, 18, 18, 55, 55, 55, 56, 57, 19, 18, 18, 18, 18, 
    55, 55, 55, 56, 57, 57, 19, 18, 18, 18, 55, 55, 55, 56, 57, 57, 57, 19, 18, 18, 
    55, 55, 55, 56, 57, 57, 57, 57, 19, 18, 55, 55, 55, 56, 57, 57, 57, 57, 57, 19, 
    38, 74, 74, 74, 75, 76, 76, 76, 76, 76, 37, 38, 74, 74, 75, 76, 76, 76, 76, 76, 
    37, 37, 38, 74, 75, 76, 76, 76, 76, 76, 37, 37, 37, 38, 75, 76, 76, 76, 76, 76, 
    36, 36, 36, 36,  0, 17, 17, 17, 17, 17, 55, 55, 55, 55, 56, 19, 18, 18, 18, 18, 
    55, 55, 55, 55, 56, 57, 19, 18, 18, 18, 55, 55, 55, 55, 56, 57, 57, 19, 18, 18, 
    55, 55, 55, 55, 56, 57, 57, 57, 19, 18, 55, 55, 55, 55, 56, 57, 57, 57, 57, 19, 
    38, 74, 74, 74, 74, 75, 76, 76, 76, 76, 37, 38, 74, 74, 74, 75, 76, 76, 76, 76, 
    37, 37, 38, 74, 74, 75, 76, 76, 76, 76, 37, 37, 37, 38, 74, 75, 76, 76, 76, 76, 
    37, 37, 37, 37, 38, 75, 76, 76, 76, 76, 36, 36, 36, 36, 36,  0, 17, 17, 17, 17, 
    55, 55, 55, 55, 55, 56, 19, 18, 18, 18, 55, 55, 55, 55, 55, 56, 57, 19, 18, 18, 
    55, 55, 55, 55, 55, 56, 57, 57, 19, 18, 55, 55, 55, 55, 55, 56, 57, 57, 57, 19, 
    38, 74, 74, 74, 74, 74, 75, 76, 76, 76, 37, 38, 74, 74, 74, 74, 75, 76, 76, 76, 
    37, 37, 38, 74, 74, 74, 75, 76, 76, 76, 37, 37, 37, 38, 74, 74, 75, 76, 76, 76, 
    37, 37, 37, 37, 38, 74, 75, 76, 76, 76, 37, 37, 37, 37, 37, 38, 75, 76, 76, 76, 
    36, 36, 36, 36, 36, 36,  0, 17, 17, 17, 55, 55, 55, 55, 55, 55, 56, 19, 18, 18, 
    55, 55, 55, 55, 55, 55, 56, 57, 19, 18, 55, 55, 55, 55, 55, 55, 56, 57, 57, 19, 
    38, 74, 74, 74, 74, 74, 74, 75, 76, 76, 37, 38, 74, 74, 74, 74, 74, 75, 76, 76, 
    37, 37, 38, 74, 74, 74, 74, 75, 76, 76, 37, 37, 37, 38, 74, 74, 74, 75, 76, 76, 
    37, 37, 37, 37, 38, 74, 74, 75, 76, 76, 37, 37, 37, 37, 37, 38, 74, 75, 76, 76, 
    37, 37, 37, 37, 37, 37, 38, 75, 76, 76, 36, 36, 36, 36, 36, 36, 36,  0, 17, 17, 
    55, 55, 55, 55, 55, 55, 55, 56, 19, 18, 55, 55, 55, 55, 55, 55, 55, 56, 57, 19, 
    38, 74, 74, 74, 74, 74, 74, 74, 75, 76, 37, 38, 74, 74, 74, 74, 74, 74, 75, 76, 
    37, 37, 38, 74, 74, 74, 74, 74, 75, 76, 37, 37, 37, 38, 74, 74, 74, 74, 75, 76, 
    37, 37, 37, 37, 38, 74, 74, 74, 75, 76, 37, 37, 37, 37, 37, 38, 74, 74, 75, 76, 
    37, 37, 37, 37, 37, 37, 38, 74, 75, 76, 37, 37, 37, 37, 37, 37, 37, 38, 75, 76, 
    36, 36, 36, 36, 36, 36, 36, 36,  0, 17, 55, 55, 55, 55, 55, 55, 55, 55, 56, 19, 
    38, 74, 74, 74, 74, 74, 74, 74, 74, 75, 37, 38, 74, 74, 74, 74, 74, 74, 74, 75, 
    37, 37, 38, 74, 74, 74, 74, 74, 74, 75, 37, 37, 37, 38, 74, 74, 74, 74, 74, 75, 
    37, 37, 37, 37, 38, 74, 74, 74, 74, 75, 37, 37, 37, 37, 37, 38, 74, 74, 74, 75, 
    37, 37, 37, 37, 37, 37, 38, 74, 74, 75, 37, 37, 37, 37, 37, 37, 37, 38, 74, 75, 
    37, 37, 37, 37, 37, 37, 37, 37, 38, 75, 36, 36, 36, 36, 36, 36, 36, 36, 36,  0 };

// 号码000-999 的组三类型（高2位）、半顺类型（第3、4位）、顺子类型值集合（低4位）
const BYTE AAB_ABD_ABC_VALUE[1000]={182,  54,  54,  54,  54,  54,  54,  54,  54,  54, 118, 182, 240, 198, 198, 198, 198, 198, 198, 243, 
    118, 241, 182, 230, 246, 246, 246, 246, 246, 214, 118, 214, 230, 182, 230, 246, 246, 246, 246, 214, 
    118, 214, 246, 230, 182, 230, 246, 246, 246, 214, 118, 214, 246, 246, 230, 182, 230, 246, 246, 214, 
    118, 214, 246, 246, 246, 230, 182, 230, 246, 214, 118, 214, 246, 246, 246, 246, 230, 182, 230, 214, 
    118, 214, 246, 246, 246, 246, 246, 230, 182, 244, 118, 242, 198, 198, 198, 198, 198, 198, 245, 182, 
    182, 118, 242, 198, 198, 198, 198, 198, 198, 245,  54, 182,  54,  54,  54,  54,  54,  54,  54,  54, 
    243, 118, 182, 240, 198, 198, 198, 198, 198, 198, 214, 118, 241, 182, 230, 246, 246, 246, 246, 246, 
    214, 118, 214, 230, 182, 230, 246, 246, 246, 246, 214, 118, 214, 246, 230, 182, 230, 246, 246, 246, 
    214, 118, 214, 246, 246, 230, 182, 230, 246, 246, 214, 118, 214, 246, 246, 246, 230, 182, 230, 246, 
    214, 118, 214, 246, 246, 246, 246, 230, 182, 230, 244, 118, 214, 246, 246, 246, 246, 246, 230, 182, 
    182, 244, 118, 214, 246, 246, 246, 246, 246, 230, 245, 182, 118, 242, 198, 198, 198, 198, 198, 198, 
    54,  54, 182,  54,  54,  54,  54,  54,  54,  54, 198, 243, 118, 182, 240, 198, 198, 198, 198, 198, 
    246, 214, 118, 241, 182, 230, 246, 246, 246, 246, 246, 214, 118, 214, 230, 182, 230, 246, 246, 246, 
    246, 214, 118, 214, 246, 230, 182, 230, 246, 246, 246, 214, 118, 214, 246, 246, 230, 182, 230, 246, 
    246, 214, 118, 214, 246, 246, 246, 230, 182, 230, 230, 214, 118, 214, 246, 246, 246, 246, 230, 182, 
    182, 230, 214, 118, 214, 246, 246, 246, 246, 230, 230, 182, 244, 118, 214, 246, 246, 246, 246, 246, 
    198, 245, 182, 118, 242, 198, 198, 198, 198, 198,  54,  54,  54, 182,  54,  54,  54,  54,  54,  54, 
    198, 198, 243, 118, 182, 240, 198, 198, 198, 198, 246, 246, 214, 118, 241, 182, 230, 246, 246, 246, 
    246, 246, 214, 118, 214, 230, 182, 230, 246, 246, 246, 246, 214, 118, 214, 246, 230, 182, 230, 246, 
    246, 246, 214, 118, 214, 246, 246, 230, 182, 230, 230, 246, 214, 118, 214, 246, 246, 246, 230, 182, 
    182, 230, 246, 214, 118, 214, 246, 246, 246, 230, 230, 182, 230, 214, 118, 214, 246, 246, 246, 246, 
    246, 230, 182, 244, 118, 214, 246, 246, 246, 246, 198, 198, 245, 182, 118, 242, 198, 198, 198, 198, 
    54,  54,  54,  54, 182,  54,  54,  54,  54,  54, 198, 198, 198, 243, 118, 182, 240, 198, 198, 198, 
    246, 246, 246, 214, 118, 241, 182, 230, 246, 246, 246, 246, 246, 214, 118, 214, 230, 182, 230, 246, 
    246, 246, 246, 214, 118, 214, 246, 230, 182, 230, 230, 246, 246, 214, 118, 214, 246, 246, 230, 182, 
    182, 230, 246, 246, 214, 118, 214, 246, 246, 230, 230, 182, 230, 246, 214, 118, 214, 246, 246, 246, 
    246, 230, 182, 230, 214, 118, 214, 246, 246, 246, 246, 246, 230, 182, 244, 118, 214, 246, 246, 246, 
    198, 198, 198, 245, 182, 118, 242, 198, 198, 198,  54,  54,  54,  54,  54, 182,  54,  54,  54,  54, 
    198, 198, 198, 198, 243, 118, 182, 240, 198, 198, 246, 246, 246, 246, 214, 118, 241, 182, 230, 246, 
    246, 246, 246, 246, 214, 118, 214, 230, 182, 230, 230, 246, 246, 246, 214, 118, 214, 246, 230, 182, 
    182, 230, 246, 246, 246, 214, 118, 214, 246, 230, 230, 182, 230, 246, 246, 214, 118, 214, 246, 246, 
    246, 230, 182, 230, 246, 214, 118, 214, 246, 246, 246, 246, 230, 182, 230, 214, 118, 214, 246, 246, 
    246, 246, 246, 230, 182, 244, 118, 214, 246, 246, 198, 198, 198, 198, 245, 182, 118, 242, 198, 198, 
    54,  54,  54,  54,  54,  54, 182,  54,  54,  54, 198, 198, 198, 198, 198, 243, 118, 182, 240, 198, 
    246, 246, 246, 246, 246, 214, 118, 241, 182, 230, 230, 246, 246, 246, 246, 214, 118, 214, 230, 182, 
    182, 230, 246, 246, 246, 246, 214, 118, 214, 230, 230, 182, 230, 246, 246, 246, 214, 118, 214, 246, 
    246, 230, 182, 230, 246, 246, 214, 118, 214, 246, 246, 246, 230, 182, 230, 246, 214, 118, 214, 246, 
    246, 246, 246, 230, 182, 230, 214, 118, 214, 246, 246, 246, 246, 246, 230, 182, 244, 118, 214, 246, 
    198, 198, 198, 198, 198, 245, 182, 118, 242, 198,  54,  54,  54,  54,  54,  54,  54, 182,  54,  54, 
    198, 198, 198, 198, 198, 198, 243, 118, 182, 240, 230, 246, 246, 246, 246, 246, 214, 118, 241, 182, 
    182, 230, 246, 246, 246, 246, 246, 214, 118, 241, 230, 182, 230, 246, 246, 246, 246, 214, 118, 214, 
    246, 230, 182, 230, 246, 246, 246, 214, 118, 214, 246, 246, 230, 182, 230, 246, 246, 214, 118, 214, 
    246, 246, 246, 230, 182, 230, 246, 214, 118, 214, 246, 246, 246, 246, 230, 182, 230, 214, 118, 214, 
    246, 246, 246, 246, 246, 230, 182, 244, 118, 214, 198, 198, 198, 198, 198, 198, 245, 182, 118, 242, 
    54,  54,  54,  54,  54,  54,  54,  54, 182,  54, 240, 198, 198, 198, 198, 198, 198, 243, 118, 182, 
    182, 240, 198, 198, 198, 198, 198, 198, 243, 118, 241, 182, 230, 246, 246, 246, 246, 246, 214, 118, 
    214, 230, 182, 230, 246, 246, 246, 246, 214, 118, 214, 246, 230, 182, 230, 246, 246, 246, 214, 118, 
    214, 246, 246, 230, 182, 230, 246, 246, 214, 118, 214, 246, 246, 246, 230, 182, 230, 246, 214, 118, 
    214, 246, 246, 246, 246, 230, 182, 230, 214, 118, 214, 246, 246, 246, 246, 246, 230, 182, 244, 118, 
    242, 198, 198, 198, 198, 198, 198, 245, 182, 118,  54,  54,  54,  54,  54,  54,  54,  54,  54, 182};

// 号码000-999 的相对大中小排列（百十个依次，0为小，1为中，2为大）
const BYTE SELFDZX_VALUE[1000]={111,   2,   2,   2,   2,   2,   2,   2,   2,   2,  20,  22,  12,  12,  12,  12,  12,  12,  12,  12, 
    20,  21,  22,  12,  12,  12,  12,  12,  12,  12,  20,  21,  21,  22,  12,  12,  12,  12,  12,  12, 
    20,  21,  21,  21,  22,  12,  12,  12,  12,  12,  20,  21,  21,  21,  21,  22,  12,  12,  12,  12, 
    20,  21,  21,  21,  21,  21,  22,  12,  12,  12,  20,  21,  21,  21,  21,  21,  21,  22,  12,  12, 
    20,  21,  21,  21,  21,  21,  21,  21,  22,  12,  20,  21,  21,  21,  21,  21,  21,  21,  21,  22, 
    200, 202, 102, 102, 102, 102, 102, 102, 102, 102, 220, 111,   2,   2,   2,   2,   2,   2,   2,   2, 
    120,  20,  22,  12,  12,  12,  12,  12,  12,  12, 120,  20,  21,  22,  12,  12,  12,  12,  12,  12, 
    120,  20,  21,  21,  22,  12,  12,  12,  12,  12, 120,  20,  21,  21,  21,  22,  12,  12,  12,  12, 
    120,  20,  21,  21,  21,  21,  22,  12,  12,  12, 120,  20,  21,  21,  21,  21,  21,  22,  12,  12, 
    120,  20,  21,  21,  21,  21,  21,  21,  22,  12, 120,  20,  21,  21,  21,  21,  21,  21,  21,  22, 
    200, 201, 202, 102, 102, 102, 102, 102, 102, 102, 210, 200, 202, 102, 102, 102, 102, 102, 102, 102, 
    220, 220, 111,   2,   2,   2,   2,   2,   2,   2, 120, 120,  20,  22,  12,  12,  12,  12,  12,  12, 
    120, 120,  20,  21,  22,  12,  12,  12,  12,  12, 120, 120,  20,  21,  21,  22,  12,  12,  12,  12, 
    120, 120,  20,  21,  21,  21,  22,  12,  12,  12, 120, 120,  20,  21,  21,  21,  21,  22,  12,  12, 
    120, 120,  20,  21,  21,  21,  21,  21,  22,  12, 120, 120,  20,  21,  21,  21,  21,  21,  21,  22, 
    200, 201, 201, 202, 102, 102, 102, 102, 102, 102, 210, 200, 201, 202, 102, 102, 102, 102, 102, 102, 
    210, 210, 200, 202, 102, 102, 102, 102, 102, 102, 220, 220, 220, 111,   2,   2,   2,   2,   2,   2, 
    120, 120, 120,  20,  22,  12,  12,  12,  12,  12, 120, 120, 120,  20,  21,  22,  12,  12,  12,  12, 
    120, 120, 120,  20,  21,  21,  22,  12,  12,  12, 120, 120, 120,  20,  21,  21,  21,  22,  12,  12, 
    120, 120, 120,  20,  21,  21,  21,  21,  22,  12, 120, 120, 120,  20,  21,  21,  21,  21,  21,  22, 
    200, 201, 201, 201, 202, 102, 102, 102, 102, 102, 210, 200, 201, 201, 202, 102, 102, 102, 102, 102, 
    210, 210, 200, 201, 202, 102, 102, 102, 102, 102, 210, 210, 210, 200, 202, 102, 102, 102, 102, 102, 
    220, 220, 220, 220, 111,   2,   2,   2,   2,   2, 120, 120, 120, 120,  20,  22,  12,  12,  12,  12, 
    120, 120, 120, 120,  20,  21,  22,  12,  12,  12, 120, 120, 120, 120,  20,  21,  21,  22,  12,  12, 
    120, 120, 120, 120,  20,  21,  21,  21,  22,  12, 120, 120, 120, 120,  20,  21,  21,  21,  21,  22, 
    200, 201, 201, 201, 201, 202, 102, 102, 102, 102, 210, 200, 201, 201, 201, 202, 102, 102, 102, 102, 
    210, 210, 200, 201, 201, 202, 102, 102, 102, 102, 210, 210, 210, 200, 201, 202, 102, 102, 102, 102, 
    210, 210, 210, 210, 200, 202, 102, 102, 102, 102, 220, 220, 220, 220, 220, 111,   2,   2,   2,   2, 
    120, 120, 120, 120, 120,  20,  22,  12,  12,  12, 120, 120, 120, 120, 120,  20,  21,  22,  12,  12, 
    120, 120, 120, 120, 120,  20,  21,  21,  22,  12, 120, 120, 120, 120, 120,  20,  21,  21,  21,  22, 
    200, 201, 201, 201, 201, 201, 202, 102, 102, 102, 210, 200, 201, 201, 201, 201, 202, 102, 102, 102, 
    210, 210, 200, 201, 201, 201, 202, 102, 102, 102, 210, 210, 210, 200, 201, 201, 202, 102, 102, 102, 
    210, 210, 210, 210, 200, 201, 202, 102, 102, 102, 210, 210, 210, 210, 210, 200, 202, 102, 102, 102, 
    220, 220, 220, 220, 220, 220, 111,   2,   2,   2, 120, 120, 120, 120, 120, 120,  20,  22,  12,  12, 
    120, 120, 120, 120, 120, 120,  20,  21,  22,  12, 120, 120, 120, 120, 120, 120,  20,  21,  21,  22, 
    200, 201, 201, 201, 201, 201, 201, 202, 102, 102, 210, 200, 201, 201, 201, 201, 201, 202, 102, 102, 
    210, 210, 200, 201, 201, 201, 201, 202, 102, 102, 210, 210, 210, 200, 201, 201, 201, 202, 102, 102, 
    210, 210, 210, 210, 200, 201, 201, 202, 102, 102, 210, 210, 210, 210, 210, 200, 201, 202, 102, 102, 
    210, 210, 210, 210, 210, 210, 200, 202, 102, 102, 220, 220, 220, 220, 220, 220, 220, 111,   2,   2, 
    120, 120, 120, 120, 120, 120, 120,  20,  22,  12, 120, 120, 120, 120, 120, 120, 120,  20,  21,  22, 
    200, 201, 201, 201, 201, 201, 201, 201, 202, 102, 210, 200, 201, 201, 201, 201, 201, 201, 202, 102, 
    210, 210, 200, 201, 201, 201, 201, 201, 202, 102, 210, 210, 210, 200, 201, 201, 201, 201, 202, 102, 
    210, 210, 210, 210, 200, 201, 201, 201, 202, 102, 210, 210, 210, 210, 210, 200, 201, 201, 202, 102, 
    210, 210, 210, 210, 210, 210, 200, 201, 202, 102, 210, 210, 210, 210, 210, 210, 210, 200, 202, 102, 
    220, 220, 220, 220, 220, 220, 220, 220, 111,   2, 120, 120, 120, 120, 120, 120, 120, 120,  20,  22, 
    200, 201, 201, 201, 201, 201, 201, 201, 201, 202, 210, 200, 201, 201, 201, 201, 201, 201, 201, 202, 
    210, 210, 200, 201, 201, 201, 201, 201, 201, 202, 210, 210, 210, 200, 201, 201, 201, 201, 201, 202, 
    210, 210, 210, 210, 200, 201, 201, 201, 201, 202, 210, 210, 210, 210, 210, 200, 201, 201, 201, 202, 
    210, 210, 210, 210, 210, 210, 200, 201, 201, 202, 210, 210, 210, 210, 210, 210, 210, 200, 201, 202, 
    210, 210, 210, 210, 210, 210, 210, 210, 200, 202, 220, 220, 220, 220, 220, 220, 220, 220, 220, 111};
