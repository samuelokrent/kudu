// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "kudu/util/easy_json.h"

#include <string>
#include <utility>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "kudu/gutil/strings/substitute.h"

using rapidjson::SizeType;
using rapidjson::Value;
using std::string;
using strings::Substitute;

namespace kudu {

EasyJson::EasyJson() : alloc_(new EasyJsonAllocator), value_(&alloc_->value()) {}

EasyJson::EasyJson(EasyJson::ComplexTypeInitializer type)
    : alloc_(new EasyJsonAllocator), value_(&alloc_->value()) {
  if (type == kObject) {
    value_->SetObject();
  } else if (type == kArray) {
    value_->SetArray();
  }
}

EasyJson EasyJson::Get(const string& key) {
  if (!value_->IsObject()) {
    value_->SetObject();
  }
  if (!value_->HasMember(key.c_str())) {
    Value key_val(key.c_str(), alloc_->allocator());
    value_->AddMember(key_val, Value().SetNull(), alloc_->allocator());
  }
  return EasyJson(&(*value_)[key.c_str()], alloc_);
}

EasyJson EasyJson::Get(int index) {
  if (!value_->IsArray()) {
    value_->SetArray();
  }
  while (SizeType(index) >= value_->Size()) {
    value_->PushBack(Value().SetNull(), alloc_->allocator());
  }
  return EasyJson(&(*value_)[index], alloc_);
}

EasyJson EasyJson::operator[](const string& key) {
  return Get(key);
}

EasyJson EasyJson::operator[](int index) {
  return Get(index);
}

EasyJson& EasyJson::operator=(const string& val) {
  value_->SetString(val.c_str(), alloc_->allocator());
  return *this;
}
template<typename T>
EasyJson& EasyJson::operator=(T val) {
  *value_ = val;
  return *this;
}
template EasyJson& EasyJson::operator=<bool>(bool val);
template EasyJson& EasyJson::operator=<int32_t>(int32_t val);
template EasyJson& EasyJson::operator=<int64_t>(int64_t val);
template EasyJson& EasyJson::operator=<double>(double val);
template<> EasyJson& EasyJson::operator=<const char*>(const char* val) {
  value_->SetString(val, alloc_->allocator());
  return *this;
}
template<> EasyJson& EasyJson::operator=<EasyJson::ComplexTypeInitializer>(
    EasyJson::ComplexTypeInitializer val) {
  if (val == kObject) {
    value_->SetObject();
  } else if (val == kArray) {
    value_->SetArray();
  }
  return (*this);
}

EasyJson& EasyJson::SetObject() {
  if (!value_->IsObject()) {
    value_->SetObject();
  }
  return *this;
}

EasyJson& EasyJson::SetArray() {
  if (!value_->IsArray()) {
    value_->SetArray();
  }
  return *this;
}

EasyJson EasyJson::Set(const string& key, const string& val) {
  return (Get(key) = val);
}
template<typename T>
EasyJson EasyJson::Set(const string& key, T val) {
  return (Get(key) = val);
}
template EasyJson EasyJson::Set<bool>(const string& key, bool val);
template EasyJson EasyJson::Set<int32_t>(const string& key, int32_t val);
template EasyJson EasyJson::Set<int64_t>(const string& key, int64_t val);
template EasyJson EasyJson::Set<double>(const string& key, double val);
template EasyJson EasyJson::Set<const char*>(const string& key, const char* val);
template EasyJson EasyJson::Set<EasyJson::ComplexTypeInitializer>(
    const string& key,
    EasyJson::ComplexTypeInitializer val);

EasyJson EasyJson::Set(int index, const string& val) {
  return (Get(index) = val);
}
template<typename T>
EasyJson EasyJson::Set(int index, T val) {
  return (Get(index) = val);
}
template EasyJson EasyJson::Set<bool>(int index, bool val);
template EasyJson EasyJson::Set<int32_t>(int index, int32_t val);
template EasyJson EasyJson::Set<int64_t>(int index, int64_t val);
template EasyJson EasyJson::Set<double>(int index, double val);
template EasyJson EasyJson::Set<const char*>(int index, const char* val);
template EasyJson EasyJson::Set<EasyJson::ComplexTypeInitializer>(
    int index,
    EasyJson::ComplexTypeInitializer val);

EasyJson EasyJson::PushBack(const string& val) {
  if (!value_->IsArray()) {
    value_->SetArray();
  }
  Value push_val(val.c_str(), alloc_->allocator());
  value_->PushBack(push_val, alloc_->allocator());
  return EasyJson(&(*value_)[value_->Size() - 1], alloc_);
}
template<typename T>
EasyJson EasyJson::PushBack(T val) {
  if (!value_->IsArray()) {
    value_->SetArray();
  }
  value_->PushBack(val, alloc_->allocator());
  return EasyJson(&(*value_)[value_->Size() - 1], alloc_);
}
template EasyJson EasyJson::PushBack<bool>(bool val);
template EasyJson EasyJson::PushBack<int32_t>(int32_t val);
template EasyJson EasyJson::PushBack<int64_t>(int64_t val);
template EasyJson EasyJson::PushBack<double>(double val);
template<> EasyJson EasyJson::PushBack<const char*>(const char* val) {
  if (!value_->IsArray()) {
    value_->SetArray();
  }
  Value push_val(val, alloc_->allocator());
  value_->PushBack(push_val, alloc_->allocator());
  return EasyJson(&(*value_)[value_->Size() - 1], alloc_);
}
template<> EasyJson EasyJson::PushBack<EasyJson::ComplexTypeInitializer>(
    EasyJson::ComplexTypeInitializer val) {
  if (!value_->IsArray()) {
    value_->SetArray();
  }
  Value push_val;
  if (val == kObject) {
    push_val.SetObject();
  } else if (val == kArray) {
    push_val.SetArray();
  }
  value_->PushBack(push_val, alloc_->allocator());
  return EasyJson(&(*value_)[value_->Size() - 1], alloc_);
}

string EasyJson::ToString(bool html) const {
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  value_->Accept(writer);
  return html ? Substitute("<pre>$0</pre>", buffer.GetString()) : buffer.GetString();
}

EasyJson::EasyJson(Value* value, const scoped_refptr<EasyJsonAllocator>& alloc)
    : alloc_(alloc), value_(value) {}

} // namespace kudu
