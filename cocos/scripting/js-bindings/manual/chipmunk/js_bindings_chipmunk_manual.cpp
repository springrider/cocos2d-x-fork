/*
 * Copyright (c) 2012 Zynga Inc.
 * Copyright (c) 2013-2014 Chukong Technologies Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "jsapi.h"
#include "jsfriendapi.h"
#include "extensions/cocos-ext.h"
#include "js_bindings_config.h"
#ifdef JSB_INCLUDE_CHIPMUNK

#include "js_bindings_chipmunk_manual.h"
#include "js_manual_conversions.h"

USING_NS_CC_EXT;
// Function declarations
void static freeSpaceChildren(cpSpace *space);

template<class T>
static bool dummy_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    TypeTest<T> t;
    T* cobj = new T();
    cobj->autorelease();
    js_type_class_t *p;
    std::string typeName = t.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    p = typeMapIter->second;
    CCASSERT(p, "The value is null.");
    
    JSObject *_tmp = JS_NewObject(cx, p->jsclass, p->proto, p->parentProto);
    js_proxy_t *pp = jsb_new_proxy(cobj, _tmp);
    JS_AddObjectRoot(cx, &pp->obj);
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(_tmp));

    return true;
}

#pragma mark - convertions

/*
 * PhysicsSprite
 */
#pragma mark - PhysicsSprite

JSClass* JSPROXY_CCPhysicsSprite_class = NULL;
JSObject* JSPROXY_CCPhysicsSprite_object = NULL;
// Constructor

// Destructor
void JSPROXY_CCPhysicsSprite_finalize(JSFreeOp *fop, JSObject *obj)
{
    CCLOGINFO("jsbindings: finalizing JS object %p (PhysicsSprite)", obj);
}

// Arguments:
// Ret value: BOOL (b)
bool JSPROXY_CCPhysicsSprite_isDirty(JSContext *cx, uint32_t argc, jsval *vp) {
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    PhysicsSprite* real = (PhysicsSprite *)(proxy ? proxy->ptr : NULL);
    TEST_NATIVE_OBJECT(cx, real)
    
    if (real->isDirty()) {
        JS_SET_RVAL(cx, vp, JSVAL_TRUE);
    }
    else JS_SET_RVAL(cx, vp, JSVAL_FALSE);
    return true;
}

// Arguments:
// Ret value: cpBody* (N/A)
bool JSPROXY_CCPhysicsSprite_getCPBody(JSContext *cx, uint32_t argc, jsval *vp) {
    
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    PhysicsSprite* real = (PhysicsSprite *)(proxy ? proxy->ptr : NULL);
    TEST_NATIVE_OBJECT(cx, real)
    cpBody* ret_val;
    
    ret_val = real->getCPBody();
    jsval ret_jsval = c_class_to_jsval( cx, ret_val, JSB_cpBody_object, JSB_cpBody_class, "cpBody" );
    JS_SET_RVAL(cx, vp, ret_jsval);
    
    return true;
}

// Arguments:
// Ret value: BOOL (b)
bool JSPROXY_CCPhysicsSprite_ignoreBodyRotation(JSContext *cx, uint32_t argc, jsval *vp) {
    
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    PhysicsSprite* real = (PhysicsSprite *)(proxy ? proxy->ptr : NULL);
    TEST_NATIVE_OBJECT(cx, real)
    
    bool ret_val;
    
    ret_val = real->isIgnoreBodyRotation();
    JS_SET_RVAL(cx, vp, BOOLEAN_TO_JSVAL(ret_val));
    return true;
}

// Arguments: cpBody*
// Ret value: void (None)
bool JSPROXY_CCPhysicsSprite_setCPBody_(JSContext *cx, uint32_t argc, jsval *vp) {
    
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    PhysicsSprite* real = (PhysicsSprite *)(proxy ? proxy->ptr : NULL);
    TEST_NATIVE_OBJECT(cx, real)
    
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    
    cpBody* arg0;
    
    ok &= jsval_to_opaque( cx, *argvp++, (void**)&arg0 );
    if( ! ok ) return false;
    
    real->setCPBody((cpBody*)arg0);
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return true;
}

// Arguments: BOOL
// Ret value: void (None)
bool JSPROXY_CCPhysicsSprite_setIgnoreBodyRotation_(JSContext *cx, uint32_t argc, jsval *vp) {
    
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    PhysicsSprite* real = (PhysicsSprite *)(proxy ? proxy->ptr : NULL);
    TEST_NATIVE_OBJECT(cx, real)
    
    jsval *argvp = JS_ARGV(cx,vp);
    bool arg0 = JS::ToBoolean( JS::RootedValue(cx, *argvp++) );
    
    real->setIgnoreBodyRotation((bool)arg0);
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return true;
}

/*
 * PhysicsDebugNode
 */
//#pragma mark - PhysicsDebugNode

JSClass* JSB_CCPhysicsDebugNode_class = NULL;
JSObject* JSB_CCPhysicsDebugNode_object = NULL;
extern JSObject *js_cocos2dx_CCDrawNode_prototype;

// Constructor

// Destructor
void JSB_CCPhysicsDebugNode_finalize(JSFreeOp *fop, JSObject *obj)
{
    CCLOGINFO("jsbindings: finalizing JS object %p (PhysicsDebugNode)", obj);
}

// Arguments: cpSpace*
// Ret value: PhysicsDebugNode* (o)
bool JSB_CCPhysicsDebugNode_debugNodeForCPSpace__static(JSContext *cx, uint32_t argc, jsval *vp) {
    JSB_PRECONDITION2( argc == 1, cx, false, "Invalid number of arguments" );
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpSpace* arg0; 

    ok &= jsval_to_opaque( cx, *argvp++, (void**)&arg0 );
    JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");

    PhysicsDebugNode* ret = PhysicsDebugNode::create(arg0);
    jsval jsret;
    do {
        if (ret) {
            TypeTest<PhysicsDebugNode> t;
            js_type_class_t *typeClass = nullptr;
            std::string typeName = t.s_name();
            auto typeMapIter = _js_global_type_map.find(typeName);
            
            CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
            typeClass = typeMapIter->second;
            CCASSERT(typeClass, "The value is null.");
            
            JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
            jsret = OBJECT_TO_JSVAL(obj);
            js_proxy_t *p = jsb_new_proxy(ret, obj);
            JS_AddNamedObjectRoot(cx, &p->obj, "CCDebugNode");
        } else {
            jsret = JSVAL_NULL;
        }
    } while (0);
    JS_SET_RVAL(cx, vp, jsret);

    return true;
}

// Arguments: cpSpace*
// Ret value: void (None)
bool JSB_CCPhysicsDebugNode_setSpace_(JSContext *cx, uint32_t argc, jsval *vp) {

    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PhysicsDebugNode* real = (PhysicsDebugNode *)(proxy ? proxy->ptr : NULL);
    TEST_NATIVE_OBJECT(cx, real)

    JSB_PRECONDITION2( argc == 1, cx, false, "Invalid number of arguments" );
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpSpace* arg0; 

    ok &= jsval_to_opaque( cx, *argvp++, (void**)&arg0 );
    JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");

    real->setSpace(arg0);
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return true;
}

// Arguments: 
// Ret value: cpSpace* (N/A)
bool JSB_CCPhysicsDebugNode_space(JSContext *cx, uint32_t argc, jsval *vp) {

    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    PhysicsDebugNode* real = (PhysicsDebugNode *)(proxy ? proxy->ptr : NULL);
    TEST_NATIVE_OBJECT(cx, real)
    JSB_PRECONDITION2( argc == 0, cx, false, "Invalid number of arguments" );
    cpSpace* ret_val;

    ret_val = real->getSpace();

    jsval ret_jsval = opaque_to_jsval( cx, ret_val );
    JS_SET_RVAL(cx, vp, ret_jsval);
    
    return true;
}

bool JSB_CCPhysicsDebugNode_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    PhysicsDebugNode* cobj = new (std::nothrow) PhysicsDebugNode();
    cocos2d::Ref *_ccobj = dynamic_cast<cocos2d::Ref *>(cobj);
    if (_ccobj) {
        _ccobj->autorelease();
    }
    TypeTest<PhysicsDebugNode> t;
    js_type_class_t *typeClass = nullptr;
    std::string typeName = t.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    typeClass = typeMapIter->second;
    CCASSERT(typeClass, "The value is null.");
    JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
    // link the native object with the javascript object
    js_proxy_t* p = jsb_new_proxy(cobj, obj);
    JS_AddNamedObjectRoot(cx, &p->obj, "PhysicsDebugNode");
    if (JS_HasProperty(cx, obj, "_ctor", &ok))
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", argc, argv);
    return true;
}

