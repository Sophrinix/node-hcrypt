#include <node.h>
#include "scarab_object.h"

using namespace v8;

Persistent<Function> Scarab::constructor;

Scarab::Scarab() {
	fhe_pk_init(pk);
    fhe_sk_init(sk);
	fhe_keygen(pk, sk);
}

Scarab::~Scarab() {
}

void Scarab::Init(Handle<Object> exports) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);

  tpl->SetClassName(String::NewSymbol("Scarab"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("encrypt"),
      FunctionTemplate::New(Encrypt)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("Scarab"), constructor);
}

Handle<Value> Scarab::New(const Arguments& args) {
  HandleScope scope;

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new Scarab(...)`
    Scarab* obj = new Scarab();
    obj->Wrap(args.This());
    return args.This();
  } else {
    // Invoked as plain function `Scarab(...)`, turn into construct call.
    const int argc = 0;
    Local<Value> argv[argc] = {};
    return scope.Close(constructor->NewInstance(argc, argv));
  }
}

Handle<Value> Scarab::Encrypt(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(String::New("Argument must be number")));
    return scope.Close(Undefined());
  }

  int m = args[0]->NumberValue();

  if ((m != 0) && (m != 1)) {
    ThrowException(Exception::TypeError(String::New("Argument must be either 0 or 1")));
    return scope.Close(Undefined());
  }

  Scarab* obj = ObjectWrap::Unwrap<Scarab>(args.This());

  mpz_t c;
  mpz_init(c);

  fhe_encrypt(c, obj->pk, m);

  char* s = mpz_get_str(NULL, 10, c);

  return scope.Close(String::New(s));
}

