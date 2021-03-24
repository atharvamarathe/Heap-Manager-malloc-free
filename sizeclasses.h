#ifndef SIZECLASSES_H
#define SIZECLASSES_H


#define STRINGIFY(a,b) a ## b
#define GETCLASSINDEX(a) STRINGIFY(CLASS_,a)

// #define STRINGIFY(a,b) a ## b
// #define GETCLASS_SIZE(a,b) STRINGIFY(a,b)
// #define GET_CLASS_SIZE(a) GETCLASS_SIZE(SIZECLASS,a)  

#define NUM_OF_CLASSES 42
//  __________________________________________________________________________
//  |  | CLASS-NAME  |   INDEX_OF_PAGE_LIST  | Default no of Pages Allocated |  
//  -------------------------------------------------------------------------|
#define CLASS_4                 0           //              5                |           
#define CLASS_8                 1           //              5                |                          
#define CLASS_16                2           //              5                |
#define CLASS_24                3           //              5                |
#define CLASS_32                4           //              5                |
#define CLASS_40                5           //              5                |
#define CLASS_48                6           //              5                |
#define CLASS_56                7           //              5                |
#define CLASS_64                8           //              5                |
#define CLASS_72                9           //              5                |
#define CLASS_80                10          //              5                |
#define CLASS_88                11          //              5                |
#define CLASS_96                12          //              5                |
#define CLASS_104               13          //              5                |
#define CLASS_112               14          //              5                |
#define CLASS_120               15          //              5                |
#define CLASS_128               16          //              5                |
#define CLASS_136               17          //              5                |
#define CLASS_144               18          //              5                |
#define CLASS_160               19          //              5                |
#define CLASS_176               20          //              5                |
#define CLASS_184               21          //              5                |
#define CLASS_192               22          //              5                |
#define CLASS_208               23          //              5                |
#define CLASS_224               24          //              5                |
#define CLASS_240               25          //              5                |
#define CLASS_256               26          //              5                |
#define CLASS_272               27          //              5                |
#define CLASS_288               28          //              5                |
#define CLASS_312               29          //              5                |
#define CLASS_336               30          //              5                |
#define CLASS_368               31          //              5                |
#define CLASS_408               32          //              5                |
#define CLASS_448               33          //              5                |
#define CLASS_480               34          //              5                |
#define CLASS_512               35          //              5                |
#define CLASS_576               36          //              5                |
#define CLASS_640               37          //              5                |
#define CLASS_704               38          //              5                |
#define CLASS_768               39          //              5                |
#define CLASS_896               40          //              5                |
#define CLASS_1024              41          //              10               |
//---------------------------------------------------------------------------|



#define SIZECLASS4      4
#define SIZECLASS8      8
#define SIZECLASS16     16
#define SIZECLASS24     24
#define SIZECLASS32     32
#define SIZECLASS40     40
#define SIZECLASS48     48
#define SIZECLASS56     56
#define SIZECLASS64     64
#define SIZECLASS72     72
#define SIZECLASS80     80
#define SIZECLASS88     88
#define SIZECLASS96     96
#define SIZECLASS104    104
#define SIZECLASS112    112
#define SIZECLASS120    120
#define SIZECLASS128    128


#endif