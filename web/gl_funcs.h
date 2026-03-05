/*
    gl_funcs.h

    GLES3/WebGL2 type definitions, constants, and function declarations
    for wasm32-freestanding builds.

    When compiled with SOKOL_EXTERNAL_GL_LOADER + SOKOL_GLES3, sokol_gfx.h
    expects these symbols to be available. In the WASM module, GL functions
    are declared as imports from the "env" module — the JavaScript host
    provides them by wrapping WebGL2 context methods.
*/
#ifndef GL_FUNCS_H
#define GL_FUNCS_H

/* ---- GL type definitions (freestanding - no stdint.h) ---- */
typedef unsigned int    GLenum;
typedef unsigned char   GLboolean;
typedef unsigned int    GLbitfield;
typedef void            GLvoid;
typedef signed char     GLbyte;
typedef unsigned char   GLubyte;
typedef short           GLshort;
typedef unsigned short  GLushort;
typedef int             GLint;
typedef unsigned int    GLuint;
typedef int             GLclampx;
typedef int             GLsizei;
typedef float           GLfloat;
typedef float           GLclampf;
typedef double          GLdouble;
typedef double          GLclampd;
typedef char            GLchar;
typedef long            GLintptr;
typedef long            GLsizeiptr;

/* ---- GL boolean values ---- */
#define GL_FALSE                          0
#define GL_TRUE                           1

/* ---- GL error codes ---- */
#define GL_NO_ERROR                       0

/* ---- Data types ---- */
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_HALF_FLOAT                     0x140B

/* ---- Primitive types ---- */
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005

/* ---- Buffer objects ---- */
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_SHADER_STORAGE_BUFFER          0x90D2
#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8

/* ---- Blend equations ---- */
#define GL_FUNC_ADD                       0x8006
#define GL_FUNC_SUBTRACT                  0x800A
#define GL_FUNC_REVERSE_SUBTRACT          0x800B
#define GL_MIN                            0x8007
#define GL_MAX                            0x8008

/* ---- Blend factors ---- */
#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305
#define GL_DST_COLOR                      0x0306
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_SRC_ALPHA_SATURATE             0x0308
#define GL_CONSTANT_COLOR                 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR       0x8002
#define GL_CONSTANT_ALPHA                 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA       0x8004
#define GL_SRC1_COLOR                     0x88F9
#define GL_ONE_MINUS_SRC1_COLOR           0x88FA
#define GL_SRC1_ALPHA                     0x8589
#define GL_ONE_MINUS_SRC1_ALPHA           0x88FB

/* ---- Enable/Disable caps ---- */
#define GL_BLEND                          0x0BE2
#define GL_CULL_FACE                      0x0B44
#define GL_DEPTH_TEST                     0x0B71
#define GL_DITHER                         0x0BD0
#define GL_POLYGON_OFFSET_FILL            0x8037
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SCISSOR_TEST                   0x0C11
#define GL_STENCIL_TEST                   0x0B90
#define GL_MULTISAMPLE                    0x809D
#define GL_PROGRAM_POINT_SIZE             0x8642
#define GL_FRAMEBUFFER_SRGB               0x8DB9
#define GL_TEXTURE_CUBE_MAP_SEAMLESS      0x884F

/* ---- Face culling ---- */
#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_CW                             0x0900
#define GL_CCW                            0x0901

/* ---- Depth / Stencil comparison ---- */
#define GL_NEVER                          0x0200
#define GL_LESS                           0x0201
#define GL_EQUAL                          0x0202
#define GL_LEQUAL                         0x0203
#define GL_GREATER                        0x0204
#define GL_NOTEQUAL                       0x0205
#define GL_GEQUAL                         0x0206
#define GL_ALWAYS                         0x0207

/* ---- Stencil ops ---- */
#define GL_KEEP                           0x1E00
#define GL_REPLACE                        0x1E01
#define GL_INCR                           0x1E02
#define GL_DECR                           0x1E03
#define GL_INVERT                         0x150A
#define GL_INCR_WRAP                      0x8507
#define GL_DECR_WRAP                      0x8508

/* ---- Texture targets ---- */
#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE_3D                     0x806F
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_2D_ARRAY               0x8C1A
#define GL_TEXTURE_2D_MULTISAMPLE         0x9100
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY   0x9102

/* ---- Cubemap faces ---- */
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A

/* ---- Texture parameters ---- */
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_TEXTURE_MIN_LOD                0x813A
#define GL_TEXTURE_MAX_LOD                0x813B
#define GL_TEXTURE_MAX_LEVEL              0x813D
#define GL_TEXTURE_COMPARE_MODE           0x884C
#define GL_TEXTURE_COMPARE_FUNC           0x884E
#define GL_TEXTURE_BORDER_COLOR           0x1004
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#define GL_COMPARE_REF_TO_TEXTURE         0x884E