void JSB_CCPhysicsDebugNode_createClass(JSContext *cx, JSObject* globalObj, const char* name )
{
    JSB_CCPhysicsDebugNode_class = (JSClass *)calloc(1, sizeof(JSClass));
    JSB_CCPhysicsDebugNode_class->name = name;
    JSB_CCPhysicsDebugNode_class->addProperty = JS_PropertyStub;
    JSB_CCPhysicsDebugNode_class->delProperty = JS_DeletePropertyStub;
    JSB_CCPhysicsDebugNode_class->getProperty = JS_PropertyStub;
    JSB_CCPhysicsDebugNode_class->setProperty = JS_StrictPropertyStub;
    JSB_CCPhysicsDebugNode_class->enumerate = JS_EnumerateStub;
    JSB_CCPhysicsDebugNode_class->resolve = JS_ResolveStub;
    JSB_CCPhysicsDebugNode_class->convert = JS_ConvertStub;
    JSB_CCPhysicsDebugNode_class->finalize = JSB_CCPhysicsDebugNode_finalize;
    JSB_CCPhysicsDebugNode_class->flags = 0;

    static JSPropertySpec properties[] = {
        {0, 0, 0, 0, 0}
    };
    static JSFunctionSpec funcs[] = {
        JS_FN("_setSpace", JSB_CCPhysicsDebugNode_setSpace_, 1, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FN("getSpace", JSB_CCPhysicsDebugNode_space, 0, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FS_END
    };
    static JSFunctionSpec st_funcs[] = {
        JS_FN("_create", JSB_CCPhysicsDebugNode_debugNodeForCPSpace__static, 1, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FS_END
    };

    TypeTest<cocos2d::DrawNode> t1;
    js_type_class_t *typeClass = nullptr;
    std::string typeName = t1.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    typeClass = typeMapIter->second;
    CCASSERT(typeClass, "The value is null.");

    JSB_CCPhysicsDebugNode_object = JS_InitClass(cx, globalObj, typeClass->proto, JSB_CCPhysicsDebugNode_class, JSB_CCPhysicsDebugNode_constructor, 0,properties,funcs,NULL,st_funcs);

    TypeTest<PhysicsDebugNode> t;
    js_type_class_t *p;
    typeName = t.s_name();
    
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = JSB_CCPhysicsDebugNode_class;
        p->proto = JSB_CCPhysicsDebugNode_object;
        p->parentProto = typeClass->proto;
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
}

// Arguments: NSString*, CGRect
// Ret value: PhysicsSprite* (o)
bool JSPROXY_CCPhysicsSprite_spriteWithFile_rect__static(JSContext *cx, uint32_t argc, jsval *vp) {

    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    if (argc == 2) {
        const char* arg0;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, argv[0], &arg0_tmp); arg0 = arg0_tmp.c_str();
        cocos2d::Rect arg1;
        ok &= jsval_to_ccrect(cx, argv[1], &arg1);
        JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");
        
        PhysicsSprite* ret = PhysicsSprite::create(arg0, arg1);

        jsval jsret;
        do {
            if (ret) {
                TypeTest<PhysicsSprite> t;
                js_type_class_t *typeClass = nullptr;
                std::string typeName = t.s_name();
                auto typeMapIter = _js_global_type_map.find(typeName);
                CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
                typeClass = typeMapIter->second;
                CCASSERT(typeClass, "The value is null.");
                
                JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
                jsret = OBJECT_TO_JSVAL(obj);
                js_proxy_t *p = jsb_new_proxy(ret, obj);
                JS_AddNamedObjectRoot(cx, &p->obj, "CCPhysicsSprite");
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 1) {
        const char* arg0;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, argv[0], &arg0_tmp); arg0 = arg0_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");
        
        PhysicsSprite* ret = PhysicsSprite::create(arg0);

        jsval jsret;
        do {
            if (ret) {
                TypeTest<PhysicsSprite> t;
                js_type_class_t *typeClass = nullptr;
                std::string typeName = t.s_name();
                auto typeMapIter = _js_global_type_map.find(typeName);
                CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
                typeClass = typeMapIter->second;
                CCASSERT(typeClass, "The value is null.");
                JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
                jsret = OBJECT_TO_JSVAL(obj);
                js_proxy_t *p = jsb_new_proxy(ret, obj);
                JS_AddNamedObjectRoot(cx, &p->obj, "CCPhysicsSprite");
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    return false;

}

// Arguments: SpriteFrame*
// Ret value: PhysicsSprite* (o)
bool JSPROXY_CCPhysicsSprite_spriteWithSpriteFrame__static(JSContext *cx, uint32_t argc, jsval *vp) {
    jsval *argv = JS_ARGV(cx, vp);
    cocos2d::SpriteFrame* arg0;
    if (argc >= 1) {
        do {
            js_proxy_t *proxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[0]);
            proxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::SpriteFrame*)(proxy ? proxy->ptr : NULL);
            TEST_NATIVE_OBJECT(cx, arg0)
        } while (0);
    }
    PhysicsSprite* ret = PhysicsSprite::createWithSpriteFrame(arg0);

    jsval jsret;
    do {
        if (ret) {
            TypeTest<PhysicsSprite> t;
            js_type_class_t *typeClass = nullptr;
            std::string typeName = t.s_name();
            auto typeMapIter = _js_global_type_map.find(typeName);
            CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
            typeClass = typeMapIter->second;
            CCASSERT(typeClass, "The value is null.");
            JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
            jsret = OBJECT_TO_JSVAL(obj);
            js_proxy_t *p = jsb_new_proxy(ret, obj);
            JS_AddNamedObjectRoot(cx, &p->obj, "CCPhysicsSprite");
        } else {
            jsret = JSVAL_NULL;
        }
    } while (0);
    JS_SET_RVAL(cx, vp, jsret);
    return true;
}

// Arguments: NSString*
// Ret value: PhysicsSprite* (o)
bool JSPROXY_CCPhysicsSprite_spriteWithSpriteFrameName__static(JSContext *cx, uint32_t argc, jsval *vp) {
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    const char* arg0;
    std::string arg0_tmp;
    if (argc == 1) {
        ok &= jsval_to_std_string(cx, argv[0], &arg0_tmp); arg0 = arg0_tmp.c_str();

        PhysicsSprite* ret = PhysicsSprite::createWithSpriteFrameName(arg0);

        jsval jsret;
        do {
            if (ret) {
                TypeTest<PhysicsSprite> t;
                js_type_class_t *typeClass = nullptr;
                std::string typeName = t.s_name();
                auto typeMapIter = _js_global_type_map.find(typeName);
                CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
                typeClass = typeMapIter->second;
                CCASSERT(typeClass, "The value is null.");
                JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
                jsret = OBJECT_TO_JSVAL(obj);
                js_proxy_t *p = jsb_new_proxy(ret, obj);
                JS_AddNamedObjectRoot(cx, &p->obj, "CCPhysicsSprite");
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}

bool JSPROXY_CCPhysicsSprite_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    PhysicsSprite* cobj = new PhysicsSprite();
    cocos2d::Ref *_ccobj = dynamic_cast<cocos2d::Ref *>(cobj);
    if (_ccobj) {
        _ccobj->autorelease();
    }
    TypeTest<cocos2d::extension::PhysicsSprite> t;
    js_type_class_t *typeClass = nullptr;
    std::string typeName = t.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    typeClass = typeMapIter->second;
    CCASSERT(typeClass, "The value is null.");
    JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
    // link the native object with the javascript object
    js_proxy_t* p = jsb_new_proxy(cobj, obj);
    JS_AddNamedObjectRoot(cx, &p->obj, "cocos2d::extension::PhysicsSprite");
    if (JS_HasProperty(cx, obj, "_ctor", &ok))
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", argc, argv);
    return true;
}

static bool JSPROXY_CCPhysicsSprite_ctor(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    PhysicsSprite *nobj = new PhysicsSprite();
    if (nobj) {
        nobj->autorelease();
    }
    js_proxy_t* p = jsb_new_proxy(nobj, obj);
    JS_AddNamedObjectRoot(cx, &p->obj, "cocos2d::extension::SpriteFrame");
    bool isFound = false;
    if (JS_HasProperty(cx, obj, "_ctor", &isFound))
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", argc, argv);
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return true;
}

void JSPROXY_CCPhysicsSprite_createClass(JSContext *cx, JSObject* globalObj)
{
    JSPROXY_CCPhysicsSprite_class = (JSClass *)calloc(1, sizeof(JSClass));
    JSPROXY_CCPhysicsSprite_class->name = "PhysicsSprite";
    JSPROXY_CCPhysicsSprite_class->addProperty = JS_PropertyStub;
    JSPROXY_CCPhysicsSprite_class->delProperty = JS_DeletePropertyStub;
    JSPROXY_CCPhysicsSprite_class->getProperty = JS_PropertyStub;
    JSPROXY_CCPhysicsSprite_class->setProperty = JS_StrictPropertyStub;
    JSPROXY_CCPhysicsSprite_class->enumerate = JS_EnumerateStub;
    JSPROXY_CCPhysicsSprite_class->resolve = JS_ResolveStub;
    JSPROXY_CCPhysicsSprite_class->convert = JS_ConvertStub;
    JSPROXY_CCPhysicsSprite_class->finalize = JSPROXY_CCPhysicsSprite_finalize;
    JSPROXY_CCPhysicsSprite_class->flags = 0;

    static JSPropertySpec properties[] = {
        {0, 0, 0, 0, 0}
    };
    static JSFunctionSpec funcs[] = {
        JS_FN("isDirty", JSPROXY_CCPhysicsSprite_isDirty, 0, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FN("getCPBody", JSPROXY_CCPhysicsSprite_getCPBody, 0, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FN("getIgnoreBodyRotation", JSPROXY_CCPhysicsSprite_ignoreBodyRotation, 0, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FN("_setCPBody", JSPROXY_CCPhysicsSprite_setCPBody_, 1, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FN("setIgnoreBodyRotation", JSPROXY_CCPhysicsSprite_setIgnoreBodyRotation_, 1, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FN("ctor", JSPROXY_CCPhysicsSprite_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };
    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", JSPROXY_CCPhysicsSprite_spriteWithFile_rect__static, 2, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FN("createWithSpriteFrame", JSPROXY_CCPhysicsSprite_spriteWithSpriteFrame__static, 1, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FN("createWithSpriteFrameName", JSPROXY_CCPhysicsSprite_spriteWithSpriteFrameName__static, 1, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FS_END
    };

    TypeTest<cocos2d::Sprite> t1;
    js_type_class_t *typeClass = nullptr;
    std::string typeName = t1.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    typeClass = typeMapIter->second;
    CCASSERT(typeClass, "The value is null.");

    JSPROXY_CCPhysicsSprite_object = JS_InitClass(cx, globalObj, typeClass->proto, JSPROXY_CCPhysicsSprite_class,/* dummy_constructor<PhysicsSprite>*/JSPROXY_CCPhysicsSprite_constructor, 0,properties,funcs,NULL,st_funcs);

    TypeTest<PhysicsSprite> t;
    js_type_class_t *p;
    typeName = t.s_name();
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = JSPROXY_CCPhysicsSprite_class;
        p->proto = JSPROXY_CCPhysicsSprite_object;
        p->parentProto = typeClass->proto;
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
}


void register_CCPhysicsSprite(JSContext *cx, JSObject *obj) {
    JS::RootedValue nsval(cx);
    JS::RootedObject ns(cx);
    JS_GetProperty(cx, obj, "cc", &nsval);
    if (nsval == JSVAL_VOID) {
        ns = JS_NewObject(cx, NULL, NULL, NULL);
        nsval = OBJECT_TO_JSVAL(ns);
        JS_SetProperty(cx, obj, "cc", nsval);
    } else {
        JS_ValueToObject(cx, nsval, &ns);
    }
    obj = ns;
    JSPROXY_CCPhysicsSprite_createClass(cx, obj);
}

void register_CCPhysicsDebugNode(JSContext *cx, JSObject *obj) {
    JS::RootedValue nsval(cx);
    JS::RootedObject ns(cx);
    JS_GetProperty(cx, obj, "cc", &nsval);
    if (nsval == JSVAL_VOID) {
        ns = JS_NewObject(cx, NULL, NULL, NULL);
        nsval = OBJECT_TO_JSVAL(ns);
        JS_SetProperty(cx, obj, "cc", nsval);
    } else {
        JS_ValueToObject(cx, nsval, &ns);
    }
    obj = ns;
    JSB_CCPhysicsDebugNode_createClass(cx, obj, "PhysicsDebugNode");
}

bool jsval_to_cpBB( JSContext *cx, jsval vp, cpBB *ret )
{
    JS::RootedObject jsobj(cx);
    bool ok = JS_ValueToObject( cx, JS::RootedValue(cx, vp), &jsobj );
    JSB_PRECONDITION( ok, "Error converting value to object");
    JSB_PRECONDITION( jsobj, "Not a valid JS object");
    
    JS::RootedValue vall(cx);
    JS::RootedValue valb(cx);
    JS::RootedValue valr(cx);
    JS::RootedValue valt(cx);
    ok = true;
    ok &= JS_GetProperty(cx, jsobj, "l", &vall);
    ok &= JS_GetProperty(cx, jsobj, "b", &valb);
    ok &= JS_GetProperty(cx, jsobj, "r", &valr);
    ok &= JS_GetProperty(cx, jsobj, "t", &valt);
    JSB_PRECONDITION( ok, "Error obtaining point properties");
    
    double l, b, r, t;
    ok &= JS::ToNumber(cx, vall, &l);
    ok &= JS::ToNumber(cx, valb, &b);
    ok &= JS::ToNumber(cx, valr, &r);
    ok &= JS::ToNumber(cx, valt, &t);
    JSB_PRECONDITION( ok, "Error converting value to numbers");
    
    ret->l = l;
    ret->b = b;
    ret->r = r;
    ret->t = t;
    
    return true;
}

jsval cpBB_to_jsval(JSContext *cx, cpBB bb )
{
    JSObject *object = JS_NewObject(cx, NULL, NULL, NULL );
    if (!object)
        return JSVAL_VOID;
    
    if (!JS_DefineProperty(cx, object, "l", DOUBLE_TO_JSVAL(bb.l), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) ||
        !JS_DefineProperty(cx, object, "b", DOUBLE_TO_JSVAL(bb.b), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) ||
        !JS_DefineProperty(cx, object, "r", DOUBLE_TO_JSVAL(bb.r), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) ||
        !JS_DefineProperty(cx, object, "t", DOUBLE_TO_JSVAL(bb.t), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) )
        return JSVAL_VOID;
    
    return OBJECT_TO_JSVAL(object);
}

// In order to be compatible with Chipmunk-JS API,
// this function expect to receive an array of numbers, and not an array of vects
// OK:  [1,2,  3,4,  5,6]   <- expected
// BAD: [{x:1, y:2}, {x:3,y:4}, {x:5, y:6}]  <- not expected
bool jsval_to_array_of_cpvect( JSContext *cx, jsval vp, cpVect**verts, int *numVerts)
{
    // Parsing sequence
    JS::RootedObject jsobj(cx);
    bool ok = JS_ValueToObject( cx, JS::RootedValue(cx, vp), &jsobj );
    JSB_PRECONDITION( ok, "Error converting value to object");
    
    JSB_PRECONDITION( jsobj && JS_IsArrayObject( cx, jsobj),  "Object must be an array");

    uint32_t len;
    JS_GetArrayLength(cx, jsobj, &len);
    
    JSB_PRECONDITION( len%2==0, "Array lenght should be even");
    
    cpVect *array = (cpVect*)malloc( sizeof(cpVect) * len/2);
    
    for( uint32_t i=0; i< len;i++ ) {
        JS::RootedValue valarg(cx);
        JS_GetElement(cx, jsobj, i, &valarg);

        double value;
        ok = JS::ToNumber(cx, valarg, &value);
        JSB_PRECONDITION( ok, "Error converting value to nsobject");
        
        if(i%2==0)
            array[i/2].x = value;
        else
            array[i/2].y = value;
    }
    
    *numVerts = len/2;
    *verts = array;
    
    return true;
}


bool jsval_to_cpVect( JSContext *cx, jsval vp, cpVect *ret )
{
#ifdef JSB_COMPATIBLE_WITH_COCOS2D_HTML5_BASIC_TYPES

    JS::RootedObject jsobj(cx);
    if( ! JS_ValueToObject( cx, JS::RootedValue(cx, vp), &jsobj ) )
        return false;

    JSB_PRECONDITION( jsobj, "Not a valid JS object");

    JS::RootedValue valx(cx);
    JS::RootedValue valy(cx);
    bool ok = true;
    ok &= JS_GetProperty(cx, jsobj, "x", &valx);
    ok &= JS_GetProperty(cx, jsobj, "y", &valy);

    if( ! ok )
        return false;

    double x, y;
    ok &= JS::ToNumber(cx, valx, &x);
    ok &= JS::ToNumber(cx, valy, &y);

    if( ! ok )
        return false;

    ret->x = x;
    ret->y = y;

    return true;

#else // #! JSB_COMPATIBLE_WITH_COCOS2D_HTML5_BASIC_TYPES

    JSObject *tmp_arg;
    if( ! JS_ValueToObject( cx, vp, &tmp_arg ) )
        return false;

    JSB_PRECONDITION( tmp_arg && JS_IsTypedArrayObject( tmp_arg, cx ), "Not a TypedArray object");

    JSB_PRECONDITION( JS_GetTypedArrayByteLength( tmp_arg, cx ) == sizeof(cpVect), "Invalid length");

    *ret = *(cpVect*)JS_GetArrayBufferViewData( tmp_arg, cx );

    return true;
#endif // #! JSB_COMPATIBLE_WITH_COCOS2D_HTML5_BASIC_TYPES
}


jsval cpVect_to_jsval( JSContext *cx, cpVect p)
{

#ifdef JSB_COMPATIBLE_WITH_COCOS2D_HTML5_BASIC_TYPES

    JSObject *object = JS_NewObject(cx, NULL, NULL, NULL );
    if (!object)
        return JSVAL_VOID;

    if (!JS_DefineProperty(cx, object, "x", DOUBLE_TO_JSVAL(p.x), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) ||
        !JS_DefineProperty(cx, object, "y", DOUBLE_TO_JSVAL(p.y), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) )
        return JSVAL_VOID;

    return OBJECT_TO_JSVAL(object);

#else // JSB_COMPATIBLE_WITH_COCOS2D_HTML5_BASIC_TYPES

#ifdef __LP64__
    JSObject *typedArray = JS_NewFloat64Array( cx, 2 );
#else
    JSObject *typedArray = JS_NewFloat32Array( cx, 2 );
#endif

    cpVect *buffer = (cpVect*)JS_GetArrayBufferViewData(typedArray, cx );
    *buffer = p;
    return OBJECT_TO_JSVAL(typedArray);
#endif // ! JSB_COMPATIBLE_WITH_COCOS2D_HTML5_BASIC_TYPES
}

#pragma mark - Collision Handler

struct collision_handler {
    cpCollisionType     typeA;
    cpCollisionType     typeB;
    
    JSObject            *begin;
    JSObject            *pre;
    JSObject            *post;
    JSObject            *separate;
    JSObject            *jsthis;
    JSContext           *cx;

    // "owner" of the collision handler
    // Needed when the space goes out of scope, it will remove all the allocated collision handlers for him.
    cpSpace             *space;
    
    unsigned long       hash_key;

    unsigned int        is_oo; // Objected oriented API ?
    UT_hash_handle  hh;
};

// hash
struct collision_handler* collision_handler_hash = NULL;

// helper pair
static unsigned long pair_ints( unsigned long A, unsigned long B )
{
    // order is not important
    unsigned long k1 = MIN(A, B );
    unsigned long k2 = MAX(A, B );
    
    return (k1 + k2) * (k1 + k2 + 1) /2 + k2;
}

static cpBool myCollisionBegin(cpArbiter *arb, cpSpace *space, void *data)
{
    struct collision_handler *handler = (struct collision_handler*) data;
    
    jsval args[2];
    if( handler->is_oo ) {
        args[0] = c_class_to_jsval(handler->cx, arb, JSB_cpArbiter_object, JSB_cpArbiter_class, "cpArbiter");
        args[1] = c_class_to_jsval(handler->cx, space, JSB_cpSpace_object, JSB_cpSpace_class, "cpArbiter");
    } else {
        args[0] = opaque_to_jsval( handler->cx, arb);
        args[1] = opaque_to_jsval( handler->cx, space );
    }
    
    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
    
    jsval rval;
    bool ok = JS_CallFunctionValue( handler->cx, handler->jsthis, OBJECT_TO_JSVAL(handler->begin), 2, args, &rval);
    JSB_PRECONDITION2(ok, handler->cx, cpFalse, "Error calling collision callback: begin");

    if( JSVAL_IS_BOOLEAN(rval) ) {
        bool ret = JSVAL_TO_BOOLEAN(rval);
        return (cpBool)ret;
    }
    return cpTrue;  
}

static cpBool myCollisionPre(cpArbiter *arb, cpSpace *space, void *data)
{
    struct collision_handler *handler = (struct collision_handler*) data;
    
    jsval args[2];
    if( handler->is_oo ) {
        args[0] = c_class_to_jsval(handler->cx, arb, JSB_cpArbiter_object, JSB_cpArbiter_class, "cpArbiter");
        args[1] = c_class_to_jsval(handler->cx, space, JSB_cpSpace_object, JSB_cpSpace_class, "cpArbiter");
    } else {
        args[0] = opaque_to_jsval( handler->cx, arb);
        args[1] = opaque_to_jsval( handler->cx, space );
    }
    
    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
    
    jsval rval;
    bool ok = JS_CallFunctionValue( handler->cx, handler->jsthis, OBJECT_TO_JSVAL(handler->pre), 2, args, &rval);
    JSB_PRECONDITION2(ok, handler->cx, false, "Error calling collision callback: pre");
    
    if( JSVAL_IS_BOOLEAN(rval) ) {
        bool ret = JSVAL_TO_BOOLEAN(rval);
        return (cpBool)ret;
    }
    return cpTrue;  
}

static void myCollisionPost(cpArbiter *arb, cpSpace *space, void *data)
{
    struct collision_handler *handler = (struct collision_handler*) data;
    
    jsval args[2];
    
    if( handler->is_oo ) {
        args[0] = c_class_to_jsval(handler->cx, arb, JSB_cpArbiter_object, JSB_cpArbiter_class, "cpArbiter");
        args[1] = c_class_to_jsval(handler->cx, space, JSB_cpSpace_object, JSB_cpSpace_class, "cpArbiter");
    } else {
        args[0] = opaque_to_jsval( handler->cx, arb);
        args[1] = opaque_to_jsval( handler->cx, space );
    }
    
    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
    
    jsval ignore;
    bool ok = JS_CallFunctionValue( handler->cx, handler->jsthis, OBJECT_TO_JSVAL(handler->post), 2, args, &ignore);
    JSB_PRECONDITION2(ok, handler->cx, , "Error calling collision callback: Post");
}

static void myCollisionSeparate(cpArbiter *arb, cpSpace *space, void *data)
{
    struct collision_handler *handler = (struct collision_handler*) data;
    if(! handler->cx || !handler->space)
        return;

    jsval args[2];
    if( handler->is_oo ) {
        args[0] = c_class_to_jsval(handler->cx, arb, JSB_cpArbiter_object, JSB_cpArbiter_class, "cpArbiter");
        args[1] = c_class_to_jsval(handler->cx, space, JSB_cpSpace_object, JSB_cpSpace_class, "cpArbiter");
    } else {
        args[0] = opaque_to_jsval( handler->cx, arb);
        args[1] = opaque_to_jsval( handler->cx, space );
    }
    
    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
    
    jsval ignore;
    bool ok = JS_CallFunctionValue( handler->cx, handler->jsthis, OBJECT_TO_JSVAL(handler->separate), 2, args, &ignore);
    JSB_PRECONDITION2(ok, handler->cx, , "Error calling collision callback: Separate");}

#pragma mark - cpSpace

#pragma mark constructor / destructor

void JSB_cpSpace_finalize(JSFreeOp *fop, JSObject *jsthis)
{
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    if( proxy ) {
        CCLOGINFO("jsbindings: finalizing JS object %p (cpSpace), handle: %p", jsthis, proxy->handle);
        
        // space
        cpSpace *space = (cpSpace*) proxy->handle;
        
        
        // Remove collision handlers, since the user might have forgotten to manually remove them
        struct collision_handler *current, *tmp;
        HASH_ITER(hh, collision_handler_hash, current, tmp) {
            if( current->space == space ) {

                JSContext *cx = current->cx;

                // unroot it
                if( current->begin ) {
                    JS_RemoveObjectRoot(cx, &current->begin);
                }
                if( current->pre )
                    JS_RemoveObjectRoot(cx, &current->pre);
                if( current->post )
                    JS_RemoveObjectRoot(cx, &current->post);
                if( current->separate )
                    JS_RemoveObjectRoot(cx, &current->separate);
                
                HASH_DEL(collision_handler_hash,current);  /* delete; users advances to next */
                free(current);            /* optional- if you want to free  */
            }
        }
        
        // Free Space Children
        freeSpaceChildren(space);
        
        jsb_del_jsobject_for_proxy(space);
        if(proxy->flags == JSB_C_FLAG_CALL_FREE)
            cpSpaceFree(space);
        jsb_del_c_proxy_for_jsobject(jsthis);
    }
}


#pragma mark addCollisionHandler

static
bool __jsb_cpSpace_addCollisionHandler(JSContext *cx, jsval *vp, jsval *argvp, cpSpace *space, unsigned int is_oo)
{
    struct collision_handler *handler = (struct collision_handler*) malloc( sizeof(*handler) );

    JSB_PRECONDITION(handler, "Error allocating memory");
    
    bool ok = true;
    
    // args
    ok &= jsval_to_int(cx, *argvp++, (int32_t*) &handler->typeA );
    ok &= jsval_to_int(cx, *argvp++, (int32_t*) &handler->typeB );
    
    // this is no longer passed, so "this" is going to be "this".
//  ok &= JS_ValueToObject(cx, *argvp++, &handler->jsthis );
    handler->jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    
    handler->begin = !JSVAL_IS_NULL(*argvp) ? JSVAL_TO_OBJECT(*argvp) : NULL;
    argvp++;
    handler->pre = !JSVAL_IS_NULL(*argvp) ? JSVAL_TO_OBJECT(*argvp) : NULL;
    argvp++;
    handler->post = !JSVAL_IS_NULL(*argvp) ? JSVAL_TO_OBJECT(*argvp) : NULL;
    argvp++;
    handler->separate = !JSVAL_IS_NULL(*argvp) ? JSVAL_TO_OBJECT(*argvp) : NULL;
    argvp++;
    
    JSB_PRECONDITION(ok, "Error parsing arguments");
    
    // Object Oriented API ?
    handler->is_oo = is_oo;
    
    // owner of the collision handler
    handler->space = space;
    
    // Root it
    if( handler->begin )
        JS_AddNamedObjectRoot(cx, &handler->begin, "begin collision_handler");
    if( handler->pre )
        JS_AddNamedObjectRoot(cx, &handler->pre, "pre collision_handler");
    if( handler->post )
        JS_AddNamedObjectRoot(cx, &handler->post, "post collision_handler");
    if( handler->separate )
        JS_AddNamedObjectRoot(cx, &handler->separate, "separate collision_handler");
    
    handler->cx = cx;
    
    cpSpaceAddCollisionHandler(space, handler->typeA, handler->typeB,
                               !handler->begin ? NULL : &myCollisionBegin,
                               !handler->pre ? NULL : &myCollisionPre,
                               !handler->post ? NULL : &myCollisionPost,
                               !handler->separate ? NULL : &myCollisionSeparate,
                               handler );
    
    
    //
    // Already added ? If so, remove it.
    // Then add new entry
    //
    struct collision_handler *hashElement = NULL;
    unsigned long paired_key = pair_ints(handler->typeA, handler->typeB );
    HASH_FIND_INT(collision_handler_hash, &paired_key, hashElement);
    if( hashElement ) {
        HASH_DEL( collision_handler_hash, hashElement );
        free( hashElement );
    }
    
    handler->hash_key = paired_key;
    HASH_ADD_INT( collision_handler_hash, hash_key, handler );
    
    
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return true;
}

bool JSB_cpSpaceAddCollisionHandler(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==7, cx, false, "Invalid number of arguments");


    jsval *argvp = JS_ARGV(cx,vp);

    // args
    cpSpace *space;
    bool ok = jsval_to_opaque( cx, *argvp++, (void**)&space);
    JSB_PRECONDITION(ok, "Error parsing arguments");
    
    return __jsb_cpSpace_addCollisionHandler(cx, vp, argvp, space, 0);
}

// method
bool JSB_cpSpace_addCollisionHandler(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==6, cx, false, "Invalid number of arguments");
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    JSB_PRECONDITION( jsthis, "Invalid jsthis object");
    
    struct jsb_c_proxy_s* proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    void *handle = proxy->handle;
    
    return __jsb_cpSpace_addCollisionHandler(cx, vp, JS_ARGV(cx,vp), (cpSpace*)handle, 1);
}

bool JSB_cpSpace_setDefaultCollisionHandler(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==4, cx, false, "Invalid number of arguments");
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s* proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* space = (cpSpace*) proxy->handle;

    collision_handler *handler = (collision_handler*) malloc( sizeof(collision_handler) );
    JSB_PRECONDITION(handler, "Error allocating memory");

    handler->typeA = 0;
    handler->typeB = 0;
    handler->jsthis = jsthis;

    jsval *argv = JS_ARGV(cx,vp);
    handler->begin = !JSVAL_IS_NULL(argv[0]) ? JSVAL_TO_OBJECT(argv[0]) : NULL;
    handler->pre = !JSVAL_IS_NULL(argv[1]) ? JSVAL_TO_OBJECT(argv[1]) : NULL;
    handler->post = !JSVAL_IS_NULL(argv[2]) ? JSVAL_TO_OBJECT(argv[2]) : NULL;
    handler->separate = !JSVAL_IS_NULL(argv[3]) ? JSVAL_TO_OBJECT(argv[3]) : NULL;

    // Object Oriented API ?
    handler->is_oo = 1;

    // owner of the collision handler
    handler->space = space;
    handler->cx = cx;

    cpSpaceSetDefaultCollisionHandler(space,
                               !handler->begin ? NULL : &myCollisionBegin,
                               !handler->pre ? NULL : &myCollisionPre,
                               !handler->post ? NULL : &myCollisionPost,
                               !handler->separate ? NULL : &myCollisionSeparate,
                               handler );

    //
    // Already added ? If so, remove it.
    // Then add new entry
    //
    struct collision_handler *hashElement = NULL;
    unsigned long paired_key = pair_ints(handler->typeA, handler->typeB );
    HASH_FIND_INT(collision_handler_hash, &paired_key, hashElement);
    if( hashElement ) {
        if( hashElement->begin ) {
            JS_RemoveObjectRoot(cx, &hashElement->begin);
        }
        if( hashElement->pre )
            JS_RemoveObjectRoot(cx, &hashElement->pre);
        if( hashElement->post )
            JS_RemoveObjectRoot(cx, &hashElement->post);
        if( hashElement->separate )
            JS_RemoveObjectRoot(cx, &hashElement->separate);
        HASH_DEL( collision_handler_hash, hashElement );
        free( hashElement );
    }

    handler->hash_key = paired_key;
    HASH_ADD_INT( collision_handler_hash, hash_key, handler );

    // Root it
    if( handler->begin )
        JS_AddNamedObjectRoot(cx, &handler->begin, "begin collision_handler");
    if( handler->pre )
        JS_AddNamedObjectRoot(cx, &handler->pre, "pre collision_handler");
    if( handler->post )
        JS_AddNamedObjectRoot(cx, &handler->post, "post collision_handler");
    if( handler->separate )
        JS_AddNamedObjectRoot(cx, &handler->separate, "separate collision_handler");

    JS_SET_RVAL(cx, vp, JSVAL_VOID);

    return true;
}

#pragma mark removeCollisionHandler

static
bool __jsb_cpSpace_removeCollisionHandler(JSContext *cx, jsval *vp, jsval *argvp, cpSpace *space)
{
    bool ok = true;
    
    cpCollisionType typeA;
    cpCollisionType typeB;
    ok &= jsval_to_int(cx, *argvp++, (int32_t*) &typeA );
    ok &= jsval_to_int(cx, *argvp++, (int32_t*) &typeB );

    JSB_PRECONDITION(ok, "Error parsing arguments");
    
    cpSpaceRemoveCollisionHandler(space, typeA, typeB );
    
    // Remove it
    struct collision_handler *hashElement = NULL;
    unsigned long key = pair_ints(typeA, typeB );
    HASH_FIND_INT(collision_handler_hash, &key, hashElement);
    if( hashElement ) {
        
        // unroot it
        if( hashElement->begin )
            JS_RemoveObjectRoot(cx, &hashElement->begin);
        if( hashElement->pre )
            JS_RemoveObjectRoot(cx, &hashElement->pre);
        if( hashElement->post )
            JS_RemoveObjectRoot(cx, &hashElement->post);
        if( hashElement->separate )
            JS_RemoveObjectRoot(cx, &hashElement->separate);
        
        HASH_DEL( collision_handler_hash, hashElement );
        free( hashElement );
    }
    
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return true;
}

// Free function
bool JSB_cpSpaceRemoveCollisionHandler(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==3, cx, false, "Invalid number of arguments");

    jsval *argvp = JS_ARGV(cx,vp);
    
    cpSpace* space;
    bool ok = jsval_to_opaque( cx, *argvp++, (void**)&space);
    
    JSB_PRECONDITION(ok, "Error parsing arguments");

    return __jsb_cpSpace_removeCollisionHandler(cx, vp, argvp, space);
}

// method
bool JSB_cpSpace_removeCollisionHandler(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==2, cx, false, "Invalid number of arguments");

    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    JSB_PRECONDITION( jsthis, "Invalid jsthis object");
    
    struct jsb_c_proxy_s* proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    void *handle = proxy->handle;
    
    return __jsb_cpSpace_removeCollisionHandler(cx, vp, JS_ARGV(cx,vp), (cpSpace*)handle);
}

#pragma mark Add functios. Root JSObjects

// Arguments: cpBody*
// Ret value: cpBody*
bool JSB_cpSpace_addBody(JSContext *cx, uint32_t argc, jsval *vp) {
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* arg0 = (cpSpace*) proxy->handle;
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpBody* arg1;
    
    jsval retval = *argvp; struct jsb_c_proxy_s *retproxy;
    ok &= jsval_to_c_class( cx, *argvp++, (void**)&arg1, &retproxy );
    JSB_PRECONDITION(ok, "Error processing arguments");
    
    cpSpaceAddBody((cpSpace*)arg0 , (cpBody*)arg1  );
    
    // Root it:
    JS_AddNamedObjectRoot(cx, &retproxy->jsobj, "cpBody");
    
    // addBody returns the same object that was added, so return it without conversions
    JS_SET_RVAL(cx, vp, retval);
    
    return true;
}

// Arguments: cpConstraint*
// Ret value: cpConstraint*
bool JSB_cpSpace_addConstraint(JSContext *cx, uint32_t argc, jsval *vp) {
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* arg0 = (cpSpace*) proxy->handle;
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpConstraint* arg1;
    
    jsval retval = *argvp; struct jsb_c_proxy_s *retproxy;
    ok &= jsval_to_c_class( cx, *argvp++, (void**)&arg1, &retproxy );
    JSB_PRECONDITION(ok, "Error processing arguments");
    
    cpSpaceAddConstraint((cpSpace*)arg0 , (cpConstraint*)arg1  );
    
    // Root it:
    JS_AddNamedObjectRoot(cx, &retproxy->jsobj, "cpConstraint");
    
    // addConstraint returns the same object that was added, so return it without conversions
    JS_SET_RVAL(cx, vp, retval);
    
    return true;
}

// Arguments: cpShape*
// Ret value: cpShape*
bool JSB_cpSpace_addShape(JSContext *cx, uint32_t argc, jsval *vp) {
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* arg0 = (cpSpace*) proxy->handle;
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpShape* arg1;

    jsval retval = *argvp; struct jsb_c_proxy_s *retproxy;
    ok &= jsval_to_c_class( cx, *argvp++, (void**)&arg1, &retproxy );
    JSB_PRECONDITION(ok, "Error processing arguments");
    
    cpSpaceAddShape((cpSpace*)arg0 , (cpShape*)arg1  );
    
    // Root it:
    JS_AddNamedObjectRoot(cx, &retproxy->jsobj, "cpShape");
    
    // addShape returns the same object that was added, so return it without conversions
    JS_SET_RVAL(cx, vp, retval);
    
    return true;
}

// Arguments: cpShape*
// Ret value: cpShape*
bool JSB_cpSpace_addStaticShape(JSContext *cx, uint32_t argc, jsval *vp) {
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* arg0 = (cpSpace*) proxy->handle;
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpShape* arg1;
    
    jsval retval = *argvp; struct jsb_c_proxy_s *retproxy;
    ok &= jsval_to_c_class( cx, *argvp++, (void**)&arg1, &retproxy );
    JSB_PRECONDITION(ok, "Error processing arguments");
    
    cpSpaceAddStaticShape((cpSpace*)arg0 , (cpShape*)arg1  );
    
    // Root it:
    JS_AddNamedObjectRoot(cx, &retproxy->jsobj, "cpShape (static)");

    // addStaticShape returns the same object that was added, so return it without conversions
    JS_SET_RVAL(cx, vp, retval);
    
    return true;
}

#pragma mark Remove functios. Untoot JSObjects

// Arguments: cpBody*
// Ret value: void
bool JSB_cpSpace_removeBody(JSContext *cx, uint32_t argc, jsval *vp) {
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* arg0 = (cpSpace*) proxy->handle;
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpBody* arg1;
    
    struct jsb_c_proxy_s *retproxy;
    ok &= jsval_to_c_class( cx, *argvp++, (void**)&arg1, &retproxy );
    JSB_PRECONDITION(ok, "Error processing arguments");
    
    cpSpaceRemoveBody((cpSpace*)arg0 , (cpBody*)arg1  );
    JS_RemoveObjectRoot(cx, &retproxy->jsobj);
    
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return true;
}

// Arguments: cpConstraint*
// Ret value: void
bool JSB_cpSpace_removeConstraint(JSContext *cx, uint32_t argc, jsval *vp) {
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* arg0 = (cpSpace*) proxy->handle;
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpConstraint* arg1;
    
    struct jsb_c_proxy_s *retproxy;
    ok &= jsval_to_c_class( cx, *argvp++, (void**)&arg1, &retproxy );
    JSB_PRECONDITION(ok, "Error processing arguments");
    
    cpSpaceRemoveConstraint((cpSpace*)arg0 , (cpConstraint*)arg1  );
    JS_RemoveObjectRoot(cx, &retproxy->jsobj);

    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return true;
}

// Arguments: cpShape*
// Ret value: void
bool JSB_cpSpace_removeShape(JSContext *cx, uint32_t argc, jsval *vp) {
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* arg0 = (cpSpace*) proxy->handle;
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpShape* arg1;
    
    struct jsb_c_proxy_s *retproxy;
    ok &= jsval_to_c_class( cx, *argvp++, (void**)&arg1, &retproxy );
    JSB_PRECONDITION(ok, "Error processing arguments");
    
    cpSpaceRemoveShape((cpSpace*)arg0 , (cpShape*)arg1  );
    JS_RemoveObjectRoot(cx, &retproxy->jsobj);
    
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return true;
}

// Arguments: cpShape*
// Ret value: void
bool JSB_cpSpace_removeStaticShape(JSContext *cx, uint32_t argc, jsval *vp) {
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* arg0 = (cpSpace*) proxy->handle;
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpShape* arg1;
    
    struct jsb_c_proxy_s *retproxy;
    ok &= jsval_to_c_class( cx, *argvp++, (void**)&arg1, &retproxy );
    JSB_PRECONDITION(ok, "Error processing arguments");
    
    cpSpaceRemoveStaticShape((cpSpace*)arg0 , (cpShape*)arg1  );
    JS_RemoveObjectRoot(cx, &retproxy->jsobj);
    
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return true;
}

#pragma mark segmentQueryFirst function

bool JSB_cpSpace_segmentQueryFirst(JSContext *cx, uint32_t argc, jsval *vp){
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* space = (cpSpace*) proxy->handle;
    jsval *argvp = JS_ARGV(cx,vp);
    
    cpVect start;
    cpVect end;
    cpLayers layers;
    cpGroup group;
    bool ok = true;
    ok &= jsval_to_cpVect( cx, argvp[0], &start );
    ok &= jsval_to_cpVect( cx, argvp[1], &end );
    ok &= jsval_to_uint32( cx, argvp[2], &layers );
    ok &= jsval_to_uint( cx, argvp[3], (unsigned int*)&group );
    JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");
    
    cpSegmentQueryInfo *out = new cpSegmentQueryInfo();
    cpShape* target = cpSpaceSegmentQueryFirst(space, start, end, layers, group, out);
    
    if(target)
    {
        JSObject *jsobj = JS_NewObject(cx, JSB_cpSegmentQueryInfo_class, JSB_cpSegmentQueryInfo_object, NULL);
        jsb_set_jsobject_for_proxy(jsobj, out);
        jsb_set_c_proxy_for_jsobject(jsobj, out, JSB_C_FLAG_CALL_FREE);
        JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(jsobj));
    }
    else
    {
        delete out;
        JS_SET_RVAL(cx, vp, JSVAL_NULL);
    }
    return true;
}

#pragma mark nearestPointQueryNearest function

bool JSB_cpSpace_nearestPointQueryNearest(JSContext *cx, uint32_t argc, jsval *vp){
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* space = (cpSpace*) proxy->handle;
    jsval *argvp = JS_ARGV(cx,vp);
    
    cpVect point;
    cpFloat maxDistance;
    cpLayers layers;
    cpGroup group;
    bool ok = true;
    ok &= jsval_to_cpVect( cx, argvp[0], &point );
    if(JSVAL_IS_INT(argvp[1]))
    {
        int arg1;
        ok &= jsval_to_int(cx, argvp[1], &arg1);
        maxDistance = (cpFloat) arg1;
    }
    else
    {
        maxDistance = JSVAL_TO_DOUBLE(argvp[1]);
    }
    ok &= jsval_to_uint32( cx, argvp[2], &layers );
    ok &= jsval_to_uint( cx, argvp[3], (unsigned int*)&group );
    JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");
    
    cpNearestPointQueryInfo* info = new cpNearestPointQueryInfo();
    cpShape* target = cpSpaceNearestPointQueryNearest(space, point, maxDistance, layers, group, info);
    
    if(target)
    {
        JSObject *jsobj = JS_NewObject(cx, JSB_cpNearestPointQueryInfo_class, JSB_cpNearestPointQueryInfo_object, NULL);
        jsb_set_jsobject_for_proxy(jsobj, info);
        jsb_set_c_proxy_for_jsobject(jsobj, info, JSB_C_FLAG_CALL_FREE);
        JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(jsobj));
    }
    else
    {
        delete info;
        JS_SET_RVAL(cx, vp, JSVAL_NULL);
    }
    return true;
}

struct JSB_cp_each_UserData
{
    JSContext *cx;
    jsval* func;
};

void JSB_cpSpace_pointQuery_func(cpShape *shape, void *data)
{
    JSObject *jsCpObject = jsb_get_jsobject_for_proxy(shape);
    if(jsCpObject)
    {
        JSContext* cx = ((JSB_cp_each_UserData*)data)->cx;
        jsval* func = ((JSB_cp_each_UserData*)data)->func;
        jsval rval;
        jsval argv = OBJECT_TO_JSVAL(jsCpObject);

        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
        JS_CallFunctionValue(cx, NULL, *func, 1, &argv, &rval);

    }
}

bool JSB_cpSpace_pointQuery(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc == 4, cx, false, "Invalid number of arguments");
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

    JSObject* jsthis = JSVAL_TO_OBJECT(args.thisv());
    struct jsb_c_proxy_s* proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* space = (cpSpace*) proxy->handle;

    cpVect point;
    cpLayers layers;
    cpGroup group;

    bool ok = jsval_to_cpVect(cx, args.get(0), &point);
    ok &= jsval_to_uint32(cx, args.get(1), &layers);
    ok &= jsval_to_uint(cx, args.get(2), (unsigned int*)&group);
    JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");


    JSB_cp_each_UserData *data = (JSB_cp_each_UserData*)malloc(sizeof(JSB_cp_each_UserData));
    if (!data)
        return false;

    data->cx = cx;
    data->func = const_cast<JS::Value*>(args.get(3).address());

    cpSpacePointQuery(space, point, layers, group, JSB_cpSpace_pointQuery_func, data);
    free(data);

    args.rval().setUndefined();
    return true;
}

void JSB_cpSpace_nearestPointQuery_func(cpShape *shape, cpFloat distance, cpVect point, void *data)
{
    JSObject *jsCpObject = jsb_get_jsobject_for_proxy(shape);
    if(jsCpObject)
    {
        JSContext* cx = ((JSB_cp_each_UserData*)data)->cx;
        jsval* func = ((JSB_cp_each_UserData*)data)->func;
        jsval rval;
        jsval argv[3];
        argv[0] = OBJECT_TO_JSVAL(jsCpObject);
        argv[1] = DOUBLE_TO_JSVAL(distance);
        argv[2] = cpVect_to_jsval(cx, point);

        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
        JS_CallFunctionValue(cx, NULL, *func, 3, argv, &rval);

    }
}

bool JSB_cpSpace_nearestPointQuery(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc == 5, cx, false, "Invalid number of arguments");
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

    JSObject* jsthis = JSVAL_TO_OBJECT(args.thisv());
    struct jsb_c_proxy_s* proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* space = (cpSpace*) proxy->handle;

    cpVect point;
    cpFloat maxDistance;
    cpLayers layers;
    cpGroup group;

    bool ok = jsval_to_cpVect(cx, args.get(0), &point);
    ok &= JS::ToNumber(cx, args.get(1), &maxDistance);
    ok &= jsval_to_uint32(cx, args.get(2), &layers);
    ok &= jsval_to_uint(cx, args.get(3), (unsigned int*)&group);
    JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");

    JSB_cp_each_UserData *data = (JSB_cp_each_UserData*)malloc(sizeof(JSB_cp_each_UserData));
    if (!data)
        return false;

    data->cx = cx;
    data->func = const_cast<JS::Value*>(args.get(4).address());

    cpSpaceNearestPointQuery(space, point, maxDistance, layers, group, JSB_cpSpace_nearestPointQuery_func, data);

    free(data);
    args.rval().setUndefined();
    return true;
}

void JSB_cpSpace_segmentQuery_func(cpShape *shape, cpFloat t, cpVect n, void *data)
{
    JSObject *jsCpObject = jsb_get_jsobject_for_proxy(shape);
    if(jsCpObject)
    {
        JSContext* cx = ((JSB_cp_each_UserData*)data)->cx;
        jsval* func = ((JSB_cp_each_UserData*)data)->func;
        jsval rval;
        jsval argv[3];
        argv[0] = OBJECT_TO_JSVAL(jsCpObject);
        argv[1] = DOUBLE_TO_JSVAL(t);
        argv[2] = cpVect_to_jsval(cx, n);

        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
        JS_CallFunctionValue(cx, NULL, *func, 3, argv, &rval);

    }
}

bool JSB_cpSpace_segmentQuery(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc == 5, cx, false, "Invalid number of arguments");
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

    JSObject* jsthis = JSVAL_TO_OBJECT(args.thisv());
    struct jsb_c_proxy_s* proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* space = (cpSpace*) proxy->handle;

    cpVect start;
    cpVect end;
    cpLayers layers;
    cpGroup group;

    bool ok = jsval_to_cpVect(cx, args.get(0), &start);
    ok = jsval_to_cpVect(cx, args.get(1), &end);
    ok &= jsval_to_uint32(cx, args.get(2), &layers);
    ok &= jsval_to_uint(cx, args.get(3), (unsigned int*)&group);
    JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");


    JSB_cp_each_UserData *data = (JSB_cp_each_UserData*)malloc(sizeof(JSB_cp_each_UserData));
    if (!data)
        return false;

    data->cx = cx;
    data->func = const_cast<JS::Value*>(args.get(4).address());

    cpSpaceSegmentQuery(space, start, end, layers, group, JSB_cpSpace_segmentQuery_func, data);

    free(data);
    args.rval().setUndefined();
    return true;
}

#define JSB_cpSpace_bbQuery_func JSB_cpSpace_pointQuery_func

bool JSB_cpSpace_bbQuery(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc == 4, cx, false, "Invalid number of arguments");
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

    JSObject* jsthis = JSVAL_TO_OBJECT(args.thisv());
    struct jsb_c_proxy_s* proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* space = (cpSpace*) proxy->handle;

    cpBB bb;
    cpLayers layers;
    cpGroup group;

    bool ok = jsval_to_cpBB(cx, args.get(0), &bb);
    ok &= jsval_to_uint32(cx, args.get(1), &layers);
    ok &= jsval_to_uint(cx, args.get(2), (unsigned int*)&group);
    JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");

    JSB_cp_each_UserData *data = (JSB_cp_each_UserData*)malloc(sizeof(JSB_cp_each_UserData));
    if (!data)
        return false;

    data->cx = cx;
    data->func = const_cast<JS::Value*>(args.get(3).address());

    cpSpaceBBQuery(space, bb, layers, group, JSB_cpSpace_bbQuery_func, data);

    free(data);
    args.rval().setUndefined();
    return true;
}

