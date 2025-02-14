// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: AutoEquip.proto

#ifndef PROTOBUF_AutoEquip_2eproto__INCLUDED
#define PROTOBUF_AutoEquip_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include "ItemModule.pb.h"
// @@protoc_insertion_point(includes)

namespace main {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_AutoEquip_2eproto();
void protobuf_AssignDesc_AutoEquip_2eproto();
void protobuf_ShutdownFile_AutoEquip_2eproto();

class AutoAllEquipRequest;
class AutoAllEquipResponse;
class AutoEquipRequest;
class AutoEquipResponse;

// ===================================================================

class AutoAllEquipRequest : public ::google::protobuf::MessageLite {
 public:
  AutoAllEquipRequest();
  virtual ~AutoAllEquipRequest();

  AutoAllEquipRequest(const AutoAllEquipRequest& from);

  inline AutoAllEquipRequest& operator=(const AutoAllEquipRequest& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::std::string* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const AutoAllEquipRequest& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const AutoAllEquipRequest* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(AutoAllEquipRequest* other);

  // implements Message ----------------------------------------------

  AutoAllEquipRequest* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const AutoAllEquipRequest& from);
  void MergeFrom(const AutoAllEquipRequest& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  void DiscardUnknownFields();
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:main.AutoAllEquipRequest)
 private:

  ::std::string _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_AutoEquip_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_AutoEquip_2eproto();
  #endif
  friend void protobuf_AssignDesc_AutoEquip_2eproto();
  friend void protobuf_ShutdownFile_AutoEquip_2eproto();

  void InitAsDefaultInstance();
  static AutoAllEquipRequest* default_instance_;
};
// -------------------------------------------------------------------

class AutoAllEquipResponse : public ::google::protobuf::MessageLite {
 public:
  AutoAllEquipResponse();
  virtual ~AutoAllEquipResponse();

  AutoAllEquipResponse(const AutoAllEquipResponse& from);

  inline AutoAllEquipResponse& operator=(const AutoAllEquipResponse& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::std::string* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const AutoAllEquipResponse& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const AutoAllEquipResponse* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(AutoAllEquipResponse* other);

  // implements Message ----------------------------------------------

  AutoAllEquipResponse* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const AutoAllEquipResponse& from);
  void MergeFrom(const AutoAllEquipResponse& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  void DiscardUnknownFields();
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 state = 1;
  inline bool has_state() const;
  inline void clear_state();
  static const int kStateFieldNumber = 1;
  inline ::google::protobuf::int32 state() const;
  inline void set_state(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:main.AutoAllEquipResponse)
 private:
  inline void set_has_state();
  inline void clear_has_state();

  ::std::string _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 state_;
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_AutoEquip_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_AutoEquip_2eproto();
  #endif
  friend void protobuf_AssignDesc_AutoEquip_2eproto();
  friend void protobuf_ShutdownFile_AutoEquip_2eproto();

  void InitAsDefaultInstance();
  static AutoAllEquipResponse* default_instance_;
};
// -------------------------------------------------------------------

class AutoEquipRequest : public ::google::protobuf::MessageLite {
 public:
  AutoEquipRequest();
  virtual ~AutoEquipRequest();

  AutoEquipRequest(const AutoEquipRequest& from);

  inline AutoEquipRequest& operator=(const AutoEquipRequest& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::std::string* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const AutoEquipRequest& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const AutoEquipRequest* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(AutoEquipRequest* other);

  // implements Message ----------------------------------------------

  AutoEquipRequest* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const AutoEquipRequest& from);
  void MergeFrom(const AutoEquipRequest& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  void DiscardUnknownFields();
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 officerId = 1;
  inline bool has_officerid() const;
  inline void clear_officerid();
  static const int kOfficerIdFieldNumber = 1;
  inline ::google::protobuf::int32 officerid() const;
  inline void set_officerid(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:main.AutoEquipRequest)
 private:
  inline void set_has_officerid();
  inline void clear_has_officerid();

  ::std::string _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 officerid_;
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_AutoEquip_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_AutoEquip_2eproto();
  #endif
  friend void protobuf_AssignDesc_AutoEquip_2eproto();
  friend void protobuf_ShutdownFile_AutoEquip_2eproto();

  void InitAsDefaultInstance();
  static AutoEquipRequest* default_instance_;
};
// -------------------------------------------------------------------

class AutoEquipResponse : public ::google::protobuf::MessageLite {
 public:
  AutoEquipResponse();
  virtual ~AutoEquipResponse();

  AutoEquipResponse(const AutoEquipResponse& from);

