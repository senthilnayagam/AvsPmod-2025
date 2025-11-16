
#include <Python.h>
#include <stddef.h>

/* this block of #ifs should be kept exactly identical between
   c/_cffi_backend.c, cffi/vengine_cpy.py, cffi/vengine_gen.py
   and cffi/_cffi_include.h */
#if defined(_MSC_VER)
# include <malloc.h>   /* for alloca() */
# if _MSC_VER < 1600   /* MSVC < 2010 */
   typedef __int8 int8_t;
   typedef __int16 int16_t;
   typedef __int32 int32_t;
   typedef __int64 int64_t;
   typedef unsigned __int8 uint8_t;
   typedef unsigned __int16 uint16_t;
   typedef unsigned __int32 uint32_t;
   typedef unsigned __int64 uint64_t;
   typedef __int8 int_least8_t;
   typedef __int16 int_least16_t;
   typedef __int32 int_least32_t;
   typedef __int64 int_least64_t;
   typedef unsigned __int8 uint_least8_t;
   typedef unsigned __int16 uint_least16_t;
   typedef unsigned __int32 uint_least32_t;
   typedef unsigned __int64 uint_least64_t;
   typedef __int8 int_fast8_t;
   typedef __int16 int_fast16_t;
   typedef __int32 int_fast32_t;
   typedef __int64 int_fast64_t;
   typedef unsigned __int8 uint_fast8_t;
   typedef unsigned __int16 uint_fast16_t;
   typedef unsigned __int32 uint_fast32_t;
   typedef unsigned __int64 uint_fast64_t;
   typedef __int64 intmax_t;
   typedef unsigned __int64 uintmax_t;
# else
#  include <stdint.h>
# endif
# if _MSC_VER < 1800   /* MSVC < 2013 */
#  ifndef __cplusplus
    typedef unsigned char _Bool;
#  endif
# endif
# define _cffi_float_complex_t   _Fcomplex    /* include <complex.h> for it */
# define _cffi_double_complex_t  _Dcomplex    /* include <complex.h> for it */
#else
# include <stdint.h>
# if (defined (__SVR4) && defined (__sun)) || defined(_AIX) || defined(__hpux)
#  include <alloca.h>
# endif
# define _cffi_float_complex_t   float _Complex
# define _cffi_double_complex_t  double _Complex
#endif

#if PY_MAJOR_VERSION < 3
# undef PyCapsule_CheckExact
# undef PyCapsule_GetPointer
# define PyCapsule_CheckExact(capsule) (PyCObject_Check(capsule))
# define PyCapsule_GetPointer(capsule, name) \
    (PyCObject_AsVoidPtr(capsule))
#endif

#if PY_MAJOR_VERSION >= 3
# define PyInt_FromLong PyLong_FromLong
#endif

#define _cffi_from_c_double PyFloat_FromDouble
#define _cffi_from_c_float PyFloat_FromDouble
#define _cffi_from_c_long PyInt_FromLong
#define _cffi_from_c_ulong PyLong_FromUnsignedLong
#define _cffi_from_c_longlong PyLong_FromLongLong
#define _cffi_from_c_ulonglong PyLong_FromUnsignedLongLong
#define _cffi_from_c__Bool PyBool_FromLong

#define _cffi_to_c_double PyFloat_AsDouble
#define _cffi_to_c_float PyFloat_AsDouble

#define _cffi_from_c_int_const(x)                                        \
    (((x) > 0) ?                                                         \
        ((unsigned long long)(x) <= (unsigned long long)LONG_MAX) ?      \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromUnsignedLongLong((unsigned long long)(x)) :       \
        ((long long)(x) >= (long long)LONG_MIN) ?                        \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromLongLong((long long)(x)))

#define _cffi_from_c_int(x, type)                                        \
    (((type)-1) > 0 ? /* unsigned */                                     \
        (sizeof(type) < sizeof(long) ?                                   \
            PyInt_FromLong((long)x) :                                    \
         sizeof(type) == sizeof(long) ?                                  \
            PyLong_FromUnsignedLong((unsigned long)x) :                  \
            PyLong_FromUnsignedLongLong((unsigned long long)x)) :        \
        (sizeof(type) <= sizeof(long) ?                                  \
            PyInt_FromLong((long)x) :                                    \
            PyLong_FromLongLong((long long)x)))