template<typename T>
void JSB_cpSpace_each_func(T* cpObject, void *data)
{
    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET

    JSObject *jsCpObject = jsb_get_jsobject_for_proxy(cpObject);
    if(jsCpObject)
    {
        JSContext* cx = ((JSB_cp_each_UserData*)data)->cx;
        jsval* func = ((JSB_cp_each_UserData*)data)->func;
        jsval rval;
        jsval argv = OBJECT_TO_JSVAL(jsCpObject);

        JS_CallFunctionValue(cx, NULL, *func, 1, &argv, &rval);
        
    }
}

bool JSB_cpSpace_eachShape(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc == 1, cx, false, "Invalid number of arguments");

    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* space = (cpSpace*)proxy->handle;
    jsval *argvp = JS_ARGV(cx, vp);

    JSB_cp_each_UserData *data = (JSB_cp_each_UserData*)malloc(sizeof(JSB_cp_each_UserData));
    if (!data)
        return false;

    data->cx = cx;
    data->func = argvp;

    cpSpaceEachShape(space, JSB_cpSpace_each_func, data);
    free(data);
    return true;
}

bool JSB_cpSpace_eachBody(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc == 1, cx, false, "Invalid number of arguments");

    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* space = (cpSpace*)proxy->handle;
    jsval *argvp = JS_ARGV(cx, vp);

    JSB_cp_each_UserData *data = (JSB_cp_each_UserData*)malloc(sizeof(JSB_cp_each_UserData));
    if (!data)
        return false;

    data->cx = cx;
    data->func = argvp;

    cpSpaceEachBody(space, JSB_cpSpace_each_func, data);
    free(data);
    return true;
}

