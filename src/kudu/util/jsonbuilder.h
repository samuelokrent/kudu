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
#ifndef KUDU_UTIL_JSONBUILDER_H_
#define KUDU_UTIL_JSONBUILDER_H_

#include <string>

#include <rapidjson/document.h>

#include "kudu/gutil/ref_counted.h"

namespace kudu {

class EasyJson {
 public:
  EasyJson();
  ~EasyJson() = default;

  bool GetBool() const;
  int32_t GetInt() const;
  uint32_t GetUint() const;
  int64_t GetInt64() const;
  uint64_t GetUint64() const;
  double GetDouble() const;
  const char* GetString() const;

  EasyJson Get(const std::string& key);
  EasyJson Get(int index);

  EasyJson operator[](const std::string& key);
  EasyJson operator[](int index);

  template<typename T>
  EasyJson& operator=(T val);

  EasyJson& SetObject();
  EasyJson& SetArray();
  template<typename T>
  EasyJson Set(const std::string& key, T val);
  template<typename T>
  EasyJson Set(int index, T val);
  template<typename T>
  EasyJson PushBack(T val);

  rapidjson::Value& value() const;

 private:
  class EasyJsonAllocator : public RefCounted<EasyJsonAllocator> {
   public:
    rapidjson::Value& value() { return value_; }
    rapidjson::Document::AllocatorType& allocator() { return doc_.GetAllocator(); }
   private:
    friend class RefCounted<EasyJsonAllocator>;
    ~EasyJsonAllocator() = default;

    rapidjson::Value value_;
    rapidjson::Document doc_;
  };

  EasyJson(rapidjson::Value& value, scoped_refptr<EasyJsonAllocator>& alloc);

  scoped_refptr<EasyJsonAllocator> alloc_;
  rapidjson::Value& value_;
};

} // namespace kudu

#endif // KUDU_UTIL_JSONBUILDER_H_
