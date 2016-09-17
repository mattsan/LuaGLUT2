extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

#include <GLUT/glut.h>

namespace
{

enum
{
    STROKE_ROMAN,
    STROKE_MONO_ROMAN
};

int primitive(lua_State* L)
{
    GLenum mode = luaL_checkinteger(L, 1);
    if(lua_isfunction(L, 2))
    {
        glBegin(mode);
        lua_call(L, 0, 0);
        glEnd();
    }
    return 0;
}

int clear(lua_State* L)
{
    GLbitfield mask = luaL_checkinteger(L, 1);
    glClear(mask);
    return 0;
}

int clear_color(lua_State* L)
{
    GLclampf red   = luaL_checknumber(L, 1);
    GLclampf green = luaL_checknumber(L, 2);
    GLclampf blue  = luaL_checknumber(L, 3);
    GLclampf alpha = luaL_checknumber(L, 4);
    glClearColor(red, green, blue, alpha);
    return 0;
}

int color3d(lua_State* L)
{
    GLdouble red   = luaL_checknumber(L, 1);
    GLdouble green = luaL_checknumber(L, 2);
    GLdouble blue  = luaL_checknumber(L, 3);
    glColor3d(red, green, blue);
    return 0;
}

int color3dv(lua_State* L)
{
    GLdouble rgb[3];
    rgb[0] = luaL_checknumber(L, 1);
    rgb[1] = luaL_checknumber(L, 2);
    rgb[2] = luaL_checknumber(L, 3);
    glColor3dv(rgb);
    return 0;
}

int flush(lua_State* L)
{
    glFlush();
    return 0;
}

int load_identity(lua_State* L)
{
    glLoadIdentity();
    return 0;
}

int ortho(lua_State* L)
{
    GLdouble left   = luaL_checknumber(L, 1);
    GLdouble right  = luaL_checknumber(L, 2);
    GLdouble bottom = luaL_checknumber(L, 3);
    GLdouble top    = luaL_checknumber(L, 4);
    GLdouble zNear  = luaL_checknumber(L, 5);
    GLdouble zFar   = luaL_checknumber(L, 6);
    glOrtho(left, right, bottom, top, zNear, zFar);
    return 0;
}

int pop_matrix(lua_State* L)
{
    glPopMatrix();
    return 0;
}

int push_matrix(lua_State* L)
{
    glPushMatrix();
    return 0;
}

int scalef(lua_State* L)
{
    GLfloat x = luaL_checknumber(L, 1);
    GLfloat y = luaL_checknumber(L, 2);
    GLfloat z = luaL_checknumber(L, 3);
    glScalef(x, y, z);
    return 0;
}

int translatef(lua_State* L)
{
    GLfloat x = luaL_checknumber(L, 1);
    GLfloat y = luaL_checknumber(L, 2);
    GLfloat z = luaL_checknumber(L, 3);
    glTranslatef(x, y, z);
    return 0;
}

int vertex2i(lua_State* L)
{
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    glVertex2i(x, y);
    return 0;
}

int viewport(lua_State* L)
{
    GLint x        = luaL_checkinteger(L, 1);
    GLint y        = luaL_checkinteger(L, 2);
    GLsizei width  = luaL_checkinteger(L, 3);
    GLsizei height = luaL_checkinteger(L, 4);
    glViewport(x, y, width, height);
    return 0;
}

const luaL_Reg gllib[] =
{
    { "primitive",     primitive     },
    { "clear",         clear         },
    { "clear_color",   clear_color   },
    { "color3d",       color3d       },
    { "color3dv",      color3dv      },
    { "flush",         flush         },
    { "load_identity", load_identity },
    { "ortho",         ortho         },
    { "pop_matrix",    pop_matrix    },
    { "push_matrix",   push_matrix   },
    { "scalef",        scalef        },
    { "translatef",    translatef    },
    { "vertex2i",      vertex2i      },
    { "viewport",      viewport      },
    { NULL,            NULL          }
};

} // anonymous namespace

#define REGISTER_CONST(L, pre, name) (lua_pushnumber(L, pre##name), lua_setfield(L, -2, #name))

extern "C"
{

LUALIB_API int luaopen_gl(lua_State* L)
{
    luaL_newlib(L, gllib);
    REGISTER_CONST(L, GL_, POINTS);
    REGISTER_CONST(L, GL_, LINES);
    REGISTER_CONST(L, GL_, LINE_LOOP);
    REGISTER_CONST(L, GL_, LINE_STRIP);
    REGISTER_CONST(L, GL_, TRIANGLES);
    REGISTER_CONST(L, GL_, TRIANGLE_STRIP);
    REGISTER_CONST(L, GL_, TRIANGLE_FAN);
    REGISTER_CONST(L, GL_, QUADS);
    REGISTER_CONST(L, GL_, QUAD_STRIP);
    REGISTER_CONST(L, GL_, POLYGON);

    REGISTER_CONST(L, GL_, CURRENT_BIT);
    REGISTER_CONST(L, GL_, POINT_BIT);
    REGISTER_CONST(L, GL_, LINE_BIT);
    REGISTER_CONST(L, GL_, POLYGON_BIT);
    REGISTER_CONST(L, GL_, POLYGON_STIPPLE_BIT);
    REGISTER_CONST(L, GL_, PIXEL_MODE_BIT);
    REGISTER_CONST(L, GL_, LIGHTING_BIT);
    REGISTER_CONST(L, GL_, FOG_BIT);
    REGISTER_CONST(L, GL_, DEPTH_BUFFER_BIT);
    REGISTER_CONST(L, GL_, ACCUM_BUFFER_BIT);
    REGISTER_CONST(L, GL_, STENCIL_BUFFER_BIT);
    REGISTER_CONST(L, GL_, VIEWPORT_BIT);
    REGISTER_CONST(L, GL_, TRANSFORM_BIT);
    REGISTER_CONST(L, GL_, ENABLE_BIT);
    REGISTER_CONST(L, GL_, COLOR_BUFFER_BIT);
    REGISTER_CONST(L, GL_, HINT_BIT);
    REGISTER_CONST(L, GL_, EVAL_BIT);
    REGISTER_CONST(L, GL_, LIST_BIT);
    REGISTER_CONST(L, GL_, TEXTURE_BIT);
    REGISTER_CONST(L, GL_, SCISSOR_BIT);
    REGISTER_CONST(L, GL_, ALL_ATTRIB_BITS);

    return 1;
}

} // extern "C"