bool JSB_cpSpace_eachConstraint(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc == 1, cx, false, "Invalid number of arguments");

    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* space = (cpSpace*)proxy->handle;
    jsval *argvp = JS_ARGV(cx, vp);

    JSB_cp_each_UserData *data = (JSB_cp_each_UserData*)malloc(sizeof(JSB_cp_each_UserData));
    if (!data)
        return false;

    data->cx = cx;
    data->func = argvp;

    cpSpaceEachConstraint(space, JSB_cpSpace_each_func, data);
    free(data);
    return true;
}

struct __PostStep_data{
    JSContext* cx;
    JS::Heap<JS::Value> func;
};

void __JSB_PostStep_callback(cpSpace *space, void *key, __PostStep_data *data)
{
    JSContext* cx = data->cx;
    jsval func = const_cast<jsval&>(data->func.get());
    jsval rval;
    jsval argv;

    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
    JS_CallFunctionValue(cx, NULL, func, 0, &argv, &rval);

    free(data);
}

bool JSB_cpSpace_addPostStepCallback(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc == 1, cx, false, "Invalid number of arguments");

    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JSObject* jsthis = JSVAL_TO_OBJECT(args.thisv());
    jsb_c_proxy_s* proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpSpace* space = (cpSpace*)proxy->handle;

    __PostStep_data* volatile data = (__PostStep_data*)malloc(sizeof(__PostStep_data));
    if (!data)
        return false;

    data->cx = cx;
    data->func = args.get(0);

    cpSpaceAddPostStepCallback(space, (cpPostStepFunc)__JSB_PostStep_callback, data, data);