#define _cffi_to_c_int(o, type)                                          \
    ((type)(                                                             \
     sizeof(type) == 1 ? (((type)-1) > 0 ? (type)_cffi_to_c_u8(o)        \
                                         : (type)_cffi_to_c_i8(o)) :     \
     sizeof(type) == 2 ? (((type)-1) > 0 ? (type)_cffi_to_c_u16(o)       \
                                         : (type)_cffi_to_c_i16(o)) :    \
     sizeof(type) == 4 ? (((type)-1) > 0 ? (type)_cffi_to_c_u32(o)       \
                                         : (type)_cffi_to_c_i32(o)) :    \
     sizeof(type) == 8 ? (((type)-1) > 0 ? (type)_cffi_to_c_u64(o)       \
                                         : (type)_cffi_to_c_i64(o)) :    \
     (Py_FatalError("unsupported size for type " #type), (type)0)))

#define _cffi_to_c_i8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[1])
#define _cffi_to_c_u8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[2])
#define _cffi_to_c_i16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[3])
#define _cffi_to_c_u16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[4])
#define _cffi_to_c_i32                                                   \
                 ((int(*)(PyObject *))_cffi_exports[5])
#define _cffi_to_c_u32                                                   \
                 ((unsigned int(*)(PyObject *))_cffi_exports[6])
#define _cffi_to_c_i64                                                   \
                 ((long long(*)(PyObject *))_cffi_exports[7])
#define _cffi_to_c_u64                                                   \
                 ((unsigned long long(*)(PyObject *))_cffi_exports[8])
#define _cffi_to_c_char                                                  \
                 ((int(*)(PyObject *))_cffi_exports[9])
#define _cffi_from_c_pointer                                             \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[10])
#define _cffi_to_c_pointer                                               \
    ((char *(*)(PyObject *, CTypeDescrObject *))_cffi_exports[11])
#define _cffi_get_struct_layout                                          \
    ((PyObject *(*)(Py_ssize_t[]))_cffi_exports[12])
#define _cffi_restore_errno                                              \
    ((void(*)(void))_cffi_exports[13])
#define _cffi_save_errno                                                 \
    ((void(*)(void))_cffi_exports[14])
#define _cffi_from_c_char                                                \
    ((PyObject *(*)(char))_cffi_exports[15])
#define _cffi_from_c_deref                                               \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[16])
#define _cffi_to_c                                                       \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[17])
#define _cffi_from_c_struct                                              \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[18])
#define _cffi_to_c_wchar_t                                               \
    ((wchar_t(*)(PyObject *))_cffi_exports[19])
#define _cffi_from_c_wchar_t                                             \
    ((PyObject *(*)(wchar_t))_cffi_exports[20])
#define _cffi_to_c_long_double                                           \
    ((long double(*)(PyObject *))_cffi_exports[21])
#define _cffi_to_c__Bool                                                 \
    ((_Bool(*)(PyObject *))_cffi_exports[22])
#define _cffi_prepare_pointer_call_argument                              \
    ((Py_ssize_t(*)(CTypeDescrObject *, PyObject *, char **))_cffi_exports[23])
#define _cffi_convert_array_from_object                                  \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[24])
#define _CFFI_NUM_EXPORTS 25

typedef struct _ctypedescr CTypeDescrObject;

static void *_cffi_exports[_CFFI_NUM_EXPORTS];
static PyObject *_cffi_types, *_cffi_VerificationError;

static int _cffi_setup_custom(PyObject *lib);   /* forward */

static PyObject *_cffi_setup(PyObject *self, PyObject *args)
{
    PyObject *library;
    int was_alive = (_cffi_types != NULL);
    (void)self; /* unused */
    if (!PyArg_ParseTuple(args, "OOO", &_cffi_types, &_cffi_VerificationError,
                                       &library))
        return NULL;
    Py_INCREF(_cffi_types);
    Py_INCREF(_cffi_VerificationError);
    if (_cffi_setup_custom(library) < 0)
        return NULL;
    return PyBool_FromLong(was_alive);
}

union _cffi_union_alignment_u {
    unsigned char m_char;
    unsigned short m_short;
    unsigned int m_int;
    unsigned long m_long;
    unsigned long long m_longlong;
    float m_float;
    double m_double;
    long double m_longdouble;
};

struct _cffi_freeme_s {
    struct _cffi_freeme_s *next;
    union _cffi_union_alignment_u alignment;
};

#ifdef __GNUC__
  __attribute__((unused))
#endif
static int _cffi_convert_array_argument(CTypeDescrObject *ctptr, PyObject *arg,
                                        char **output_data, Py_ssize_t datasize,
                                        struct _cffi_freeme_s **freeme)
{
    char *p;
    if (datasize < 0)
        return -1;

    p = *output_data;
    if (p == NULL) {
        struct _cffi_freeme_s *fp = (struct _cffi_freeme_s *)PyObject_Malloc(
            offsetof(struct _cffi_freeme_s, alignment) + (size_t)datasize);
        if (fp == NULL)
            return -1;
        fp->next = *freeme;
        *freeme = fp;
        p = *output_data = (char *)&fp->alignment;
    }
    memset((void *)p, 0, (size_t)datasize);
    return _cffi_convert_array_from_object(p, ctptr, arg);
}

#ifdef __GNUC__
  __attribute__((unused))
#endif
static void _cffi_free_array_arguments(struct _cffi_freeme_s *freeme)
{
    do {
        void *p = (void *)freeme;
        freeme = freeme->next;
        PyObject_Free(p);
    } while (freeme != NULL);
}

static int _cffi_init(void)
{
    PyObject *module, *c_api_object = NULL;

    module = PyImport_ImportModule("_cffi_backend");
    if (module == NULL)
        goto failure;

    c_api_object = PyObject_GetAttrString(module, "_C_API");
    if (c_api_object == NULL)
        goto failure;
    if (!PyCapsule_CheckExact(c_api_object)) {
        PyErr_SetNone(PyExc_ImportError);
        goto failure;
    }
    memcpy(_cffi_exports, PyCapsule_GetPointer(c_api_object, "cffi"),
           _CFFI_NUM_EXPORTS * sizeof(void *));

    Py_DECREF(module);
    Py_DECREF(c_api_object);
    return 0;

  failure:
    Py_XDECREF(module);
    Py_XDECREF(c_api_object);
    return -1;
}

#define _cffi_type(num) ((CTypeDescrObject *)PyList_GET_ITEM(_cffi_types, num))

/**********/



#pragma warning(disable: 4028 4047) // because of the function ptr hack
#include <windows.h>
#define AVSC_NO_DECLSPEC
#include "avisynth_c.h"

AVS_Library * library;

static const AVS_Value * avs_void_p = &avs_void;


// no wrapper needed for the following functions

avs_add_function_func avs_add_function;
avs_at_exit_func avs_at_exit;
avs_bit_blt_func avs_bit_blt;
avs_check_version_func avs_check_version;
avs_clip_get_error_func avs_clip_get_error;
avs_copy_clip_func avs_copy_clip;
avs_copy_video_frame_func avs_copy_video_frame;
avs_create_script_environment_func avs_create_script_environment;
avs_delete_script_environment_func avs_delete_script_environment;
avs_function_exists_func avs_function_exists;
avs_get_audio_func avs_get_audio;
avs_get_cpu_flags_func avs_get_cpu_flags;
avs_get_error_func avs_get_error;
avs_get_frame_func avs_get_frame;
avs_get_parity_func avs_get_parity;
avs_get_version_func avs_get_version;
avs_get_video_info_func avs_get_video_info;
avs_make_writable_func avs_make_writable;
avs_new_c_filter_func avs_new_c_filter;
avs_new_video_frame_a_func avs_new_video_frame_a;
avs_release_clip_func avs_release_clip;
avs_release_video_frame_func avs_release_video_frame;
avs_save_string_func avs_save_string;
avs_set_cache_hints_func avs_set_cache_hints;
avs_set_memory_max_func avs_set_memory_max;
avs_set_working_dir_func avs_set_working_dir;
avs_sprintf_func avs_sprintf;
avs_subframe_func avs_subframe;
avs_subframe_planar_func avs_subframe_planar;
avs_vsprintf_func avs_vsprintf;

AVS_Library * avs_load_library_w(){
    library = avs_load_library();
    if (library == NULL)
        return NULL;
    avs_add_function=library->avs_add_function;
    avs_at_exit=library->avs_at_exit;
    avs_bit_blt=library->avs_bit_blt;
    avs_check_version=library->avs_check_version;
    avs_clip_get_error=library->avs_clip_get_error;
    avs_copy_clip=library->avs_copy_clip;
    avs_copy_video_frame=library->avs_copy_video_frame;
    avs_create_script_environment=library->avs_create_script_environment;
    avs_function_exists=library->avs_function_exists;
    avs_delete_script_environment=library->avs_delete_script_environment;
    avs_get_audio=library->avs_get_audio;
    avs_get_cpu_flags=library->avs_get_cpu_flags;
    avs_get_error=library->avs_get_error;
    avs_get_frame=library->avs_get_frame;
    avs_get_parity=library->avs_get_parity;
    avs_get_version=library->avs_get_version;
    avs_get_video_info=library->avs_get_video_info;
    avs_make_writable=library->avs_make_writable;
    avs_new_c_filter=library->avs_new_c_filter;
    avs_new_video_frame_a=library->avs_new_video_frame_a;
    avs_release_clip=library->avs_release_clip;
    avs_release_video_frame=library->avs_release_video_frame;
    avs_save_string=library->avs_save_string;
    avs_set_cache_hints=library->avs_set_cache_hints;
    avs_set_memory_max=library->avs_set_memory_max;
    avs_set_working_dir=library->avs_set_working_dir;
    avs_sprintf=library->avs_sprintf;
    avs_subframe=library->avs_subframe;
    avs_subframe_planar=library->avs_subframe_planar;
    avs_vsprintf=library->avs_vsprintf;
    return library;
}

// wrapped functions


// AVS_Value

void avs_copy_value_w(AVS_Value * dest, AVS_Value * src){
    library->avs_copy_value(dest, *src);
}
void avs_release_value_w(AVS_Value *value){
    library->avs_release_value(*value);
}

int avs_defined_w(AVS_Value *v){return avs_defined(*v);}
int avs_is_clip_w(AVS_Value *v){return avs_is_clip(*v);}
int avs_is_bool_w(AVS_Value *v){return avs_is_bool(*v);}
int avs_is_int_w(AVS_Value *v){return avs_is_int(*v);}
int avs_is_float_w(AVS_Value *v){return avs_is_float(*v);}
int avs_is_string_w(AVS_Value *v){return avs_is_string(*v);}
int avs_is_array_w(AVS_Value *v){return avs_is_array(*v);}
int avs_is_error_w(AVS_Value *v){return avs_is_error(*v);}

AVS_Clip * avs_take_clip_w(AVS_Value *val, AVS_ScriptEnvironment *env){
    return library->avs_take_clip(*val, env);
}

int avs_as_bool_w(AVS_Value *v){return avs_as_bool(*v);}
int avs_as_int_w(AVS_Value *v){return avs_as_int(*v);}
const char * avs_as_string_w(AVS_Value *v){return avs_as_string(*v);}
double avs_as_float_w(AVS_Value *v){return avs_as_float(*v);}
const char * avs_as_error_w(AVS_Value *v){return avs_as_error(*v);}
const AVS_Value * avs_as_array_w(AVS_Value *v){return avs_as_array(*v);}
int avs_array_size_w(AVS_Value *v){return avs_array_size(*v);}
AVS_Value * avs_array_elt_w(AVS_Value *v, int index){
    return avs_is_array(*v)? &((*v).d.array[index]): v;
}


// AVS_ScriptEnvironment

void avs_invoke_w(AVS_ScriptEnvironment *env, const char * name, 
                  AVS_Value *args, const char** arg_names, AVS_Value *val){
    AVS_Value ret = library->avs_invoke(env, name, *args, arg_names);
    library->avs_copy_value(val, ret);
    library->avs_release_value(ret);
    library->avs_copy_value(&ret, avs_void);
}

void avs_get_var_w(AVS_ScriptEnvironment *env, const char* name, AVS_Value* val){
    AVS_Value ret = library->avs_get_var(env, name);
    library->avs_copy_value(val, ret);
    library->avs_release_value(ret);
    library->avs_copy_value(&ret, avs_void);
}
int avs_set_var_w(AVS_ScriptEnvironment *env, const char* name, AVS_Value* val){
    return library->avs_set_var(env, name, *val);
}
int avs_set_global_var_w(AVS_ScriptEnvironment *env, const char* name, 
                         const AVS_Value* val){
    return library->avs_set_global_var(env, name, *val);
}

void avs_new_value_bool_w(int v0, AVS_Value * v){
    (*v).type = 'b'; 
    (*v).d.boolean = v0 == 0 ? 0 : 1;
}   
void avs_new_value_int_w(int v0, AVS_Value * v){
    (*v).type = 'i'; 
    (*v).d.integer = v0;
}   
void avs_new_value_string_w(const char * v0, AVS_Value * v){
    (*v).type = 's'; 
    (*v).d.string = v0;
}   
void avs_new_value_float_w(float v0, AVS_Value * v){
    (*v).type = 'f'; 
    (*v).d.floating_pt = v0;
}   
void avs_new_value_error_w(const char * v0, AVS_Value * v){
    (*v).type = 'e'; 
    (*v).d.string = v0;
}   
void avs_new_value_clip_w(AVS_Clip * v0, AVS_Value * v){
    library->avs_set_to_clip(v, v0);
}
void avs_new_value_array_w(AVS_Value * v0, int size, AVS_Value * v){
    (*v).type = 'a'; 
    (*v).d.array = v0;
    (*v).array_size = size;
}   



static int _cffi_const_AVISYNTH_INTERFACE_VERSION(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVISYNTH_INTERFACE_VERSION);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVISYNTH_INTERFACE_VERSION", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return ((void)lib,0);
}

static int _cffi_e_____D_enum____D_11(PyObject *lib)
{
  if ((AVS_CPU_FORCE) <= 0 || (unsigned long)(AVS_CPU_FORCE) != 1UL) {
    char buf[64];
    if ((AVS_CPU_FORCE) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPU_FORCE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPU_FORCE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPU_FORCE", buf, "1");
    return -1;
  }
  if ((AVS_CPU_FPU) <= 0 || (unsigned long)(AVS_CPU_FPU) != 2UL) {
    char buf[64];
    if ((AVS_CPU_FPU) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPU_FPU));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPU_FPU));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPU_FPU", buf, "2");
    return -1;
  }
  if ((AVS_CPU_MMX) <= 0 || (unsigned long)(AVS_CPU_MMX) != 4UL) {
    char buf[64];
    if ((AVS_CPU_MMX) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPU_MMX));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPU_MMX));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPU_MMX", buf, "4");
    return -1;
  }
  if ((AVS_CPU_INTEGER_SSE) <= 0 || (unsigned long)(AVS_CPU_INTEGER_SSE) != 8UL) {
    char buf[64];
    if ((AVS_CPU_INTEGER_SSE) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPU_INTEGER_SSE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPU_INTEGER_SSE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPU_INTEGER_SSE", buf, "8");
    return -1;
  }
  if ((AVS_CPU_SSE) <= 0 || (unsigned long)(AVS_CPU_SSE) != 16UL) {
    char buf[64];
    if ((AVS_CPU_SSE) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPU_SSE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPU_SSE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPU_SSE", buf, "16");
    return -1;
  }
  if ((AVS_CPU_SSE2) <= 0 || (unsigned long)(AVS_CPU_SSE2) != 32UL) {
    char buf[64];
    if ((AVS_CPU_SSE2) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPU_SSE2));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPU_SSE2));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPU_SSE2", buf, "32");
    return -1;
  }
  if ((AVS_CPU_3DNOW) <= 0 || (unsigned long)(AVS_CPU_3DNOW) != 64UL) {
    char buf[64];
    if ((AVS_CPU_3DNOW) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPU_3DNOW));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPU_3DNOW));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPU_3DNOW", buf, "64");
    return -1;
  }
  if ((AVS_CPU_3DNOW_EXT) <= 0 || (unsigned long)(AVS_CPU_3DNOW_EXT) != 128UL) {
    char buf[64];
    if ((AVS_CPU_3DNOW_EXT) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPU_3DNOW_EXT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPU_3DNOW_EXT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPU_3DNOW_EXT", buf, "128");
    return -1;
  }
  if ((AVS_CPU_X86_64) <= 0 || (unsigned long)(AVS_CPU_X86_64) != 160UL) {
    char buf[64];
    if ((AVS_CPU_X86_64) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPU_X86_64));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPU_X86_64));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPU_X86_64", buf, "160");
    return -1;
  }
  if ((AVS_CPUF_SSE3) <= 0 || (unsigned long)(AVS_CPUF_SSE3) != 256UL) {
    char buf[64];
    if ((AVS_CPUF_SSE3) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPUF_SSE3));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPUF_SSE3));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPUF_SSE3", buf, "256");
    return -1;
  }
  if ((AVS_CPUF_SSSE3) <= 0 || (unsigned long)(AVS_CPUF_SSSE3) != 512UL) {
    char buf[64];
    if ((AVS_CPUF_SSSE3) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPUF_SSSE3));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPUF_SSSE3));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPUF_SSSE3", buf, "512");
    return -1;
  }
  if ((AVS_CPUF_SSE4) <= 0 || (unsigned long)(AVS_CPUF_SSE4) != 1024UL) {
    char buf[64];
    if ((AVS_CPUF_SSE4) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPUF_SSE4));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPUF_SSE4));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPUF_SSE4", buf, "1024");
    return -1;
  }
  if ((AVS_CPUF_SSE4_1) <= 0 || (unsigned long)(AVS_CPUF_SSE4_1) != 1024UL) {
    char buf[64];
    if ((AVS_CPUF_SSE4_1) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPUF_SSE4_1));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPUF_SSE4_1));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPUF_SSE4_1", buf, "1024");
    return -1;
  }
  if ((AVS_CPUF_SSE4_2) <= 0 || (unsigned long)(AVS_CPUF_SSE4_2) != 2048UL) {
    char buf[64];
    if ((AVS_CPUF_SSE4_2) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CPUF_SSE4_2));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CPUF_SSE4_2));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$11: ", "AVS_CPUF_SSE4_2", buf, "2048");
    return -1;
  }
  return ((void)lib,0);
}

