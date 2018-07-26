/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.4.0-dev at Thu Jul 26 21:26:51 2018. */

#include "MS3000.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

const int32_t RGB_R_default = 0;
const int32_t RGB_G_default = 0;
const int32_t RGB_B_default = 0;


const pb_field_t RGB_fields[4] = {
    PB_FIELD(  1, INT32   , REQUIRED, STATIC  , FIRST, RGB, R, R, &RGB_R_default),
    PB_FIELD(  2, INT32   , REQUIRED, STATIC  , OTHER, RGB, G, R, &RGB_G_default),
    PB_FIELD(  3, INT32   , REQUIRED, STATIC  , OTHER, RGB, B, G, &RGB_B_default),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_fields[2] = {
    PB_FIELD(  1, MESSAGE , REQUIRED, STATIC  , FIRST, MS3KG, modes, modes, &MS3KG_Modes_fields),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_Modes_fields[3] = {
    PB_FIELD(  1, STRING  , REQUIRED, CALLBACK, FIRST, MS3KG_Modes, current, current, 0),
    PB_FIELD(  2, MESSAGE , REQUIRED, STATIC  , OTHER, MS3KG_Modes, light, current, &MS3KG_Modes_Light_fields),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_Modes_MagicPOV_fields[2] = {
    PB_FIELD(  1, STRING  , REQUIRED, CALLBACK, FIRST, MS3KG_Modes_MagicPOV, name, name, 0),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_Modes_MagicPOV_User_fields[2] = {
    PB_FIELD(  2, STRING  , REQUIRED, CALLBACK, FIRST, MS3KG_Modes_MagicPOV_User, image, image, 0),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_Modes_Light_fields[4] = {
    PB_FIELD(  1, STRING  , REQUIRED, CALLBACK, FIRST, MS3KG_Modes_Light, name, name, 0),
    PB_FIELD(  2, UENUM   , REQUIRED, STATIC  , OTHER, MS3KG_Modes_Light, subMode, name, 0),
    PB_FIELD(  3, MESSAGE , REQUIRED, STATIC  , OTHER, MS3KG_Modes_Light, color, subMode, &RGB_fields),
    PB_LAST_FIELD
};

const pb_field_t MS3KG_Modes_Arpi_fields[2] = {
    PB_FIELD(  1, STRING  , REQUIRED, CALLBACK, FIRST, MS3KG_Modes_Arpi, name, name, 0),
    PB_LAST_FIELD
};



/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(MS3KG, modes) < 65536 && pb_membersize(MS3KG_Modes, light) < 65536 && pb_membersize(MS3KG_Modes_Light, color) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_RGB_MS3KG_MS3KG_Modes_MS3KG_Modes_MagicPOV_MS3KG_Modes_MagicPOV_User_MS3KG_Modes_Light_MS3KG_Modes_Arpi)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_16BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(MS3KG, modes) < 256 && pb_membersize(MS3KG_Modes, light) < 256 && pb_membersize(MS3KG_Modes_Light, color) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_RGB_MS3KG_MS3KG_Modes_MS3KG_Modes_MagicPOV_MS3KG_Modes_MagicPOV_User_MS3KG_Modes_Light_MS3KG_Modes_Arpi)
#endif


/* @@protoc_insertion_point(eof) */