//    free(data);
    args.rval().setUndefined();
    return true;
}

template<typename T>
void JSB_cpBody_each_func(cpBody* body, T* cpObject, void* data)
{
    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET

    JSObject *jsCpObject = jsb_get_jsobject_for_proxy(cpObject);
    if(jsCpObject)
    {
        JSContext* cx = ((JSB_cp_each_UserData*)data)->cx;
        jsval* func = ((JSB_cp_each_UserData*)data)->func;
        jsval rval;
        jsval argv = OBJECT_TO_JSVAL(jsCpObject);

        JS_CallFunctionValue(cx, NULL, *func, 1, &argv, &rval);

    }
}

bool JSB_cpBody_eachShape(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc == 1, cx, false, "Invalid number of arguments");

    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpBody* body = (cpBody*)proxy->handle;
    jsval *argvp = JS_ARGV(cx, vp);

    JSB_cp_each_UserData *data = (JSB_cp_each_UserData*)malloc(sizeof(JSB_cp_each_UserData));
    if (!data)
        return false;

    data->cx = cx;
    data->func = argvp;

    cpBodyEachShape(body, JSB_cpBody_each_func, data);
    free(data);
    return true;
}

bool JSB_cpBody_eachConstraint(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc == 1, cx, false, "Invalid number of arguments");

    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpBody* body = (cpBody*)proxy->handle;
    jsval *argvp = JS_ARGV(cx, vp);

    JSB_cp_each_UserData *data = (JSB_cp_each_UserData*)malloc(sizeof(JSB_cp_each_UserData));
    if (!data)
        return false;

    data->cx = cx;
    data->func = argvp;

    cpBodyEachConstraint(body, JSB_cpBody_each_func, data);
    free(data);
    return true;
}