static int _cffi_const_AVS_SAMPLE_INT8(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_SAMPLE_INT8);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_SAMPLE_INT8", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVISYNTH_INTERFACE_VERSION(lib);
}

static int _cffi_const_AVS_SAMPLE_INT16(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_SAMPLE_INT16);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_SAMPLE_INT16", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_SAMPLE_INT8(lib);
}

static int _cffi_const_AVS_SAMPLE_INT24(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_SAMPLE_INT24);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_SAMPLE_INT24", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_SAMPLE_INT16(lib);
}

static int _cffi_const_AVS_SAMPLE_INT32(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_SAMPLE_INT32);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_SAMPLE_INT32", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_SAMPLE_INT24(lib);
}

static int _cffi_const_AVS_SAMPLE_FLOAT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_SAMPLE_FLOAT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_SAMPLE_FLOAT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_SAMPLE_INT32(lib);
}

static int _cffi_const_AVS_PLANAR_Y(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_Y);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_Y", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_SAMPLE_FLOAT(lib);
}

static int _cffi_const_AVS_PLANAR_U(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_U);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_U", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_Y(lib);
}

static int _cffi_const_AVS_PLANAR_V(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_V);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_V", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_U(lib);
}

static int _cffi_const_AVS_PLANAR_ALIGNED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_ALIGNED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_ALIGNED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_V(lib);
}

static int _cffi_const_AVS_PLANAR_Y_ALIGNED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_Y_ALIGNED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_Y_ALIGNED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_ALIGNED(lib);
}

static int _cffi_const_AVS_PLANAR_U_ALIGNED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_U_ALIGNED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_U_ALIGNED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_Y_ALIGNED(lib);
}

static int _cffi_const_AVS_PLANAR_V_ALIGNED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_V_ALIGNED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_V_ALIGNED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_U_ALIGNED(lib);
}

static int _cffi_const_AVS_PLANAR_A(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_A);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_A", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_V_ALIGNED(lib);
}

static int _cffi_const_AVS_PLANAR_R(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_R);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_R", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_A(lib);
}

static int _cffi_const_AVS_PLANAR_G(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_G);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_G", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_R(lib);
}

static int _cffi_const_AVS_PLANAR_B(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_B);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_B", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_G(lib);
}

static int _cffi_const_AVS_PLANAR_A_ALIGNED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_A_ALIGNED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_A_ALIGNED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_B(lib);
}

static int _cffi_const_AVS_PLANAR_R_ALIGNED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_R_ALIGNED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_R_ALIGNED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_A_ALIGNED(lib);
}

static int _cffi_const_AVS_PLANAR_G_ALIGNED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_G_ALIGNED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_G_ALIGNED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_R_ALIGNED(lib);
}

static int _cffi_const_AVS_PLANAR_B_ALIGNED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_PLANAR_B_ALIGNED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_PLANAR_B_ALIGNED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_G_ALIGNED(lib);
}

static int _cffi_const_AVS_CS_BGR(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_BGR);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_BGR", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_PLANAR_B_ALIGNED(lib);
}

static int _cffi_const_AVS_CS_YUV(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_YUV);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_YUV", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_BGR(lib);
}

static int _cffi_const_AVS_CS_INTERLEAVED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_INTERLEAVED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_INTERLEAVED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_YUV(lib);
}

static int _cffi_const_AVS_CS_PLANAR(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_PLANAR);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_PLANAR", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_INTERLEAVED(lib);
}

static int _cffi_const_AVS_CS_SHIFT_SUB_WIDTH(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SHIFT_SUB_WIDTH);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SHIFT_SUB_WIDTH", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_PLANAR(lib);
}

static int _cffi_const_AVS_CS_SHIFT_SUB_HEIGHT(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SHIFT_SUB_HEIGHT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SHIFT_SUB_HEIGHT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SHIFT_SUB_WIDTH(lib);
}

static int _cffi_const_AVS_CS_SHIFT_SAMPLE_BITS(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SHIFT_SAMPLE_BITS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SHIFT_SAMPLE_BITS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SHIFT_SUB_HEIGHT(lib);
}

static int _cffi_const_AVS_CS_SUB_WIDTH_MASK(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SUB_WIDTH_MASK);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SUB_WIDTH_MASK", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SHIFT_SAMPLE_BITS(lib);
}

static int _cffi_const_AVS_CS_SUB_WIDTH_1(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SUB_WIDTH_1);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SUB_WIDTH_1", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SUB_WIDTH_MASK(lib);
}

static int _cffi_const_AVS_CS_SUB_WIDTH_2(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SUB_WIDTH_2);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SUB_WIDTH_2", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SUB_WIDTH_1(lib);
}

static int _cffi_const_AVS_CS_SUB_WIDTH_4(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SUB_WIDTH_4);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SUB_WIDTH_4", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SUB_WIDTH_2(lib);
}

static int _cffi_const_AVS_CS_VPLANEFIRST(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_VPLANEFIRST);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_VPLANEFIRST", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SUB_WIDTH_4(lib);
}

static int _cffi_const_AVS_CS_UPLANEFIRST(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_UPLANEFIRST);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_UPLANEFIRST", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_VPLANEFIRST(lib);
}

static int _cffi_const_AVS_CS_SUB_HEIGHT_MASK(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SUB_HEIGHT_MASK);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SUB_HEIGHT_MASK", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_UPLANEFIRST(lib);
}

static int _cffi_const_AVS_CS_SUB_HEIGHT_1(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SUB_HEIGHT_1);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SUB_HEIGHT_1", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SUB_HEIGHT_MASK(lib);
}

static int _cffi_const_AVS_CS_SUB_HEIGHT_2(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SUB_HEIGHT_2);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SUB_HEIGHT_2", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SUB_HEIGHT_1(lib);
}

static int _cffi_const_AVS_CS_SUB_HEIGHT_4(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SUB_HEIGHT_4);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SUB_HEIGHT_4", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SUB_HEIGHT_2(lib);
}

static int _cffi_const_AVS_CS_SAMPLE_BITS_MASK(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SAMPLE_BITS_MASK);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SAMPLE_BITS_MASK", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SUB_HEIGHT_4(lib);
}

static int _cffi_const_AVS_CS_SAMPLE_BITS_8(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SAMPLE_BITS_8);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SAMPLE_BITS_8", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SAMPLE_BITS_MASK(lib);
}

static int _cffi_const_AVS_CS_SAMPLE_BITS_16(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SAMPLE_BITS_16);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SAMPLE_BITS_16", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SAMPLE_BITS_8(lib);
}

static int _cffi_const_AVS_CS_SAMPLE_BITS_32(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_SAMPLE_BITS_32);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_SAMPLE_BITS_32", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SAMPLE_BITS_16(lib);
}

