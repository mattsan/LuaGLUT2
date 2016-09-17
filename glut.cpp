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

lua_State* G_L;

int create_window(lua_State* L)
{
    const char* title = luaL_checkstring(L, 1);
    lua_pushinteger(L, glutCreateWindow(title));
    return 1;
}

int init(lua_State* L)
{
    if( ! lua_istable(L, 1))
    {
        return 0;
    }

    int argc = 0;
    for(;;)
    {
        lua_rawgeti(L, 1, argc);
        if(lua_isnil(L, -1))
        {
            lua_pop(L, 1);
            break;
        }
        ++argc;
    }

    if(argc == 0)
    {
        return 0;
    }

    char** argv = new char*[argc];

    for(int i = 0; i < argc; ++i)
    {
        argv[i] = const_cast<char*>(luaL_checkstring(L, i - argc));
    }
    glutInit(&argc, argv);
    delete[] argv;
    return 0;
}

int init_display_mode(lua_State* L)
{
    unsigned int mode = static_cast<unsigned int>(luaL_checkinteger(L, 1));
    glutInitDisplayMode(mode);
    return 0;
}

int init_window_position(lua_State* L)
{
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    glutInitWindowPosition(x, y);
    return 0;
}

int init_window_size(lua_State* L)
{
    int width  = luaL_checkinteger(L, 1);
    int height = luaL_checkinteger(L, 2);
    glutInitWindowSize(width, height);
    return 0;
}

void* getFont(int font_id)
{
    switch(font_id)
    {
    case STROKE_ROMAN:      return GLUT_STROKE_ROMAN;
    case STROKE_MONO_ROMAN: return GLUT_STROKE_MONO_ROMAN;
    default:                return 0;
    }
}

int stroke_character(lua_State* L)
{
    void* font      = getFont(luaL_checkinteger(L, 1));
    int   character = luaL_checkinteger(L, 2);
    if(font != 0)
    {
        glutStrokeCharacter(font, character);
    }
    return 0;
}

int swap_buffers(lua_State* L)
{
    glutSwapBuffers();
    return 0;
}

void display()
{
    lua_getfield(G_L, 1, "display");
    if( ! lua_isfunction(G_L, -1))
    {
        lua_pop(G_L, 1);
        return;
    }

    lua_pushvalue(G_L, 1);
    lua_call(G_L, 1, 0);
}

void reshape(int w, int h)
{
    lua_getfield(G_L, 1, "reshape");
    if( ! lua_isfunction(G_L, -1))
    {
        lua_pop(G_L, 1);
        return;
    }

    lua_pushvalue(G_L, 1);
    lua_pushinteger(G_L, w);
    lua_pushinteger(G_L, h);
    lua_call(G_L, 3, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    lua_getfield(G_L, 1, "keyboard");
    if( ! lua_isfunction(G_L, -1))
    {
        lua_pop(G_L, 1);
        return;
    }

    lua_pushvalue(G_L, 1);
    lua_pushinteger(G_L, key);
    lua_pushinteger(G_L, x);
    lua_pushinteger(G_L, y);
    lua_call(G_L, 4, 0);
}

void special(int key, int x, int y)
{
    lua_getfield(G_L, 1, "special");
    if( ! lua_isfunction(G_L, -1))
    {
        lua_pop(G_L, 1);
        return;
    }

    lua_pushvalue(G_L, 1);
    lua_pushinteger(G_L, key);
    lua_pushinteger(G_L, x);
    lua_pushinteger(G_L, y);
    lua_call(G_L, 4, 0);
}

void timer(int n)
{
    lua_getfield(G_L, 1, "timer");
    if( ! lua_isfunction(G_L, -1))
    {
        lua_pop(G_L, 1);
        return;
    }

    lua_pushvalue(G_L, 1);
    lua_pushinteger(G_L, n);
    lua_call(G_L, 2, 0);
    lua_getfield(G_L, 1, "timer_enabled");
    if(lua_toboolean(G_L, -1))
    {
        lua_getfield(G_L, 1, "interval");
        unsigned int interval = static_cast<unsigned int>(luaL_checkinteger(G_L, -1));
        glutTimerFunc(interval, timer, 0);
        lua_pop(G_L, 1);
    }
    lua_pop(G_L, 1);
}

int main_loop(lua_State* L)
{
    G_L = L;

    lua_getfield(L, 1, "display");
    if(lua_isfunction(L, -1))
    {
        glutDisplayFunc(display);
    }

    lua_getfield(L, 1, "keyboard");
    if(lua_isfunction(L, -1))
    {
        glutKeyboardFunc(keyboard);
    }

    lua_getfield(L, 1, "reshape");
    if(lua_isfunction(L, -1))
    {
        glutReshapeFunc(reshape);
    }

    lua_getfield(L, 1, "special");
    if(lua_isfunction(L, -1))
    {
        glutSpecialFunc(special);
    }

    lua_getfield(L, 1, "timer");
    if(lua_isfunction(L, -1))
    {
        lua_getfield(L, 1, "interval");
        unsigned int interval = static_cast<unsigned int>(luaL_checkinteger(L, -1));
        glutTimerFunc(interval, timer, 0);
    }
    lua_settop(L, 1);

    glutMainLoop();

    return 0;
}

const luaL_Reg glutlib[] =
{
    { "create_window",        create_window        },
    { "init",                 init                 },
    { "init_display_mode",    init_display_mode    },
    { "init_window_position", init_window_position },
    { "init_window_size",     init_window_size     },
    { "stroke_character",     stroke_character     },
    { "swap_buffers",         swap_buffers         },
    { "main_loop",            main_loop            },
    { NULL,                   NULL                 }
};

} // anonymous namespace

#define REGISTER_CONST(L, pre, name) (lua_pushnumber(L, pre##name), lua_setfield(L, -2, #name))

extern "C"
{

LUALIB_API int luaopen_glut(lua_State* L)
{
    luaL_newlib(L, glutlib);
    REGISTER_CONST(L, GLUT_, RGBA);
    REGISTER_CONST(L, GLUT_, DOUBLE);
    REGISTER_CONST(L, GLUT_, KEY_LEFT);
    REGISTER_CONST(L, GLUT_, KEY_UP);
    REGISTER_CONST(L, GLUT_, KEY_RIGHT);
    REGISTER_CONST(L, GLUT_, KEY_DOWN);
    lua_pushnumber(L, STROKE_ROMAN);
    lua_setfield(L, -2, "STROKE_ROMAN");
    lua_pushnumber(L, STROKE_MONO_ROMAN);
    lua_setfield(L, -2, "STROKE_MONO_ROMAN");

    return 1;
}

} // extern "C"