  inline AutoEquipResponse& operator=(const AutoEquipResponse& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::std::string* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const AutoEquipResponse& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const AutoEquipResponse* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(AutoEquipResponse* other);

  // implements Message ----------------------------------------------

  AutoEquipResponse* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const AutoEquipResponse& from);
  void MergeFrom(const AutoEquipResponse& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  void DiscardUnknownFields();
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 state = 1;
  inline bool has_state() const;
  inline void clear_state();
  static const int kStateFieldNumber = 1;
  inline ::google::protobuf::int32 state() const;
  inline void set_state(::google::protobuf::int32 value);

  // optional .main.UpdateBagInfo updateBagInfo = 2;
  inline bool has_updatebaginfo() const;
  inline void clear_updatebaginfo();
  static const int kUpdateBagInfoFieldNumber = 2;
  inline const ::main::UpdateBagInfo& updatebaginfo() const;
  inline ::main::UpdateBagInfo* mutable_updatebaginfo();
  inline ::main::UpdateBagInfo* release_updatebaginfo();
  inline void set_allocated_updatebaginfo(::main::UpdateBagInfo* updatebaginfo);

  // @@protoc_insertion_point(class_scope:main.AutoEquipResponse)
 private:
  inline void set_has_state();
  inline void clear_has_state();
  inline void set_has_updatebaginfo();
  inline void clear_has_updatebaginfo();

  ::std::string _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::main::UpdateBagInfo* updatebaginfo_;
  ::google::protobuf::int32 state_;
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_AutoEquip_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_AutoEquip_2eproto();
  #endif
  friend void protobuf_AssignDesc_AutoEquip_2eproto();
  friend void protobuf_ShutdownFile_AutoEquip_2eproto();

  void InitAsDefaultInstance();
  static AutoEquipResponse* default_instance_;
};
// ===================================================================


// ===================================================================

// AutoAllEquipRequest

// -------------------------------------------------------------------

// AutoAllEquipResponse

// optional int32 state = 1;
inline bool AutoAllEquipResponse::has_state() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AutoAllEquipResponse::set_has_state() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AutoAllEquipResponse::clear_has_state() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AutoAllEquipResponse::clear_state() {
  state_ = 0;
  clear_has_state();
}
inline ::google::protobuf::int32 AutoAllEquipResponse::state() const {
  // @@protoc_insertion_point(field_get:main.AutoAllEquipResponse.state)
  return state_;
}
inline void AutoAllEquipResponse::set_state(::google::protobuf::int32 value) {
  set_has_state();
  state_ = value;
  // @@protoc_insertion_point(field_set:main.AutoAllEquipResponse.state)
}

// -------------------------------------------------------------------

// AutoEquipRequest

// optional int32 officerId = 1;
inline bool AutoEquipRequest::has_officerid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AutoEquipRequest::set_has_officerid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AutoEquipRequest::clear_has_officerid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AutoEquipRequest::clear_officerid() {
  officerid_ = 0;
  clear_has_officerid();
}
inline ::google::protobuf::int32 AutoEquipRequest::officerid() const {
  // @@protoc_insertion_point(field_get:main.AutoEquipRequest.officerId)
  return officerid_;
}
inline void AutoEquipRequest::set_officerid(::google::protobuf::int32 value) {
  set_has_officerid();
  officerid_ = value;
  // @@protoc_insertion_point(field_set:main.AutoEquipRequest.officerId)
}

// -------------------------------------------------------------------

// AutoEquipResponse

// optional int32 state = 1;
inline bool AutoEquipResponse::has_state() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AutoEquipResponse::set_has_state() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AutoEquipResponse::clear_has_state() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AutoEquipResponse::clear_state() {
  state_ = 0;
  clear_has_state();
}
inline ::google::protobuf::int32 AutoEquipResponse::state() const {
  // @@protoc_insertion_point(field_get:main.AutoEquipResponse.state)
  return state_;
}
inline void AutoEquipResponse::set_state(::google::protobuf::int32 value) {
  set_has_state();
  state_ = value;
  // @@protoc_insertion_point(field_set:main.AutoEquipResponse.state)
}

// optional .main.UpdateBagInfo updateBagInfo = 2;
inline bool AutoEquipResponse::has_updatebaginfo() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void AutoEquipResponse::set_has_updatebaginfo() {
  _has_bits_[0] |= 0x00000002u;
}
inline void AutoEquipResponse::clear_has_updatebaginfo() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void AutoEquipResponse::clear_updatebaginfo() {
  if (updatebaginfo_ != NULL) updatebaginfo_->::main::UpdateBagInfo::Clear();
  clear_has_updatebaginfo();
}
inline const ::main::UpdateBagInfo& AutoEquipResponse::updatebaginfo() const {
  // @@protoc_insertion_point(field_get:main.AutoEquipResponse.updateBagInfo)
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  return updatebaginfo_ != NULL ? *updatebaginfo_ : *default_instance().updatebaginfo_;
#else
  return updatebaginfo_ != NULL ? *updatebaginfo_ : *default_instance_->updatebaginfo_;
#endif
}
inline ::main::UpdateBagInfo* AutoEquipResponse::mutable_updatebaginfo() {
  set_has_updatebaginfo();
  if (updatebaginfo_ == NULL) updatebaginfo_ = new ::main::UpdateBagInfo;
  // @@protoc_insertion_point(field_mutable:main.AutoEquipResponse.updateBagInfo)
  return updatebaginfo_;
}
inline ::main::UpdateBagInfo* AutoEquipResponse::release_updatebaginfo() {
  clear_has_updatebaginfo();
  ::main::UpdateBagInfo* temp = updatebaginfo_;
  updatebaginfo_ = NULL;
  return temp;
}
inline void AutoEquipResponse::set_allocated_updatebaginfo(::main::UpdateBagInfo* updatebaginfo) {
  delete updatebaginfo_;
  updatebaginfo_ = updatebaginfo;
  if (updatebaginfo) {
    set_has_updatebaginfo();
  } else {
    clear_has_updatebaginfo();
  }
  // @@protoc_insertion_point(field_set_allocated:main.AutoEquipResponse.updateBagInfo)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace main

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_AutoEquip_2eproto__INCLUDED