bool JSB_cpBody_eachArbiter(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc == 1, cx, false, "Invalid number of arguments");

    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    cpBody* body = (cpBody*)proxy->handle;
    jsval *argvp = JS_ARGV(cx, vp);

    JSB_cp_each_UserData *data = (JSB_cp_each_UserData*)malloc(sizeof(JSB_cp_each_UserData));
    if (!data)
        return false;

    data->cx = cx;
    data->func = argvp;

    cpBodyEachArbiter(body, JSB_cpBody_each_func, data);
    free(data);
    return true;
}

#pragma mark - Arbiter

#pragma mark getBodies
static
bool __jsb_cpArbiter_getBodies(JSContext *cx, jsval *vp, jsval *argvp, cpArbiter *arbiter, unsigned int is_oo)
{
    cpBody *bodyA;
    cpBody *bodyB;
    cpArbiterGetBodies(arbiter, &bodyA, &bodyB);
    
    JS::RootedValue valA(cx);
    JS::RootedValue valB(cx);
    if( is_oo ) {
        valA = c_class_to_jsval(cx, bodyA, JSB_cpBody_object, JSB_cpBody_class, "cpArbiter");
        valB = c_class_to_jsval(cx, bodyB, JSB_cpBody_object, JSB_cpBody_class, "cpArbiter");
    } else {
        valA = opaque_to_jsval(cx, bodyA);
        valB = opaque_to_jsval(cx, bodyB);      
    }
    
    JS::RootedObject jsobj(cx, JS_NewArrayObject(cx, 2, NULL));
    JS_SetElement(cx, jsobj, 0, &valA);
    JS_SetElement(cx, jsobj, 1, &valB);
    
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(jsobj));
    
    return true;    
}