/* ---- Texture filter values ---- */
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703

/* ---- Texture wrap values ---- */
#define GL_REPEAT                         0x2901
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_CLAMP_TO_BORDER                0x812D
#define GL_MIRRORED_REPEAT                0x8370

/* ---- Pixel store ---- */
#define GL_UNPACK_ALIGNMENT               0x0CF5

/* ---- Texture unit ---- */
#define GL_TEXTURE0                       0x84C0
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_MAX_TEXTURE_IMAGE_UNITS        0x8872

/* ---- Pixel / internal formats ---- */
#define GL_RED                            0x1903
#define GL_RED_INTEGER                    0x8D94
#define GL_RG                             0x8227
#define GL_RG_INTEGER                     0x8228
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_RGBA_INTEGER                   0x8D99
#define GL_LUMINANCE                      0x1909
#define GL_BGRA                           0x80E1

#define GL_R8                             0x8229
#define GL_R8_SNORM                       0x8F94
#define GL_R8UI                           0x8232
#define GL_R8I                            0x8231
#define GL_R16                            0x822A
#define GL_R16_SNORM                      0x8F98
#define GL_R16UI                          0x8234
#define GL_R16I                           0x8233
#define GL_R16F                           0x822D
#define GL_R32UI                          0x8236
#define GL_R32I                           0x8235
#define GL_R32F                           0x822E
#define GL_RG8                            0x822B
#define GL_RG8_SNORM                      0x8F95
#define GL_RG8UI                          0x8238
#define GL_RG8I                           0x8237
#define GL_RG16                           0x822C
#define GL_RG16_SNORM                     0x8F99
#define GL_RG16UI                         0x823A
#define GL_RG16I                          0x8239
#define GL_RG16F                          0x822F
#define GL_RG32UI                         0x823C
#define GL_RG32I                          0x823B
#define GL_RG32F                          0x8230
#define GL_RGBA8                          0x8058
#define GL_SRGB8_ALPHA8                   0x8C43
#define GL_RGBA8_SNORM                    0x8F97
#define GL_RGBA8UI                        0x8D7C
#define GL_RGBA8I                         0x8D8E
#define GL_RGB10_A2                       0x8059
#define GL_R11F_G11F_B10F                 0x8C3A
#define GL_RGB9_E5                        0x8C3D
#define GL_RGBA16                         0x805B
#define GL_RGBA16_SNORM                   0x8F9B
#define GL_RGBA16UI                       0x8D76
#define GL_RGBA16I                        0x8D88
#define GL_RGBA16F                        0x881A
#define GL_RGBA32UI                       0x8D70
#define GL_RGBA32I                        0x8D82
#define GL_RGBA32F                        0x8814
#define GL_RGB5_A1                        0x8057
#define GL_RGB5                           0x8050
#define GL_RGBA4                          0x8056
#define GL_RGB8                           0x8051
#define GL_RGB8_SNORM                     0x8F96
#define GL_RGB8UI                         0x8D7D
#define GL_RGB8I                          0x8D8F
#define GL_RGB16_SNORM                    0x8F9A
#define GL_RGB16UI                        0x8D77
#define GL_RGB16I                         0x8D89
#define GL_RGB32UI                        0x8D71
#define GL_RGB32I                         0x8D83
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define GL_UNSIGNED_INT_2_10_10_10_REV    0x8368
#define GL_UNSIGNED_INT_10F_11F_11F_REV   0x8C3B
#define GL_UNSIGNED_INT_5_9_9_9_REV       0x8C3E
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_INT_2_10_10_10_REV             0x8D9F

/* ---- Depth / stencil formats ---- */
#define GL_DEPTH_COMPONENT                0x1902
#define GL_DEPTH_COMPONENT32F             0x8CAC
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_DEPTH24_STENCIL8               0x88F0

/* ---- Compressed texture formats ---- */
#define GL_COMPRESSED_RGB8_ETC2                            0x9274
#define GL_COMPRESSED_SRGB8_ETC2                           0x9275
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2        0x9276
#define GL_COMPRESSED_RGBA8_ETC2_EAC                       0x9278
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC                0x9279
#define GL_COMPRESSED_R11_EAC                              0x9270
#define GL_COMPRESSED_SIGNED_R11_EAC                       0x9271
#define GL_COMPRESSED_RG11_EAC                             0x9272
#define GL_COMPRESSED_SIGNED_RG11_EAC                      0x9273
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT                   0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT                   0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT                   0x83F3
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT             0x8C4F
#define GL_COMPRESSED_RED_RGTC1                            0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1                     0x8DBC
#define GL_COMPRESSED_RED_GREEN_RGTC2                      0x8DBD
#define GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2               0x8DBE
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB            0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB          0x8E8F
#define GL_COMPRESSED_RGBA_BPTC_UNORM_ARB                  0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB            0x8E8D
#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR                    0x93B0
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR            0x93D0