static int _cffi_const_AVS_CS_PLANAR_MASK(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_PLANAR_MASK);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_PLANAR_MASK", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_SAMPLE_BITS_32(lib);
}

static int _cffi_const_AVS_CS_PLANAR_FILTER(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_PLANAR_FILTER);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_PLANAR_FILTER", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_PLANAR_MASK(lib);
}

static int _cffi_const_AVS_CS_UNKNOWN(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_UNKNOWN);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_UNKNOWN", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_PLANAR_FILTER(lib);
}

static int _cffi_const_AVS_CS_BGR24(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_BGR24);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_BGR24", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_UNKNOWN(lib);
}

static int _cffi_const_AVS_CS_BGR32(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_BGR32);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_BGR32", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_BGR24(lib);
}

static int _cffi_const_AVS_CS_YUY2(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_YUY2);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_YUY2", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_BGR32(lib);
}

static int _cffi_const_AVS_CS_RAW32(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_RAW32);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_RAW32", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_YUY2(lib);
}

static int _cffi_const_AVS_CS_YV24(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_YV24);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_YV24", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_RAW32(lib);
}

static int _cffi_const_AVS_CS_YV16(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_YV16);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_YV16", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_YV24(lib);
}

static int _cffi_const_AVS_CS_YV12(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_YV12);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_YV12", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_YV16(lib);
}

static int _cffi_const_AVS_CS_I420(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_I420);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_I420", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_YV12(lib);
}

static int _cffi_const_AVS_CS_IYUV(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_IYUV);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_IYUV", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_I420(lib);
}

static int _cffi_const_AVS_CS_YV411(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_YV411);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_YV411", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_IYUV(lib);
}

static int _cffi_const_AVS_CS_YUV9(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_YUV9);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_YUV9", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_YV411(lib);
}

static int _cffi_const_AVS_CS_Y8(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_CS_Y8);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_CS_Y8", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_YUV9(lib);
}

static int _cffi_const_AVS_IT_BFF(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_IT_BFF);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_IT_BFF", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_CS_Y8(lib);
}

static int _cffi_const_AVS_IT_TFF(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_IT_TFF);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_IT_TFF", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_IT_BFF(lib);
}

static int _cffi_const_AVS_IT_FIELDBASED(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_IT_FIELDBASED);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_IT_FIELDBASED", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_IT_TFF(lib);
}

static int _cffi_e_____D_enum____D_7(PyObject *lib)
{
  if ((AVS_FILTER_TYPE) <= 0 || (unsigned long)(AVS_FILTER_TYPE) != 1UL) {
    char buf[64];
    if ((AVS_FILTER_TYPE) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_TYPE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_TYPE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$7: ", "AVS_FILTER_TYPE", buf, "1");
    return -1;
  }
  if ((AVS_FILTER_INPUT_COLORSPACE) <= 0 || (unsigned long)(AVS_FILTER_INPUT_COLORSPACE) != 2UL) {
    char buf[64];
    if ((AVS_FILTER_INPUT_COLORSPACE) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_INPUT_COLORSPACE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_INPUT_COLORSPACE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$7: ", "AVS_FILTER_INPUT_COLORSPACE", buf, "2");
    return -1;
  }
  if ((AVS_FILTER_OUTPUT_TYPE) <= 0 || (unsigned long)(AVS_FILTER_OUTPUT_TYPE) != 9UL) {
    char buf[64];
    if ((AVS_FILTER_OUTPUT_TYPE) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_OUTPUT_TYPE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_OUTPUT_TYPE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$7: ", "AVS_FILTER_OUTPUT_TYPE", buf, "9");
    return -1;
  }
  if ((AVS_FILTER_NAME) <= 0 || (unsigned long)(AVS_FILTER_NAME) != 4UL) {
    char buf[64];
    if ((AVS_FILTER_NAME) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_NAME));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_NAME));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$7: ", "AVS_FILTER_NAME", buf, "4");
    return -1;
  }
  if ((AVS_FILTER_AUTHOR) <= 0 || (unsigned long)(AVS_FILTER_AUTHOR) != 5UL) {
    char buf[64];
    if ((AVS_FILTER_AUTHOR) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_AUTHOR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_AUTHOR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$7: ", "AVS_FILTER_AUTHOR", buf, "5");
    return -1;
  }
  if ((AVS_FILTER_VERSION) <= 0 || (unsigned long)(AVS_FILTER_VERSION) != 6UL) {
    char buf[64];
    if ((AVS_FILTER_VERSION) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_VERSION));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_VERSION));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$7: ", "AVS_FILTER_VERSION", buf, "6");
    return -1;
  }
  if ((AVS_FILTER_ARGS) <= 0 || (unsigned long)(AVS_FILTER_ARGS) != 7UL) {
    char buf[64];
    if ((AVS_FILTER_ARGS) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_ARGS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_ARGS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$7: ", "AVS_FILTER_ARGS", buf, "7");
    return -1;
  }
  if ((AVS_FILTER_ARGS_INFO) <= 0 || (unsigned long)(AVS_FILTER_ARGS_INFO) != 8UL) {
    char buf[64];
    if ((AVS_FILTER_ARGS_INFO) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_ARGS_INFO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_ARGS_INFO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$7: ", "AVS_FILTER_ARGS_INFO", buf, "8");
    return -1;
  }
  if ((AVS_FILTER_ARGS_DESCRIPTION) <= 0 || (unsigned long)(AVS_FILTER_ARGS_DESCRIPTION) != 10UL) {
    char buf[64];
    if ((AVS_FILTER_ARGS_DESCRIPTION) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_ARGS_DESCRIPTION));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_ARGS_DESCRIPTION));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$7: ", "AVS_FILTER_ARGS_DESCRIPTION", buf, "10");
    return -1;
  }
  if ((AVS_FILTER_DESCRIPTION) <= 0 || (unsigned long)(AVS_FILTER_DESCRIPTION) != 11UL) {
    char buf[64];
    if ((AVS_FILTER_DESCRIPTION) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_DESCRIPTION));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_DESCRIPTION));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$7: ", "AVS_FILTER_DESCRIPTION", buf, "11");
    return -1;
  }
  return _cffi_e_____D_enum____D_11(lib);
}

static int _cffi_e_____D_enum____D_8(PyObject *lib)
{
  if ((AVS_FILTER_TYPE_AUDIO) <= 0 || (unsigned long)(AVS_FILTER_TYPE_AUDIO) != 1UL) {
    char buf[64];
    if ((AVS_FILTER_TYPE_AUDIO) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_TYPE_AUDIO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_TYPE_AUDIO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$8: ", "AVS_FILTER_TYPE_AUDIO", buf, "1");
    return -1;
  }
  if ((AVS_FILTER_TYPE_VIDEO) <= 0 || (unsigned long)(AVS_FILTER_TYPE_VIDEO) != 2UL) {
    char buf[64];
    if ((AVS_FILTER_TYPE_VIDEO) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_TYPE_VIDEO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_TYPE_VIDEO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$8: ", "AVS_FILTER_TYPE_VIDEO", buf, "2");
    return -1;
  }
  if ((AVS_FILTER_OUTPUT_TYPE_SAME) <= 0 || (unsigned long)(AVS_FILTER_OUTPUT_TYPE_SAME) != 3UL) {
    char buf[64];
    if ((AVS_FILTER_OUTPUT_TYPE_SAME) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_OUTPUT_TYPE_SAME));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_OUTPUT_TYPE_SAME));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$8: ", "AVS_FILTER_OUTPUT_TYPE_SAME", buf, "3");
    return -1;
  }
  if ((AVS_FILTER_OUTPUT_TYPE_DIFFERENT) <= 0 || (unsigned long)(AVS_FILTER_OUTPUT_TYPE_DIFFERENT) != 4UL) {
    char buf[64];
    if ((AVS_FILTER_OUTPUT_TYPE_DIFFERENT) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_FILTER_OUTPUT_TYPE_DIFFERENT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_FILTER_OUTPUT_TYPE_DIFFERENT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$8: ", "AVS_FILTER_OUTPUT_TYPE_DIFFERENT", buf, "4");
    return -1;
  }
  return _cffi_e_____D_enum____D_7(lib);
}

static int _cffi_e_____D_enum____D_9(PyObject *lib)
{
  if ((AVS_CACHE_NOTHING) > 0 || (long)(AVS_CACHE_NOTHING) != 0L) {
    char buf[64];
    if ((AVS_CACHE_NOTHING) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CACHE_NOTHING));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CACHE_NOTHING));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$9: ", "AVS_CACHE_NOTHING", buf, "0");
    return -1;
  }
  if ((AVS_CACHE_RANGE) <= 0 || (unsigned long)(AVS_CACHE_RANGE) != 1UL) {
    char buf[64];
    if ((AVS_CACHE_RANGE) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CACHE_RANGE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CACHE_RANGE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$9: ", "AVS_CACHE_RANGE", buf, "1");
    return -1;
  }
  if ((AVS_CACHE_ALL) <= 0 || (unsigned long)(AVS_CACHE_ALL) != 2UL) {
    char buf[64];
    if ((AVS_CACHE_ALL) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CACHE_ALL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CACHE_ALL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$9: ", "AVS_CACHE_ALL", buf, "2");
    return -1;
  }
  if ((AVS_CACHE_AUDIO) <= 0 || (unsigned long)(AVS_CACHE_AUDIO) != 3UL) {
    char buf[64];
    if ((AVS_CACHE_AUDIO) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CACHE_AUDIO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CACHE_AUDIO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$9: ", "AVS_CACHE_AUDIO", buf, "3");
    return -1;
  }
  if ((AVS_CACHE_AUDIO_NONE) <= 0 || (unsigned long)(AVS_CACHE_AUDIO_NONE) != 4UL) {
    char buf[64];
    if ((AVS_CACHE_AUDIO_NONE) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CACHE_AUDIO_NONE));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CACHE_AUDIO_NONE));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$9: ", "AVS_CACHE_AUDIO_NONE", buf, "4");
    return -1;
  }
  if ((AVS_CACHE_AUDIO_AUTO) <= 0 || (unsigned long)(AVS_CACHE_AUDIO_AUTO) != 5UL) {
    char buf[64];
    if ((AVS_CACHE_AUDIO_AUTO) <= 0)
        snprintf(buf, 63, "%ld", (long)(AVS_CACHE_AUDIO_AUTO));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(AVS_CACHE_AUDIO_AUTO));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "enum $enum_$9: ", "AVS_CACHE_AUDIO_AUTO", buf, "5");
    return -1;
  }
  return _cffi_e_____D_enum____D_8(lib);
}