// Free function
bool JSB_cpArbiterGetBodies(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    
    jsval *argvp = JS_ARGV(cx,vp);
    
    cpArbiter* arbiter;
    if( ! jsval_to_opaque( cx, *argvp++, (void**)&arbiter ) )
        return false;

    return __jsb_cpArbiter_getBodies(cx, vp, argvp, arbiter, 0);
}

// Method
bool JSB_cpArbiter_getBodies(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==0, cx, false, "Invalid number of arguments");

    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    JSB_PRECONDITION( jsthis, "Invalid jsthis object");
    
    struct jsb_c_proxy_s* proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    JSB_PRECONDITION( proxy, "Invalid private object");
    void *handle = proxy->handle;
    
    return __jsb_cpArbiter_getBodies(cx, vp, JS_ARGV(cx,vp), (cpArbiter*)handle, 1);
}

#pragma mark getShapes
static
bool __jsb_cpArbiter_getShapes(JSContext *cx, jsval *vp, jsval *argvp, cpArbiter *arbiter, unsigned int is_oo)
{
    cpShape *shapeA;
    cpShape *shapeB;
    cpArbiterGetShapes(arbiter, &shapeA, &shapeB);

    JS::RootedValue valA(cx);
    JS::RootedValue valB(cx);
    if( is_oo ) {
        valA = c_class_to_jsval(cx, shapeA, JSB_cpShape_object, JSB_cpShape_class, "cpShape");
        valB = c_class_to_jsval(cx, shapeB, JSB_cpShape_object, JSB_cpShape_class, "cpShape");
    } else {
        valA = opaque_to_jsval(cx, shapeA);
        valB = opaque_to_jsval(cx, shapeB);
    }
    
    JS::RootedObject jsobj(cx, JS_NewArrayObject(cx, 2, NULL));
    JS_SetElement(cx, jsobj, 0, &valA);
    JS_SetElement(cx, jsobj, 1, &valB);
    
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(jsobj));
    
    return true;
}

// function
bool JSB_cpArbiterGetShapes(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    
    jsval *argvp = JS_ARGV(cx,vp);
    
    cpArbiter* arbiter;
    if( ! jsval_to_opaque( cx, *argvp++, (void**) &arbiter ) )
       return false;

    return __jsb_cpArbiter_getShapes(cx, vp, argvp, arbiter, 0);
}

// method
bool JSB_cpArbiter_getShapes(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==0, cx, false, "Invalid number of arguments");
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    JSB_PRECONDITION( jsthis, "Invalid jsthis object");
    
    struct jsb_c_proxy_s* proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    void *handle = proxy->handle;
    
    return __jsb_cpArbiter_getShapes(cx, vp, JS_ARGV(cx,vp), (cpArbiter*)handle, 1);
}

#pragma mark - Body

#pragma mark constructor

// Manually added to identify static vs dynamic bodies
bool JSB_cpBody_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==2, cx, false, "Invalid number of arguments");
    JSObject *jsobj = JS_NewObject(cx, JSB_cpBody_class, JSB_cpBody_object, NULL);
    JS::CallArgs argvp = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    double m; double i;
    
    ok &= JS::ToNumber( cx, argvp[0], &m );
    ok &= JS::ToNumber( cx, argvp[1], &i );
    JSB_PRECONDITION(ok, "Error processing arguments");
    
    cpBody *ret_body = NULL;
    if( m == INFINITY && i == INFINITY) {
        ret_body = cpBodyNewStatic();
        
        // XXX: Hack. IT WILL LEAK "rogue" objects., But at least it prevents a crash.
        // The thing is that "rogue" bodies needs to be freed after the its shape, and I am not sure
        // how to do it in a "js" way.
        jsb_set_c_proxy_for_jsobject(jsobj, ret_body, JSB_C_FLAG_DO_NOT_CALL_FREE);
    } else {
        ret_body = cpBodyNew((cpFloat)m , (cpFloat)i  );
        jsb_set_c_proxy_for_jsobject(jsobj, ret_body, JSB_C_FLAG_CALL_FREE);
    }
    
    jsb_set_jsobject_for_proxy(jsobj, ret_body);
    
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(jsobj));
    return true;
}

#pragma mark getUserData

static
bool __jsb_cpBody_getUserData(JSContext *cx, jsval *vp, jsval *argvp, cpBody *body)
{
    JSObject *data = (JSObject*) cpBodyGetUserData(body);
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(data));
    
    return true;
}

// free function
bool JSB_cpBodyGetUserData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");

    jsval *argvp = JS_ARGV(cx,vp);
    cpBody *body;
    if( ! jsval_to_opaque( cx, *argvp++, (void**) &body ) )
        return false;

    return __jsb_cpBody_getUserData(cx, vp, argvp, body);
}

// method
bool JSB_cpBody_getUserData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==0, cx, false, "Invalid number of arguments");
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    JSB_PRECONDITION( jsthis, "Invalid jsthis object");
    
    struct jsb_c_proxy_s* proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    void *handle = proxy->handle;
    
    return __jsb_cpBody_getUserData(cx, vp, JS_ARGV(cx,vp), (cpBody*)handle);
}


#pragma mark setUserData

static
bool __jsb_cpBody_setUserData(JSContext *cx, jsval *vp, jsval *argvp, cpBody *body)
{
    JS::RootedObject jsobj(cx);

    bool ok = JS_ValueToObject(cx, JS::RootedValue(cx, *argvp), &jsobj);

    JSB_PRECONDITION(ok, "Error parsing arguments");
    
    cpBodySetUserData(body, jsobj);
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    
    return true;
}

// free function
bool JSB_cpBodySetUserData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==2, cx, false, "Invalid number of arguments");

    jsval *argvp = JS_ARGV(cx,vp);
    cpBody *body;
    bool ok = jsval_to_opaque( cx, *argvp++, (void**) &body );
    JSB_PRECONDITION(ok, "Error parsing arguments");
    return __jsb_cpBody_setUserData(cx, vp, argvp, body);
}