/* ---- Framebuffer objects ---- */
#define GL_FRAMEBUFFER                    0x8D40
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_RENDERBUFFER                   0x8D41
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_FRAMEBUFFER_UNDEFINED          0x8219

/* ---- Clear buffer bits ---- */
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400

/* ---- Clear buffer types for glClearBuffer ---- */
#define GL_COLOR                          0x1800
#define GL_DEPTH                          0x1801
#define GL_STENCIL                        0x1802

/* ---- Shader types ---- */
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_COMPUTE_SHADER                 0x91B9

/* ---- Shader status ---- */
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84

/* ---- Get parameter names ---- */
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_NUM_EXTENSIONS                 0x821D
#define GL_EXTENSIONS                     0x1F03
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_MAX_VERTEX_ATTRIBS             0x8869
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS  0x8B4A
#define GL_MAX_3D_TEXTURE_SIZE            0x8073
#define GL_MAX_TEXTURE_SIZE               0x0D33
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
#define GL_MAX_ARRAY_TEXTURE_LAYERS       0x88FF
#define GL_MAX_DRAW_BUFFERS               0x8824
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_IMAGE_UNITS                0x8F38

/* ---- ES version check (used in sokol for feature detection) ---- */
#define GL_ES_VERSION_3_2                 0
#define GL_VERSION_3_2                    0
#define GL_VERSION_3_3                    0
#define GL_VERSION_4_2                    0
#define GL_VERSION_4_3                    0

/* ---- Draw buffers ---- */
#define GL_NONE                           0

/* ---- Image access / compute barriers ---- */
#define GL_READ_WRITE                     0x88BA
#define GL_WRITE_ONLY                     0x88B9
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT  0x00000020
#define GL_SHADER_STORAGE_BARRIER_BIT     0x00002000
#define GL_TEXTURE_FETCH_BARRIER_BIT      0x00000008
#define GL_FRAMEBUFFER_BARRIER_BIT        0x00000400
#define GL_ELEMENT_ARRAY_BARRIER_BIT      0x00000002
#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT 0x00000001

/* ---- Invalidate framebuffer ---- */
/* GL_COLOR, GL_DEPTH, GL_STENCIL already defined above */

/* ---- WASM import declarations ---- */
/* These functions will be provided by the JavaScript host. */
#define GL_IMPORT __attribute__((import_module("env")))

