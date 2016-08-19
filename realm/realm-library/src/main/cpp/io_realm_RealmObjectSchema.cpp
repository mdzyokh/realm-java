/*
 * Copyright 2016 Realm Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jni.h>
#include "io_realm_RealmObjectSchema.h"

#include <object-store/src/shared_realm.hpp>
#include <object-store/src/object_schema.hpp>
#include <object-store/src/property.hpp>

#include "util.hpp"

using namespace realm;

JNIEXPORT jlong JNICALL
Java_io_realm_RealmObjectSchema_nativeCreateObjectSchema__(
        JNIEnv *env, jclass
) {
    TR_ENTER()
    try {
        auto* object_schema = new ObjectSchema();
        return reinterpret_cast<jlong>(object_schema);
    }
    CATCH_STD()
    return 0;
}

JNIEXPORT jlong JNICALL
Java_io_realm_RealmObjectSchema_nativeCreateObjectSchema__Ljava_lang_String_2(JNIEnv *env, jclass,
                                                                              jstring className_) {
    TR_ENTER()
    try {
        JStringAccessor name(env, className_);
        auto* object_schema = new ObjectSchema();
        object_schema->name = name;
        return reinterpret_cast<jlong>(object_schema);
    }
    CATCH_STD()
    return 0;
}

JNIEXPORT jlong JNICALL
Java_io_realm_RealmObjectSchema_nativeCreateObjectSchema__JLjava_lang_String_2
(JNIEnv *env, jclass, jlong nativeSharedRealmPtr, jstring className) {
    TR_ENTER_PTR(nativeSharedRealmPtr)
    try {
        auto shared_realm = *(reinterpret_cast<SharedRealm*>(nativeSharedRealmPtr));
        JStringAccessor name(env, className);
        auto tmp = ObjectSchema(shared_realm->read_group(), name);
        ObjectSchema* object_schema = new ObjectSchema(tmp);
        return reinterpret_cast<jlong>(object_schema);
    }
    CATCH_STD()
    return 0;
}

JNIEXPORT void JNICALL
Java_io_realm_RealmObjectSchema_nativeClose
(JNIEnv *env, jclass, jlong native_ptr) {
    TR_ENTER_PTR(native_ptr)
    try {
        auto* object_schema = reinterpret_cast<ObjectSchema*>(native_ptr);
        delete object_schema;
    }
    CATCH_STD()
}

JNIEXPORT jstring JNICALL
Java_io_realm_RealmObjectSchema_nativeGetClassName(JNIEnv *env, jclass type, jlong native_ptr) {
    TR_ENTER_PTR(native_ptr)
    try {
        auto* object_schema = reinterpret_cast<ObjectSchema*>(native_ptr);
        auto name = object_schema->name;
        return to_jstring(env, name);
    }
    CATCH_STD()
    return nullptr;
}

JNIEXPORT jlong JNICALL
Java_io_realm_RealmObjectSchema_nativeGetPropertyByName
(JNIEnv *env, jclass, jlong native_ptr, jstring name) {
    TR_ENTER_PTR(native_ptr)
    auto* object_schema = reinterpret_cast<ObjectSchema*>(native_ptr);
    try {
        JStringAccessor str(env, name);
        Property *property = object_schema->property_for_name(str);
        return reinterpret_cast<jlong>(property);
    }
    CATCH_STD()
    return 0;
}

JNIEXPORT jlong JNICALL
Java_io_realm_RealmObjectSchema_nativeGetPropertyByPrimaryKey
(JNIEnv *env, jclass, jlong native_ptr) {
    TR_ENTER_PTR(native_ptr)
    auto* object_schema = reinterpret_cast<ObjectSchema*>(native_ptr);
    try {
        Property *property = object_schema->primary_key_property();
        return reinterpret_cast<jlong>(property);
    }
    CATCH_STD()
    return 0;
}

JNIEXPORT void JNICALL
Java_io_realm_RealmObjectSchema_nativeAddProperty
(JNIEnv *env, jclass, jlong native_ptr, jlong native_property_ptr) {
    TR_ENTER_PTR(native_ptr);
    try {
        auto* object_schema = reinterpret_cast<ObjectSchema*>(native_ptr);
        auto* property = reinterpret_cast<Property*>(native_property_ptr);
        TR("native_property_ptr = %p", VOID_PTR(property))
        object_schema->persisted_properties.push_back(*property);
    }
    CATCH_STD()
}

JNIEXPORT void JNICALL
Java_io_realm_RealmObjectSchema_nativeRemovePropertyByName
(JNIEnv *env, jclass, jlong native_ptr, jstring name) {
    TR_ENTER_PTR(native_ptr)
    try {
        auto* object_schema = reinterpret_cast<ObjectSchema*>(native_ptr);
        JStringAccessor str(env, name);
        object_schema->persisted_properties.erase(std::remove_if(object_schema->persisted_properties.begin(),
                                                                 object_schema->persisted_properties.end(),
                                                                 [&str](const Property& p){ return p.name == str; }));
    }
    CATCH_STD()
}





