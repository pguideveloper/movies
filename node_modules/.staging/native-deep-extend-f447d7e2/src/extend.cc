//#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <v8.h>

using namespace node;
using namespace v8;


Local<Value> Clone(Local<Value> target) {
    Local<Value> result = target;

    if(target->IsFunction()) {result = Local<Value>::New(Local<Function>::Cast(target)->Clone());} else 
    if(target->IsArray()) {result = Local<Value>::New(Local<Array>::Cast(target)->Clone());} else
    if(target->IsDate()) {result = Local<Value>::New(Local<Date>::Cast(target)->Clone());} else
    if(target->IsObject()) {result = Local<Value>::New(Local<Object>::Cast(target)->Clone());} else
    if(target->IsStringObject()) {result = Local<Value>::New(Local<StringObject>::Cast(target)->Clone());} else
//    if(target->IsBoolean()) {result = Local<Boolean>::Cast(target);} else
    if(target->IsBooleanObject()) {result = Local<Value>::New(Local<BooleanObject>::Cast(target)->Clone());} else
//    if(target->IsExternal()) {result = Local<External>::Cast(target);} else
//    if(target->IsFalse()) {result = Local<False>::Cast(target);} else
//    if(target->IsInt32()) {result = Local<Int32>::Cast(target);} else
//    if(target->IsNativeError()) {result = Local<NativeError>::Cast(target);} else
//    if(target->IsNull()) {result = Local<Null>::Cast(target);} else
//    if(target->IsNumber()) {result = Local<Number>::Cast(target);} else
    if(target->IsNumberObject()) {result = Local<Value>::New(Local<NumberObject>::Cast(target)->Clone());}
//    if(target->IsString()) {result = Local<String>::Cast(target);} else
//    if(target->IsRegExp()) {result = Local<RegExp>::Cast(target)->Clone();} else
//    if(target->IsTrue()) {result = Local<True>::Cast(target);} else
//    if(target->IsUint32()) {result = Local<Uint32>::Cast(target);} else
//    if(target->IsUndefined()) {result = Local<Undefined>::Cast(target);} else

    return result;
}

void ObjectExtend(Local<Object>target, Local<Object>source);

void ArrayExtend(Local<Array>target, Local<Array>source) {
    const uint32_t length = source->Length();
    for (uint32_t i = 0; i < length; i++) {
        Handle<Value> arrayIndex = Number::New(i);
        if (source->Get(arrayIndex)->IsArray()) {
            if (!target->Get(arrayIndex)->IsArray()) {
                target->Set(arrayIndex, Local<Array>::Cast(source->Get(arrayIndex))->Clone());
            }
            ArrayExtend(Local<Array>::Cast(target->Get(arrayIndex)), Local<Array>::Cast(source->Get(arrayIndex)));
        } else if (source->Get(arrayIndex)->IsObject()) {
            if (!target->Get(arrayIndex)->IsObject()) {
                target->Set(arrayIndex, Local<Object>::Cast(source->Get(arrayIndex))->Clone());
            }
            ObjectExtend(Local<Object>::Cast(target->Get(arrayIndex)), Local<Object>::Cast(source->Get(arrayIndex)));
        } else if (!source->Get(arrayIndex)->IsUndefined()){
            target->Set(arrayIndex, Clone(source->Get(arrayIndex)));
        }
    }
};

void ObjectExtend(Local<Object>target, Local<Object>source) {
    Local<Array> keys = source->GetOwnPropertyNames();
    const int keyLength = keys->Length();
    
    for (int i = 0; i < keyLength; i++) {
        Handle<Value> keyName = Handle<String>::Cast(keys->Get(Integer::New(i)));
        
        if (source->Get(keyName)->IsArray()) {
            if (!target->Get(keyName)->IsArray()) {
                target->Set(keyName, Local<Array>::Cast(source->Get(keyName))->Clone());
            }
            ArrayExtend(Local<Array>::Cast(target->Get(keyName)), Local<Array>::Cast(source->Get(keyName)));
        } else if (source->Get(keyName)->IsObject()) {
            if(!target->Get(keyName)->IsObject()) {
                target->Set(keyName, Local<Object>::Cast(source->Get(keyName))->Clone());
            }
            ObjectExtend(Local<Object>::Cast(target->Get(keyName)), Local<Object>::Cast(source->Get(keyName)));
        } else if (!source->Get(keyName)->IsUndefined()){
            target->Set(keyName, Clone(source->Get(keyName)));
        }
    }
}

Handle<Value> Extend(const Arguments& args) {
    HandleScope scope;
    const int argc = args.Length();
    Local<Value> target = args[0];
    if (argc == 1) {
        if(target->IsArray()) {
            ArrayExtend(Local<Array>::Cast(Local<Array>::Cast(target)->Clone()), Local<Array>::Cast(target));
        } else if (target->IsObject()) {
            ObjectExtend(Local<Object>::Cast(target)->Clone(), Local<Object>::Cast(target));
        }
    } else if (!(target->IsObject() || target->IsArray())) {
        return scope.Close(Clone(target));
    } else {
        for (int i = 1; i < argc; i++) {
            if (args[i]->IsObject() && target->IsObject()) {
                ObjectExtend(Local<Object>::Cast(target), Local<Object>::Cast(args[i]));
            } else if (args[i]->IsArray() && target->IsArray()) {
                ArrayExtend(Local<Array>::Cast(target), Local<Array>::Cast(args[i]));
            }
        }
    }
    return scope.Close(target);
}

Handle<Value> CloneValue(const Arguments& args) {
    HandleScope scope;
    if (!args.Length()) return scope.Close(Undefined());
    
    Local<Value> target = args[0];
    Handle<Value> handleTarget = Clone(Local<Value>::New(target));
    return scope.Close(handleTarget);
}

extern "C" {
    void init(Handle<Object> target) {

      target->Set(String::NewSymbol("clone"),
          FunctionTemplate::New(CloneValue)->GetFunction());
      target->Set(String::NewSymbol("extend"),
          FunctionTemplate::New(Extend)->GetFunction());
    }

    NODE_MODULE(extend, init)
}