// method
bool JSB_cpBody_setUserData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    JSB_PRECONDITION( jsthis, "Invalid jsthis object");
    
    struct jsb_c_proxy_s* proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    void *handle = proxy->handle;
    
    return __jsb_cpBody_setUserData(cx, vp, JS_ARGV(cx,vp), (cpBody*)handle);
}

#pragma mark - Poly related

// cpFloat cpAreaForPoly(const int numVerts, const cpVect *verts);
bool JSB_cpAreaForPoly(JSContext *cx, uint32_t argc, jsval *vp)
{   
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpVect *verts;
    int numVerts;
    
    ok &= jsval_to_array_of_cpvect( cx, *argvp++, &verts, &numVerts);
    JSB_PRECONDITION2(ok, cx, false, "Error parsing array");
    
    cpFloat area = cpAreaForPoly(numVerts, verts);
    
    free(verts);
    
    JS_SET_RVAL(cx, vp, DOUBLE_TO_JSVAL(area));
    return true;
}

// cpFloat cpMomentForPoly(cpFloat m, int numVerts, const cpVect *verts, cpVect offset);
bool JSB_cpMomentForPoly(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==3, cx, false, "Invalid number of arguments");
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpVect *verts; cpVect offset;
    int numVerts;
    double m;
    
    ok &= JS::ToNumber(cx, JS::RootedValue(cx, *argvp++), &m);
    ok &= jsval_to_array_of_cpvect( cx, *argvp++, &verts, &numVerts);
    ok &= jsval_to_cpVect( cx, *argvp++, (cpVect*) &offset );

    JSB_PRECONDITION2(ok, cx, false, "Error parsing args");
    
    cpFloat moment = cpMomentForPoly((cpFloat)m, numVerts, verts, offset);
    
    free(verts);
    
    JS_SET_RVAL(cx, vp, DOUBLE_TO_JSVAL(moment));
    return true;
}

// cpVect cpCentroidForPoly(const int numVerts, const cpVect *verts);
bool JSB_cpCentroidForPoly(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpVect *verts;
    int numVerts;
    
    ok &= jsval_to_array_of_cpvect( cx, *argvp++, &verts, &numVerts);
    JSB_PRECONDITION2(ok, cx, false, "Error parsing args");
    
    cpVect centroid = cpCentroidForPoly(numVerts, verts);
    
    free(verts);
    
    JS_SET_RVAL(cx, vp, cpVect_to_jsval(cx, (cpVect)centroid));
    return true;
}

// void cpRecenterPoly(const int numVerts, cpVect *verts);
bool JSB_cpRecenterPoly(JSContext *cx, uint32_t argc, jsval *vp)
{
    CCASSERT(false, "NOT IMPLEMENTED");
    return false;
}

#pragma mark - Object Oriented Chipmunk

/*
 * Chipmunk Base Object
 */

JSClass* JSB_cpBase_class = NULL;
JSObject* JSB_cpBase_object = NULL;
// Constructor
bool JSB_cpBase_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2( argc==1, cx, false, "Invalid arguments. Expecting 1");
    
    JSObject *jsobj = JS_NewObject(cx, JSB_cpBase_class, JSB_cpBase_object, NULL);
    
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    
    void *handle = NULL;
    
    ok = jsval_to_opaque(cx, *argvp++, &handle);
    
    JSB_PRECONDITION(ok, "Error converting arguments for JSB_cpBase_constructor");

    jsb_set_c_proxy_for_jsobject(jsobj, handle, JSB_C_FLAG_DO_NOT_CALL_FREE);
    jsb_set_jsobject_for_proxy(jsobj, handle);
    
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(jsobj));
    return true;
}

// Destructor
void JSB_cpBase_finalize(JSFreeOp *fop, JSObject *obj)
{
    CCLOGINFO("jsbindings: finalizing JS object %p (cpBase)", obj);
    
    // should not delete the handle since it was manually added
}

bool JSB_cpBase_getHandle(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    JSB_PRECONDITION( jsthis, "Invalid jsthis object");
    JSB_PRECONDITION2(argc==0, cx, false, "Invalid number of arguments");

    struct jsb_c_proxy_s* proxy = jsb_get_c_proxy_for_jsobject(jsthis);
    void *handle = proxy->handle;
    
    jsval ret_val = opaque_to_jsval(cx, handle);
    JS_SET_RVAL(cx, vp, ret_val);
    return true;
}

bool JSB_cpBase_setHandle(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject* jsthis = (JSObject *)JS_THIS_OBJECT(cx, vp);
    JSB_PRECONDITION( jsthis, "Invalid jsthis object");
    JSB_PRECONDITION2(argc==1, cx, false, "Invalid number of arguments");
    
    jsval *argvp = JS_ARGV(cx,vp);
    
    void *handle;
    bool ok = jsval_to_opaque(cx, *argvp++, &handle);
    JSB_PRECONDITION( ok, "Invalid parsing arguments");

    jsb_set_c_proxy_for_jsobject(jsthis, handle, JSB_C_FLAG_DO_NOT_CALL_FREE);
    jsb_set_jsobject_for_proxy(jsthis, handle);
    
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return true;
}


void JSB_cpBase_createClass(JSContext *cx, JSObject* globalObj, const char* name )
{
    JSB_cpBase_class = (JSClass *)calloc(1, sizeof(JSClass));
    JSB_cpBase_class->name = name;
    JSB_cpBase_class->addProperty = JS_PropertyStub;
    JSB_cpBase_class->delProperty = JS_DeletePropertyStub;
    JSB_cpBase_class->getProperty = JS_PropertyStub;
    JSB_cpBase_class->setProperty = JS_StrictPropertyStub;
    JSB_cpBase_class->enumerate = JS_EnumerateStub;
    JSB_cpBase_class->resolve = JS_ResolveStub;
    JSB_cpBase_class->convert = JS_ConvertStub;
    JSB_cpBase_class->finalize = JSB_cpBase_finalize;
    JSB_cpBase_class->flags = JSCLASS_HAS_PRIVATE;
    
    static JSPropertySpec properties[] = {
        {0, 0, 0, 0, 0}
    };
    static JSFunctionSpec funcs[] = {
        JS_FN("getHandle", JSB_cpBase_getHandle, 0, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FN("setHandle", JSB_cpBase_setHandle, 1, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FS_END
    };
    static JSFunctionSpec st_funcs[] = {
        JS_FS_END
    };
    
    JSB_cpBase_object = JS_InitClass(cx, globalObj, NULL, JSB_cpBase_class, JSB_cpBase_constructor,0,properties,funcs,NULL,st_funcs);
//  bool found;
//  JS_SetPropertyAttributes(cx, globalObj, name, JSPROP_ENUMERATE | JSPROP_READONLY, &found);
}

// Manual "methods"
// Constructor
bool JSB_cpPolyShape_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc==3, cx, false, "Invalid number of arguments");
    JSObject *jsobj = JS_NewObject(cx, JSB_cpPolyShape_class, JSB_cpPolyShape_object, NULL);
    jsval *argvp = JS_ARGV(cx,vp);
    bool ok = true;
    cpBody* body; cpVect *verts; cpVect offset;
    int numVerts;
    
    ok &= jsval_to_c_class( cx, *argvp++, (void**)&body, NULL );
    ok &= jsval_to_array_of_cpvect( cx, *argvp++, &verts, &numVerts);
    ok &= jsval_to_cpVect( cx, *argvp++, (cpVect*) &offset );
    JSB_PRECONDITION(ok, "Error processing arguments");
    cpShape *shape = cpPolyShapeNew(body, numVerts, verts, offset);

    jsb_set_c_proxy_for_jsobject(jsobj, shape, JSB_C_FLAG_CALL_FREE);
    jsb_set_jsobject_for_proxy(jsobj, shape);
    
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(jsobj));
    
    free(verts);
    
    return true;
}


#pragma mark Space Free functions
//
// When the space is removed, it should all remove its children. But not "free" them.
// "free" will be performed by the JS Garbage Collector
//
// Functions copied & pasted from ChipmunkDemo.c
// https://github.com/slembcke/Chipmunk-Physics/blob/master/Demo/ChipmunkDemo.c#L89
//

static void unroot_jsobject_from_handle(void *handle)
{
    JSObject *jsobj = jsb_get_jsobject_for_proxy(handle);
    //2014.9.19 by joshua
    //add safe guard
    if(jsobj)
    {
        struct jsb_c_proxy_s *proxy = jsb_get_c_proxy_for_jsobject(jsobj);
    
        // HACK context from global
        JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
        JS_RemoveObjectRoot(cx, &proxy->jsobj);
    }
    
}
static void shapeFreeWrap(cpSpace *space, cpShape *shape, void *unused){
    cpSpaceRemoveShape(space, shape);
    unroot_jsobject_from_handle(shape);
//  cpShapeFree(shape);
}

static void postShapeFree(cpShape *shape, cpSpace *space){
    cpSpaceAddPostStepCallback(space, (cpPostStepFunc)shapeFreeWrap, shape, NULL);
}

static void constraintFreeWrap(cpSpace *space, cpConstraint *constraint, void *unused){
    cpSpaceRemoveConstraint(space, constraint);
    unroot_jsobject_from_handle(constraint);
//  cpConstraintFree(constraint);
}

static void postConstraintFree(cpConstraint *constraint, cpSpace *space){
    cpSpaceAddPostStepCallback(space, (cpPostStepFunc)constraintFreeWrap, constraint, NULL);
}

static void bodyFreeWrap(cpSpace *space, cpBody *body, void *unused){
    cpSpaceRemoveBody(space, body);
    unroot_jsobject_from_handle(body);
//  cpBodyFree(body);
}

static void postBodyFree(cpBody *body, cpSpace *space){
    cpSpaceAddPostStepCallback(space, (cpPostStepFunc)bodyFreeWrap, body, NULL);
}

// Safe and future proof way to remove and free all objects that have been added to the space.
void static freeSpaceChildren(cpSpace *space)
{
    // Must remove these BEFORE freeing the body or you will access dangling pointers.
    cpSpaceEachShape(space, (cpSpaceShapeIteratorFunc)postShapeFree, space);
    cpSpaceEachConstraint(space, (cpSpaceConstraintIteratorFunc)postConstraintFree, space);
    
    cpSpaceEachBody(space, (cpSpaceBodyIteratorFunc)postBodyFree, space);
}

#endif // JSB_INCLUDE_CHIPMUNK