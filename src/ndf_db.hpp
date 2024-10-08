#pragma once

#include "ndf_properties.hpp"
#include "sqlite3.h"
#include "sqlite_helpers.hpp"
#include <filesystem>

#include "ndf.hpp"

namespace fs = std::filesystem;

class NDF_DB {
private:
  sqlite3 *db;
  // insertion statements
  SQLStatement<5, 0> stmt_insert_ndf_file;
  SQLStatement<5, 0> stmt_insert_ndf_object;
  SQLStatement<8, 0> stmt_insert_ndf_property;
  SQLStatement<1, 0> stmt_insert_ndf_bool;
  SQLStatement<1, 0> stmt_insert_ndf_int8;
  SQLStatement<1, 0> stmt_insert_ndf_uint8;
  SQLStatement<1, 0> stmt_insert_ndf_int16;
  SQLStatement<1, 0> stmt_insert_ndf_uint16;
  SQLStatement<1, 0> stmt_insert_ndf_int32;
  SQLStatement<1, 0> stmt_insert_ndf_uint32;
  SQLStatement<1, 0> stmt_insert_ndf_float32;
  SQLStatement<1, 0> stmt_insert_ndf_float64;
  SQLStatement<1, 0> stmt_insert_ndf_string;
  SQLStatement<1, 0> stmt_insert_ndf_widestring;
  SQLStatement<2, 0> stmt_insert_ndf_F32_vec2;
  SQLStatement<3, 0> stmt_insert_ndf_F32_vec3;
  SQLStatement<4, 0> stmt_insert_ndf_F32_vec4;
  SQLStatement<2, 0> stmt_insert_ndf_S32_vec2;
  SQLStatement<3, 0> stmt_insert_ndf_S32_vec3;
  SQLStatement<4, 0> stmt_insert_ndf_S32_vec4;
  SQLStatement<4, 0> stmt_insert_ndf_color;
  SQLStatement<2, 0> stmt_insert_ndf_object_reference;
  SQLStatement<2, 0> stmt_insert_ndf_import_reference;
  SQLStatement<1, 0> stmt_insert_ndf_GUID;
  SQLStatement<1, 0> stmt_insert_ndf_path_reference;
  SQLStatement<1, 0> stmt_insert_ndf_localisation_hash;
  SQLStatement<1, 0> stmt_insert_ndf_hash;
  // accessors for objects
  SQLStatement<1, 1> stmt_get_object_from_name;
  SQLStatement<1, 1> stmt_get_object_from_export_path;
  SQLStatement<1, 1> stmt_get_object_ndf_id;
  SQLStatement<1, 1> stmt_get_object_name;
  SQLStatement<1, 1> stmt_get_object_names;
  SQLStatement<1, 1> stmt_get_object_class_names;
  SQLStatement<1, 1> stmt_get_object_export_path;
  SQLStatement<1, 5> stmt_get_object;
  SQLStatement<1, 1> stmt_get_object_properties;
  SQLStatement<1, 1> stmt_get_property_names;
  SQLStatement<1, 8> stmt_get_property;
  // accessors for values, should only return a single row each
  SQLStatement<1, 1> stmt_get_bool_value;
  SQLStatement<1, 1> stmt_get_int8_value;
  SQLStatement<1, 1> stmt_get_uint8_value;
  SQLStatement<1, 1> stmt_get_int16_value;
  SQLStatement<1, 1> stmt_get_uint16_value;
  SQLStatement<1, 1> stmt_get_int32_value;
  SQLStatement<1, 1> stmt_get_uint32_value;
  SQLStatement<1, 1> stmt_get_float32_value;
  SQLStatement<1, 1> stmt_get_float64_value;
  SQLStatement<1, 1> stmt_get_string_value;
  SQLStatement<1, 1> stmt_get_widestring_value;
  SQLStatement<1, 2> stmt_get_F32_vec2_value;
  SQLStatement<1, 3> stmt_get_F32_vec3_value;
  SQLStatement<1, 4> stmt_get_F32_vec4_value;
  SQLStatement<1, 2> stmt_get_S32_vec2_value;
  SQLStatement<1, 3> stmt_get_S32_vec3_value;
  SQLStatement<1, 4> stmt_get_S32_vec4_value;
  SQLStatement<1, 4> stmt_get_color_value;
  SQLStatement<1, 2> stmt_get_object_reference_value;
  SQLStatement<1, 2> stmt_get_import_reference_value;
  SQLStatement<1, 1> stmt_get_GUID_value;
  SQLStatement<1, 1> stmt_get_path_reference_value;
  SQLStatement<1, 1> stmt_get_localisation_hash_value;
  SQLStatement<1, 1> stmt_get_hash_value;
  // accessor used by lists, maps and pairs, returns all associated property ids
  // in order
  SQLStatement<1, 1> stmt_get_list_items;
  SQLStatement<1, 1> stmt_get_objects_referencing;
  SQLStatement<1, 1> stmt_get_objects_importing;