GL_IMPORT void glActiveTexture(GLenum texture);
GL_IMPORT void glAttachShader(GLuint program, GLuint shader);
GL_IMPORT void glBindBuffer(GLenum target, GLuint buffer);
GL_IMPORT void glBindBufferBase(GLenum target, GLuint index, GLuint buffer);
GL_IMPORT void glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
GL_IMPORT void glBindFramebuffer(GLenum target, GLuint framebuffer);
GL_IMPORT void glBindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
GL_IMPORT void glBindRenderbuffer(GLenum target, GLuint renderbuffer);
GL_IMPORT void glBindSampler(GLuint unit, GLuint sampler);
GL_IMPORT void glBindTexture(GLenum target, GLuint texture);
GL_IMPORT void glBindVertexArray(GLuint array);
GL_IMPORT void glBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GL_IMPORT void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
GL_IMPORT void glBlendFunc(GLenum sfactor, GLenum dfactor);
GL_IMPORT void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
GL_IMPORT void glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GL_IMPORT void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
GL_IMPORT void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
GL_IMPORT GLenum glCheckFramebufferStatus(GLenum target);
GL_IMPORT void glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
GL_IMPORT void glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat* value);
GL_IMPORT void glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint* value);
GL_IMPORT void glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint* value);
GL_IMPORT void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
GL_IMPORT void glColorMaski(GLuint index, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
GL_IMPORT void glCompileShader(GLuint shader);
GL_IMPORT void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
GL_IMPORT void glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
GL_IMPORT void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
GL_IMPORT void glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
GL_IMPORT GLuint glCreateProgram(void);
GL_IMPORT GLuint glCreateShader(GLenum type);
GL_IMPORT void glCullFace(GLenum mode);
GL_IMPORT void glDeleteBuffers(GLsizei n, const GLuint* buffers);
GL_IMPORT void glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers);
GL_IMPORT void glDeleteProgram(GLuint program);
GL_IMPORT void glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers);
GL_IMPORT void glDeleteSamplers(GLsizei count, const GLuint* samplers);
GL_IMPORT void glDeleteShader(GLuint shader);
GL_IMPORT void glDeleteTextures(GLsizei n, const GLuint* textures);
GL_IMPORT void glDeleteVertexArrays(GLsizei n, const GLuint* arrays);
GL_IMPORT void glDepthFunc(GLenum func);
GL_IMPORT void glDepthMask(GLboolean flag);
GL_IMPORT void glDisable(GLenum cap);
GL_IMPORT void glDisableVertexAttribArray(GLuint index);
GL_IMPORT void glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
GL_IMPORT void glDrawArrays(GLenum mode, GLint first, GLsizei count);
GL_IMPORT void glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
GL_IMPORT void glDrawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
GL_IMPORT void glDrawBuffers(GLsizei n, const GLenum* bufs);
GL_IMPORT void glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices);
GL_IMPORT void glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const void* indices, GLint basevertex);
GL_IMPORT void glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount);
GL_IMPORT void glDrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex);
GL_IMPORT void glDrawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
GL_IMPORT void glEnable(GLenum cap);
GL_IMPORT void glEnableVertexAttribArray(GLuint index);
GL_IMPORT void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
GL_IMPORT void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GL_IMPORT void glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
GL_IMPORT void glFrontFace(GLenum mode);
GL_IMPORT void glGenBuffers(GLsizei n, GLuint* buffers);
GL_IMPORT void glGenFramebuffers(GLsizei n, GLuint* framebuffers);
GL_IMPORT void glGenRenderbuffers(GLsizei n, GLuint* renderbuffers);
GL_IMPORT void glGenSamplers(GLsizei count, GLuint* samplers);
GL_IMPORT void glGenTextures(GLsizei n, GLuint* textures);
GL_IMPORT void glGenVertexArrays(GLsizei n, GLuint* arrays);
GL_IMPORT GLint glGetAttribLocation(GLuint program, const GLchar* name);
GL_IMPORT GLenum glGetError(void);
GL_IMPORT void glGetIntegerv(GLenum pname, GLint* data);
GL_IMPORT void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
GL_IMPORT void glGetProgramiv(GLuint program, GLenum pname, GLint* params);
GL_IMPORT void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
GL_IMPORT void glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
GL_IMPORT const GLubyte* glGetStringi(GLenum name, GLuint index);
GL_IMPORT GLint glGetUniformLocation(GLuint program, const GLchar* name);
GL_IMPORT void glInvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum* attachments);
GL_IMPORT void glLinkProgram(GLuint program);
GL_IMPORT void glMemoryBarrier(GLbitfield barriers);
GL_IMPORT void glPixelStorei(GLenum pname, GLint param);
GL_IMPORT void glPolygonOffset(GLfloat factor, GLfloat units);
GL_IMPORT void glReadBuffer(GLenum src);
GL_IMPORT void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
GL_IMPORT void glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GL_IMPORT void glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param);
GL_IMPORT void glSamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat* param);
GL_IMPORT void glSamplerParameteri(GLuint sampler, GLenum pname, GLint param);
GL_IMPORT void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
GL_IMPORT void glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
GL_IMPORT void glStencilFunc(GLenum func, GLint ref, GLuint mask);
GL_IMPORT void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
GL_IMPORT void glStencilMask(GLuint mask);
GL_IMPORT void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass);
GL_IMPORT void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
GL_IMPORT void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
GL_IMPORT void glTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
GL_IMPORT void glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
GL_IMPORT void glTexImage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
GL_IMPORT void glTexParameterf(GLenum target, GLenum pname, GLfloat param);
GL_IMPORT void glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params);
GL_IMPORT void glTexParameteri(GLenum target, GLenum pname, GLint param);
GL_IMPORT void glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
GL_IMPORT void glTexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
GL_IMPORT void glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
GL_IMPORT void glTexStorage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
GL_IMPORT void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
GL_IMPORT void glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
GL_IMPORT void glTextureView(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
GL_IMPORT void glUniform1fv(GLint location, GLsizei count, const GLfloat* value);
GL_IMPORT void glUniform1i(GLint location, GLint v0);
GL_IMPORT void glUniform1iv(GLint location, GLsizei count, const GLint* value);
GL_IMPORT void glUniform2fv(GLint location, GLsizei count, const GLfloat* value);
GL_IMPORT void glUniform2iv(GLint location, GLsizei count, const GLint* value);
GL_IMPORT void glUniform3fv(GLint location, GLsizei count, const GLfloat* value);
GL_IMPORT void glUniform3iv(GLint location, GLsizei count, const GLint* value);
GL_IMPORT void glUniform4fv(GLint location, GLsizei count, const GLfloat* value);
GL_IMPORT void glUniform4iv(GLint location, GLsizei count, const GLint* value);
GL_IMPORT void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GL_IMPORT void glUseProgram(GLuint program);
GL_IMPORT void glVertexAttribDivisor(GLuint index, GLuint divisor);
GL_IMPORT void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
GL_IMPORT void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
GL_IMPORT void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

#undef GL_IMPORT

#endif /* GL_FUNCS_H */