static int _cffi_const_avs_void(PyObject *lib)
{
  PyObject *o;
  int res;
  AVS_Value i;
  i = (avs_void);
  o = _cffi_from_c_struct((char *)&i, _cffi_type(0));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_void", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_e_____D_enum____D_9(lib);
}

static PyObject *
_cffi_f_FreeLibrary(PyObject *self, PyObject *arg0)
{
  void * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(1), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { FreeLibrary(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_GetProcAddress(PyObject *self, PyObject *args)
{
  void * x0;
  char const * x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  void * result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:GetProcAddress", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(1), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(3), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = GetProcAddress(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(1));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_LoadLibrary(PyObject *self, PyObject *arg0)
{
  char const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  void * result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(3), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = LoadLibrary(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(1));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_array_elt_w(PyObject *self, PyObject *args)
{
  AVS_Value * x0;
  int x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  AVS_Value * result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_array_elt_w", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_array_elt_w(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(4));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_array_size_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_array_size_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_as_array_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  AVS_Value const * result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_as_array_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(5));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_as_bool_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_as_bool_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_as_error_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  char const * result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_as_error_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(3));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_as_float_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  double result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_as_float_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_double(result);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_as_int_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_as_int_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_as_string_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  char const * result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_as_string_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(3));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_audio_channels(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_audio_channels(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_audio_samples_from_bytes(PyObject *self, PyObject *args)
{
  AVS_VideoInfo const * x0;
  int64_t x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int64_t result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_audio_samples_from_bytes", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int64_t);
  if (x1 == (int64_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_audio_samples_from_bytes(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int64_t);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_audio_samples_from_frames(PyObject *self, PyObject *args)
{
  AVS_VideoInfo const * x0;
  int64_t x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int64_t result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_audio_samples_from_frames", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int64_t);
  if (x1 == (int64_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_audio_samples_from_frames(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int64_t);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_bits_per_pixel(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_bits_per_pixel(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_bmp_size(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_bmp_size(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_bytes_from_audio_samples(PyObject *self, PyObject *args)
{
  AVS_VideoInfo const * x0;
  int64_t x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int64_t result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_bytes_from_audio_samples", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int64_t);
  if (x1 == (int64_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_bytes_from_audio_samples(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int64_t);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_bytes_from_pixels(PyObject *self, PyObject *args)
{
  AVS_VideoInfo const * x0;
  int x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_bytes_from_pixels", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_bytes_from_pixels(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_bytes_per_audio_sample(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_bytes_per_audio_sample(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_bytes_per_channel_sample(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_bytes_per_channel_sample(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_clear_property(PyObject *self, PyObject *args)
{
  AVS_VideoInfo * x0;
  int x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_clear_property", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(7), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(7), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_clear_property(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_copy_value_w(PyObject *self, PyObject *args)
{
  AVS_Value * x0;
  AVS_Value * x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_copy_value_w", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_copy_value_w(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_defined_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_defined_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_frames_from_audio_samples(PyObject *self, PyObject *args)
{
  AVS_VideoInfo const * x0;
  int64_t x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_frames_from_audio_samples", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int64_t);
  if (x1 == (int64_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_frames_from_audio_samples(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_free_library(PyObject *self, PyObject *arg0)
{
  AVS_Library * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(8), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(8), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_free_library(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_get_height(PyObject *self, PyObject *arg0)
{
  AVS_VideoFrame const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(9), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_get_height(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_get_height_p(PyObject *self, PyObject *args)
{
  AVS_VideoFrame const * x0;
  int x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_get_height_p", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(9), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_get_height_p(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_get_pitch(PyObject *self, PyObject *arg0)
{
  AVS_VideoFrame const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(9), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_get_pitch(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_get_pitch_p(PyObject *self, PyObject *args)
{
  AVS_VideoFrame const * x0;
  int x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_get_pitch_p", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(9), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_get_pitch_p(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_get_read_ptr(PyObject *self, PyObject *arg0)
{
  AVS_VideoFrame const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  unsigned char const * result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(9), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_get_read_ptr(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(10));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_get_read_ptr_p(PyObject *self, PyObject *args)
{
  AVS_VideoFrame const * x0;
  int x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  unsigned char const * result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_get_read_ptr_p", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(9), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_get_read_ptr_p(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(10));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_get_row_size(PyObject *self, PyObject *arg0)
{
  AVS_VideoFrame const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(9), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_get_row_size(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_get_row_size_p(PyObject *self, PyObject *args)
{
  AVS_VideoFrame const * x0;
  int x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_get_row_size_p", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(9), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_get_row_size_p(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_get_var_w(PyObject *self, PyObject *args)
{
  AVS_ScriptEnvironment * x0;
  char const * x1;
  AVS_Value * x2;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:avs_get_var_w", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(11), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(11), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(3), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg2, (char **)&x2);
  if (datasize != 0) {
    x2 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg2, (char **)&x2,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_get_var_w(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_get_write_ptr(PyObject *self, PyObject *arg0)
{
  AVS_VideoFrame const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  unsigned char * result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(9), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_get_write_ptr(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(12));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_get_write_ptr_p(PyObject *self, PyObject *args)
{
  AVS_VideoFrame const * x0;
  int x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  unsigned char * result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_get_write_ptr_p", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(9), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_get_write_ptr_p(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(12));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_has_audio(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_has_audio(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_has_video(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_has_video(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_invoke_w(PyObject *self, PyObject *args)
{
  AVS_ScriptEnvironment * x0;
  char const * x1;
  AVS_Value * x2;
  char const * * x3;
  AVS_Value * x4;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:avs_invoke_w", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(11), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(11), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(3), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg2, (char **)&x2);
  if (datasize != 0) {
    x2 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg2, (char **)&x2,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(13), arg3, (char **)&x3);
  if (datasize != 0) {
    x3 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(13), arg3, (char **)&x3,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg4, (char **)&x4);
  if (datasize != 0) {
    x4 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg4, (char **)&x4,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_invoke_w(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_is_array_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_array_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_bff(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_bff(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_bool_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_bool_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_clip_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_clip_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_color_space(PyObject *self, PyObject *args)
{
  AVS_VideoInfo const * x0;
  int x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_is_color_space", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_color_space(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_error_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_error_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_field_based(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_field_based(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_float_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_float_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_int_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_int_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_parity_known(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_parity_known(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_planar(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_planar(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_property(PyObject *self, PyObject *args)
{
  AVS_VideoInfo const * x0;
  int x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_is_property", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_property(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_rgb(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_rgb(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_rgb24(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_rgb24(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_rgb32(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_rgb32(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_same_colorspace(PyObject *self, PyObject *args)
{
  AVS_VideoInfo * x0;
  AVS_VideoInfo * x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_is_same_colorspace", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(7), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(7), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(7), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(7), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_same_colorspace(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_string_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_string_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_tff(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_tff(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_writable(PyObject *self, PyObject *arg0)
{
  AVS_VideoFrame const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(9), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_writable(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_y8(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_y8(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_yuv(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_yuv(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_yuy2(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_yuy2(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_yv12(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_yv12(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_yv16(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_yv16(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_yv24(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_yv24(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_is_yv411(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_is_yv411(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_load_library(PyObject *self, PyObject *noarg)
{
  AVS_Library * result;
  PyObject *pyresult;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_load_library(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  (void)noarg; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(8));
  return pyresult;
}

static PyObject *
_cffi_f_avs_load_library_w(PyObject *self, PyObject *noarg)
{
  AVS_Library * result;
  PyObject *pyresult;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_load_library_w(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  (void)noarg; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(8));
  return pyresult;
}

static PyObject *
_cffi_f_avs_new_value_array_w(PyObject *self, PyObject *args)
{
  AVS_Value * x0;
  int x1;
  AVS_Value * x2;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:avs_new_value_array_w", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg2, (char **)&x2);
  if (datasize != 0) {
    x2 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg2, (char **)&x2,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_new_value_array_w(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_new_value_bool_w(PyObject *self, PyObject *args)
{
  int x0;
  AVS_Value * x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_new_value_bool_w", &arg0, &arg1))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_new_value_bool_w(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_new_value_clip_w(PyObject *self, PyObject *args)
{
  AVS_Clip * x0;
  AVS_Value * x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_new_value_clip_w", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(14), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(14), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_new_value_clip_w(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_new_value_error_w(PyObject *self, PyObject *args)
{
  char const * x0;
  AVS_Value * x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_new_value_error_w", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(3), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_new_value_error_w(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_new_value_float_w(PyObject *self, PyObject *args)
{
  float x0;
  AVS_Value * x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_new_value_float_w", &arg0, &arg1))
    return NULL;

  x0 = (float)_cffi_to_c_float(arg0);
  if (x0 == (float)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_new_value_float_w(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_new_value_int_w(PyObject *self, PyObject *args)
{
  int x0;
  AVS_Value * x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_new_value_int_w", &arg0, &arg1))
    return NULL;

  x0 = _cffi_to_c_int(arg0, int);
  if (x0 == (int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_new_value_int_w(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_new_value_string_w(PyObject *self, PyObject *args)
{
  char const * x0;
  AVS_Value * x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_new_value_string_w", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(3), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_new_value_string_w(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_release_value_w(PyObject *self, PyObject *arg0)
{
  AVS_Value * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_release_value_w(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_row_size(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_row_size(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_sample_type(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_sample_type(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_samples_per_second(PyObject *self, PyObject *arg0)
{
  AVS_VideoInfo const * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(6), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_samples_per_second(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_set_field_based(PyObject *self, PyObject *args)
{
  AVS_VideoInfo * x0;
  int x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_set_field_based", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(7), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(7), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_set_field_based(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_set_fps(PyObject *self, PyObject *args)
{
  AVS_VideoInfo * x0;
  unsigned int x1;
  unsigned int x2;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:avs_set_fps", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(7), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(7), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, unsigned int);
  if (x2 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_set_fps(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_set_global_var_w(PyObject *self, PyObject *args)
{
  AVS_ScriptEnvironment * x0;
  char const * x1;
  AVS_Value const * x2;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:avs_set_global_var_w", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(11), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(11), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(3), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(5), arg2, (char **)&x2);
  if (datasize != 0) {
    x2 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(5), arg2, (char **)&x2,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_set_global_var_w(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_set_property(PyObject *self, PyObject *args)
{
  AVS_VideoInfo * x0;
  int x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_set_property", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(7), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(7), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, int);
  if (x1 == (int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { avs_set_property(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_avs_set_var_w(PyObject *self, PyObject *args)
{
  AVS_ScriptEnvironment * x0;
  char const * x1;
  AVS_Value * x2;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  int result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:avs_set_var_w", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(11), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(11), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(3), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(3), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg2, (char **)&x2);
  if (datasize != 0) {
    x2 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg2, (char **)&x2,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_set_var_w(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_int(result, int);
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_avs_take_clip_w(PyObject *self, PyObject *args)
{
  AVS_Value * x0;
  AVS_ScriptEnvironment * x1;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;
  AVS_Clip * result;
  PyObject *pyresult;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:avs_take_clip_w", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(4), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(4), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(11), arg1, (char **)&x1);
  if (datasize != 0) {
    x1 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(11), arg1, (char **)&x1,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = avs_take_clip_w(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(14));
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  return pyresult;
}

static PyObject *
_cffi_f_free(PyObject *self, PyObject *arg0)
{
  void * x0;
  Py_ssize_t datasize;
  struct _cffi_freeme_s *large_args_free = NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    x0 = ((size_t)datasize) <= 640 ? alloca((size_t)datasize) : NULL;
    if (_cffi_convert_array_argument(_cffi_type(1), arg0, (char **)&x0,
            datasize, &large_args_free) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { free(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  if (large_args_free != NULL) _cffi_free_array_arguments(large_args_free);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_malloc(PyObject *self, PyObject *arg0)
{
  size_t x0;
  void * result;
  PyObject *pyresult;

  x0 = _cffi_to_c_int(arg0, size_t);
  if (x0 == (size_t)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = malloc(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  pyresult = _cffi_from_c_pointer((char *)result, _cffi_type(1));
  return pyresult;
}

static int _cffi_const_AVS_FRAME_ALIGN(PyObject *lib)
{
  PyObject *o;
  int res;
  o = _cffi_from_c_int_const(AVS_FRAME_ALIGN);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "AVS_FRAME_ALIGN", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_avs_void(lib);
}

static void _cffi_check_struct_AVS_Library(struct AVS_Library *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
}
static PyObject *
_cffi_layout_struct_AVS_Library(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct AVS_Library y; };
  static Py_ssize_t nums[] = {
    sizeof(struct AVS_Library),
    offsetof(struct _cffi_aligncheck, y),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_AVS_Library(0);
}

static void _cffi_check_struct_AVS_Value(struct AVS_Value *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->type) << 1);
  (void)((p->array_size) << 1);
  /* cannot generate 'union $10' in field 'd': unknown type name */
}
static PyObject *
_cffi_layout_struct_AVS_Value(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct AVS_Value y; };
  static Py_ssize_t nums[] = {
    sizeof(struct AVS_Value),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct AVS_Value, type),
    sizeof(((struct AVS_Value *)0)->type),
    offsetof(struct AVS_Value, array_size),
    sizeof(((struct AVS_Value *)0)->array_size),
    offsetof(struct AVS_Value, d),
    sizeof(((struct AVS_Value *)0)->d),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_AVS_Value(0);
}

static void _cffi_check_struct_AVS_VideoFrame(struct AVS_VideoFrame *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->refcount) << 1);
  { AVS_VideoFrameBuffer * *tmp = &p->vfb; (void)tmp; }
  (void)((p->offset) << 1);
  (void)((p->pitch) << 1);
  (void)((p->row_size) << 1);
  (void)((p->height) << 1);
  (void)((p->offsetU) << 1);
  (void)((p->offsetV) << 1);
  (void)((p->pitchUV) << 1);
  (void)((p->row_sizeUV) << 1);
  (void)((p->heightUV) << 1);
}
static PyObject *
_cffi_layout_struct_AVS_VideoFrame(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct AVS_VideoFrame y; };
  static Py_ssize_t nums[] = {
    sizeof(struct AVS_VideoFrame),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct AVS_VideoFrame, refcount),
    sizeof(((struct AVS_VideoFrame *)0)->refcount),
    offsetof(struct AVS_VideoFrame, vfb),
    sizeof(((struct AVS_VideoFrame *)0)->vfb),
    offsetof(struct AVS_VideoFrame, offset),
    sizeof(((struct AVS_VideoFrame *)0)->offset),
    offsetof(struct AVS_VideoFrame, pitch),
    sizeof(((struct AVS_VideoFrame *)0)->pitch),
    offsetof(struct AVS_VideoFrame, row_size),
    sizeof(((struct AVS_VideoFrame *)0)->row_size),
    offsetof(struct AVS_VideoFrame, height),
    sizeof(((struct AVS_VideoFrame *)0)->height),
    offsetof(struct AVS_VideoFrame, offsetU),
    sizeof(((struct AVS_VideoFrame *)0)->offsetU),
    offsetof(struct AVS_VideoFrame, offsetV),
    sizeof(((struct AVS_VideoFrame *)0)->offsetV),
    offsetof(struct AVS_VideoFrame, pitchUV),
    sizeof(((struct AVS_VideoFrame *)0)->pitchUV),
    offsetof(struct AVS_VideoFrame, row_sizeUV),
    sizeof(((struct AVS_VideoFrame *)0)->row_sizeUV),
    offsetof(struct AVS_VideoFrame, heightUV),
    sizeof(((struct AVS_VideoFrame *)0)->heightUV),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_AVS_VideoFrame(0);
}

static void _cffi_check_struct_AVS_VideoFrameBuffer(struct AVS_VideoFrameBuffer *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { unsigned char * *tmp = &p->data; (void)tmp; }
  (void)((p->data_size) << 1);
  (void)((p->sequence_number) << 1);
  (void)((p->refcount) << 1);
}
static PyObject *
_cffi_layout_struct_AVS_VideoFrameBuffer(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct AVS_VideoFrameBuffer y; };
  static Py_ssize_t nums[] = {
    sizeof(struct AVS_VideoFrameBuffer),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct AVS_VideoFrameBuffer, data),
    sizeof(((struct AVS_VideoFrameBuffer *)0)->data),
    offsetof(struct AVS_VideoFrameBuffer, data_size),
    sizeof(((struct AVS_VideoFrameBuffer *)0)->data_size),
    offsetof(struct AVS_VideoFrameBuffer, sequence_number),
    sizeof(((struct AVS_VideoFrameBuffer *)0)->sequence_number),
    offsetof(struct AVS_VideoFrameBuffer, refcount),
    sizeof(((struct AVS_VideoFrameBuffer *)0)->refcount),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_AVS_VideoFrameBuffer(0);
}

static void _cffi_check_struct_AVS_VideoInfo(struct AVS_VideoInfo *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->width) << 1);
  (void)((p->height) << 1);
  (void)((p->fps_numerator) << 1);
  (void)((p->fps_denominator) << 1);
  (void)((p->num_frames) << 1);
  (void)((p->pixel_type) << 1);
  (void)((p->audio_samples_per_second) << 1);
  (void)((p->sample_type) << 1);
  (void)((p->num_audio_samples) << 1);
  (void)((p->nchannels) << 1);
  (void)((p->image_type) << 1);
}
static PyObject *
_cffi_layout_struct_AVS_VideoInfo(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct AVS_VideoInfo y; };
  static Py_ssize_t nums[] = {
    sizeof(struct AVS_VideoInfo),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct AVS_VideoInfo, width),
    sizeof(((struct AVS_VideoInfo *)0)->width),
    offsetof(struct AVS_VideoInfo, height),
    sizeof(((struct AVS_VideoInfo *)0)->height),
    offsetof(struct AVS_VideoInfo, fps_numerator),
    sizeof(((struct AVS_VideoInfo *)0)->fps_numerator),
    offsetof(struct AVS_VideoInfo, fps_denominator),
    sizeof(((struct AVS_VideoInfo *)0)->fps_denominator),
    offsetof(struct AVS_VideoInfo, num_frames),
    sizeof(((struct AVS_VideoInfo *)0)->num_frames),
    offsetof(struct AVS_VideoInfo, pixel_type),
    sizeof(((struct AVS_VideoInfo *)0)->pixel_type),
    offsetof(struct AVS_VideoInfo, audio_samples_per_second),
    sizeof(((struct AVS_VideoInfo *)0)->audio_samples_per_second),
    offsetof(struct AVS_VideoInfo, sample_type),
    sizeof(((struct AVS_VideoInfo *)0)->sample_type),
    offsetof(struct AVS_VideoInfo, num_audio_samples),
    sizeof(((struct AVS_VideoInfo *)0)->num_audio_samples),
    offsetof(struct AVS_VideoInfo, nchannels),
    sizeof(((struct AVS_VideoInfo *)0)->nchannels),
    offsetof(struct AVS_VideoInfo, image_type),
    sizeof(((struct AVS_VideoInfo *)0)->image_type),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_AVS_VideoInfo(0);
}

static int _cffi_var_avs_add_function(PyObject *lib)
{
  PyObject *o;
  int res;
  int(* * i)(AVS_ScriptEnvironment *, char const *, char const *, void *, void *);
  i = (&avs_add_function);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(15));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_add_function", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_AVS_FRAME_ALIGN(lib);
}

static int _cffi_var_avs_at_exit(PyObject *lib)
{
  PyObject *o;
  int res;
  void(* * i)(AVS_ScriptEnvironment *, void(*)(void *, AVS_ScriptEnvironment *), void *);
  i = (&avs_at_exit);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(16));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_at_exit", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_add_function(lib);
}

static int _cffi_var_avs_bit_blt(PyObject *lib)
{
  PyObject *o;
  int res;
  void(* * i)(AVS_ScriptEnvironment *, unsigned char *, int, unsigned char const *, int, int, int);
  i = (&avs_bit_blt);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(17));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_bit_blt", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_at_exit(lib);
}

static int _cffi_var_avs_check_version(PyObject *lib)
{
  PyObject *o;
  int res;
  int(* * i)(AVS_ScriptEnvironment *, int);
  i = (&avs_check_version);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(18));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_check_version", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_bit_blt(lib);
}

static int _cffi_var_avs_clip_get_error(PyObject *lib)
{
  PyObject *o;
  int res;
  char const *(* * i)(AVS_Clip *);
  i = (&avs_clip_get_error);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(19));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_clip_get_error", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_check_version(lib);
}

static int _cffi_var_avs_copy_clip(PyObject *lib)
{
  PyObject *o;
  int res;
  AVS_Clip *(* * i)(AVS_Clip *);
  i = (&avs_copy_clip);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(20));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_copy_clip", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_clip_get_error(lib);
}

static int _cffi_var_avs_copy_video_frame(PyObject *lib)
{
  PyObject *o;
  int res;
  AVS_VideoFrame *(* * i)(AVS_VideoFrame *);
  i = (&avs_copy_video_frame);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(21));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_copy_video_frame", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_copy_clip(lib);
}

static int _cffi_var_avs_create_script_environment(PyObject *lib)
{
  PyObject *o;
  int res;
  AVS_ScriptEnvironment *(* * i)(int);
  i = (&avs_create_script_environment);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(22));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_create_script_environment", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_copy_video_frame(lib);
}

static int _cffi_var_avs_delete_script_environment(PyObject *lib)
{
  PyObject *o;
  int res;
  void(* * i)(AVS_ScriptEnvironment *);
  i = (&avs_delete_script_environment);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(23));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_delete_script_environment", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_create_script_environment(lib);
}

static int _cffi_var_avs_function_exists(PyObject *lib)
{
  PyObject *o;
  int res;
  int(* * i)(AVS_ScriptEnvironment *, char const *);
  i = (&avs_function_exists);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(24));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_function_exists", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_delete_script_environment(lib);
}

static int _cffi_var_avs_get_audio(PyObject *lib)
{
  PyObject *o;
  int res;
  int(* * i)(AVS_Clip *, void *, int64_t, int64_t);
  i = (&avs_get_audio);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(25));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_get_audio", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_function_exists(lib);
}

static int _cffi_var_avs_get_cpu_flags(PyObject *lib)
{
  PyObject *o;
  int res;
  long(* * i)(AVS_ScriptEnvironment *);
  i = (&avs_get_cpu_flags);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(26));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_get_cpu_flags", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_get_audio(lib);
}

static int _cffi_var_avs_get_error(PyObject *lib)
{
  PyObject *o;
  int res;
  char const *(* * i)(AVS_ScriptEnvironment *);
  i = (&avs_get_error);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(27));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_get_error", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_get_cpu_flags(lib);
}

static int _cffi_var_avs_get_frame(PyObject *lib)
{
  PyObject *o;
  int res;
  AVS_VideoFrame *(* * i)(AVS_Clip *, int);
  i = (&avs_get_frame);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(28));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_get_frame", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_get_error(lib);
}

static int _cffi_var_avs_get_parity(PyObject *lib)
{
  PyObject *o;
  int res;
  int(* * i)(AVS_Clip *, int);
  i = (&avs_get_parity);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(29));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_get_parity", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_get_frame(lib);
}

static int _cffi_var_avs_get_version(PyObject *lib)
{
  PyObject *o;
  int res;
  int(* * i)(AVS_Clip *);
  i = (&avs_get_version);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(30));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_get_version", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_get_parity(lib);
}

static int _cffi_var_avs_get_video_info(PyObject *lib)
{
  PyObject *o;
  int res;
  AVS_VideoInfo const *(* * i)(AVS_Clip *);
  i = (&avs_get_video_info);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(31));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_get_video_info", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_get_version(lib);
}

static int _cffi_var_avs_make_writable(PyObject *lib)
{
  PyObject *o;
  int res;
  int(* * i)(AVS_ScriptEnvironment *, AVS_VideoFrame * *);
  i = (&avs_make_writable);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(32));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_make_writable", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_get_video_info(lib);
}

static int _cffi_var_avs_new_c_filter(PyObject *lib)
{
  PyObject *o;
  int res;
  AVS_Clip *(* * i)(AVS_ScriptEnvironment *, void * *, void *, int);
  i = (&avs_new_c_filter);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(33));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_new_c_filter", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_make_writable(lib);
}

static int _cffi_var_avs_new_video_frame_a(PyObject *lib)
{
  PyObject *o;
  int res;
  AVS_VideoFrame *(* * i)(AVS_ScriptEnvironment *, AVS_VideoInfo const *, int);
  i = (&avs_new_video_frame_a);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(34));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_new_video_frame_a", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_new_c_filter(lib);
}

static int _cffi_var_avs_release_clip(PyObject *lib)
{
  PyObject *o;
  int res;
  void(* * i)(AVS_Clip *);
  i = (&avs_release_clip);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(35));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_release_clip", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_new_video_frame_a(lib);
}

static int _cffi_var_avs_release_video_frame(PyObject *lib)
{
  PyObject *o;
  int res;
  void(* * i)(AVS_VideoFrame *);
  i = (&avs_release_video_frame);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(36));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_release_video_frame", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_release_clip(lib);
}

static int _cffi_var_avs_save_string(PyObject *lib)
{
  PyObject *o;
  int res;
  char *(* * i)(AVS_ScriptEnvironment *, char const *, int);
  i = (&avs_save_string);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(37));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_save_string", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_release_video_frame(lib);
}

static int _cffi_var_avs_set_cache_hints(PyObject *lib)
{
  PyObject *o;
  int res;
  int(* * i)(AVS_Clip *, int, int);
  i = (&avs_set_cache_hints);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(38));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_set_cache_hints", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_save_string(lib);
}

static int _cffi_var_avs_set_memory_max(PyObject *lib)
{
  PyObject *o;
  int res;
  int(* * i)(AVS_ScriptEnvironment *, int);
  i = (&avs_set_memory_max);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(18));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_set_memory_max", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_set_cache_hints(lib);
}

static int _cffi_var_avs_set_working_dir(PyObject *lib)
{
  PyObject *o;
  int res;
  int(* * i)(AVS_ScriptEnvironment *, char const *);
  i = (&avs_set_working_dir);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(24));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_set_working_dir", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_set_memory_max(lib);
}

static int _cffi_var_avs_sprintf(PyObject *lib)
{
  PyObject *o;
  int res;
  char *(* * i)(AVS_ScriptEnvironment *, char const *, ...);
  i = (&avs_sprintf);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(39));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_sprintf", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_set_working_dir(lib);
}

static int _cffi_var_avs_subframe(PyObject *lib)
{
  PyObject *o;
  int res;
  AVS_VideoFrame *(* * i)(AVS_ScriptEnvironment *, AVS_VideoFrame *, int, int, int, int);
  i = (&avs_subframe);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(40));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_subframe", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_sprintf(lib);
}

static int _cffi_var_avs_subframe_planar(PyObject *lib)
{
  PyObject *o;
  int res;
  AVS_VideoFrame *(* * i)(AVS_ScriptEnvironment *, AVS_VideoFrame *, int, int, int, int, int, int, int);
  i = (&avs_subframe_planar);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(41));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_subframe_planar", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_subframe(lib);
}

static int _cffi_var_avs_void_p(PyObject *lib)
{
  PyObject *o;
  int res;
  AVS_Value const * * i;
  i = (&avs_void_p);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(42));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_void_p", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_subframe_planar(lib);
}

static int _cffi_var_avs_vsprintf(PyObject *lib)
{
  PyObject *o;
  int res;
  char *(* * i)(AVS_ScriptEnvironment *, char const *, void *);
  i = (&avs_vsprintf);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(43));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "avs_vsprintf", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_void_p(lib);
}

static int _cffi_var_library(PyObject *lib)
{
  PyObject *o;
  int res;
  AVS_Library * * i;
  i = (&library);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(44));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "library", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_var_avs_vsprintf(lib);
}

static int _cffi_setup_custom(PyObject *lib)
{
  return _cffi_var_library(lib);
}

static PyMethodDef _cffi_methods[] = {
  {"FreeLibrary", _cffi_f_FreeLibrary, METH_O, NULL},
  {"GetProcAddress", _cffi_f_GetProcAddress, METH_VARARGS, NULL},
  {"LoadLibrary", _cffi_f_LoadLibrary, METH_O, NULL},
  {"avs_array_elt_w", _cffi_f_avs_array_elt_w, METH_VARARGS, NULL},
  {"avs_array_size_w", _cffi_f_avs_array_size_w, METH_O, NULL},
  {"avs_as_array_w", _cffi_f_avs_as_array_w, METH_O, NULL},
  {"avs_as_bool_w", _cffi_f_avs_as_bool_w, METH_O, NULL},
  {"avs_as_error_w", _cffi_f_avs_as_error_w, METH_O, NULL},
  {"avs_as_float_w", _cffi_f_avs_as_float_w, METH_O, NULL},
  {"avs_as_int_w", _cffi_f_avs_as_int_w, METH_O, NULL},
  {"avs_as_string_w", _cffi_f_avs_as_string_w, METH_O, NULL},
  {"avs_audio_channels", _cffi_f_avs_audio_channels, METH_O, NULL},
  {"avs_audio_samples_from_bytes", _cffi_f_avs_audio_samples_from_bytes, METH_VARARGS, NULL},
  {"avs_audio_samples_from_frames", _cffi_f_avs_audio_samples_from_frames, METH_VARARGS, NULL},
  {"avs_bits_per_pixel", _cffi_f_avs_bits_per_pixel, METH_O, NULL},
  {"avs_bmp_size", _cffi_f_avs_bmp_size, METH_O, NULL},
  {"avs_bytes_from_audio_samples", _cffi_f_avs_bytes_from_audio_samples, METH_VARARGS, NULL},
  {"avs_bytes_from_pixels", _cffi_f_avs_bytes_from_pixels, METH_VARARGS, NULL},
  {"avs_bytes_per_audio_sample", _cffi_f_avs_bytes_per_audio_sample, METH_O, NULL},
  {"avs_bytes_per_channel_sample", _cffi_f_avs_bytes_per_channel_sample, METH_O, NULL},
  {"avs_clear_property", _cffi_f_avs_clear_property, METH_VARARGS, NULL},
  {"avs_copy_value_w", _cffi_f_avs_copy_value_w, METH_VARARGS, NULL},
  {"avs_defined_w", _cffi_f_avs_defined_w, METH_O, NULL},
  {"avs_frames_from_audio_samples", _cffi_f_avs_frames_from_audio_samples, METH_VARARGS, NULL},
  {"avs_free_library", _cffi_f_avs_free_library, METH_O, NULL},
  {"avs_get_height", _cffi_f_avs_get_height, METH_O, NULL},
  {"avs_get_height_p", _cffi_f_avs_get_height_p, METH_VARARGS, NULL},
  {"avs_get_pitch", _cffi_f_avs_get_pitch, METH_O, NULL},
  {"avs_get_pitch_p", _cffi_f_avs_get_pitch_p, METH_VARARGS, NULL},
  {"avs_get_read_ptr", _cffi_f_avs_get_read_ptr, METH_O, NULL},
  {"avs_get_read_ptr_p", _cffi_f_avs_get_read_ptr_p, METH_VARARGS, NULL},
  {"avs_get_row_size", _cffi_f_avs_get_row_size, METH_O, NULL},
  {"avs_get_row_size_p", _cffi_f_avs_get_row_size_p, METH_VARARGS, NULL},
  {"avs_get_var_w", _cffi_f_avs_get_var_w, METH_VARARGS, NULL},
  {"avs_get_write_ptr", _cffi_f_avs_get_write_ptr, METH_O, NULL},
  {"avs_get_write_ptr_p", _cffi_f_avs_get_write_ptr_p, METH_VARARGS, NULL},
  {"avs_has_audio", _cffi_f_avs_has_audio, METH_O, NULL},
  {"avs_has_video", _cffi_f_avs_has_video, METH_O, NULL},
  {"avs_invoke_w", _cffi_f_avs_invoke_w, METH_VARARGS, NULL},
  {"avs_is_array_w", _cffi_f_avs_is_array_w, METH_O, NULL},
  {"avs_is_bff", _cffi_f_avs_is_bff, METH_O, NULL},
  {"avs_is_bool_w", _cffi_f_avs_is_bool_w, METH_O, NULL},
  {"avs_is_clip_w", _cffi_f_avs_is_clip_w, METH_O, NULL},
  {"avs_is_color_space", _cffi_f_avs_is_color_space, METH_VARARGS, NULL},
  {"avs_is_error_w", _cffi_f_avs_is_error_w, METH_O, NULL},
  {"avs_is_field_based", _cffi_f_avs_is_field_based, METH_O, NULL},
  {"avs_is_float_w", _cffi_f_avs_is_float_w, METH_O, NULL},
  {"avs_is_int_w", _cffi_f_avs_is_int_w, METH_O, NULL},
  {"avs_is_parity_known", _cffi_f_avs_is_parity_known, METH_O, NULL},
  {"avs_is_planar", _cffi_f_avs_is_planar, METH_O, NULL},
  {"avs_is_property", _cffi_f_avs_is_property, METH_VARARGS, NULL},
  {"avs_is_rgb", _cffi_f_avs_is_rgb, METH_O, NULL},
  {"avs_is_rgb24", _cffi_f_avs_is_rgb24, METH_O, NULL},
  {"avs_is_rgb32", _cffi_f_avs_is_rgb32, METH_O, NULL},
  {"avs_is_same_colorspace", _cffi_f_avs_is_same_colorspace, METH_VARARGS, NULL},
  {"avs_is_string_w", _cffi_f_avs_is_string_w, METH_O, NULL},
  {"avs_is_tff", _cffi_f_avs_is_tff, METH_O, NULL},
  {"avs_is_writable", _cffi_f_avs_is_writable, METH_O, NULL},
  {"avs_is_y8", _cffi_f_avs_is_y8, METH_O, NULL},
  {"avs_is_yuv", _cffi_f_avs_is_yuv, METH_O, NULL},
  {"avs_is_yuy2", _cffi_f_avs_is_yuy2, METH_O, NULL},
  {"avs_is_yv12", _cffi_f_avs_is_yv12, METH_O, NULL},
  {"avs_is_yv16", _cffi_f_avs_is_yv16, METH_O, NULL},
  {"avs_is_yv24", _cffi_f_avs_is_yv24, METH_O, NULL},
  {"avs_is_yv411", _cffi_f_avs_is_yv411, METH_O, NULL},
  {"avs_load_library", _cffi_f_avs_load_library, METH_NOARGS, NULL},
  {"avs_load_library_w", _cffi_f_avs_load_library_w, METH_NOARGS, NULL},
  {"avs_new_value_array_w", _cffi_f_avs_new_value_array_w, METH_VARARGS, NULL},
  {"avs_new_value_bool_w", _cffi_f_avs_new_value_bool_w, METH_VARARGS, NULL},
  {"avs_new_value_clip_w", _cffi_f_avs_new_value_clip_w, METH_VARARGS, NULL},
  {"avs_new_value_error_w", _cffi_f_avs_new_value_error_w, METH_VARARGS, NULL},
  {"avs_new_value_float_w", _cffi_f_avs_new_value_float_w, METH_VARARGS, NULL},
  {"avs_new_value_int_w", _cffi_f_avs_new_value_int_w, METH_VARARGS, NULL},
  {"avs_new_value_string_w", _cffi_f_avs_new_value_string_w, METH_VARARGS, NULL},
  {"avs_release_value_w", _cffi_f_avs_release_value_w, METH_O, NULL},
  {"avs_row_size", _cffi_f_avs_row_size, METH_O, NULL},
  {"avs_sample_type", _cffi_f_avs_sample_type, METH_O, NULL},
  {"avs_samples_per_second", _cffi_f_avs_samples_per_second, METH_O, NULL},
  {"avs_set_field_based", _cffi_f_avs_set_field_based, METH_VARARGS, NULL},
  {"avs_set_fps", _cffi_f_avs_set_fps, METH_VARARGS, NULL},
  {"avs_set_global_var_w", _cffi_f_avs_set_global_var_w, METH_VARARGS, NULL},
  {"avs_set_property", _cffi_f_avs_set_property, METH_VARARGS, NULL},
  {"avs_set_var_w", _cffi_f_avs_set_var_w, METH_VARARGS, NULL},
  {"avs_take_clip_w", _cffi_f_avs_take_clip_w, METH_VARARGS, NULL},
  {"free", _cffi_f_free, METH_O, NULL},
  {"malloc", _cffi_f_malloc, METH_O, NULL},
  {"_cffi_layout_struct_AVS_Library", _cffi_layout_struct_AVS_Library, METH_NOARGS, NULL},
  {"_cffi_layout_struct_AVS_Value", _cffi_layout_struct_AVS_Value, METH_NOARGS, NULL},
  {"_cffi_layout_struct_AVS_VideoFrame", _cffi_layout_struct_AVS_VideoFrame, METH_NOARGS, NULL},
  {"_cffi_layout_struct_AVS_VideoFrameBuffer", _cffi_layout_struct_AVS_VideoFrameBuffer, METH_NOARGS, NULL},
  {"_cffi_layout_struct_AVS_VideoInfo", _cffi_layout_struct_AVS_VideoInfo, METH_NOARGS, NULL},
  {"_cffi_setup", _cffi_setup, METH_VARARGS, NULL},
  {NULL, NULL, 0, NULL}    /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef _cffi_module_def = {
  PyModuleDef_HEAD_INIT,
  "avisynth_cffi_ext",
  NULL,
  -1,
  _cffi_methods,
  NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_avisynth_cffi_ext(void)
{
  PyObject *lib;
  lib = PyModule_Create(&_cffi_module_def);
  if (lib == NULL)
    return NULL;
  if (_cffi_const_AVS_IT_FIELDBASED(lib) < 0 || _cffi_init() < 0) {
    Py_DECREF(lib);
    return NULL;
  }
  return lib;
}

#else

PyMODINIT_FUNC
initavisynth_cffi_ext(void)
{
  PyObject *lib;
  lib = Py_InitModule("avisynth_cffi_ext", _cffi_methods);
  if (lib == NULL)
    return;
  if (_cffi_const_AVS_IT_FIELDBASED(lib) < 0 || _cffi_init() < 0)
    return;
  return;
}

#endif