  // update statements
  SQLStatement<2, 0> stmt_set_bool_value;
  SQLStatement<2, 0> stmt_set_uint8_value;
  SQLStatement<2, 0> stmt_set_int8_value;
  SQLStatement<2, 0> stmt_set_uint16_value;
  SQLStatement<2, 0> stmt_set_int16_value;
  SQLStatement<2, 0> stmt_set_uint32_value;
  SQLStatement<2, 0> stmt_set_int32_value;
  SQLStatement<2, 0> stmt_set_float32_value;
  SQLStatement<2, 0> stmt_set_float64_value;
  SQLStatement<2, 0> stmt_set_string_value;
  SQLStatement<2, 0> stmt_set_widestring_value;
  SQLStatement<3, 0> stmt_set_F32_vec2_value;
  SQLStatement<4, 0> stmt_set_F32_vec3_value;
  SQLStatement<5, 0> stmt_set_F32_vec4_value;
  SQLStatement<3, 0> stmt_set_S32_vec2_value;
  SQLStatement<4, 0> stmt_set_S32_vec3_value;
  SQLStatement<5, 0> stmt_set_S32_vec4_value;
  SQLStatement<5, 0> stmt_set_color_value;
  SQLStatement<2, 0> stmt_set_import_reference_value;
  SQLStatement<2, 0> stmt_set_object_reference_value;
  SQLStatement<2, 0> stmt_set_GUID_value;
  SQLStatement<2, 0> stmt_set_path_reference_value;
  SQLStatement<2, 0> stmt_set_localisation_hash_value;
  SQLStatement<2, 0> stmt_set_hash_value;
  // object update statements
  SQLStatement<2, 0> stmt_set_object_name;
  SQLStatement<2, 0> stmt_set_object_export_path;
  // fix references, to be called after inserting objects
  SQLStatement<1, 0> stmt_update_object_references;
  SQLStatement<0, 0> stmt_update_import_references;
  // delete statements
  SQLStatement<0, 0> stmt_delete_ndf_file;

  bool init_statements();

  friend struct NDFProperty;
  friend struct NDFPropertyBool;
  friend struct NDFPropertyUInt8;
  friend struct NDFPropertyInt8;
  friend struct NDFPropertyUInt16;
  friend struct NDFPropertyInt16;
  friend struct NDFPropertyUInt32;
  friend struct NDFPropertyInt32;
  friend struct NDFPropertyFloat32;
  friend struct NDFPropertyFloat64;
  friend struct NDFPropertyString;
  friend struct NDFPropertyWideString;
  friend struct NDFPropertyF32_vec2;
  friend struct NDFPropertyF32_vec3;
  friend struct NDFPropertyF32_vec4;
  friend struct NDFPropertyS32_vec2;
  friend struct NDFPropertyS32_vec3;
  friend struct NDFPropertyColor;
  friend struct NDFPropertyImportReference;
  friend struct NDFPropertyObjectReference;
  friend struct NDFPropertyGUID;
  friend struct NDFPropertyPathReference;
  friend struct NDFPropertyLocalisationHash;
  friend struct NDFPropertyHash;
  friend struct NDFPropertyList;
  friend struct NDFPropertyMap;
  friend struct NDFPropertyPair;

public:
  bool init();
  bool init(fs::path path);
  ~NDF_DB();

  std::optional<int> insert_file(std::string vfs_path, std::string dat_path,
                                 std::string fs_path, std::string version,
                                 bool is_current = true);
  std::optional<int> insert_object(int ndf_idx, const NDFObject &object);
  bool insert_property(const NDFProperty &property, int object_idx,
                       int parent = -1, int position = -1);

  std::optional<NDFObject> get_object(int object_idx);
  // std::optional<std::vector<NDFObject>> get_objects(int ndf_idx);
  std::optional<std::unique_ptr<NDFProperty>> get_property(int property_idx);

  bool change_object_name(int object_idx, std::string new_name);
  bool change_export_path(int object_idx, std::string new_path);
  bool fix_references(int ndf_id);
};
